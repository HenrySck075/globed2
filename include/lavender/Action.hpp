#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

#include "ConstrainedLayout.hpp"

namespace ui {
    struct Action : public BaseInitializer<Action> {
        LAVENDER_ADD_ID();

        cocos2d::CCAction* action = nullptr;

        LAVENDER_ADD_CHILD();
    
        cocos2d::CCNode* construct(BuildContext* ctx) const {
            auto node = cocos2d::CCNode::create();

            auto c = utils::applyChild(this, node, ctx);
            utils::applySingleConstrainedLayout(this, node);

            utils::applyID(this, node);

            if (this->action) {
                c->runAction(this->action);
            }

            delete this;
            return node;
        }
    };
}
