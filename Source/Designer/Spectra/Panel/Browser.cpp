#include "Browser.h"

#include <imgui/imgui.h>
#include "Ultra/Scene/Components.h"

namespace Ultra {

SceneBrowser::SceneBrowser() {}

SceneBrowser::SceneBrowser(const Reference<Scene> &scene) {
    SetContext(scene);
    SelectionContext = {};
}

void SceneBrowser::GuiUpdate() {
    ImGui::Begin("Scene Browser");

    ImGui::BeginGroup();
    Context->Registry.each([&](auto id) {
        Entity entity { id, Context.get() };
        DrawEntityNode(entity);
    });
    ImGui::EndGroup();

    if (ImGui::BeginPopupContextWindow(0, 1, false)) {
        if (ImGui::Selectable("New Entity")) {
            static uint32_t counter = 0;
            Context->CreateEntity("New Entity #" + std::to_string(++counter));
        }

        ImGui::EndPopup();
    }

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
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    bool opened = ImGui::TreeNodeEx((void *)(uint32_t)entity, flags, tag);
    if (ImGui::IsItemClicked()) {
        SelectionContext = entity;
    }

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) {
        SelectionContext = {};
    }
    
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::Selectable("Remove")) {
            Context->DestroyEntity(entity);
            SelectionContext = {};
        }
        ImGui::EndPopup();
    }

    if (opened) {
        ImGui::TreePop();
    }

    //if (ImGui::CollapsingHeader(tag)) {}
}

}
