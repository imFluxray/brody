#include "BrodySettingsLayer.hpp"
#include <fmt/format.h>

using namespace geode::prelude;

BrodySettingsLayer* BrodySettingsLayer::create() {
    auto ret = new BrodySettingsLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void BrodySettingsLayer::show() {
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene) return;
    auto layer = BrodySettingsLayer::create();
    scene->addChild(layer, 100000); // High Z-Order
}

bool BrodySettingsLayer::init() {
    if (!CCLayer::init()) return false;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    // Dim Background
    auto dim = CCLayerColor::create({0, 0, 0, 150});
    this->addChild(dim);
    
    loadCurrentSettings();
    
    m_mainLayer = CCLayer::create();
    m_mainLayer->setAnchorPoint({0.5f, 0.5f});
    m_mainLayer->setPosition(winSize / 2);
    m_mainLayer->ignoreAnchorPointForPosition(false);
    this->addChild(m_mainLayer);
    
    // Expanded Size for Clean Layout
    float w = 420.f;
    float h = 310.f; 
    m_mainLayer->setContentSize({w, h});
    
    // Background
    m_bg = CCScale9Sprite::create("GJ_square02.png");
    m_bg->setContentSize({w, h});
    m_bg->setPosition({w/2, h/2});
    m_mainLayer->addChild(m_bg);
    
    // Title
    auto title = CCLabelBMFont::create("Brody Settings", "goldFont.fnt");
    title->setPosition({w/2, h - 25.f});
    title->setScale(0.85f);
    m_mainLayer->addChild(title);
    
    m_buttonMenu = CCMenu::create();
    m_buttonMenu->setPosition({0, 0});
    m_mainLayer->addChild(m_buttonMenu, 10);
    
    // Clean Layout using calculated Y positions
    float currentY = h - 60.f;
    
    // 1. Presets (Top)
    auto presetLabel = CCLabelBMFont::create("- Presets -", "goldFont.fnt");
    presetLabel->setScale(0.5f);
    presetLabel->setPosition({w/2, currentY});
    m_mainLayer->addChild(presetLabel);
    
    currentY -= 35.f;
    createPresetRow(w/2, currentY);
    
    // 2. Frequency (Mid)
    currentY -= 45.f;
    createFrequencyControl(w/2, currentY);
    
    // 3. Toggles (2 Columns)
    currentY -= 40.f;
    float col1X = w * 0.25f + 15.f;
    float col2X = w * 0.75f - 15.f;
    float startTogglesY = currentY;
    float gap = 35.f; // Generous gap
    
    createToggleRow("prophecy-gravity-flip", "Gravity Flip", col1X, startTogglesY);
    createToggleRow("prophecy-speed-change", "Speed Change", col1X, startTogglesY - gap);
    createToggleRow("prophecy-size-change", "Size Change", col1X, startTogglesY - gap * 2);
    
    createToggleRow("prophecy-level-complete", "Level Complete", col2X, startTogglesY);
    createToggleRow("prophecy-spikes-removed", "Spikes Removed", col2X, startTogglesY - gap);
    
    createActionButtons(w, h);
    createCloseButton(w, h); // Standard X button
    
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    
    return true;
}

void BrodySettingsLayer::loadCurrentSettings() {
    auto mod = Mod::get();
    m_pendingState.frequency = mod->getSavedValue<double>("frequency", 8.0);
    
    std::vector<std::string> keys = {
        "prophecy-gravity-flip", "prophecy-speed-change", "prophecy-size-change",
        "prophecy-level-complete", "prophecy-spikes-removed"
    };
    
    for (const auto& key : keys) {
        m_pendingState.toggles[key] = mod->getSavedValue<bool>(key, true);
    }
}

void BrodySettingsLayer::saveSettings() {
    auto mod = Mod::get();
    mod->setSavedValue("frequency", m_pendingState.frequency);
    
    for (const auto& [key, val] : m_pendingState.toggles) {
        mod->setSavedValue(key, val);
    }
    
    close();
}

void BrodySettingsLayer::createActionButtons(float w, float h) {
    auto applySpr = ButtonSprite::create("Apply", 60, true, "goldFont.fnt", "GJ_button_01.png", 30, 0.8f);
    auto applyBtn = CCMenuItemSpriteExtra::create(applySpr, this, menu_selector(BrodySettingsLayer::onApply));
    applyBtn->setPosition({w/2 + 80.f, 30.f});
    m_buttonMenu->addChild(applyBtn);
    
    auto cancelSpr = ButtonSprite::create("Cancel", 60, true, "goldFont.fnt", "GJ_button_06.png", 30, 0.8f); 
    auto cancelBtn = CCMenuItemSpriteExtra::create(cancelSpr, this, menu_selector(BrodySettingsLayer::onCancel));
    cancelBtn->setPosition({w/2 - 80.f, 30.f});
    m_buttonMenu->addChild(cancelBtn);
}

void BrodySettingsLayer::createCloseButton(float w, float h) {
    auto spr = ButtonSprite::create("X", 20, true, "goldFont.fnt", "GJ_button_06.png", 30, 0.7f);
    // Actually standard close button uses GJ_closeBtn_001.png?
    // Let's use standard sprite.
    auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    closeSpr->setScale(0.8f);
    
    auto btn = CCMenuItemSpriteExtra::create(closeSpr, this, menu_selector(BrodySettingsLayer::onCancel));
    btn->setPosition({-15.f, h - 15.f}); // Absolute top left of layer? No, relative to menu 0,0 (bottom left).
    // Let's put it Top Left corner of the BOX.
    btn->setPosition({18.f, h - 18.f});
    m_buttonMenu->addChild(btn);
}

void BrodySettingsLayer::createPresetRow(float centerX, float y) {
    float startX = centerX - 120.f; // Spread out
    float gap = 80.f;
    
    auto mkBtn = [&](const char* txt, const char* img, SEL_MenuHandler cb, float px) {
        auto spr = ButtonSprite::create(txt, 40, true, "goldFont.fnt", img, 30, 0.6f);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, cb);
        btn->setPosition({px, y});
        m_buttonMenu->addChild(btn);
    };
    
    mkBtn("No Bad", "GJ_button_02.png", menu_selector(BrodySettingsLayer::onPresetNoBad), startX);
    mkBtn("No Good", "GJ_button_03.png", menu_selector(BrodySettingsLayer::onPresetNoGood), startX + gap);
    mkBtn("All", "GJ_button_01.png", menu_selector(BrodySettingsLayer::onPresetAll), startX + gap * 2);
    mkBtn("Chaos", "GJ_button_06.png", menu_selector(BrodySettingsLayer::onPresetChaos), startX + gap * 3);
}

void BrodySettingsLayer::createFrequencyControl(float x, float y) {
    m_frequencySlider = Slider::create(this, menu_selector(BrodySettingsLayer::onFrequencyChanged), 0.7f); // Slightly larger
    m_frequencySlider->setPosition({x, y});
    
    double val = m_pendingState.frequency;
    float sliderVal = static_cast<float>((val - 3.0) / 27.0);
    if (sliderVal < 0) sliderVal = 0;
    if (sliderVal > 1) sliderVal = 1;
    m_frequencySlider->setValue(sliderVal);
    
    m_buttonMenu->addChild(m_frequencySlider);
    
    m_frequencyLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_frequencyLabel->setScale(0.45f);
    m_frequencyLabel->setPosition({x, y + 22.f});
    m_mainLayer->addChild(m_frequencyLabel);
    
    m_frequencyLabel->setString(fmt::format("Frequency: {:.1f}s", val).c_str());
}

void BrodySettingsLayer::onFrequencyChanged(CCObject*) {
    if (!m_frequencySlider) return;
    float val = m_frequencySlider->getValue(); 
    double freq = 3.0 + (val * 27.0); 
    freq = round(freq * 2.0) / 2.0;
    
    m_pendingState.frequency = freq;
    
    if (m_frequencyLabel) {
        m_frequencyLabel->setString(fmt::format("Frequency: {:.1f}s", freq).c_str());
    }
}

void BrodySettingsLayer::createToggleRow(const char* key, const char* label, float x, float y) {
    // 2 Column Layout - Label Left, Toggle Right
    auto labelNode = CCLabelBMFont::create(label, "bigFont.fnt");
    labelNode->setScale(0.5f); // Slightly larger font
    labelNode->setAnchorPoint({1.0f, 0.5f}); 
    labelNode->setPosition({x + 40.f, y});
    m_mainLayer->addChild(labelNode);
    
    bool toggled = m_pendingState.toggles[key];
    
    auto toggle = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(BrodySettingsLayer::onToggle), 0.7f
    );
    toggle->toggle(toggled);
    toggle->setUserData((void*)key);
    toggle->setPosition({x + 65.f, y}); 
    m_buttonMenu->addChild(toggle);
    
    m_toggleNodes[key] = toggle;
}

void BrodySettingsLayer::onToggle(CCObject* sender) {
    auto toggle = static_cast<CCMenuItemToggler*>(sender);
    if (!toggle) return;
    const char* key = (const char*)toggle->getUserData();
    if (key) {
        m_pendingState.toggles[key] = !toggle->isToggled();
    }
}

void BrodySettingsLayer::updateVisuals() {
    for (auto& [key, toggle] : m_toggleNodes) {
        if (toggle) toggle->toggle(m_pendingState.toggles[key]);
    }
    
    if (m_frequencySlider) {
        double val = m_pendingState.frequency;
        float sliderVal = static_cast<float>((val - 3.0) / 27.0);
        m_frequencySlider->setValue(sliderVal);
        if (m_frequencyLabel) {
            m_frequencyLabel->setString(fmt::format("Frequency: {:.1f}s", val).c_str());
        }
    }
}

void BrodySettingsLayer::onPresetNoBad(CCObject*) {
    m_pendingState.toggles["prophecy-gravity-flip"] = false;
    m_pendingState.toggles["prophecy-speed-change"] = false;
    m_pendingState.toggles["prophecy-size-change"] = false;
    m_pendingState.toggles["prophecy-level-complete"] = true;
    m_pendingState.toggles["prophecy-spikes-removed"] = true;
    updateVisuals();
}

void BrodySettingsLayer::onPresetNoGood(CCObject*) {
    m_pendingState.toggles["prophecy-gravity-flip"] = true;
    m_pendingState.toggles["prophecy-speed-change"] = true;
    m_pendingState.toggles["prophecy-size-change"] = true;
    m_pendingState.toggles["prophecy-level-complete"] = false;
    m_pendingState.toggles["prophecy-spikes-removed"] = false;
    updateVisuals();
}

void BrodySettingsLayer::onPresetAll(CCObject*) {
    for (auto& [key, val] : m_pendingState.toggles) val = true;
    updateVisuals();
}

void BrodySettingsLayer::onPresetChaos(CCObject*) {
    for (auto& [key, val] : m_pendingState.toggles) val = true;
    m_pendingState.frequency = 3.0; // Force 3s
    updateVisuals();
}

void BrodySettingsLayer::onApply(CCObject*) {
    saveSettings();
}

void BrodySettingsLayer::onCancel(CCObject*) {
    close();
}

void BrodySettingsLayer::onClose(CCObject*) {
    close();
}

void BrodySettingsLayer::onEnter() {
    CCLayer::onEnter();
    m_mainLayer->setScale(0.0f);
    m_mainLayer->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1.0f), 0.6f));
}

void BrodySettingsLayer::registerWithTouchDispatcher() { CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true); }
bool BrodySettingsLayer::ccTouchBegan(CCTouch*, CCEvent*) { return true; }
void BrodySettingsLayer::ccTouchMoved(CCTouch*, CCEvent*) {}
void BrodySettingsLayer::ccTouchEnded(CCTouch*, CCEvent*) {}
void BrodySettingsLayer::ccTouchCancelled(CCTouch*, CCEvent*) {}
void BrodySettingsLayer::keyBackClicked() { this->close(); }

void BrodySettingsLayer::close() {
    auto scale = CCScaleTo::create(0.2f, 0.0f);
    auto ease = CCEaseBackIn::create(scale);
    auto remove = CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParent));
    m_mainLayer->runAction(CCSequence::create(ease, remove, nullptr));
}
