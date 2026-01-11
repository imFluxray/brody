#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BrodyOverlay : public CCLayer {
protected:
    CCScale9Sprite* m_bg;
    CCSprite* m_avatar;
    CCLabelBMFont* m_title;
    CCLabelBMFont* m_prophecyLabel;
    
    std::string m_textTemplate;
    float m_timeLeft;
    bool m_active;
    
    bool init();
    void update(float dt) override;
    void updateLabel();
    void onExit() override; 
    
    void disableVisibility(); // Helper for callback
    
public:
    static BrodyOverlay* create();
    static void show(const std::string& text, float delayTime);
    static void hide();
    
    friend class ProphecyManager;
};
