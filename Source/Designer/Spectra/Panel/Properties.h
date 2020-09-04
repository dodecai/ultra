#pragma once

#include "Ultra/Core.h"
#include "Ultra/Scene/Entity.h"

namespace Ultra {

class PropertiesPanel {
public:
    PropertiesPanel();
    PropertiesPanel(Entity &entity);

    void GuiUpdate();

    void SetContext(Entity &entity);

private:
    void Draw();

private:
    Entity Context;
};

}
