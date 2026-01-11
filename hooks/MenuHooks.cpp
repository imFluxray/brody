#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../BrodySettingsLayer.hpp"

using namespace geode::prelude;

class $modify(BrodyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        return true;
    }
};
