#pragma once

#include "utils.hpp"
#include <typeindex>
#include <atomic>
#include <concepts>
#include <future>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

class BaseEvent {
public:

    inline virtual void cancel() noexcept {
    }; // no-op by default

    const inline virtual bool is_cancelable() const noexcept { return false; };
    inline virtual bool is_canceled() const noexcept { return false; };

private:

};
using PFNinternaleventfun = std::function<void(BaseEvent*)>;

class CancelableEvent : public BaseEvent {
public:

    inline virtual void cancel() noexcept override {
        m_Canceled = true;
    };

    const inline virtual bool is_cancelable() const noexcept override { return true; };
    inline virtual bool is_canceled() const noexcept override { return m_Canceled; };

private:
    std::atomic<bool> m_Canceled = false;
};

template <typename T>
concept event_class = std::is_base_of_v<BaseEvent, T>;

template <event_class T>
constexpr bool is_event_cancelable() { return std::is_base_of_v<CancelableEvent, T>; };

class EventManager;

class EventLayer {
public:

    EventLayer(int idx);

    int getIndex();
    bool operator<(const EventLayer& rhs) const;

    void addListener(std::type_index ti, PFNinternaleventfun fun);

    template<event_class E>
    inline void addListener(std::function<void(E*)> fun) {
        addListener(std::type_index(typeid(E)), [fun](BaseEvent* evt) {
            E* evt_ = reinterpret_cast<E*>(evt);
            fun(evt_);
            });
    };

    template<event_class E, typename T>
    inline void addListener(void(T::* fun)(E*), T* o) {
        addListener(std::type_index(typeid(E)), [fun, o](BaseEvent* evt) {
            E* evt_ = reinterpret_cast<E*>(evt);
            std::invoke(fun, o, evt_);
            });
    };

    std::vector<PFNinternaleventfun>& getListeners(std::type_index ti);

    template<event_class E>
    inline std::vector<PFNinternaleventfun>& getListeners() {
        return getListeners(std::type_index(typeid(E)));
    };

private:

    int m_Index;
    EventManager* m_EventManager;
    std::unordered_map<std::type_index, std::vector<PFNinternaleventfun> > m_Listeners;

};

class EventManager {
public:

    EventManager();
    virtual ~EventManager();

    virtual void post_(std::type_index ti, BaseEvent* evt) = 0;

    template<event_class E>
    inline void post(E* evt) {
        post_(std::type_index(typeid(E)), dynamic_cast<BaseEvent*>(evt));
    };

    std::set<EventLayer*, ptr_less<EventLayer> > getLayers();

    void addListener(std::type_index ti, PFNinternaleventfun fun);
    EventLayer* createLayer(int idx);

    template<event_class E>
    inline void addListener(std::function<void(E*)> fun) {
        m_DefaultLayer->addListener<E>(fun);
    };

    template<event_class E, typename T>
    inline void addListener(void(T::* fun)(E*), T* o) {
        m_DefaultLayer->addListener<E>(fun, o);
    };


protected:
    std::set<EventLayer*, ptr_less<EventLayer> > m_Layers;
    EventLayer* m_DefaultLayer;
};

/**
 * @brief Most basic event manager. synchronous and runs in-time
*/
class OnDemandEventManager : public EventManager {
public:

    virtual void post_(std::type_index ti, BaseEvent* evt) override;

private:

};

const size_t MAX_EVENT_THREADS = 10;


struct evt_item {
    std::type_index first;
    BaseEvent* second;
};

/**
 * @brief
 *
 * Separate posted events are handled asynchronously, but within the handling of a single event the listeners and layers are called synchronously
 * This prevents slowdowns due to thread syncronization, but leverages the strengths of
 *
*/
class AsyncEventManager : public EventManager {
public:

    AsyncEventManager();

    virtual void post_(std::type_index ti, BaseEvent* evt) override;

    evt_item popEvent(); // will wait for event if one doesn't exist
    std::mutex m_LayersMutex;

    inline void end() {
        m_ThreadPool->stop(m_EAVcv);
    };

    inline void end_after_done() {
        while (!m_EventQueue.empty()) {
            std::this_thread::yield();
        }
        end();
    };

private:
    thread_pool<MAX_EVENT_THREADS>* m_ThreadPool;
    std::mutex m_EQMutex;
    std::queue<evt_item> m_EventQueue;
    std::condition_variable m_EAVcv;
    std::mutex m_EAVcvMutex;
};

void event_thread(std::stop_token st, AsyncEventManager* evtm);

