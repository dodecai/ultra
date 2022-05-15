#pragma once

#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>

#include "Omnia/Utility/String.h"

/**
* @brief Helper: File System Object Information
*/

/* Retrieve the extension of a given file system object. */
static bool FileSystemObjectExists(const std::string &object) {
    return std::filesystem::exists(object);
}

/* Retrieve the extension of a given file system object. */
static const std::string GetFileExtension(const std::string &object) noexcept {
	std::filesystem::path result = object;
	return result.extension().string();
}

/* Retrieve the name of a given file system object. */
static const std::string GetFileName(const std::string &object) noexcept {
	std::filesystem::path result = object;
	return result.stem().string();
}

/* Retrieve the path of a given file system object. */
static const std::string GetFilePath(const std::string &object) noexcept {
	std::filesystem::path result = object;
	return result.parent_path().string();
}

/* Retrieve the root directory of a given file system object. */
static const std::string GetFileRoot(const std::string &object) noexcept {
	std::filesystem::path result = object;
	return result.root_path().string();
}

/**
* @brief Helper: File System Object Operations
*/

/* Read data from file system object. */
static std::string ReadFile(const std::string &file) {
	std::string result;
	std::ifstream stream(file, std::ios::in | std::ios::binary); // ate?
    // if is_open file in use
	if (stream) {
		stream.seekg(0, std::ios::end);
		size_t size = stream.tellg();
		if (size != -1) {
			result.resize(size);
			stream.seekg(0, std::ios::beg);
			stream.read(&result[0], size);
			stream.close();
		} else {
			// Error while reading file
		}
	} else {
		// Error while open file
	}
	return result;
}

/* Test: Read data from file system object. */
static std::string ReadFile2(const std::string &object) {
	static std::ifstream FileStream;
	static std::stringstream FileCache;
	
	FileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		if(object != "") {
			FileStream.open(object);
			FileCache << FileStream.rdbuf();
			FileStream.close();
			//result = FileCache.str();
			FileCache.str(std::string());
			FileCache.clear();
		}
	} catch (std::ifstream::failure error) {
		std::cout << "An error occured while loading shaders!" << std::endl;
	}

	std::ifstream stream(object, std::ios::binary|std::ios::ate|std::ios::in);
	if (!stream) throw std::runtime_error(object + ": " + std::strerror(errno));


	std::string result = "";
	//if (stream) {
	//	std::stringstream FileCache;

	//	FileStream.open(object);
	//	FileCache << FileStream.rdbuf();
	//	FileStream.close();


	//} else {
	//	applog << Log::Error << "The specified file '" << object << "' doesn't exist!\n";
	//}


	//


	//std::string result;
	//std::ifstream stream(file, std::ios::in | std::ios::binary);
	//if (stream) {
	//	stream.seekg(0, std::ios::end);
	//	size_t size = stream.tellg();
	//	if (size != -1) {
	//		result.resize(size);
	//		stream.seekg(0, std::ios::beg);
	//		stream.read(&result[0], size);
	//		stream.close();
	//	} else {
	//		// Error while reading file
	//	}
	//} else {
	//	// Error while open file
	//}
	return result;
}

static void LoadFile(const std::string object) {
	//ofstream fso;
	//fso.open(object);

	//if(fso.is_open()) {
	//	cout << "Done" << endl;

	//	fso.close();
	//} else {
	//	cout << "Could'nt open '" + object + "1" << endl;
	//}

}

static bool WriteFile(const std::string &file, const std::string &data) {
    auto Directory = std::filesystem::path(file).parent_path();
    if (!Directory.empty()) std::filesystem::create_directories(Directory);

    std::ofstream fileStream(file, std::ios::binary);
    fileStream.write(data.data(), data.size());
    fileStream.close();
    return true;
}


static std::vector<std::string> SearchFiles(const std::string &object, const std::string &token) {
    using namespace std::filesystem;

    std::vector<std::string> result;
    auto directory = path(object).parent_path();

    if (exists(directory) && is_directory(directory)) {
        for(auto &object: recursive_directory_iterator(directory, directory_options::skip_permission_denied)) {
            if (object.is_regular_file() && Omnia::String::Contains(object.path().filename().string(), token)) {
                result.push_back(object.path().filename().string());
            }
        }
    }
    return result;
}
