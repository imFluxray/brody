#include "BrodyVisuals.hpp"

namespace BrodyVisuals {
    
    CCScale9Sprite* createStyledPanel(float width, float height) {
        auto panel = CCScale9Sprite::create("square02_001.png");
        if (!panel) return nullptr;
        
        panel->setContentSize({width, height});
        panel->setColor(PANEL_BG);
        panel->setOpacity(PANEL_OPACITY);
        
        // Add inner shadow/depth
        auto innerShadow = CCScale9Sprite::create("square02_001.png");
        if (innerShadow) {
            innerShadow->setContentSize({width - 4, height - 4});
            innerShadow->setColor({220, 210, 235});
            innerShadow->setOpacity(100);
            innerShadow->setPosition({width / 2, height / 2 - 1});
            panel->addChild(innerShadow, -1);
        }
        
        // Add purple border accent (top)
        auto borderTop = CCScale9Sprite::create("square02_001.png");
        if (borderTop) {
            borderTop->setContentSize({width - 10, BORDER_THICKNESS});
            borderTop->setColor(PANEL_BORDER);
            borderTop->setOpacity(BORDER_OPACITY);
            borderTop->setPosition({width / 2, height - BORDER_THICKNESS / 2 - 3});
            panel->addChild(borderTop, 5);
        }
        
        // Add purple border accent (bottom)
        auto borderBottom = CCScale9Sprite::create("square02_001.png");
        if (borderBottom) {
            borderBottom->setContentSize({width - 10, BORDER_THICKNESS});
            borderBottom->setColor(PANEL_BORDER);
            borderBottom->setOpacity(BORDER_OPACITY);
            borderBottom->setPosition({width / 2, BORDER_THICKNESS / 2 + 3});
            panel->addChild(borderBottom, 5);
        }
        
        return panel;
    }
    
    CCNode* createCharacterBox() {
        auto container = CCNode::create();
        if (!container) return nullptr;
        
        // Background for character
        auto bg = CCScale9Sprite::create("square02_001.png");
        if (bg) {
            bg->setContentSize({CHARACTER_BOX_SIZE, CHARACTER_BOX_SIZE});
            bg->setColor(CHARACTER_BG);
            bg->setOpacity(255);
            container->addChild(bg, 0);
        }
        
        // Try to load Brody sprite
        auto brodySprite = CCSprite::create("brody.png"_spr);
        if (brodySprite) {
            // Scale to fit in character box with padding
            float targetSize = CHARACTER_BOX_SIZE - 8;
            float scale = targetSize / std::max(brodySprite->getContentSize().width, brodySprite->getContentSize().height);
            brodySprite->setScale(scale);
            brodySprite->setPosition({0, 0});
            container->addChild(brodySprite, 1);
        } else {
            // Fallback: show text if image not found
            auto fallbackLabel = CCLabelBMFont::create("B", "goldFont.fnt");
            if (fallbackLabel) {
                fallbackLabel->setScale(0.8f);
                fallbackLabel->setColor(TEXT_SECONDARY);
                container->addChild(fallbackLabel, 1);
            }
        }
        
        return container;
    }
    
    CCSprite* createPulseIndicator() {
        auto indicator = CCSprite::create("square.png");
        if (!indicator) return nullptr;
        
        indicator->setScale(0.12f);
        indicator->setColor(URGENCY_PULSE);
        indicator->setOpacity(200);
        
        return indicator;
    }
    
    void styleLabel(CCLabelBMFont* label, bool isPrimary) {
        if (!label) return;
        
        if (isPrimary) {
            label->setColor(TEXT_PRIMARY);
        } else {
            label->setColor(TEXT_SECONDARY);
        }
    }
    
    void styleShadow(CCLabelBMFont* shadow) {
        if (!shadow) return;
        shadow->setColor({180, 170, 200});
        shadow->setOpacity(80);
    }
}
