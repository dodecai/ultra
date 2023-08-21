module;

#define YAML_BUILD_SHARED_LIBS OFF
#include <yaml-cpp/yaml.h>

export module Ultra.Config;

import Ultra.Core;
import Ultra.Logger;

export namespace Ultra {

///
/// @brief Application Configuration
/// @note under construction!
/// 
class Config {
public:
    Config() {};
    ~Config() = delete;

    void Load(const string &object) {
        mConfigFile = object;
        try {
            mConfigData = YAML::LoadFile(mConfigFile);
        } catch (std::exception ex) {
            LogError("[Config] Something went wrong while loading Config!");
        }

        try {
            if (mConfigData["App"]) {
                AppCaption = mConfigData["App"]["Caption"].as<std::string>();
                AppVersion = mConfigData["App"]["Version"].as<std::string>();
            }

            if (mConfigData["Settings"]["Engine"]) {
                string delimiter = "x";
                string resolution = mConfigData["Settings"]["Engine"]["Resolution"].as<std::string>();
                string tokenW = resolution.substr(0, resolution.find(delimiter));
                string tokenH = resolution.substr(resolution.find(delimiter) + 1);
                std::istringstream(tokenH) >> WindowHeight;
                std::istringstream(tokenW) >> WindowWidth;
            }
        } catch (std::exception ex) {
            std::cout << "Something went wrong!" << "\n";
        }
    }

    template <typename T = string>
    T GetSetting(const string &key, const string &value) const {
        if (mConfigData["Settings"][key][value].IsDefined()) {
            try {
                return mConfigData["Settings"][key][value].as<T>();
            } catch (std::exception ex) {
                LogError("[Config]: Could not deduce the type of '", key, ":", value, "'!");
                return T {};
            }
        } else {
            LogWarning("[Config]: The requested setting '", key, ":", value, "' doesn't exist!");
        }
        return T {};
    }

private:
    string mConfigFile = "null";
    YAML::Node mConfigData;

    string AppCaption;
    string AppVersion;

    size_t WindowHeight;
    size_t WindowWidth;
};

}
