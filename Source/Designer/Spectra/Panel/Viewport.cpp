#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>

#include "Ultra/Renderer/Camera.h"
#include "Viewport.h"
#include "Omnia/Core/Application.h"
#include "Platform/GFX/Vulkan/VKContext.h"

#include "../Designer.h"

// ToDo: Including header leads to linker error...
#include "Ultra/Math/Math.cpp"

namespace Ultra {

ViewportPanel::ViewportPanel() {
    Context = Texture2D::Create("./Assets/Textures/Checkerboard.png");
    FramebufferProperties fbProperties;
    Buffer = Framebuffer::Create(fbProperties);
}

ViewportPanel::ViewportPanel(FramebufferProperties &properties) {
    Context = Texture2D::Create("./Assets/Textures/Checkerboard.png");
    Buffer = Framebuffer::Create(properties);
}

void ViewportPanel::Update() {
    if (Enabled) {
        static bool viewportOpen = true;

        ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoScrollbar;

        if (ImGui::Begin("Scene", &viewportOpen, viewportFlags)) {
            ImVec2 contentRegion = ImGui::GetContentRegionMax();
            Buffer->Resize(static_cast<uint32_t>(contentRegion.x), static_cast<uint32_t>(contentRegion.y));

            if (Context::API == GraphicsAPI::Vulkan) {
                void *rendererID = Buffer->GetColorAttachmentRendererIDRaw();
                ImGui::Image(rendererID, contentRegion, ImVec2(0,1), ImVec2(1,0));
            } else {
                uint32_t rendererID = Buffer->GetColorAttachmentRendererID();
                ImGui::Image((void *)rendererID, contentRegion, ImVec2(0,1), ImVec2(1,0));
            }

            if (mContext && mGizmoType != -1) {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                float windowWidth = (float)ImGui::GetWindowWidth();
                float windowHeight = (float)ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                auto cameraEntity = Designer::GetCurrentScene()->GetCameraEntity();
                const auto &camera = cameraEntity.GetComponent<Component::Camera>().mCamera;
                const glm::mat4 &cameraProjection = camera.GetProjection();
                glm::mat4 cameraView = glm::inverse((glm::mat4)cameraEntity.GetComponent<Component::Transform>());

                auto &transformEntity = mContext.GetComponent<Component::Transform>();
                glm::mat4 transform = (glm::mat4)mContext.GetComponent<Component::Transform>();

                // Snapping
                bool snap = Input::GetKeyState(KeyCode::Control);
                float snapValue = 0.5f;
                if (mGizmoType == ImGuizmo::OPERATION::ROTATE) {
                    snapValue = 45.0f;
                }

                float snapValues[3] = { snapValue, snapValue, snapValue };

                ImGuizmo::Manipulate(
                    glm::value_ptr(cameraView),
                    glm::value_ptr(cameraProjection),
                    (ImGuizmo::OPERATION)mGizmoType,
                    ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
                    nullptr,
                    snap ? snapValues : nullptr
                );

                if (ImGuizmo::IsUsing()) {
                    glm::vec3 position = {};
                    glm::vec3 rotation = {};
                    glm::vec3 scale = {};

                    Math::DecomposeTransform(transform, position, rotation, scale);
                    glm::vec3 deltaRotation = rotation - transformEntity.Rotation;

                    transformEntity.Position = position;
                    transformEntity.Rotation += deltaRotation;
                    transformEntity.Scale = scale;
                }
            }

            Active = ImGui::IsWindowFocused() ? true : false;
            ImGui::End();
        };
    }
}

void ViewportPanel::Reload() {
    Context.reset();
    Buffer.reset();

    Context = Texture2D::Create("./Assets/Textures/Checkerboard.png");
    FramebufferProperties fbProperties;
    Buffer = Framebuffer::Create(fbProperties);
}

void ViewportPanel::SetContext(Entity &entity) {
    mContext = entity;
}

void ViewportPanel::SetGizmoType(int type) {
    mGizmoType = type;
}

}
