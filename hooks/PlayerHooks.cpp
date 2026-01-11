#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

// Player hooks are minimal since most prophecy effects are triggered
// directly through ProphecyManager::fulfillProphecy()
// This file exists for potential future expansion

class $modify(ProphecyPlayerObject, PlayerObject) {
    // Reserved for future player-specific prophecy effects
    // Currently, all effects are triggered via ProphecyManager
};
