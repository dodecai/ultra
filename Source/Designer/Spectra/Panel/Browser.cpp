#include "Browser.h"

#include <imgui/imgui.h>
#include "Ultra/Scene/Components.h"

namespace Ultra {

SceneBrowser::SceneBrowser() {}

SceneBrowser::SceneBrowser(const Reference<Scene> &scene) {
    SetContext(scene);
}

void SceneBrowser::GuiUpdate() {
    ImGui::Begin("Scene Browser");

    Context->Registry.each([&](auto id) {
        Entity entity { id, Context.get() };
        DrawEntityNode(entity);
    });

    ImGui::End();
}

Entity &SceneBrowser::GetSelectedNode() {
    return SelectionContext;
}

void SceneBrowser::SetContext(const Reference<Scene> &scene) {
    Context = scene;
}

void SceneBrowser::DrawEntityNode(Entity entity) {
    const char *tag = entity.GetComponent<Component::Tag>();
    ImGuiTreeNodeFlags flags = ((SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    
    bool opened = ImGui::TreeNodeEx((void *)(uint32_t)entity, flags, tag);
    if (ImGui::IsItemClicked()) {
        SelectionContext = entity;
    }

    if (opened) {


        ImGui::TreePop();
    }


    //if (ImGui::CollapsingHeader(tag)) {}
}

}
