module;

#include <glad/gl.h>

module Ultra.Engine.UIRenderer;

namespace Ultra {

void ClipRect::Activate() {
    if (mCurrentIndex < 0) return;
    auto &current = mRectangle[mCurrentIndex];

    if (current.Enabled) {
        Vector2Di vpSize;
        Viewport::GetSize(&vpSize);

        auto x = current.Position.X;
        auto y = current.Position.Y;
        auto width = current.Size.Width;
        auto height = current.Size.Height;
        TransformRectangle(x, y, width, height);

        glEnable(GL_SCISSOR_TEST);
        glScissor(x, vpSize.y - (y + height), width, height);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}

void ClipRect::Push(const Position &position, const Size &size) {
    if (mCurrentIndex + 1 >= mStackDepth) return;

    mCurrentIndex++;
    auto &current = mRectangle[mCurrentIndex];
    current = { true, position, size };
    
    Activate();
}

void ClipRect::PushCombined(const Position &position, const Size &size) {
    if (mCurrentIndex < 0) {
        Push(position, size);
        return;
    }

    auto &current = mRectangle[mCurrentIndex];
    if (!current.Enabled) return;

    float maxX = position.X + size.Width;
    float maxY = position.Y + size.Height;
    auto x = std::max(position.X, current.Position.X);
    auto y = std::max(position.Y, current.Position.Y);

    Push(position, { std::min(maxX, current.Position.X + current.Size.Width) - x, std::min(maxY, current.Position.Y + current.Size.Height) - y });
}

void ClipRect::PushDisabled() {
    if (!Validate()) return;

    mCurrentIndex++;
    auto &current = mRectangle[mCurrentIndex];
    current.Enabled = false;

    Activate();
}

void ClipRect::PushTransform(const Position &position, const Size &size) {
    if (!Validate()) return;
    mCurrentTransformIndex++;

    auto &transform = mTransform[mCurrentTransformIndex];
    transform.tx = position.X;
    transform.ty = position.Y;
    transform.sx = size.Width;
    transform.sy = size.Height;
    if (mCurrentIndex >= 0) Activate();
}

void ClipRect::Pop() {
    if (!Validate()) return;
    mCurrentIndex--;

    Activate();
}

void ClipRect::PopTransform() {
    if (!Validate()) return;
    mCurrentTransformIndex--;

    if (mCurrentIndex >= 0) Activate();
}

void ClipRect::TransformRectangle(float &x, float &y, float &sx, float &sy) {
    if (mCurrentTransformIndex >= 0) {
        auto &transform = mTransform[mCurrentTransformIndex];
        x = transform.sx * x + transform.tx;
        y = transform.sy * y + transform.ty;
        sx = transform.sx * sx;
        sy = transform.sy * sy;
    }
}

bool ClipRect::Validate() {
    if (mCurrentIndex < 0) {
        LogFatal("Attempting to pop an empty stack");
        return false;
    } else if (mCurrentIndex + 1 >= mStackDepth) {
        LogFatal("Maximum stack depth exceeded");
        return false;
    }
    return true;
}

}
