#pragma once

#include "Core.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace Ultra {

class SceneBrowser {
    friend class Scene;

public:
    SceneBrowser();
    SceneBrowser(const Reference<Scene> &scene);
    
    void GuiUpdate();

    Entity &GetSelectedNode();
    void SetContext(const Reference<Scene> &scene);

private:
    void DrawEntityNode(Entity entity);

private:
    Reference<Scene> Context;
    Entity SelectionContext;
};

}
