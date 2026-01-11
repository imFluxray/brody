#include "ProphecySettingsLayer.hpp"
#include "BrodyVisuals.hpp"

bool ProphecySettingsLayer::setup() {
    this->setTitle("Prophecy Settings");
    
    auto contentSize = m_mainLayer->getContentSize();
    
    // === PROPHECY TOGGLES SECTION ===
    auto toggleTitle = CCLabelBMFont::create("Prophecy Types", "bigFont.fnt");
    if (!toggleTitle) return false;
    toggleTitle->setScale(0.5f);
    toggleTitle->setPosition({contentSize.width / 2, contentSize.height - 50});
    m_mainLayer->addChild(toggleTitle);
    
    m_toggleMenu = CCMenu::create();
    if (!m_toggleMenu) return false;
    m_toggleMenu->setPosition({0, 0});
    m_mainLayer->addChild(m_toggleMenu);
    
    // Individual prophecy toggles
    float startY = contentSize.height - 80;
    createProphecyToggle("prophecy-gravity-flip", "Gravity Flip", startY);
    createProphecyToggle("prophecy-speed-change", "Speed Change", startY - 28);
    createProphecyToggle("prophecy-size-change", "Size Change", startY - 56);
    createProphecyToggle("prophecy-level-complete", "Level Complete", startY - 84);
    createProphecyToggle("prophecy-spikes-removed", "Spikes Removed", startY - 112);
    
    // === PRESETS SECTION ===
    auto presetTitle = CCLabelBMFont::create("Presets", "bigFont.fnt");
    if (!presetTitle) return false;
    presetTitle->setScale(0.4f);
    presetTitle->setPosition({contentSize.width / 2, startY - 145});
    m_mainLayer->addChild(presetTitle);
    
    m_presetMenu = CCMenu::create();
    if (!m_presetMenu) return false;
    m_presetMenu->setPosition({0, 0});
    m_mainLayer->addChild(m_presetMenu);
    
    float presetY = startY - 175;
    float spacing = 80.f;
    float startX = contentSize.width / 2 - spacing * 1.5f;
    
    // Preset buttons - use simpler approach
    auto btn1 = ButtonSprite::create("No Bad", "goldFont.fnt", "GJ_button_01.png", 0.6f);
    if (btn1) {
        auto item1 = CCMenuItemSpriteExtra::create(btn1, this, menu_selector(ProphecySettingsLayer::onDisableBad));
        if (item1) {
            item1->setPosition({startX, presetY});
            item1->setScale(0.75f);
            m_presetMenu->addChild(item1);
        }
    }
    
    auto btn2 = ButtonSprite::create("No Good", "goldFont.fnt", "GJ_button_01.png", 0.6f);
    if (btn2) {
        auto item2 = CCMenuItemSpriteExtra::create(btn2, this, menu_selector(ProphecySettingsLayer::onDisableGood));
        if (item2) {
            item2->setPosition({startX + spacing, presetY});
            item2->setScale(0.75f);
            m_presetMenu->addChild(item2);
        }
    }
    
    auto btn3 = ButtonSprite::create("All On", "goldFont.fnt", "GJ_button_01.png", 0.6f);
    if (btn3) {
        auto item3 = CCMenuItemSpriteExtra::create(btn3, this, menu_selector(ProphecySettingsLayer::onEnableAll));
        if (item3) {
            item3->setPosition({startX + spacing * 2, presetY});
            item3->setScale(0.75f);
            m_presetMenu->addChild(item3);
        }
    }
    
    auto btn4 = ButtonSprite::create("Chaos!", "goldFont.fnt", "GJ_button_06.png", 0.6f);
    if (btn4) {
        auto item4 = CCMenuItemSpriteExtra::create(btn4, this, menu_selector(ProphecySettingsLayer::onChaosMode));
        if (item4) {
            item4->setPosition({startX + spacing * 3, presetY});
            item4->setScale(0.75f);
            m_presetMenu->addChild(item4);
        }
    }
    
    return true;
}

void ProphecySettingsLayer::createProphecyToggle(const char* key, const char* label, float yPos) {
    auto contentSize = m_mainLayer->getContentSize();
    
    // Label
    auto labelNode = CCLabelBMFont::create(label, "bigFont.fnt");
    if (!labelNode) return;
    labelNode->setScale(0.35f);
    labelNode->setAnchorPoint({0, 0.5f});
    labelNode->setPosition({contentSize.width / 2 - 100, yPos});
    m_mainLayer->addChild(labelNode);
    
    // Toggle using simple on/off sprites
    bool currentValue = Mod::get()->getSavedValue<bool>(key, true);
    
    auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    
    if (!toggleOff || !toggleOn) return;
    
    // Create a simple button that toggles appearance
    auto toggle = CCMenuItemToggler::create(
        toggleOff, toggleOn, this,
        menu_selector(ProphecySettingsLayer::onToggle)
    );
    if (!toggle) return;
    
    toggle->setScale(0.7f);
    toggle->toggle(currentValue);
    toggle->setPosition({contentSize.width / 2 + 100, yPos});
    
    // Store key in user data
    toggle->setUserData((void*)key);
    
    m_toggleMenu->addChild(toggle);
}

void ProphecySettingsLayer::onToggle(CCObject* sender) {
    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    if (!toggler) return;
    
    const char* key = static_cast<const char*>(toggler->getUserData());
    if (!key) return;
    
    bool newValue = !toggler->isToggled();
    Mod::get()->setSavedValue<bool>(key, newValue);
    log::info("[Prophecy] {} set to {}", key, newValue);
}

void ProphecySettingsLayer::createPresetButton(const char* label, std::function<void()> callback, float xPos, float yPos) {
    // Not used - implemented inline
}

void ProphecySettingsLayer::onDisableBad(CCObject*) {
    Mod::get()->setSavedValue<bool>("prophecy-gravity-flip", false);
    Mod::get()->setSavedValue<bool>("prophecy-speed-change", false);
    Mod::get()->setSavedValue<bool>("prophecy-size-change", false);
    Mod::get()->setSavedValue<bool>("prophecy-level-complete", true);
    Mod::get()->setSavedValue<bool>("prophecy-spikes-removed", true);
    
    FLAlertLayer::create("Done", "Bad prophecies disabled!", "OK")->show();
    this->onClose(nullptr);
}

void ProphecySettingsLayer::onDisableGood(CCObject*) {
    Mod::get()->setSavedValue<bool>("prophecy-gravity-flip", true);
    Mod::get()->setSavedValue<bool>("prophecy-speed-change", true);
    Mod::get()->setSavedValue<bool>("prophecy-size-change", true);
    Mod::get()->setSavedValue<bool>("prophecy-level-complete", false);
    Mod::get()->setSavedValue<bool>("prophecy-spikes-removed", false);
    
    FLAlertLayer::create("Done", "Good prophecies disabled!", "OK")->show();
    this->onClose(nullptr);
}

void ProphecySettingsLayer::onEnableAll(CCObject*) {
    Mod::get()->setSavedValue<bool>("prophecy-gravity-flip", true);
    Mod::get()->setSavedValue<bool>("prophecy-speed-change", true);
    Mod::get()->setSavedValue<bool>("prophecy-size-change", true);
    Mod::get()->setSavedValue<bool>("prophecy-level-complete", true);
    Mod::get()->setSavedValue<bool>("prophecy-spikes-removed", true);
    
    FLAlertLayer::create("Done", "All prophecies enabled!", "OK")->show();
    this->onClose(nullptr);
}

void ProphecySettingsLayer::onChaosMode(CCObject*) {
    Mod::get()->setSavedValue<bool>("prophecy-gravity-flip", true);
    Mod::get()->setSavedValue<bool>("prophecy-speed-change", true);
    Mod::get()->setSavedValue<bool>("prophecy-size-change", true);
    Mod::get()->setSavedValue<bool>("prophecy-level-complete", true);
    Mod::get()->setSavedValue<bool>("prophecy-spikes-removed", true);
    Mod::get()->setSettingValue<double>("frequency", 3.0);
    
    FLAlertLayer::create("CHAOS!", "All on + 3 sec frequency!", "OK")->show();
    this->onClose(nullptr);
}

ProphecySettingsLayer* ProphecySettingsLayer::create() {
    auto ret = new ProphecySettingsLayer();
    if (ret && ret->initAnchored(350, 280)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void ProphecySettingsLayer::show() {
    auto layer = ProphecySettingsLayer::create();
    if (layer) {
        layer->show();
    }
}
