#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include "../ProphecyManager.hpp"

using namespace geode::prelude;

class $modify(ProphecyGameObject, GameObject) {
    void setVisible(bool visible) {
        if (this->m_objectType == GameObjectType::Hazard) {
            if (ProphecyManager::get()->areSpikesHarmless()) {
                visible = false;
            }
        }
        GameObject::setVisible(visible);
    }
    
    void setPosition(const CCPoint& pos) {
        CCPoint startPos = pos;
        if (this->m_objectType == GameObjectType::Hazard) {
            if (ProphecyManager::get()->areSpikesHarmless()) {
                startPos = ccp(-99999.f, -99999.f);
            }
        }
        GameObject::setPosition(startPos);
    }

    void draw() {
        if (this->m_objectType == GameObjectType::Hazard) {
            if (ProphecyManager::get()->areSpikesHarmless()) {
                return;
            }
        }
        GameObject::draw();
    }
};
