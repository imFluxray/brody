#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Centralized visual styling for Brody overlay
namespace BrodyVisuals {
    // === COLOR SCHEME (Bright theme) ===
    constexpr ccColor3B PANEL_BG = {245, 240, 255};      // Light lavender
    constexpr ccColor3B PANEL_BORDER = {180, 100, 255};  // Purple accent
    constexpr ccColor3B CHARACTER_BG = {230, 220, 245};  // Slightly darker lavender
    constexpr ccColor3B TEXT_PRIMARY = {30, 25, 40};     // Near black
    constexpr ccColor3B TEXT_SECONDARY = {80, 60, 120};  // Dark purple
    constexpr ccColor3B ACCENT_GLOW = {200, 120, 255};   // Bright purple
    constexpr ccColor3B URGENCY_PULSE = {255, 100, 150}; // Pink-magenta
    
    // === SIZING ===
    constexpr float PANEL_WIDTH = 400.f;
    constexpr float PANEL_HEIGHT = 75.f;
    constexpr float CHARACTER_BOX_SIZE = 65.f;
    constexpr float BORDER_THICKNESS = 3.f;
    constexpr float CORNER_RADIUS = 12.f;
    
    // === ANIMATIONS ===
    constexpr float SLIDE_IN_TIME = 0.4f;
    constexpr float SLIDE_OUT_TIME = 0.3f;
    constexpr float PULSE_SPEED = 3.5f;
    
    // === OPACITY ===
    constexpr GLubyte PANEL_OPACITY = 245;
    constexpr GLubyte BORDER_OPACITY = 220;
    constexpr GLubyte SHADOW_OPACITY = 60;
    
    // === POSITIONING ===
    constexpr float TOP_OFFSET = 60.f;  // Distance from top of screen
    
    // Create styled panel with border
    CCScale9Sprite* createStyledPanel(float width, float height);
    
    // Create character box with Brody sprite
    CCNode* createCharacterBox();
    
    // Create urgency pulse indicator
    CCSprite* createPulseIndicator();
    
    // Apply text style
    void styleLabel(CCLabelBMFont* label, bool isPrimary = true);
    void styleShadow(CCLabelBMFont* shadow);
}
