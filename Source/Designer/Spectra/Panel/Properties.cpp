#include "Properties.h"

#include "Ultra.pch"
#include "Ultra/Core.h"
#include "Omnia/UI/GuiLayer.h"

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>


#include "Ultra/Scene/Components.h"

#include <imgui/imgui_internal.h>

namespace Omnia {

namespace UI {

/// Vector 3 Property
inline void Property(const string &label, glm::vec3 &values, float resetValue = 0.0f, float columnWidth = 100.0f) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());

    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y *2.0f;
    ImVec2 buttonSize = { lineHeight * 1.28f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 0.72f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.25f, 0.72f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 0.72f});
    if (ImGui::Button("X", buttonSize)) values.x = resetValue;
    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.01f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PopStyleColor(3);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 0.72f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.2f, 0.3f, 0.72f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.3f, 0.72f});
    if (ImGui::Button("Y", buttonSize)) values.y = resetValue;
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.2f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PopStyleColor(3);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 0.72f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 0.72f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 0.72f});
    if (ImGui::Button("Z", buttonSize)) values.z = resetValue;
    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::PopStyleColor(3);

    ImGui::PopStyleVar(1);
    ImGui::Columns(1);

    ImGui::PopID();
}

}

}

namespace Ultra {

PropertiesPanel::PropertiesPanel() {}

PropertiesPanel::PropertiesPanel(Entity &entity) {
    SetContext(entity);
    mContext = {};
}

void PropertiesPanel::GuiUpdate() {
    ImGui::Begin("Properties");

    // If an entity is selected ...
    if (mContext) {
        // .. and it has a tag component, show all well-known properties.
        if (mContext.HasComponent<Component::Tag>()) {
            // Caption
            string id = mContext.GetComponent<Component::Identifier>();
            string &tag = mContext.GetComponent<Component::Tag>();
            tag.reserve(1024);

            ImGui::InputText("", tag.data(), 1024);
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::Button("Add Component")) ImGui::OpenPopup("AddComponent");
            if (ImGui::BeginPopup("AddComponent")) {
                if (ImGui::Selectable("Camera")) mContext.AddComponent<Component::Camera>();
                if (ImGui::Selectable("Sound")) mContext.AddComponent<Component::Sound>();
                if (ImGui::Selectable("Sprite")) mContext.AddComponent<Component::Sprite>();
                ImGui::EndPopup();
             }

            UI::Property("ID", "%s", id.c_str());
            UI::Property("Type", "%s", "undefined");

            ImGui::Separator();

            const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

            // Transform
            DrawComponent<Component::Transform>("Transform", mContext, [&](auto &component) {
                UI::Property("Position", component.Position);
                glm::vec3 rotation = glm::degrees(component.Rotation);
                UI::Property("Rotation", rotation);
                component.Rotation = glm::radians(rotation);
                UI::Property("Scale", component.Scale, 1.0f);
            });

            // Camera
            DrawComponent<Component::Camera>("Camera", mContext, [&](auto &component) {
                auto &cameraComponent = component;
                auto &camera = component.mCamera;

                const char *projectionTypes[] = { "Perspective", "Orthographic" };
                static int currentType = cameraComponent;
                if (ImGui::Combo("Projection", &currentType, projectionTypes, IM_ARRAYSIZE(projectionTypes))) {
                    cameraComponent = (SceneCamera::ProjectionType)currentType;
                }

                if (ImGui::Checkbox("Primary", &cameraComponent.Primary)) {
                    auto cameras = mContext.GetScene()->Registry.view<Component::Camera>();
                    for (auto entity : cameras) {
                        auto &camera = cameras.get<Component::Camera>(entity);

                        if (cameraComponent != camera && camera.Primary) {
                            camera.Primary = false;
                        }
                    }
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
                    float fov = glm::degrees(camera.GetPerspectiveVerticalFOV());
                    if (ImGui::DragFloat("FOV", &fov, 1.0f, 0.1f, 120.0f)) {
                        camera.SetPerspectiveVerticalFOV(glm::radians(fov));
                    }
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
                    if (ImGui::Checkbox("Fix Aspect Ratio", &cameraComponent.FixedAspectRatio)) {
                    }

                    float os = camera.GetOrthographicSize();
                    if (ImGui::DragFloat("Size", &os, 1.0f, 0.1f, 120.0f)) {
                        camera.SetOrthographicSize(os);
                    }
                }

                float near = camera.GetNearClip();
                if (ImGui::DragFloat("Near", &near, 1.0f, -120.0f, 120.0f)) {
                    camera.SetNearClip(near);
                }

                float far = camera.GetFarClip();
                if (ImGui::DragFloat("Far", &far, 1.0f, -120.0f, 120.0f)) {
                    camera.SetFarClip(far);
                }
            });
 
            // Sound
            DrawComponent<Component::Sound>("Sound", mContext, [&](auto &component) {
                string &source = component;
                source.reserve(1024);
                ImGui::InputText("Source", source.data(), 1024);
            });

            // Sprite
            DrawComponent<Component::Sprite>("Sprite", mContext, [&](auto &component) {
                glm::vec4 &color = component;
                UI::Property("Color", glm::value_ptr(color));
            });

            ImGui::PopItemWidth();
        }
    }
    ImGui::End();
}

template<typename T, typename UIFunction>
void PropertiesPanel::DrawComponent(const string &name, Entity entity, UIFunction uiFunction) {
    const ImGuiTreeNodeFlags treeNodeFlags =
        ImGuiTreeNodeFlags_AllowItemOverlap |
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_Framed |
        ImGuiTreeNodeFlags_FramePadding |
        ImGuiTreeNodeFlags_SpanFullWidth;

    if (mContext.HasComponent<T>()) {
        auto &component = mContext.GetComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
        
        ImGui::PushID(name.c_str());

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        //(void *)typeid(T).hash_code()
        bool open = ImGui::CollapsingHeader(name.c_str(), treeNodeFlags);
        ImGui::PopStyleVar();

        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

        if (ImGui::Button("+", ImVec2(24, 24))) ImGui::OpenPopup("ComponentSettings");

        if (open) {
            uiFunction(component);
        }

        if (ImGui::BeginPopup("ComponentSettings")) {
            if (ImGui::Selectable("Remove")) mContext.RemoveComponent<T>();
            ImGui::EndPopup();
        }

        ImGui::PopID();
    }
}

void PropertiesPanel::SetContext(Entity &entity) {
    mContext = entity;
}

void PropertiesPanel::Draw() {}

}
