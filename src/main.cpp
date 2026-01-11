#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>
#include "BrodySettingsLayer.hpp"

using namespace geode::prelude;

// Custom setting node for the "Open Prophecy Manager" button
class BrodyManagerSettingValue : public SettingV3 {
public:
    static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
        auto res = std::make_shared<BrodyManagerSettingValue>();
        auto root = checkJson(json, "BrodyManagerSettingValue");
        res->init(key, modID, root);
        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }
    
    bool load(matjson::Value const& json) override { return true; }
    bool save(matjson::Value& json) const override { return true; }
    bool isDefaultValue() const override { return true; }
    void reset() override {}
    
    SettingNodeV3* createNode(float width) override;
};

class BrodyManagerSettingNode : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<BrodyManagerSettingValue> setting, float width) {
        if (!SettingNodeV3::init(setting, width)) return false;
        
        // Create the button
        auto buttonSprite = ButtonSprite::create("Open Manager", 0, false, "goldFont.fnt", "GJ_button_01.png", 30, 0.8f);
        buttonSprite->setScale(0.6f); // Scale the sprite visibly
        
        auto button = CCMenuItemSpriteExtra::create(
            buttonSprite, this, menu_selector(BrodyManagerSettingNode::onOpenManager)
        );
        // button->setScale(1.0f); // Keep button at 1.0 to ensure correct bounce animation
        
        // Position in center
        this->getButtonMenu()->addChildAtPosition(button, Anchor::Center, {0, 0});
        this->setContentHeight(40);
        this->getButtonMenu()->updateLayout();
        
        return true;
    }
    
    void onOpenManager(CCObject*) {
        BrodySettingsLayer::show();
    }
    
    void onCommit() override {}
    void onResetToDefault() override {}
    bool hasUncommittedChanges() const override { return false; }
    bool hasNonDefaultValue() const override { return false; }
    
public:
    static BrodyManagerSettingNode* create(std::shared_ptr<BrodyManagerSettingValue> setting, float width) {
        auto ret = new BrodyManagerSettingNode();
        if (ret && ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

SettingNodeV3* BrodyManagerSettingValue::createNode(float width) {
    return BrodyManagerSettingNode::create(
        std::static_pointer_cast<BrodyManagerSettingValue>(shared_from_this()),
        width
    );
}

$on_mod(Loaded) {
    log::info("Brody loaded! Always correct...");
    
    // Register custom setting type
    (void)Mod::get()->registerCustomSettingType("brody-manager-button", &BrodyManagerSettingValue::parse);
}
