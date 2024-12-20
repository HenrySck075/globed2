#pragma once

#include "../../include/lavender/widgets/BuildContext.hpp"
#include "../../include/lavender/Base.hpp"

namespace ui {
template<ExtendsWidget T>
T* BuildContext::findAncestorWidgetOfExactType() {
    if (parentContext) return nullptr;

    auto p = parentContext;
    while (p) {
      if (auto iw = dynamic_cast<Base*>(p->widget)) 
        return iw;
      
      p = p->parentContext;
    }

    return nullptr;
  }
}
