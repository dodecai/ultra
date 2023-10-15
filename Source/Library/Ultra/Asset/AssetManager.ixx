export module Ultra.Asset.Manager;

import Ultra.Core;
import Ultra.Core.String;
import Ultra.Logger;
import Ultra.Asset;
import Ultra.System.FileSystem;

export namespace Ultra {

class AssetManager {
    using AssetFiles = unordered_map<Asset, vector<string>>;
    using AssetMappings = unordered_map<AssetType, vector<string>>;

public:
    // Constructors & Destructors
    AssetManager() {
        mAssetExtensionMappings = {
            { AssetType::Font,    { "otf", "ttf" } },
            { AssetType::Image,   { "jpg", "png" } },
            { AssetType::Model,   { "obj" } },
            { AssetType::Script,  { "cs", "lua" } },
            { AssetType::Shader,  { "glsl", "vert", "frag", "spriv_v" } },
            { AssetType::Sound,   { "mp3", "ogg", "wav" } },
        };

    };
    ~AssetManager() = default;

    // Methods
    static AssetManager &Instance() {
        static AssetManager instance {};
        return instance;
    }

    void Load(string_view root = "Assets/") {
        if (mAssetExtensionMappings.empty()) {
            LogError("The asset extension mappings are empty!");
            return;
        }

        for (auto &[type, extensions] : mAssetExtensionMappings) {
            LogInfo("Loading '{}' assets from '{}' ...", to_string(type), root);

            auto files = Directory::GetFiles(root, String::Join(extensions, '|'));
            if (!files.empty()) mAssets[type].insert(mAssets[type].end(), files.begin(), files.end());
        }
    }
    void Reset() {
        mAssets.clear();
    }
    string Resolve(AssetType type, string_view path) {
        

        //auto entries = sPaths[type];
        //if (!entries.empty()) {
        //    for (auto &entry : entries) {
        //        auto resource = std::vformat(entry, std::make_format_args(name));
        //        if (!resource.empty()) {
        //            if (File::Exists(resource)) {
        //                return resource;
        //            }
        //        }
        //    }
        //}

        //// Search literal path
        //if (File::Exists(name)) {
        //    return name;
        //}

        //LogWarning("Could not resolve path of {} '{}'!", ToString(type), name);
        return {};
    }

    bool Exists(string_view name) {
        //return mAssets.find(name.data()) != mAssets.end();
    }
    vector<uint32_t> GetAsByteArray(AssetType type, string_view name) {
        //string path = Resolve(type, name);
        //auto data = File::LoadAsBinary(path.c_str());
        //if (data.empty()) LogFatal("Resource_LoadBytes: Failed to load {} <{}> at <{}>", Resource::ToString(type), name, path);
        //return data;

        return {};
    }
    string GetAsString(AssetType type, string_view name) {
        //string path = Resolve(type, name);
        //string data = File::LoadAsString(path);
        //if (data.empty()) LogFatal("Resource_LoadCstr: Failed to load {} <{}> at <{}>", Resource::ToString(type), name, path);
        //return data;

        return {};
    }

private:
    static void AddPath(PhyResourceType, const string &format);
    static string Resolve(PhyResourceType type, const string &name);
    static string ToString(PhyResourceType);

private:
    // Properties
    unordered_map<AssetType, vector<string>> mAssets;
    unordered_map<AssetType, vector<string>> mAssetExtensionMappings;
};

}
