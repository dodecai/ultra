export module Ultra.Asset.Resource;

import Ultra.Core;

///
/// @brief Resource Manager
///
export namespace Ultra {

enum class PhyResourceType {
    Font = 0,
    Mesh = 1,
    Other = 2,
    Script = 3,
    Shader = 4,
    Sound = 5,
    Tex1D = 6,
    Tex2D = 7,
    Tex3D = 8,
    TexCube = 9,
};

class Resource {

public:
    Resource();
    ~Resource();

    static const Resource &Instance() {
        static Resource instance {};
        return instance;
    }

    static bool Exists(PhyResourceType, const string &name);
    static string GetPath(PhyResourceType, const string &name);
    static vector<uint32_t> LoadBytes(PhyResourceType, const string &name);
    static string LoadCstr(PhyResourceType, const string &name);
    static void Reset();

private:
    static void AddPath(PhyResourceType, const string &format);
    static string Resolve(PhyResourceType type, const string &name);
    static string ToString(PhyResourceType);
};

}
