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
    void DrawComponent();

private:
    Entity Context;
};

}
