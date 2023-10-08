export module Ultra.Engine.AssetManager;

import Ultra.CoreNew;
import Ultra.Engine.Asset;

export namespace Ultra {

class AssetImporter {
public:
    void LoadAssets() {

    }

private:
   unordered_map<string, Reference<Asset>> mAssets;
};

}
