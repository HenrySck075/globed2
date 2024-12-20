#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

#include "Axis.hpp"

namespace ui {
    namespace impl {
        class ScrollLayerLayout : public geode::Layout {
        public:
            Axis m_axis;

            static ScrollLayerLayout* create(Axis axis) {
                auto ret = new (std::nothrow) ScrollLayerLayout();
                if (ret) {
                    ret->m_axis = axis;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto [minSize, maxSize] = utils::getConstraints(in);
                auto emulatedMinSize = minSize;
                auto emulatedMaxSize = maxSize;
                switch (m_axis) {
                    case Axis::Horizontal:
                        emulatedMinSize.width = maxSize.width;
                        emulatedMaxSize.width = FLT_MAX;
                        break;
                    case Axis::Vertical:
                        emulatedMinSize.height = maxSize.height;
                        emulatedMaxSize.height = FLT_MAX;
                        break;
                }

                if (auto scrollLayer = geode::cast::typeinfo_cast<geode::ScrollLayer*>(in)) {
                    auto child = scrollLayer->m_contentLayer;

                    utils::setConstraints(child, emulatedMinSize, emulatedMaxSize);
                    child->updateLayout();

                    switch (m_axis) {
                        case Axis::Horizontal:
                            in->setContentHeight(std::max(child->getContentHeight(), minSize.height));
                            in->setContentWidth(maxSize.width);
                            child->setPositionY(0.f);
                            break;
                        case Axis::Vertical:
                            in->setContentWidth(std::max(child->getContentWidth(), minSize.width));
                            in->setContentHeight(maxSize.height);
                            child->setPositionX(0.f);
                            break;
                    }

                    scrollLayer->scrollToTop();
                }
            }

            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };
    }

    struct ScrollLayer : public BaseInitializer<ScrollLayer> {
        LAVENDER_ADD_ID();

        Axis axis = Axis::Vertical;

        VerticalDirection verticalDirection = VerticalDirection::TopToBottom;
        HorizontalDirection horizontalDirection = HorizontalDirection::LeftToRight;

        LAVENDER_ADD_CHILDREN();
        LAVENDER_ADD_CHILDREN_BUILDER();
    
        cocos2d::CCNode* construct(BuildContext* ctx) const {
            auto node = geode::ScrollLayer::create(ccp(0, 0), true, this->axis == Axis::Vertical);

            if (!utils::applyChildrenBuilder(this, node->m_contentLayer, ctx)) {
                (void)utils::applyChildren(this, node->m_contentLayer, ctx);
            }

            node->setLayout(impl::ScrollLayerLayout::create(this->axis));

            utils::applyID(this, node);

            if (this->axis == Axis::Vertical) {
                node->m_contentLayer->setLayout(impl::ColumnLayout::create(
                    MainAxisAlignment::Start, CrossAxisAlignment::Stretch, verticalDirection
                ));
            }
            else {
                node->m_contentLayer->setLayout(impl::RowLayout::create(
                    MainAxisAlignment::Start, CrossAxisAlignment::Stretch, horizontalDirection
                ));
            }

            delete this;
            return node;
        }
    };
}
