#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "ConstrainedLayout.hpp"

namespace ui {
    namespace impl {
        class Expanded : public cocos2d::CCNode {
        public:
            size_t m_flex;
            Axis m_mainAxis;

            static Expanded* create(size_t flex) {
                auto ret = new (std::nothrow) Expanded();
                if (ret && ret->init(flex)) {
                    ret->autorelease();
                    return ret;
                }
                delete ret;
                return nullptr;
            }
            
            bool init(size_t flex) {
                if (!cocos2d::CCNode::init()) {
                    return false;
                }
                
                m_flex = flex;
                return true;
            }

            virtual size_t getABIVersion() const {
                return 1;
            }

            virtual size_t getFlex() const {
                return m_flex;
            }

            virtual Axis getMainAxis() const {
                return m_mainAxis;
            }

            virtual void setMainAxis(Axis axis) {
                m_mainAxis = axis;
            }
        };

        class ExpandedLayout : public geode::Layout {
        public:
            size_t m_flex;

            static ExpandedLayout* create(size_t flex) {
                auto ret = new (std::nothrow) ExpandedLayout();
                if (ret) {
                    ret->m_flex = flex;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto [minSize, maxSize] = utils::getConstraints(in);
                auto expanded = geode::cast::typeinfo_cast<Expanded*>(in); 
                if (expanded && expanded->getABIVersion() >= 1) {
                    switch (expanded->getMainAxis()) {
                        case Axis::Horizontal:
                            minSize.width = maxSize.width;
                            break;
                        case Axis::Vertical:
                            minSize.height = maxSize.height;
                            break;
                    }
                }
                if (auto child = utils::getChild(in)) {
                    utils::setConstraints(child, minSize, maxSize);
                    child->updateLayout();

                    in->setContentSize(child->getContentSize());

                    child->ignoreAnchorPointForPosition(false);
                    child->setPosition(in->getContentSize() / 2.f);
                    child->setAnchorPoint(ccp(0.5f, 0.5f));
                }
                else {
                    in->setContentSize(minSize);
                }
            }

            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };
    }

    struct Expanded : BaseInitializer<Expanded> {
        LAVENDER_ADD_ID();

        size_t flex = 1;

        LAVENDER_ADD_CHILD();

        cocos2d::CCNode* construct(BuildContext* ctx) const {
            if (this->flex < 1) {
                delete this;
                return nullptr;
            }

            auto node = impl::Expanded::create(this->flex);

            (void)utils::applyChild(this, node, ctx);
            node->setLayout(impl::ExpandedLayout::create(this->flex));

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}
