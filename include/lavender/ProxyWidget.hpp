#pragma once

#include "Base.hpp"
#include "BuildContext.hpp"
#include "Utils.hpp"

namespace ui {
    struct ProxyWidget : public BaseInitializer<ProxyWidget> {
        LAVENDER_ADD_CHILD();

        cocos2d::CCNode* construct(BuildContext* ctx) const {
            return child->construct(ctx);
        };
    };
};
