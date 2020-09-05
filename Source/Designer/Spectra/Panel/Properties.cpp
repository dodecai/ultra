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


namespace Ultra {
PropertiesPanel::PropertiesPanel() {}

PropertiesPanel::PropertiesPanel(Entity &entity) {
    SetContext(entity);
}

void PropertiesPanel::GuiUpdate() {
    ImGui::Begin("Properties");

    // If an entity is selected ...
    if (Context) {
        // .. and it has a tag component, show all well-known properties.
        if (Context.HasComponent<Component::Tag>()) {
            const char *tag = Context.GetComponent<Component::Tag>();
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            if (ImGui::CollapsingHeader(tag)) {

                // Caption
                string &tag = Context.GetComponent<Component::Tag>();
                tag.reserve(1024);
                ImGui::InputText("Tag", tag.data(), 1024);

                // Camera
                if (Context.HasComponent<Component::Camera>()) {
                    static bool primary = Context.GetComponent<Component::Camera>().Primary;
                    UI::Property("Primary", primary);
                    Context.GetComponent<Component::Camera>().Primary = primary;

                    auto &camera = Context.GetComponent<Component::Camera>().mCamera;
                    float os = camera.GetOrthographicSize();

                    if (ImGui::DragFloat("Zoom", &os, 1.0f, 0.1f, 120.0f)) {
                        camera.SetOrthographicSize(os);
                    }
                }

                // Sound
                if (Context.HasComponent<Component::Sound>()) {
                    string &source = Context.GetComponent<Component::Sound>();
                    source.reserve(1024);
                    ImGui::InputText("Source", source.data(), 1024);
                }

                // Transform
                if (Context.HasComponent<Component::Transform>()) {
                    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                    if (ImGui::CollapsingHeader("Transform")) {
                        auto &transform = Context.GetComponent<Component::Transform>().mTransform;

                        glm::vec3 scale;
                        glm::quat orientation;
                        glm::vec3 translation;
                        glm::vec3 skew;
                        glm::vec4 perspective;

                        glm::decompose(transform, scale, orientation, translation, skew, perspective);

                        ImGui::DragFloat3("Size", glm::value_ptr(scale));
                        ImGui::DragFloat3("Rotation", glm::value_ptr(orientation));
                        ImGui::DragFloat3("Position", glm::value_ptr(transform[3]));
                    }
                }

                // Information
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                if (ImGui::CollapsingHeader("Information")) {
                    const string &id = Context.GetComponent<Component::Identifier>();
                    UI::Property("ID", "%s", id.c_str());
                    UI::Property("Type", "%s", "undefined");
                }

            }
        }
        ImGui::Separator();
    }
    ImGui::End();
}

void PropertiesPanel::SetContext(Entity &entity) {
    Context = entity;
}

void PropertiesPanel::Draw() {}

}
