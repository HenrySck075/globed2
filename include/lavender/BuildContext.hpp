#pragma once
#include <utility>

namespace ui {
    struct Base;

    template<class T>
    concept ExtendsWidget = requires {
        static_cast<Base*>(std::declval<T*>());
    };
    // A handle to the location of a widget in the widget tree.
    struct BuildContext {
        friend class ui::Base;
    private:
        BuildContext* parentContext;
        ui::Base* widget;

        int refcount = 0;
    public:
        void ref() {refcount++;}
        void unref() {
            if (--refcount <= 0) delete this;
        }
        template<ExtendsWidget T>
        T* findAncestorWidgetOfExactType(); 
    };

}
