module;

#define YAML_CPP_STATIC_DEFINE

export module Ultra.Config;

import Ultra.Core;
import Ultra.Logger;

import <yaml-cpp/yaml.h>;

export namespace Ultra {

///
/// @brief Configuration
/// @note ToDo: Instead of fixed properties, we want tu use the multimap directly
/// 
/// @example
/// Scope<Config> config = CreateScope<Config>();
/// config->Load("Data/Config.yml");
/// auto setting = config->GetSetting<string>("key", "value");
///
class Config {
public:
    Config() = default;
    ~Config() = default;

    void Load(const string &file) {
        mConfigFile = file;
        try {
            //mConfigData = YAML::LoadFile(mConfigFile);
        } catch (std::exception ex) {
            LogError("Something went wrong while loading Config!");
        }

        try {
            //if (mConfigData["App"]) {
            //    AppCaption = mConfigData["App"]["Caption"].as<std::string>();
            //    AppDescription = mConfigData["App"]["Description"].as<std::string>();
            //    AppVersion = mConfigData["App"]["Version"].as<std::string>();
            //}

            //if (mConfigData["Settings"]["Engine"]) {
            //    string delimiter = "x";
            //    string resolution = mConfigData["Settings"]["Engine"]["Resolution"].as<std::string>();
            //    string tokenW = resolution.substr(0, resolution.find(delimiter));
            //    string tokenH = resolution.substr(resolution.find(delimiter) + 1);
            //    std::istringstream(tokenH) >> WindowHeight;
            //    std::istringstream(tokenW) >> WindowWidth;
            //}
        } catch (std::exception ex) {
            std::cout << "Something went wrong!" << "\n";
        }
    }

    template <typename T = string>
    T GetSetting(const string &key, const string &value) const {
        //if (mConfigData["Settings"][key][value].IsDefined()) {
            try {
                //return mConfigData["Settings"][key][value].as<T>();
            } catch (std::exception ex) {
                LogError("Could not deduce the type of '{}:{}'!", key, value);
                return T {};
            }
    /*    } else {
            LogWarning("The requested setting '{}:{}' doesn't exist!", key, value);
        }*/
        return T {};
    }

private:
    string mConfigFile {};
    //YAML::Node mConfigData;

    string AppCaption {};
    string AppDescription {};
    string AppVersion {};

    size_t WindowHeight {};
    size_t WindowWidth {};
};

}
