#pragma once

#include "Omnia/Core/Layer.h"
#include "Style/Styles.h"

#pragma warning(push, 0)
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>
#pragma warning(pop)

namespace Omnia {

class GuiLayer: public Layer {
public:
	GuiLayer();
	virtual ~GuiLayer();

	virtual void Attach() override;
	virtual void Detach() override;
	virtual void GuiUpdate() override;
	virtual void Update(Timestamp deltaTime) override;

	void Prepare();
	void Finish();

	virtual void Event(void *event) override;
	virtual void ControllerEvent(ControllerEventData data) override;
	virtual void KeyboardEvent(KeyboardEventData data) override;
	virtual void MouseEvent(MouseEventData data) override;
	virtual void TouchEvent(TouchEventData data) override;
	virtual void WindowEvent(WindowEventData data) override;
};

// Helpers
namespace UI {

// Labels
template <typename ... Args>
auto Label(const string &format, const Args &...arguments) {
	ImGui::Text(format.c_str(), arguments...);
}

template <typename ... Args>
auto LabelX(const string &format, const Args &...arguments) {
	((ImGui::Text(format.c_str(), arguments)), ...);
}


/**
 * @brief Properties
*/

/// Checkbox Property
inline void Property(const string &label, bool &value) {
	ImGui::Checkbox(label.c_str(), &value);
}

/// Color Property
inline void Property(const string &label, float *colors) {
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 96.0f);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();
    ImGui::ColorEdit4("", colors);
    ImGui::Columns(1);
}

/// Floating-Point Property
inline void Property(const string &caption, float &value, float min = 0.0f, float max = 1.0f) {
	ImGui::SliderFloat(caption.c_str(), &value, min, max);
}

/// State Property
template <typename ... Args>
inline void Property(const string &label, const string &format, const Args &...arguments) {
	ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 96.0f);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	//ImGui::TextColored(ImTextColorHighlightWarn, format.c_str(), arguments...);
    ImGui::PushStyleColor(ImGuiCol_Text, GetTextColorHighlight());
    ImGui::Text(format.c_str(), arguments...);
    ImGui::PopStyleColor();
	ImGui::Columns(1);
}


}

}
