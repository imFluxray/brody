#include "ProphecyManager.hpp"
#include "OracleOverlay.hpp"
#include "ProphecyTexts.hpp"

ProphecyManager* ProphecyManager::s_instance = nullptr;

ProphecyManager::ProphecyManager() 
    : m_rng(std::random_device{}())
    , m_prophecyPending(false)
    , m_prophecyFulfilled(true)
    , m_pendingTimer(0.f)
    , m_areSpikesHarmless(false) 
{}

void ProphecyManager::fulfillProphecy() {
    if (!m_prophecyPending) return;
    
     auto playLayer = PlayLayer::get();
    if (!playLayer || playLayer->m_hasCompletedLevel) {
        m_prophecyPending = false;
        m_prophecyFulfilled = true;
        return;
    }
    
    auto player = playLayer->m_player1;
    if (!player) {
        m_prophecyPending = false;
        m_prophecyFulfilled = true;
        return;
    }
    
    switch (m_pendingType) {
        case ProphecyType::GravityFlip:
            player->flipGravity(!player->m_isUpsideDown, true);
            break;
        case ProphecyType::SpeedChange:
            {
                float newSpeed = (player->m_playerSpeed > 1.0f) ? 0.9f : 1.1f;
                player->updateTimeMod(newSpeed, true);
            }
            break;
        case ProphecyType::SizeChange:
            {
                bool isMini = player->m_vehicleSize < 1.0f;
                player->togglePlayerScale(!isMini, true);
            }
            break;
        case ProphecyType::LevelComplete:
            playLayer->playEndAnimationToPos({0, 0});
            break;
        case ProphecyType::SpikesRemoved:
            m_areSpikesHarmless = true;
            m_hasTriggeredSpikes = true;
            
            if (playLayer->m_objects) {
                CCObject* obj = nullptr;
                CCARRAY_FOREACH(playLayer->m_objects, obj) {
                     GameObject* gameObj = typeinfo_cast<GameObject*>(obj);
                     if (gameObj && gameObj->m_objectType == GameObjectType::Hazard) {
                         gameObj->setVisible(false);
                         gameObj->setPosition(ccp(-99999.f, -99999.f));
                     }
                }
            }
            break;
    }
    
    m_prophecyPending = false;
    m_prophecyFulfilled = true;
}

void ProphecyManager::reset() {
    m_prophecyPending = false;
    m_prophecyFulfilled = true;
    m_pendingTimer = 0.f;
    m_areSpikesHarmless = false;
    m_hasTriggeredSpikes = false;
    BrodyOverlay::hide();
}

ProphecyManager* ProphecyManager::get() {
    if (!s_instance) {
        s_instance = new ProphecyManager();
    }
    return s_instance;
}

bool ProphecyManager::isProphecyTypeEnabled(ProphecyType type) {
    std::string key;
    switch(type) {
        case ProphecyType::GravityFlip: key = "prophecy-gravity-flip"; break;
        case ProphecyType::SpeedChange: key = "prophecy-speed-change"; break;
        case ProphecyType::SizeChange: key = "prophecy-size-change"; break;
        case ProphecyType::LevelComplete: key = "prophecy-level-complete"; break;
        case ProphecyType::SpikesRemoved: key = "prophecy-spikes-removed"; break;
        default: return true;
    }
    return Mod::get()->getSavedValue<bool>(key, true);
}

bool ProphecyManager::canTriggerProphecy() {
    if (m_prophecyPending || !m_prophecyFulfilled) return false;
    auto playLayer = PlayLayer::get();
    if (!playLayer || playLayer->m_hasCompletedLevel) return false;
    if (!Mod::get()->getSettingValue<bool>("enabled")) return false;
    return true;
}

std::string ProphecyManager::getRandomText(const std::vector<std::string>& texts) {
    if (texts.empty()) return "";
    std::uniform_int_distribution<size_t> dist(0, texts.size() - 1);
    return texts[dist(m_rng)];
}

Prophecy ProphecyManager::generateProphecy() {
    Prophecy prophecy;
    bool animeMode = Mod::get()->getSettingValue<bool>("anime-mode");
    bool canDoGood = true;
    
    std::vector<ProphecyType> availableTypes;
    
    if (isProphecyTypeEnabled(ProphecyType::GravityFlip)) availableTypes.push_back(ProphecyType::GravityFlip);
    if (isProphecyTypeEnabled(ProphecyType::SpeedChange)) availableTypes.push_back(ProphecyType::SpeedChange);
    if (isProphecyTypeEnabled(ProphecyType::SizeChange)) availableTypes.push_back(ProphecyType::SizeChange);
    
    if (isProphecyTypeEnabled(ProphecyType::LevelComplete)) availableTypes.push_back(ProphecyType::LevelComplete);
    if (isProphecyTypeEnabled(ProphecyType::SpikesRemoved)) availableTypes.push_back(ProphecyType::SpikesRemoved);
    
    if (m_hasTriggeredSpikes) {
        std::erase_if(availableTypes, [](ProphecyType t){ return t == ProphecyType::SpikesRemoved; });
    }
    
    if (availableTypes.empty()) {
        prophecy.type = ProphecyType::GravityFlip;
        prophecy.text = "";
        prophecy.delay = 5.0f;
        prophecy.displayTime = 0.f;
        prophecy.isNeutral = true;
        prophecy.isBad = false;
        return prophecy;
    }

    std::uniform_int_distribution<size_t> typeDist(0, availableTypes.size() - 1);
    prophecy.type = availableTypes[typeDist(m_rng)];
    prophecy.isNeutral = false;
    
    std::uniform_real_distribution<float> delayDist(1.5f, 4.0f);
    prophecy.delay = delayDist(m_rng);
    prophecy.displayTime = prophecy.delay + 0.5f;
    
    switch (prophecy.type) {
        case ProphecyType::GravityFlip:
            prophecy.isBad = true;
            prophecy.text = getRandomText(animeMode ? ProphecyTexts::ANIME_GRAVITY_FLIP : ProphecyTexts::GRAVITY_FLIP);
            break;
        case ProphecyType::SpeedChange:
            prophecy.isBad = true;
            prophecy.text = getRandomText(animeMode ? ProphecyTexts::ANIME_SPEED_CHANGE : ProphecyTexts::SPEED_CHANGE);
            break;
        case ProphecyType::SizeChange:
            prophecy.isBad = true;
            prophecy.text = getRandomText(animeMode ? ProphecyTexts::ANIME_SIZE_CHANGE : ProphecyTexts::SIZE_CHANGE);
            break;
        case ProphecyType::LevelComplete:
            {
                prophecy.isBad = false;
                std::uniform_real_distribution<float> longDelay(5.0f, 15.0f);
                prophecy.delay = longDelay(m_rng);
                prophecy.displayTime = prophecy.delay + 1.0f;
                prophecy.text = getRandomText(animeMode ? ProphecyTexts::ANIME_LEVEL_COMPLETE : ProphecyTexts::LEVEL_COMPLETE);
            }
            break;
        case ProphecyType::SpikesRemoved:
            prophecy.isBad = false;
            prophecy.text = getRandomText(animeMode ? ProphecyTexts::ANIME_SPIKES_REMOVED : ProphecyTexts::SPIKES_REMOVED);
            break;
    }
    
    return prophecy;
}

void ProphecyManager::triggerRandomProphecy() {
    if (!canTriggerProphecy()) return;
    
    Prophecy prophecy = generateProphecy();
    
    if (prophecy.text.empty()) {
        return;
    }
    
    BrodyOverlay::show(prophecy.text, prophecy.delay);
    
    if (prophecy.isNeutral) return;
    
    m_prophecyPending = true;
    m_prophecyFulfilled = false;
    m_pendingType = prophecy.type;
    m_pendingTimer = prophecy.delay;
}

void ProphecyManager::update(float dt) {
    if (!m_prophecyPending) return;
    
    auto playLayer = PlayLayer::get();
    if (playLayer && playLayer->m_hasCompletedLevel) {
        m_prophecyPending = false;
        m_prophecyFulfilled = true;
        BrodyOverlay::hide();
        return;
    }
    
    m_pendingTimer -= dt;
    if (m_pendingTimer <= 0.f) {
        fulfillProphecy();
    }
}
