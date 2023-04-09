module;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

export module Ultra.Engine.DesignerCamera;

import Ultra.Core;
import Ultra.Logger;

import Ultra.Engine.Camera;

export namespace Ultra {

class DesignerCamera: public Camera {
public:
    DesignerCamera() = default;
    ~DesignerCamera() = default;
};

}
