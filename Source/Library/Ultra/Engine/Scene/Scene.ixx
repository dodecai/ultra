export module Ultra.Engine.Scene;

import <entt/entt.hpp>;
import Ultra.Core;
import Ultra.Logger;

class Entity;

export namespace Ultra {

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

private:
    //SceneCamera *pCamera = nullptr;
    entt::registry Registry;
    vector<Entity *> mEntities;

    std::string mCaption;
    bool mActive = false;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
};

}
