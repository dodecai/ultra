export module Ultra.Config;

import Ultra.Core;

export namespace Ultra {

///
/// @brief Application Configuration
/// @note under construction!
/// 
class Config {
public:
    Config() = delete;
    ~Config() = delete;

    void Load(const string &object) {
        mConfigFile = object;
        //try {
        //    ConfigData = YAML::LoadFile(ConfigFile);
        //} catch (std::exception ex) {
        //    AppLogError("[Config] Something went wrong while loading Config!");
        //}

        //try {
        //    if (ConfigData["App"]) {
        //        AppCaption = ConfigData["App"]["Caption"].as<std::string>();
        //        AppVersion = ConfigData["App"]["Version"].as<std::string>();
        //    }

        //    if (ConfigData["Settings"]["Engine"]) {
        //        string delimiter = "x";
        //        string resolution = ConfigData["Settings"]["Engine"]["Resolution"].as<std::string>();
        //        string tokenW = resolution.substr(0, resolution.find(delimiter));
        //        string tokenH = resolution.substr(resolution.find(delimiter) + 1);
        //        std::istringstream(tokenH) >> WindowHeight;
        //        std::istringstream(tokenW) >> WindowWidth;
        //    }
        //} catch (std::exception ex) {
        //    std::cout << "Something went wrong!" << "\n";
        //}
    }

    template <typename T = string>
    T GetSetting(const string &key, const string &value) const {
        //if (ConfigData["Settings"][key][value].IsDefined()) {
        //    try {
        //        return ConfigData["Settings"][key][value].as<T>();
        //    } catch (std::exception ex) {
        //        AppLogError("[Config]: Could not deduce the type of '", key, ":", value, "'!");
        //        return T {};
        //    }
        //} else {
        //    AppLogWarning("[Config]: The requested setting '", key, ":", value, "' doesn't exist!");
        //}
        return T {};
    }

private:
    string mConfigFile = "null";
    //YAML::Node mConfigData;

    string AppCaption;
    string AppVersion;

    size_t WindowHeight;
    size_t WindowWidth;
};

}
