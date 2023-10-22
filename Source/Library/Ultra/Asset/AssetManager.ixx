export module Ultra.Asset.Manager;

import Ultra.Core;
import Ultra.Core.String;
import Ultra.Logger;
import Ultra.Asset;
import Ultra.System.FileSystem;

export namespace Ultra {

class AssetManager {
    using AssetFiles = unordered_map<AssetType, vector<string>>;
    using AssetMappings = unordered_map<AssetType, vector<string>>;

public:
    // Constructors & Destructors
    AssetManager() {
        mAssetMappings = {
            { AssetType::Font,    { "otf", "ttf" } },
            { AssetType::Image,   { "jpg", "png" } },
            { AssetType::Model,   { "obj" } },
            { AssetType::Script,  { "cs", "lua" } },
            { AssetType::Shader,  { "glsl", "vert", "frag", "spriv_v" } },
            { AssetType::Sound,   { "mp3", "ogg", "wav" } },
            { AssetType::Texture, { "jpg", "png" } },
        };

    };
    ~AssetManager() = default;

    // Methods
    static AssetManager &Instance() {
        static AssetManager instance {};
        return instance;
    }

    void Load(string_view root = "Assets/") {
        if (mAssetMappings.empty()) {
            LogError("The asset extension mappings are empty!");
            return;
        }

        for (auto &[type, extensions] : mAssetMappings) {
            LogInfo("Loading '{}' assets from '{}' ...", to_string(type), root);

            auto files = Directory::GetFiles(root, String::Join(extensions, '|'));
            if (!files.empty()) mAssets[type].insert(mAssets[type].end(), files.begin(), files.end());
        }
    }
    void Reset() {
        mAssets.clear();
    }
    string Resolve(AssetType type, string_view path) {
        auto &resources = mAssets[type];

        for (auto &resource : resources) {
            if (String::Contains(resource, path)) {
                return resource;
            }
        }
        LogWarning("Could not resolve asset '{}' under '{}'!", to_string(type), path);
        return {};
    }

    vector<uint32_t> GetAsByteArray(AssetType type, string_view name) {
        if (AssetType::Unknown == type) return {};
        auto data = File::LoadAsBinary(name);
        return data;
    }
    string GetAsString(AssetType type, string_view name) {
        if (AssetType::Unknown == type) return {};
        string data = File::LoadAsString(name);
        return data;
    }

private:
    // Properties
    AssetFiles mAssets;
    AssetMappings mAssetMappings;
};

}
