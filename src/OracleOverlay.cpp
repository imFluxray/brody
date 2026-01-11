#include "OracleOverlay.hpp"
#include <fmt/format.h> 

using namespace geode::prelude;

static BrodyOverlay* s_instance = nullptr;

bool BrodyOverlay::init() {
    if (!CCLayer::init()) return false;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setPosition({winSize.width / 2, winSize.height - 50.f});
    this->setID("brody-overlay");
    
    // Background Container
    m_bg = CCScale9Sprite::create("GJ_square01.png");
    m_bg->setContentSize({350.f, 70.f}); 
    m_bg->setOpacity(255);
    // Anchor point 0.5, 0.5 is default for sprites, ensures scale anims occur from center
    this->addChild(m_bg);
    
    // Initial Setup - Placeholder
    m_avatar = CCSprite::create("fluxray.brody/brody.png");
    if (!m_avatar) m_avatar = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    m_avatar->setScale(0.22f); 
    auto elemSize = m_bg->getContentSize();
    
    // Position: X=45 (slightly more right), Y=Center (No offset)
    m_avatar->setPosition({45.f, elemSize.height / 2});
    m_avatar->setAnchorPoint({0.5f, 0.5f});
    m_bg->addChild(m_avatar, 5);
    
    m_title = CCLabelBMFont::create("BRODY", "goldFont.fnt");
    m_title->setScale(0.6f);
    m_title->setAnchorPoint({0, 0.5f});
    m_title->setPosition({85.f, elemSize.height / 2 + 12.f});
    m_bg->addChild(m_title, 5);
    
    m_prophecyLabel = CCLabelBMFont::create("Prophecy pending...", "bigFont.fnt");
    m_prophecyLabel->setScale(0.45f); 
    m_prophecyLabel->setAnchorPoint({0, 0.5f});
    m_prophecyLabel->setPosition({85.f, elemSize.height / 2 - 12.f});
    m_bg->addChild(m_prophecyLabel, 5);
    
    this->setVisible(false);
    this->scheduleUpdate();
    
    return true;
}

BrodyOverlay* BrodyOverlay::create() {
    auto ret = new BrodyOverlay();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void BrodyOverlay::onExit() {
    CCLayer::onExit();
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

void BrodyOverlay::show(const std::string& text, float delayTime) {
    if (!s_instance) {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (scene) {
            s_instance = BrodyOverlay::create();
            scene->addChild(s_instance, 1000); 
        }
    }
    
    if (s_instance) {
        bool anime = Mod::get()->getSettingValue<bool>("anime-mode");
        const char* img = anime ? "fluxray.brody/brody_anime.png" : "fluxray.brody/brody.png";
        
        auto tex = CCTextureCache::sharedTextureCache()->addImage(img, false);
        if (tex && s_instance->m_avatar) {
             s_instance->m_avatar->setTexture(tex);
             CCRect rect = CCRectZero;
             rect.size = tex->getContentSize();
             s_instance->m_avatar->setTextureRect(rect);
             
             // Dynamic Scale: Reduced Target height ~42px (was 55) for better fit
             float targetHeight = 42.f;
             float scale = targetHeight / rect.size.height;
             s_instance->m_avatar->setScale(scale);
        }

        if (s_instance->m_title) {
             s_instance->m_title->setString(anime ? "YUNA HOSHIMI" : "BRODY");
        }

        s_instance->m_textTemplate = text;
        s_instance->m_timeLeft = delayTime;
        s_instance->m_active = true;
        
        // VISIBILITY & ANIMATION "BOUNCE"
        s_instance->setVisible(true);
        s_instance->stopAllActions();
        s_instance->m_bg->stopAllActions();
        
        // Reset state
        s_instance->m_bg->setOpacity(255);
        s_instance->m_bg->setScale(0.0f); // HIDDEN START
        s_instance->m_avatar->setOpacity(255);
        if (s_instance->m_title) s_instance->m_title->setOpacity(255);
        if (s_instance->m_prophecyLabel) s_instance->m_prophecyLabel->setOpacity(255);
        
        // Elastic Entrance
        auto scaleUp = CCScaleTo::create(0.5f, 1.0f);
        auto elastic = CCEaseElasticOut::create(scaleUp, 0.6f);
        s_instance->m_bg->runAction(elastic);
        
        s_instance->updateLabel();
    }
}

void BrodyOverlay::hide() {
    if (s_instance && s_instance->isVisible()) {
        s_instance->stopAllActions();
        s_instance->m_bg->stopAllActions();
        
        // Exit Animation: Scale Down BackIn
        auto scaleDown = CCScaleTo::create(0.3f, 0.0f);
        auto easeExit = CCEaseBackIn::create(scaleDown);
        auto hideCall = CCCallFunc::create(s_instance, callfunc_selector(BrodyOverlay::disableVisibility));
        
        s_instance->m_bg->runAction(CCSequence::create(easeExit, hideCall, nullptr));
        
        s_instance->m_active = false;
    }
}

void BrodyOverlay::disableVisibility() {
    this->setVisible(false);
}

void BrodyOverlay::update(float dt) {
    if (!m_active) return;
    
    auto playLayer = PlayLayer::get();
    if (playLayer && playLayer->m_isPaused) {
        return; 
    }
    
    m_timeLeft -= dt;
    if (m_timeLeft < 0) m_timeLeft = 0;
    
    updateLabel();
    
    if (m_timeLeft <= 0) {
        hide();
    }
}

void BrodyOverlay::updateLabel() {
    if (m_prophecyLabel) {
        std::string timeStr = fmt::format("{:.1f}s", m_timeLeft);
        std::string finalStr = m_textTemplate;
        
        size_t pos = finalStr.find("{}");
        if (pos != std::string::npos) {
            finalStr.replace(pos, 2, timeStr);
        }
        else {
             pos = finalStr.find("{time}");
             if (pos != std::string::npos) {
                 finalStr.replace(pos, 6, timeStr);
             }
        }
        
        m_prophecyLabel->setString(finalStr.c_str());
    }
}
