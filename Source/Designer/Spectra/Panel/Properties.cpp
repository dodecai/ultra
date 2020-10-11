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
    if (mContext) {
        // .. and it has a tag component, show all well-known properties.
        if (mContext.HasComponent<Component::Tag>()) {
            // Caption
            string id = mContext.GetComponent<Component::Identifier>();
            string &tag = mContext.GetComponent<Component::Tag>();
            tag.reserve(1024);
            ImGui::InputText("Caption", tag.data(), 1024);
            ImGui::LabelText("ID", "%s", id.c_str());
            UI::Property("Type", "%s", "undefined");
            ImGui::Separator();

            // Settings
            if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
                // Camera
                if (mContext.HasComponent<Component::Camera>()) {
                    auto &camera = mContext.GetComponent<Component::Camera>().mCamera;
                    auto &cameraComponent = mContext.GetComponent<Component::Camera>();

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

                }


                // Sound
                if (mContext.HasComponent<Component::Sound>()) {
                    string &source = mContext.GetComponent<Component::Sound>();
                    source.reserve(1024);
                    ImGui::InputText("Source", source.data(), 1024);
                }

                // Sprite
                if (mContext.HasComponent<Component::Sprite>()) {
                    glm::vec4 &color = mContext.GetComponent<Component::Sprite>();
                    ImGui::ColorEdit4("Color", glm::value_ptr(color));
                }
            }

            // Transform
            if (mContext.HasComponent<Component::Transform>()) {
                if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                    auto &transform = mContext.GetComponent<Component::Transform>().mTransform;

                    glm::vec3 scale;
                    glm::quat orientation;
                    glm::vec3 translation;
                    glm::vec3 skew;
                    glm::vec4 perspective;

                    glm::decompose(transform, scale, orientation, translation, skew, perspective);

                    ImGui::DragFloat3("Size", glm::value_ptr(scale));
                    ImGui::DragFloat3("Rotation", glm::value_ptr(orientation));
                    ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.01f);
                }
            }

        }
        ImGui::Separator();
    }
    ImGui::End();
}

void PropertiesPanel::DrawComponent() {

}

void PropertiesPanel::SetContext(Entity &entity) {
    mContext = entity;
}

void PropertiesPanel::Draw() {}

}
