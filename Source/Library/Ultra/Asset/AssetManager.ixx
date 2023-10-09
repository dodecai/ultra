export module Ultra.Asset.Manager;

import Ultra.Core;
import Ultra.Asset;

export namespace Ultra {

class AssetImporter {
public:
    void LoadAssets() {

    }

private:
   unordered_map<string, Reference<Asset>> mAssets;
};

}
