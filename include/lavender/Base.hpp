#pragma once

#include <Geode/Geode.hpp>
#include "BuildContext.hpp"

namespace ui {
    using ConstructorType = cocos2d::CCNode*(*)(void const*, BuildContext*);

    struct Base {
    private:
        BuildContext* newContext(BuildContext* parent) {
            auto ret = new BuildContext();
            ret->parentContext = parent;
            ret->widget = this;
            return ret;
        }
    public:
        ConstructorType constructor;

        cocos2d::CCNode* construct(BuildContext* ctx) const {
            return this->constructor(this, ctx);
        }

        cocos2d::CCNode* get(BuildContext* ctx) {
            auto ret = this->constructor(this, ctx);
            ret->updateLayout();
            return ret;
        }
    };

    template <class Parent>
    struct BaseInitializer : public Base {
        BaseInitializer() {
            this->constructor = [](void const* ptr, BuildContext* ctx) {
                auto p = static_cast<Parent const*>(ptr);
                auto nctx = p->newContext(ctx);
                nctx->ref();
                auto ret = p->construct(nctx);
                nctx->unref();
                return ret;
            };
        }
    };
}
