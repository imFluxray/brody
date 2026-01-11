#pragma once

#include <Geode/Geode.hpp>
#include <map>
#include <string>

using namespace geode::prelude;

struct ProphecySettingsState {
    std::map<std::string, bool> toggles;
    double frequency;
};

// Reverted to CCLayer for Custom Overlay behavior
class BrodySettingsLayer : public CCLayer {
protected:
    CCMenu* m_buttonMenu = nullptr;
    CCScale9Sprite* m_bg = nullptr;
    CCLayer* m_mainLayer = nullptr;
    
    // State
    ProphecySettingsState m_pendingState;
    std::map<std::string, CCMenuItemToggler*> m_toggleNodes;
    CCLabelBMFont* m_frequencyLabel = nullptr;
    Slider* m_frequencySlider = nullptr;
    
    bool init() override;
    void onEnter() override;
    
    // Input Handling
    void registerWithTouchDispatcher() override;
    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    void ccTouchMoved(CCTouch* touch, CCEvent* event) override;
    void ccTouchEnded(CCTouch* touch, CCEvent* event) override;
    void ccTouchCancelled(CCTouch* touch, CCEvent* event) override;
    void keyBackClicked() override;
    
    // Creation Helpers
    void createToggleRow(const char* key, const char* label, float x, float y);
    void createPresetRow(float centerX, float y);
    void createFrequencyControl(float x, float y);
    void createActionButtons(float w, float h);
    void createCloseButton(float w, float h);
    
    // Callbacks
    void onToggle(CCObject* sender);
    void onPresetNoBad(CCObject*);
    void onPresetNoGood(CCObject*);
    void onPresetAll(CCObject*);
    void onPresetChaos(CCObject*);
    void onFrequencyChanged(CCObject*);
    
    void onApply(CCObject*);
    void onCancel(CCObject*);
    void onClose(CCObject*);
    
    void updateVisuals();
    void loadCurrentSettings();
    void saveSettings();
    
    void close();
    
public:
    static BrodySettingsLayer* create();
    static void show();
};
