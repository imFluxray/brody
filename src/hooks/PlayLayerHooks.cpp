#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../ProphecyManager.hpp"
#include "../OracleOverlay.hpp"

using namespace geode::prelude;

class $modify(ProphecyPlayLayer, PlayLayer) {
    struct Fields {
        float m_timeSinceLastProphecy = 0.f;
        float m_nextProphecyTime = 0.f;
        bool m_initialized = false;
    };
    
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }
        
        ProphecyManager::get()->reset();
        
        m_fields->m_timeSinceLastProphecy = 0.f;
        m_fields->m_initialized = true;
        
        float frequency = Mod::get()->getSavedValue<double>("frequency", 8.0);
        if (frequency < 3.0) frequency = 3.0; // Hard clamp min
        
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(frequency * 0.5f, frequency * 1.5f);
        m_fields->m_nextProphecyTime = dist(rng);
        
        this->schedule(schedule_selector(ProphecyPlayLayer::prophecyUpdate), 0.f);
        
        return true;
    }
    
    void prophecyUpdate(float dt) {
        if (!m_fields->m_initialized) return;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;
        
        // Strict Level End Check
        if (this->m_hasCompletedLevel) return;
        
        ProphecyManager::get()->update(dt);
        
        if (!ProphecyManager::get()->hasPendingProphecy()) {
            m_fields->m_timeSinceLastProphecy += dt;
            
            if (m_fields->m_timeSinceLastProphecy >= m_fields->m_nextProphecyTime) {
                ProphecyManager::get()->triggerRandomProphecy();
                m_fields->m_timeSinceLastProphecy = 0.f;
                
                float frequency = Mod::get()->getSavedValue<double>("frequency", 8.0);
                if (frequency < 3.0) frequency = 3.0; // Hard clamp min
                
                std::mt19937 rng(std::random_device{}());
                std::uniform_real_distribution<float> dist(frequency * 0.5f, frequency * 1.5f);
                m_fields->m_nextProphecyTime = dist(rng);
            }
        }
    }
    
    void resetLevel() {
        PlayLayer::resetLevel();
        
        ProphecyManager::get()->reset();
        m_fields->m_timeSinceLastProphecy = 0.f;
        
        float frequency = Mod::get()->getSavedValue<double>("frequency", 8.0);
        if (frequency < 3.0) frequency = 3.0;
        
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(frequency * 0.5f, frequency * 1.5f);
        m_fields->m_nextProphecyTime = dist(rng);
    }
    
    void onQuit() {
        this->unschedule(schedule_selector(ProphecyPlayLayer::prophecyUpdate));
        ProphecyManager::get()->reset();
        PlayLayer::onQuit();
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (object && object->m_objectType == GameObjectType::Hazard) {
            if (ProphecyManager::get()->areSpikesHarmless()) {
                return;
            }
        }
        
        ProphecyManager::get()->reset();
        BrodyOverlay::hide();
        
        PlayLayer::destroyPlayer(player, object);
    }
};
