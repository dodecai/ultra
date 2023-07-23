module;

#include "MemPool.h"

module Ultra.Engine.UIRenderer;


namespace Ultra {

ShaderData *UILayer::mShader = nullptr;

struct UIRendererLayer {
    UIRendererLayer *parent;
    UIRendererLayer *next;
    UIRendererLayer *children;

    Vector2Df pos;
    Vector2Df size;
    bool clip;
};

struct UIRendererData {
    UIRendererLayer *root;
    UIRendererLayer *layer;

    MemPool *layerPool;
} static self = { 0 };


static void Init() {
    static bool init = false;
    if (init) return;
    init = true;

    auto initialize = UIRenderer::Instance();

    self.root = 0;
    self.layer = 0;

    self.layerPool = MemPool_CreateAuto(sizeof(UIRendererLayer));
}

static void DrawLayer(UIRendererLayer const *self) {
    if (self->clip) ClipRect::PushCombined(self->pos.x, self->pos.y, self->size.x, self->size.y);

    auto layer = UIRenderer::Instance().GetLastLayer();
    if (layer->mClip) {
        ClipRect::PushCombined(layer->mPosition.X, layer->mPosition.Y, layer->mSize.Width, layer->mSize.Height);
    }

    layer->Draw();

    if (self->clip != layer->mClip) {
        LogInfo("Clip: {}={}", self->clip, layer->mClip);

    }

    for (UIRendererLayer const *e = self->children; e; e = e->next) {
        DrawLayer(e);
    }

    if (layer->mClip) ClipRect::Pop();
    if (self->clip) ClipRect::Pop();

    UIRenderer::Instance().mLayers.pop_back();
}

void UIRenderer::Begin() {
    Init();

    self.root = 0;
    self.layer = 0;

    Instance().mLayers.clear();

    MemPool_Clear(self.layerPool);

    Vector2Di vp; Viewport::GetSize(&vp);
    
    BeginLayer(0, 0, vp.x, vp.y, true);
    self.root = self.layer;

}

void UIRenderer::BeginLayer(float x, float y, float sx, float sy, bool clip) {
    UIRendererLayer *layer = (UIRendererLayer *)MemPool_Alloc(self.layerPool);
    layer->parent = self.layer;
    layer->next = 0;
    layer->children = 0;

    layer->pos = { x, y };
    layer->size = { sx, sy };
    layer->clip = clip;

    self.layer = layer;

    Instance().AddLayer(CreateReference<UILayer>(Position{x, y}, Size{sx, sy}, clip));
}

void UIRenderer::EndLayer() {
    if (self.layer->parent) {
        self.layer->next = self.layer->parent->children;
        self.layer->parent->children = self.layer;
    }
    self.layer = self.layer->parent;
}

void UIRenderer::End() {
    EndLayer();
}

void UIRenderer::Draw() {
    RenderState::PushBlendMode(PhxBlendMode::Alpha);
    DrawLayer(self.root);
    //UIRenderer::Instance().Render();
    RenderState::PopBlendMode();
}


void UIRenderer::Image(Tex2DData *image, float x, float y, float sx, float sy) {
    Instance().mLayers.back()->AddElement(CreateScope<Ultra::Image>(Position{x, y}, Size{sx, sy}, image));
}

void UIRenderer::Panel(float x, float y, float sx, float sy, float r, float g, float b, float a, float bevel, float innerAlpha) {
    Instance().mLayers.back()->AddPanel(CreateScope<Ultra::Panel>(Position{ x, y }, Size{ sx, sy }, Color{ r, g, b, a }, bevel, innerAlpha));
}

void UIRenderer::Rect(float x, float y, float sx, float sy, float r, float g, float b, float a, bool outline) {
    Instance().mLayers.back()->AddElement(CreateScope<Rectangle>(Position{ x, y }, Size{ sx, sy }, Color{ r, g, b, a }, outline));
}

void UIRenderer::Text(FontData *font, const char * text, float x, float y, float r, float g, float b, float a) {
    Instance().mLayers.back()->AddElement(CreateScope<Ultra::Text>(Position{ x, y }, Size{ 0.0f, 0.0f }, text, Color{ r, g, b, a }, font));
}

}
