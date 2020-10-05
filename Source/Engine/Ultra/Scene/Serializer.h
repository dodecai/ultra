#pragma once

#include "Scene.h"

namespace Ultra {

class Serializer {
public:
    Serializer(const Reference<Scene> &scene);
    ~Serializer() = default;

    void Serialize(const string &path);
    bool Deserialize(const string &path);

private:
    Reference<Scene> mScene;
};

}
