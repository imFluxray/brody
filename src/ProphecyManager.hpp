#pragma once

#include <Geode/Geode.hpp>
#include <random>

using namespace geode::prelude;

enum class ProphecyType {
    GravityFlip,
    SpeedChange,
    SizeChange,
    LevelComplete,
    SpikesRemoved
};

struct Prophecy {
    ProphecyType type;
    std::string text;
    float delay;
    float displayTime;
    bool isBad;
    bool isNeutral = false;
};

class ProphecyManager {
private:
    static ProphecyManager* s_instance;
    
    std::mt19937 m_rng;
    bool m_prophecyPending;
    bool m_prophecyFulfilled;
    ProphecyType m_pendingType;
    float m_pendingTimer;
    
    bool m_areSpikesHarmless = false;
    bool m_hasTriggeredSpikes = false;
    
    ProphecyManager();
    
    std::string getRandomText(const std::vector<std::string>& texts);
    Prophecy generateProphecy();
    bool isProphecyTypeEnabled(ProphecyType type);
    bool canTriggerProphecy();
    
public:
    static ProphecyManager* get();
    
    void triggerRandomProphecy();
    void update(float dt);
    void fulfillProphecy();
    void reset();
    
    bool hasPendingProphecy() const { return m_prophecyPending; }
    bool isProphecyFulfilled() const { return m_prophecyFulfilled; }
    bool areSpikesHarmless() const { return m_areSpikesHarmless; }
};
