#pragma once

#include <Geode/Geode.hpp>
#include "ConstrainedObject.hpp"

namespace ui::utils {
    #define LAVENDER_ADD_ID() \
        std::optional<std::string> id; \
        cocos2d::CCNode** store = nullptr
    
    inline void applyID(auto const* data, cocos2d::CCNode* node) {
        if (data->id.has_value()) {
            node->setID(data->id.value().c_str());
        }
        if (data->store) {
            *data->store = node;
        }
    }

    #define LAVENDER_ADD_CHILDREN() \
        std::vector<Base*> children

    inline bool applyChildren(auto const* data, cocos2d::CCNode* node) {
        for (auto child : data->children) {
            if (child == nullptr) continue;
            auto childNode = child->construct();
            node->addChild(childNode);
        }
        return data->children.size() > 0;
    }

    #define LAVENDER_ADD_CHILDREN_BUILDER() \
        size_t count = 0; \
        std::function<Base*(size_t)> builder
        

    inline bool applyChildrenBuilder(auto const* data, cocos2d::CCNode* node) {
        if (data->builder) {
            for (size_t i = 0; i < data->count; i++) {
                auto child = data->builder(i);
                auto childNode = child->construct();
                node->addChild(childNode);
            }
            return true;
        }
        return false;
    }

    #define LAVENDER_ADD_CHILD() \
        Base* child

    inline cocos2d::CCNode* applyChild(auto const* data, cocos2d::CCNode* node) {
        if (data->child != nullptr) {
            auto childNode = data->child->construct();
            node->addChild(childNode);
            return childNode;
        }
        return nullptr;
    }

    #define LAVENDER_ADD_SIZE() \
        std::optional<cocos2d::CCSize> size; \
        std::optional<float> width; \
        std::optional<float> height

    inline void applySize(auto const* data, cocos2d::CCNode* node) {
        if (data->width.has_value()) {
            node->setContentWidth(data->width.value());
        }
        if (data->height.has_value()) {
            node->setContentHeight(data->height.value());
        }
        if (data->size.has_value()) {
            node->setContentSize(data->size.value());
        }
    }

    #define LAVENDER_ADD_SCALE() \
        std::optional<float> scaleX; \
        std::optional<float> scaleY; \
        std::optional<float> scale

    inline void applyScale(auto const* data, cocos2d::CCNode* node) {
        if (data->scaleX.has_value()) {
            node->setScaleX(data->scaleX.value());
        }
        if (data->scaleY.has_value()) {
            node->setScaleY(data->scaleY.value());
        }
        if (data->scale.has_value()) {
            node->setScale(data->scale.value());
        }
    }

    #define LAVENDER_ADD_ROTATION() \
        std::optional<float> rotation

    inline void applyRotation(auto const* data, cocos2d::CCNode* node) {
        if (data->rotation.has_value()) {
            node->setRotation(data->rotation.value());
        }
    }

    inline cocos2d::CCNode* generateWrapper(cocos2d::CCNode* node) {
        auto wrapper = cocos2d::CCNode::create();
        wrapper->addChild(node);
        wrapper->setContentSize(node->boundingBox().size);
        wrapper->setAnchorPoint(cocos2d::CCPoint { 0.5, 0.5 });
        node->setPosition(cocos2d::CCPoint {
            wrapper->getContentSize().width / 2,
            wrapper->getContentSize().height / 2
        });
        return wrapper;
    }

    #define LAVENDER_ADD_COLOR() \
        std::optional<Color3B> color

    #define LAVENDER_ADD_COLOR4() \
        std::optional<Color4B> color

    inline void applyColor(auto const* data, cocos2d::CCNodeRGBA* node) {
        if (data->color.has_value()) {
            node->setColor(data->color.value());
        }
    }

    inline void applyColor4(auto const* data, cocos2d::CCNodeRGBA* node) {
        if (data->color.has_value()) {
            node->setColor(cocos2d::ccc3(data->color->r, data->color->g, data->color->b));
            node->setOpacity(data->color->a);
        }
    }

    inline std::pair<cocos2d::CCSize, cocos2d::CCSize> getConstraints(cocos2d::CCNode* node) {
        auto const constrain = geode::cast::typeinfo_cast<impl::ConstrainedObject*>(node->getUserObject("constrain"_spr));
        if (constrain && constrain->getABIVersion() >= 1) {
            return { constrain->getMinSize(), constrain->getMaxSize() };
        }
        return { cocos2d::CCSize{ 0.f, 0.f }, cocos2d::CCSize{ FLT_MAX, FLT_MAX } };
    }

    inline void setConstraints(cocos2d::CCNode* child, cocos2d::CCSize minSize, cocos2d::CCSize maxSize) {
        auto constrain = impl::ConstrainedObject::create(minSize, maxSize);
        child->setUserObject("constrain"_spr, constrain);
    }

    inline cocos2d::CCNode* getChild(cocos2d::CCNode* node) {
        auto children = node->getChildren();
        if (!children || children->count() != 1) return nullptr;
        return static_cast<cocos2d::CCNode*>(children->objectAtIndex(0));
    }
}

namespace ui {
    enum class Axis : int {
        Horizontal,
        Vertical
    };

    enum class MainAxisAlignment {
        // Align items to the start
        // |ooo......|
        Start,
        // All items are centered
        // |...ooo...|
        Center,
        // Align items to the end
        // |......ooo|
        End,
        // Each item gets the same portion from the layout (disregards gap)
        // |.o..o..o.|
        Even,
        // Space between each item is the same (disregards gap)
        // |o...o...o|
        Between,
        // Space around each item is the same (disregards gap)
        // |.o..o..o.|
        Around,
    };

    enum class CrossAxisAlignment {
        // Align items to the start
        // |ooo......|
        Start,
        // All items are centered
        // |...ooo...|
        Center,
        // Align items to the end
        // |......ooo|
        End,
        // Align items to the stretch
        // |ooooooooo|
        Stretch,
    };

    enum class VerticalDirection {
        // Items are laid out from top to bottom
        TopToBottom,
        // Items are laid out from bottom to top
        BottomToTop,
    };

    enum class HorizontalDirection {
        // Items are laid out from left to right
        LeftToRight,
        // Items are laid out from right to left
        RightToLeft,
    };

    struct Alignment {
        float x = 0.f;
        float y = 0.f;

        static Alignment TopLeft;
        static Alignment TopCenter;
        static Alignment TopRight;
        static Alignment CenterLeft;
        static Alignment Center;
        static Alignment CenterRight;
        static Alignment BottomLeft;
        static Alignment BottomCenter;
        static Alignment BottomRight;
    };

    inline Alignment Alignment::TopLeft = { -1.f, 1.f };
    inline Alignment Alignment::TopCenter = { 0.f, 1.f };
    inline Alignment Alignment::TopRight = { 1.f, 1.f };
    inline Alignment Alignment::CenterLeft = { -1.f, 0.f };
    inline Alignment Alignment::Center = { 0.f, 0.f };
    inline Alignment Alignment::CenterRight = { 1.f, 0.f };
    inline Alignment Alignment::BottomLeft = { -1.f, -1.f };
    inline Alignment Alignment::BottomCenter = { 0.f, -1.f };
    inline Alignment Alignment::BottomRight = { 1.f, -1.f };

    enum class TextAlignment {
        Left,
        Center,
        Right,
    };

    struct Color3B {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;

        Color3B() = default;
        Color3B(cocos2d::ccColor3B color) : r(color.r), g(color.g), b(color.b) {}
        Color3B(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

        operator cocos2d::ccColor3B() const {
            return { r, g, b };
        }
    };

    struct Color4B {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 255;

        Color4B() = default;
        Color4B(cocos2d::ccColor3B color) : r(color.r), g(color.g), b(color.b) {}
        Color4B(cocos2d::ccColor4B color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
        Color4B(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
        Color4B(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}

        operator cocos2d::ccColor3B() const {
            return { r, g, b };
        }

        operator cocos2d::ccColor4B() const {
            return { r, g, b, a };
        }
    };
}