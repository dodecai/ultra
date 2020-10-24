#pragma once

#include "Ultra/Core.h"
#include "Ultra/Scene/Entity.h"
#include "Ultra/Scene/Scene.h"

namespace Ultra {

class PropertiesPanel {
public:
    PropertiesPanel();
    PropertiesPanel(Entity &entity);

    void GuiUpdate();

    void SetContext(Entity &entity);

private:
    void Draw();
    template<typename T, typename UIFunction>
    void DrawComponent(const string &name, Entity entity, UIFunction uiFunction);

private:
    Entity mContext;
};

}
