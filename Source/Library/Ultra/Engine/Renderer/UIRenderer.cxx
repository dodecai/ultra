module;

#include "MemPool.h"

module Ultra.Engine.UIRenderer;

import Ultra.Math;

namespace Ultra {

struct UIRendererImage {
    UIRendererImage *next;
    Tex2DData *image;
    Vector2Df pos;
    Vector2Df size;
};

struct UIRendererPanel {
    UIRendererPanel *next;
    Vector2Df pos;
    Vector2Df size;
    Vector4Df color;
    float bevel;
    float innerAlpha;
};

struct UIRendererRect {
    UIRendererRect *next;
    Vector2Df pos;
    Vector2Df size;
    Vector4Df color;
    bool outline;
};

struct UIRendererText {
    UIRendererText *next;
    FontData *font;
    cstr text;
    Vector2Df pos;
    Vector4Df color;
};

struct UIRendererLayer {
    UIRendererLayer *parent;
    UIRendererLayer *next;
    UIRendererLayer *children;

    UIRendererImage *imageList;
    UIRendererPanel *panelList;
    UIRendererRect *rectList;
    UIRendererText *textList;

    Vector2Df pos;
    Vector2Df size;
    bool clip;
};

struct UIRendererData {
    UIRendererLayer *root;
    UIRendererLayer *layer;

    MemPool *layerPool;
    MemPool *imagePool;
    MemPool *panelPool;
    MemPool *rectPool;
    MemPool *textPool;
} static self = { 0 };


static void Init() {
    static bool init = false;
    if (init) return;
    init = true;

    self.root = 0;
    self.layer = 0;

    self.layerPool = MemPool_CreateAuto(sizeof(UIRendererLayer));
    self.imagePool = MemPool_CreateAuto(sizeof(UIRendererImage));
    self.panelPool = MemPool_CreateAuto(sizeof(UIRendererPanel));
    self.rectPool = MemPool_CreateAuto(sizeof(UIRendererRect));
    self.textPool = MemPool_CreateAuto(sizeof(UIRendererText));
}

static void DrawLayer(UIRendererLayer const *self) {
    if (self->clip) ClipRect::PushCombined(self->pos.x, self->pos.y, self->size.x, self->size.y);

    if (self->panelList) {
        static ShaderData *shader = 0;
        if (!shader) shader = PhxShader::Load("vertex/ui", "fragment/ui/panel");

        const float pad = 64.0f;
        PhxShader::Start(shader);
        PhxShader::SetFloat("padding", pad);

        for (UIRendererPanel const *e = self->panelList; e; e = e->next) {
            float x = e->pos.x - pad;
            float y = e->pos.y - pad;
            float sx = e->size.x + 2.0f * pad;
            float sy = e->size.y + 2.0f * pad;

            PhxShader::SetFloat("innerAlpha", e->innerAlpha);
            PhxShader::SetFloat("bevel", e->bevel);
            PhxShader::SetFloat2("size", sx, sy);
            PhxShader::SetFloat4("color", e->color.x, e->color.y, e->color.z, e->color.w);
            Draw::Rect(x, y, sx, sy);
        }

        PhxShader::Stop(shader);
    }

    for (UIRendererImage const *e = self->imageList; e; e = e->next) {
        Tex2D::Draw(e->image, e->pos.x, e->pos.y, e->size.x, e->size.y);
    }

    for (UIRendererRect const *e = self->rectList; e; e = e->next) {
        Draw::Color(e->color.x, e->color.y, e->color.z, e->color.w);
        if (e->outline) Draw::Border(1.0f, e->pos.x, e->pos.y, e->size.x, e->size.y);
        else Draw::Rect(e->pos.x, e->pos.y, e->size.x, e->size.y);
    }

    for (UIRendererText const *e = self->textList; e; e = e->next) {
        Font::Draw(e->font, e->text, e->pos.x, e->pos.y, e->color.x, e->color.y, e->color.z, e->color.w);
    }

    for (UIRendererLayer const *e = self->children; e; e = e->next) {
        DrawLayer(e);
    }

    if (self->clip) ClipRect::Pop();
}


void UIRenderer::Begin() {
    Init();

    self.root = 0;
    self.layer = 0;

    MemPool_Clear(self.layerPool);
    MemPool_Clear(self.imagePool);
    MemPool_Clear(self.panelPool);
    MemPool_Clear(self.rectPool);
    MemPool_Clear(self.textPool);

    Vector2Di vp; Viewport::GetSize(&vp);
    BeginLayer(0, 0, vp.x, vp.y, true);
    self.root = self.layer;
}

void UIRenderer::End() {
    EndLayer();
}

void UIRenderer::Draw() {
    RenderState::PushBlendMode(PhxBlendMode::Alpha);
    DrawLayer(self.root);
    RenderState::PopBlendMode();
}

void UIRenderer::BeginLayer(float x, float y, float sx, float sy, bool clip) {
    UIRendererLayer *layer = (UIRendererLayer *)MemPool_Alloc(self.layerPool);
    layer->parent = self.layer;
    layer->next = 0;
    layer->children = 0;

    layer->pos = { x, y };
    layer->size = { sx, sy };
    layer->clip = clip;

    layer->imageList = 0;
    layer->panelList = 0;
    layer->rectList = 0;
    layer->textList = 0;
    self.layer = layer;
}

void UIRenderer::EndLayer() {
    if (self.layer->parent) {
        self.layer->next = self.layer->parent->children;
        self.layer->parent->children = self.layer;
    }
    self.layer = self.layer->parent;
}

void UIRenderer::Image(Tex2DData *image, float x, float y, float sx, float sy) {
    UIRendererImage *e = (UIRendererImage *)MemPool_Alloc(self.imagePool);
    e->next = self.layer->imageList;
    e->image = image;
    e->pos = { x, y };
    e->size = { sx, sy };
    self.layer->imageList = e;
}

void UIRenderer::Panel(float x, float y, float sx, float sy, float r, float g, float b, float a, float bevel, float innerAlpha) {
    UIRendererPanel *e = (UIRendererPanel *)MemPool_Alloc(self.panelPool);
    e->next = self.layer->panelList;
    e->pos = { x, y };
    e->size = { sx, sy };
    e->color = { r, g, b, a };
    e->bevel = bevel;
    e->innerAlpha = innerAlpha;
    self.layer->panelList = e;
}

void UIRenderer::Rect(float x, float y, float sx, float sy, float r, float g, float b, float a, bool outline) {
    UIRendererRect *e = (UIRendererRect *)MemPool_Alloc(self.rectPool);
    e->next = self.layer->rectList;
    e->pos = { x, y };
    e->size = { sx, sy };
    e->color = { r, g, b, a };
    e->outline = outline;
    self.layer->rectList = e;
}

void UIRenderer::Text(FontData *font, cstr text, float x, float y, float r, float g, float b, float a) {
    UIRendererText *e = (UIRendererText *)MemPool_Alloc(self.textPool);
    e->next = self.layer->textList;
    e->font = font;
    e->text = text;
    e->pos = { x, y };
    e->color = { r, g, b, a };
    self.layer->textList = e;
}

}
