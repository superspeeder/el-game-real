#include "event.hpp"
#include <iostream>

EventLayer::EventLayer(int idx) {
    m_Index = idx;
}

int EventLayer::getIndex() {
    return m_Index;
};

bool EventLayer::operator<(const EventLayer& rhs) const {
    return m_Index > rhs.m_Index;
}


void EventLayer::addListener(std::type_index ti, PFNinternaleventfun fun) {
    if (m_Listeners.find(ti) == m_Listeners.end()) {
        m_Listeners[ti] = {};
    }

    m_Listeners[ti].push_back(fun);
}

std::vector<PFNinternaleventfun>& EventLayer::getListeners(std::type_index ti) {
    if (m_Listeners.find(ti) == m_Listeners.end()) {
        m_Listeners[ti] = {};
    }

    return m_Listeners[ti];
}


void OnDemandEventManager::post_(std::type_index ti, BaseEvent* evt) {
    for (EventLayer* el : m_Layers) {
        for (PFNinternaleventfun& ief : el->getListeners(ti)) {
            ief(evt);
        }

        if (evt->is_canceled()) {
            break;
        }
    }
    delete evt;
};

EventManager::EventManager() {
    m_DefaultLayer = new EventLayer(0);
    m_Layers.insert(m_DefaultLayer);
}

EventManager::~EventManager() {

}

EventLayer* EventManager::createLayer(int idx) {
    EventLayer* el = new EventLayer(idx);
    m_Layers.insert(el);
    return el;
}

void EventManager::addListener(std::type_index ti, PFNinternaleventfun fun) {
    m_DefaultLayer->addListener(ti, fun);
}

AsyncEventManager::AsyncEventManager() {
    m_ThreadPool = new thread_pool<MAX_EVENT_THREADS>([this](std::stop_token t) { event_thread(t, this); });
}


void AsyncEventManager::post_(std::type_index ti, BaseEvent* evt) {
    std::lock_guard<std::mutex> lockg(m_EQMutex);
    m_EventQueue.push({ ti, evt });
    m_EAVcv.notify_one(); // tell 1 of the waiting threads that an event is available
};

evt_item AsyncEventManager::popEvent() {
    {
        std::lock_guard<std::mutex> lockg(m_EQMutex);
        if (!m_EventQueue.empty()) {
            auto val = m_EventQueue.front();
            m_EventQueue.pop();
            return val;
        }
    }
    std::unique_lock<std::mutex> eavcvlk(m_EAVcvMutex);
    while (true) {
        m_EAVcv.wait(eavcvlk);
        std::lock_guard<std::mutex> lockg(m_EQMutex);
        if (!m_EventQueue.empty()) { // prevent race condition where thread A is waiting in this loop, but thread B just finished processing an event and grabbed the event this thread was notified for before this thread could
            auto val = m_EventQueue.front();
            m_EventQueue.pop();
            return val;
        }
        throw std::runtime_error("ahshit");
    }
};

std::set<EventLayer*, ptr_less<EventLayer>> EventManager::getLayers() {
    return m_Layers;
}

void event_thread(std::stop_token st, AsyncEventManager* evtm) {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string sid = ss.str();
    spdlog::info("Began thread #{}", sid);
    while (!st.stop_requested()) {
        try {
            evt_item evt = evtm->popEvent();
            std::vector<std::vector<PFNinternaleventfun> > listeners;
            {
                std::lock_guard<std::mutex> lk(evtm->m_LayersMutex); // safely grab all the listeners
                for (EventLayer* el : evtm->getLayers()) {
                    listeners.push_back(el->getListeners(evt.first));
                }
            }

            for (auto& vec : listeners) {
                for (PFNinternaleventfun& f : vec) {
                    f(evt.second);
                }
                if (evt.second->is_canceled()) {
                    break;
                }
            }

            delete evt.second;

            // finished processing of event
            std::this_thread::yield(); // yield to others execution to ensure other threads get to run
        }
        catch (std::runtime_error e) {
            // do nothing
        }
    }

    spdlog::info("Ending thread #{}", sid);
}

