#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp> 

using namespace geode::prelude;

class $modify(BrodyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // REVERT: Create sprite, scale IT, pass to button. Button stays 1.0.
        // This avoids click-scale issues.
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
        if (!buttonSprite) return;
        
        buttonSprite->setScale(0.6f);
        
        auto button = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(BrodyPauseLayer::onBrodySettings)
        );
        if (!button) return;
        
        auto menu = CCMenu::create();
        menu->setPosition({38.f, winSize.height - 38.f}); 
        menu->addChild(button);
        this->addChild(menu, 100);
    }
    
    void onBrodySettings(CCObject*) {
        geode::openSettingsPopup(Mod::get());
    }
};
