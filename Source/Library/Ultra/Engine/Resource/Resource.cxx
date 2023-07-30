module Ultra.Engine.Resource;

import Ultra.Logger;
import Ultra.System.FileSystem;

namespace Ultra {

using PathList = std::list<string>;
static unordered_map<PhyResourceType, PathList> sPaths;

Resource::Resource() {
    Resource::AddPath(PhyResourceType::Font, "./Assets/Phoenix/res/font/{}.ttf");
    Resource::AddPath(PhyResourceType::Font, "./Assets/Phoenix/res/font/{}.otf");
    Resource::AddPath(PhyResourceType::Mesh, "./Assets/Phoenix/res/mesh/{}.bin");
    Resource::AddPath(PhyResourceType::Mesh, "./Assets/Phoenix/res/mesh/{}.obj");
    Resource::AddPath(PhyResourceType::Script, "./Assets/Phoenix/res/script/{}.lua");
    Resource::AddPath(PhyResourceType::Shader, "./Assets/Phoenix/res/shader/{}.glsl");
    Resource::AddPath(PhyResourceType::Sound, "./Assets/Phoenix/res/sound/{}.mp3");
    Resource::AddPath(PhyResourceType::Sound, "./Assets/Phoenix/res/sound/{}.ogg");
    Resource::AddPath(PhyResourceType::Sound, "./Assets/Phoenix/res/sound/{}.ogx");
    Resource::AddPath(PhyResourceType::Sound, "./Assets/Phoenix/res/sound/{}.wav");
    Resource::AddPath(PhyResourceType::Tex1D, "./Assets/Phoenix/res/tex1d/{}.bin");
    Resource::AddPath(PhyResourceType::Tex2D, "./Assets/Phoenix/res/tex2d/{}.jpg");
    Resource::AddPath(PhyResourceType::Tex2D, "./Assets/Phoenix/res/tex2d/{}.png");
    Resource::AddPath(PhyResourceType::Tex3D, "./Assets/Phoenix/res/tex3d/{}.bin");
    Resource::AddPath(PhyResourceType::TexCube, "./Assets/Phoenix/res/texcube/{}");
    Resource::AddPath(PhyResourceType::Other, "./Assets/Phoenix/res/{}");
}

Resource::~Resource() {
}

bool Resource::Exists(PhyResourceType type, const string &name) {
    return Resolve(type, name).empty() ? false : true;
}

string Resource::GetPath(PhyResourceType type, const string &name) {
    return Resolve(type, name);
}

vector<uint32_t> Resource::LoadBytes(PhyResourceType type, const string &name) {
    string path = Resolve(type, name);
    auto data = ReadFileBinary(path.c_str());
    if (data.empty()) LogFatal("Resource_LoadBytes: Failed to load %s <%s> at <%s>", Resource::ToString(type), name, path);
    return data;
}

string Resource::LoadCstr(PhyResourceType type, const string &name) {
    string path = Resolve(type, name);
    string data = ReadFile(path);
    if (data.empty()) LogFatal("Resource_LoadCstr: Failed to load %s <%s> at <%s>", Resource::ToString(type), name, path);
    return data;
}

void Resource::AddPath(PhyResourceType type, const string &format) {
    sPaths[type].push_back({ format });
}

string Resource::Resolve(PhyResourceType type, const string &name) {
    auto entries = sPaths[type];
    if (!entries.empty()) {
        for (auto &entry : entries) {
            auto resource = std::vformat(entry, std::make_format_args(name));
            if (!resource.empty()) {
                if (FileSystemObjectExists(resource)) {
                    return resource;
                }
            }
        }
    }

    // Search literal path
    if (FileSystemObjectExists(name)) {
        return name;
    }

    LogWarning("Could not resolve path of {} '{}'!", ToString(type), name);
    return {};
}

string Resource::ToString(PhyResourceType type) {
    switch (type) {
        case PhyResourceType::Font:    return "Font";
        case PhyResourceType::Mesh:    return "Mesh";
        case PhyResourceType::Other:   return "Other";
        case PhyResourceType::Script:  return "Script";
        case PhyResourceType::Shader:  return "Shader";
        case PhyResourceType::Sound:   return "Sound";
        case PhyResourceType::Tex1D:   return "Tex1D";
        case PhyResourceType::Tex2D:   return "Tex2D";
        case PhyResourceType::Tex3D:   return "Tex3D";
        case PhyResourceType::TexCube: return "TexCube";
    }
    return 0;
}

}

