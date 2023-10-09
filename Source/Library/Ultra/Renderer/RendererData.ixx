export module Ultra.Renderer.Data;

import Ultra.Core;

export namespace Ultra {

using RendererID = uint32_t;

struct Color {
    float Red {};
    float Green {};
    float Blue {};
    float Alpha {};
};

struct Position {
    float X;
    float Y;
};

struct Size {
    float Width;
    float Height;
};

}
