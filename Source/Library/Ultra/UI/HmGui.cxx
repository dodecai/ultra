module Ultra.UI.HmGui;


namespace Ultra::UI {

///
/// @brief Switches
///

constexpr bool DrawLayoutFrames = false;
constexpr bool EnableWindowDragging = true;

void Container::Draw() {
    if (Clip) UIRenderer::Clip(Position, Size);
    for (auto &element : GetChildren()) {
        switch (element->Type) {
            // Container
            case ComponentType::Window: {
                if constexpr (EnableWindowDragging) {
                    if (element->Active) {
                        if (HmGui::sInputState.MouseClicked) {
                            HmGui::GetRoot()->MoveToTop(element->GetID());
                        } else if (HmGui::sInputState.MousePressed) {
                            element->Offset = HmGui::sInputState.DragCurrentPosition;
                            element->As<Container>()->LayoutGroup();

                        } else if (!HmGui::sInputState.MousePressed && element->Offset.X != 0 && element->Offset.Y != 0) {
                            element->Position.X += element->Offset.X;
                            element->Position.Y += element->Offset.Y;
                            element->Offset = {};
                            element->As<Container>()->LayoutGroup();
                        }
                    }
                }

                const auto color = Ultra::Color(0.1f, 0.12f, 0.13f, 1.0f);
                auto position = element->Position;
                position.X += element->Offset.X;
                position.Y += element->Offset.Y;
                UIRenderer::AddPanel(position, element->Size, color, 8.0f, element->FrameOpacity);
                element->As<Container>()->Draw();
                break;
            }
            case ComponentType::ScrollView: {
                float offsetY {};
                if (element->Active) {
                    auto scrollDelta = Input::GetMouseWheelDelta();
                    if (scrollDelta) offsetY = -12.0f * scrollDelta;
                }

                float maxScroll = std::max(0.0f, element->MinSize.Height - element->Size.Height);


                element->Offset.Y = std::clamp(element->Offset.Y + offsetY, 0.0f, maxScroll);
                element->As<Container>()->LayoutGroup();

                auto position = element->Position;
                position.X += element->Size.Width;
                if (maxScroll > 0) {
                    float handleSize = element->Size.Height * (element->Size.Height / element->MinSize.Height);
                    float handlePos = std::lerp(0.0f, element->Size.Height - handleSize, element->Offset.Y / maxScroll);
                    UIRenderer::AddRectangle(position, { 6.0, handlePos }, {}, false);
                    UIRenderer::AddRectangle(position, { 6.0, handleSize }, { 0.1f, 0.1f, 0.1f, 0.5f }, false);
                } else {
                    UIRenderer::AddRectangle(position, { 6.0f, 16.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, false);
                }
                if constexpr (DrawLayoutFrames) {
                    UIRenderer::AddBorder(1.0f, element->Position, element->Size, { 1.0f, 1.0f, 1.0f, 1.0f });
                }
                element->As<Container>()->Draw();
                break;
            }

            case ComponentType::Panel: [[fallthrough]];
            case ComponentType::Container: {
                if constexpr (DrawLayoutFrames) {
                    UIRenderer::AddBorder(1.0f, element->Position, element->Size, { 1.0f, 1.0f, 1.0f, 1.0f });
                }
                element->As<Container>()->Draw();
                break;
            }

            // Controls
            default: {
                element->Draw();
                break;
            }
        }
    }
    if (Clip) UIRenderer::Unclip();
}

void Button::Draw() {
    if (Active) {
        // Execute Callbacks
        if (HmGui::sInputState.MouseClicked) {
            if (Click) Click();
        }

        const auto color = HmGui::sInputState.MousePressed ?
            Ultra::Color(0.2f, 0.7f, 1.0f, 1.0f) :
            Ultra::Color(0.1f, 0.5f, 1.0f, 1.0f);
        UIRenderer::AddPanel(Position, Size, color, 0.0f, 1.0f);
    } else {
        const auto color = Ultra::Color(0.15f, 0.15f, 0.15f, 0.8f);
        UIRenderer::AddPanel(Position, Size, color, 0.0f, FrameOpacity);
    }

    if constexpr (DrawLayoutFrames) {
        UIRenderer::AddBorder(1.0f, Position, Size, { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    Ultra::Position position = {
        (Position.X + 8.0f + (Size.Width - MinSize.Width) * Alignment.X),
        (Position.Y + 4.0f + (Size.Height) * Alignment.Y),
    };
    UIRenderer::AddText(position, Text, Color, Font);
}

void CheckBox::Draw() {
    if (Active) {
        if (HmGui::sInputState.MouseClicked) Value = !Value;

        auto color = Ultra::Color(0.3f, 0.3f, 0.3f, 0.5f);
        UIRenderer::AddRectangle(Position, Size, color, true);
    }

    if constexpr (DrawLayoutFrames) {
        UIRenderer::AddBorder(1.0f, Position, Size, { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    // CheckBox Label
    Ultra::Position position = { Position.X + 8.0f, (Position.Y + MinSize.Height / 2 + 4.0f) };
    UIRenderer::AddText(position, Text, Color, Font);

    // CheckBox Outer Rectangle
    position.X = Position.X + Size.Width - OutherSize.Width - 4.0f;
    position.Y -= 12.0f;
    UIRenderer::AddRectangle(position, OutherSize, OutherColor, true);

    // CheckBox Inner Rectangle
    position.X += 3.0f;
    position.Y += 3.0f;
    if (Value) {
        UIRenderer::AddRectangle(position, InnerSize, InnerColor, false);
    } else {
        UIRenderer::AddRectangle(position, InnerSize, {}, false);
    }
}

void ColorPicker::Draw() {
    // Implement the drawing of the color picker
    auto hovered = Hovered(HmGui::sInputState.LastMousePosition);

    // Draw the color picker area
    const auto bgColor = Ultra::Color(0.1f, 0.1f, 0.1f, 1.0f);
    UIRenderer::AddPanel(Position, Size, bgColor, 0.0f, FrameOpacity);

    // Simulate color spectrum representation
    for (float y = 0.0f; y < Size.Height; y++) {
        for (float x = 0.0f; x < Size.Width; x++) {
            float r = x / Size.Width;
            float g = y / Size.Height;
            float b = 0.5f; // Fixed blue value for simplicity
            Ultra::Color color(r, g, b, 1.0f);
            Ultra::Position pixelPos = {
                Position.X + x,
                Position.Y + y,
            };
            //UIRenderer::AddPixel(pixelPos, color);
        }
    }

    // Draw selected color
    Ultra::Position selectedColorPos = {
        Position.X + Size.Width + 8.0f,
        Position.Y + Size.Height / 2,
    };
    Ultra::Color selectedColor(1.0f, 1.0f, 1.0f, 1.0f); // Simulated selected color
    UIRenderer::AddPanel(selectedColorPos, { 30.0f, 30.0f }, selectedColor, 0.0f, 1.0f);
}

void InputBox::Draw() {
    const auto bgColor = Active ? Ultra::Color(0.2f, 0.2f, 0.2f, 1.0f) : Ultra::Color(0.15f, 0.15f, 0.15f, 0.8f);
    UIRenderer::AddPanel(Position, Size, bgColor, 0.0f, FrameOpacity);

    Ultra::Position textPos = {
        Position.X + 8.0f,
        Position.Y + MinSize.Height / 2 + 4.0f,
    };
    UIRenderer::AddText(textPos, Text, Color, Font);
}

void Label::Draw() {
    if constexpr (DrawLayoutFrames) {
        UIRenderer::AddBorder(1.0f, Position, Size, { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    Ultra::Position position = { Position.X, Position.Y + MinSize.Height };
    UIRenderer::AddText(position, Text, Color, Font);
}

void Image::Draw() {
    //UIRenderer::Image(Position, Size, CreateReference<Texture>(Data));
}

void Seperator::Draw() {
    // ToDo: Check also line shader "vertex/ui" "fragment/ui/line"
    // ToDo: Additive BlendMode
    //const float padding = 64.0f;
    //float minX = std::min(native->Start.X, native->Stop.X) - padding;
    //float minY = std::min(native->Start.Y, native->Stop.Y) - padding;
    //float maxX = std::max(native->Start.X, native->Stop.X) + padding;
    //float maxY = std::max(native->Start.Y, native->Stop.Y) + padding;
    //float width = maxX - minX;
    //float height = maxY - minY;
    // UIRenderer::Line(e->Start, e->Stop, e->Color);
}

void Selection::Draw() {
    auto hovered = Hovered(HmGui::sInputState.LastMousePosition);

    // Draw the selection area
    const auto bgColor = Ultra::Color(0.15f, 0.15f, 0.15f, 1.0f);
    UIRenderer::AddPanel(Position, Size, bgColor, 0.0f, FrameOpacity);

    // Draw the currently selected option
    const Ultra::Position textPos = {
        Position.X + 10.0f,
        Position.Y + Size.Height / 2,
    };
    UIRenderer::AddText(textPos, Options[SelectedIndex], Ultra::Color(1.0f, 1.0f, 1.0f, 1.0f), nullptr);

    // Simulate dropdown functionality
    //if (UIRenderer::AddButton(Position + Ultra::Position(Size.Width - 20.0f, 0.0f), { 20.0f, Size.Height }, "▼")) {
    //    ShowDropdown = !ShowDropdown;
    //}

    // Draw dropdown options if open
    if (ShowDropdown) {
        DrawDropdown();
    }
}

void Selection::DrawDropdown() {
    // Draw the dropdown options
    //const float optionHeight = 25.0f;
    //for (size_t i = 0; i < Options.size(); ++i) {
    //    const Ultra::Position optionPos = {
    //        Position.X,
    //        Position.Y + Size.Height + optionHeight * static_cast<float>(i),
    //    };
    //    const bool selected = (i == SelectedIndex);
    //    //const bool clicked = UIRenderer::AddButton(optionPos, { Size.Width, optionHeight }, Options[i], selected);
    //    if (clicked) {
    //        SelectedIndex = i;
    //        ShowDropdown = false;
    //    }
    //}
}

void Slider::Draw() {
    const bool hovered = Hovered(HmGui::sInputState.LastMousePosition);
    const float handleWidth = 12.0f;
    const Ultra::Position handlePosition = {
        Position.X + (Size.Width - handleWidth) * Value,
        Position.Y
    };

    // Draw slider track
    const Ultra::Color trackColor = Ultra::Color(0.2f, 0.2f, 0.2f, 1.0f);
    UIRenderer::AddRectangle(Position, Size, trackColor, true);

    // Draw slider handle
    if (hovered || Dragged) {
        const Ultra::Color handleColor = Ultra::Color(0.4f, 0.4f, 0.4f, 1.0f);
        UIRenderer::AddRectangle(handlePosition, { handleWidth, Size.Height }, handleColor, true);

        if (HmGui::sInputState.MouseClicked) {
            Dragged = true;
            DragOffset = HmGui::sInputState.DragCurrentPosition.X - handlePosition.X;
        }
    } else {
        const Ultra::Color handleColor = Ultra::Color(0.3f, 0.3f, 0.3f, 1.0f);
        UIRenderer::AddRectangle(handlePosition, { handleWidth, Size.Height }, handleColor, true);
    }

    if (Dragged && HmGui::sInputState.MousePressed) {
        const float newHandleX = HmGui::sInputState.LastMousePosition.X - DragOffset;
        Value = std::clamp((newHandleX - Position.X) / (Size.Width - handleWidth), 0.0f, 1.0f);
    } else {
        Dragged = false;
    }

}

void Table::DrawHeader() {
    //const Ultra::Position textPos = {
    //    Position.X,
    //    Position.Y + RowHeight / 2,
    //};
    //for (size_t i = 0; i < Header.size(); ++i) {
    //    UIRenderer::AddText(textPos + Ultra::Position(ColumnWidths[i] / 2, 0), Header[i], Ultra::Color(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    //    textPos.X += ColumnWidths[i];
    //}
}

void Table::DrawRows() {
    //Ultra::Position textPos = {
    //    Position.X,
    //    Position.Y + RowHeight,
    //};
    //for (const auto &row : Data) {
    //    for (size_t i = 0; i < row.size(); ++i) {
    //        UIRenderer::AddText(textPos + Ultra::Position(ColumnWidths[i] / 2, 0), row[i], Ultra::Color(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    //        textPos.X += ColumnWidths[i];
    //    }
    //    textPos.X = Position.X;
    //    textPos.Y += RowHeight;
    //}
}

void Tree::Draw() {
    // Implement the drawing of the tree
    auto hovered = Hovered(HmGui::sInputState.LastMousePosition);

    // Draw the tree area
    const auto bgColor = Ultra::Color(0.15f, 0.15f, 0.15f, 1.0f);
    UIRenderer::AddPanel(Position, Size, bgColor, 0.0f, FrameOpacity);

    // Simulate tree nodes
    DrawTreeNode({ Position.X + 20.0f, Position.Y + 20.0f }, "Node 1");
    DrawTreeNode({ Position.X + 20.0f, Position.Y + 60.0f }, "Node 2");
    DrawTreeNode({ Position.X + 40.0f, Position.Y + 100.0f }, "Node 2.1");
    DrawTreeNode({ Position.X + 40.0f, Position.Y + 140.0f }, "Node 2.2");
}

void Tree::DrawTreeNode(const Ultra::Position &position, const string_view &text) {
    //// Draw a tree node with expand/collapse functionality
    //auto hovered = Hovered(HmGui::sLastMousePosition);
    //const float nodeWidth = 20.0f;
    //const float nodeHeight = 20.0f;
    //const float textIndent = 25.0f;
    //bool expanded = true; // Simulated node expansion state

    //// Draw expand/collapse button
    //const Ultra::Position buttonPos = {
    //    position.X - 5.0f,
    //    position.Y + nodeHeight / 2,
    //};
    //if (UIRenderer::AddButton(buttonPos, { 15.0f, 15.0f }, expanded)) {
    //    expanded = !expanded;
    //}

    //// Draw node text
    //const Ultra::Position textPos = {
    //    position.X + textIndent,
    //    position.Y + nodeHeight / 2,
    //};
    //UIRenderer::AddText(textPos, text, Ultra::Color(1.0f, 1.0f, 1.0f, 1.0f), nullptr);

    //// Draw child nodes if expanded
    //if (expanded) {
    //    DrawTreeNode({ position.X, position.Y + 30.0f }, "Child Node 1");
    //    DrawTreeNode({ position.X, position.Y + 60.0f }, "Child Node 2");
    //}
}

}

