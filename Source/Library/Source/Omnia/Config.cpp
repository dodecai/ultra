#include "Config.h"

namespace Omnia {

Config::Config(const string &object) {
	ConfigFile = object;
	try {
		ConfigData = YAML::LoadFile(ConfigFile);
	} catch (std::exception ex) {
		AppLogError("[Config] Something went wrong while loading Config!");
	}
	
	try {
		if (ConfigData["App"]) {
			AppCaption = ConfigData["App"]["Caption"].as<std::string>();
			AppVersion = ConfigData["App"]["Version"].as<std::string>();
		}

		if (ConfigData["Settings"]["Engine"]) {
			string delimiter = "x";
			string resolution = ConfigData["Settings"]["Engine"]["Resolution"].as<std::string>();
			string tokenW = resolution.substr(0, resolution.find(delimiter));
			string tokenH = resolution.substr(resolution.find(delimiter) + 1);
			std::istringstream(tokenH) >> WindowHeight;
			std::istringstream(tokenW) >> WindowWidth;
		}
	} catch (std::exception ex) {
		std::cout << "Something went wrong!" << "\n";
	}
}

}
