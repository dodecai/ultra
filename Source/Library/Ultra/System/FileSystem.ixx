export module Ultra.System.FileSystem;

import Ultra.Core;
import Ultra.Core.String;
import Ultra.Logger;

///
/// @brief Helper: File System Object Information
///
export namespace Ultra {

class Directory {
public:
    // Check if a given directory exists.
    inline static bool Exists(string_view object) noexcept {
        return std::filesystem::exists(object.data()) && std::filesystem::is_directory(object.data());
    }

    // Retrieve files from a given directory.
    inline static vector<string> GetFiles(string_view object, string_view token) {
        auto directory = std::filesystem::path(object.data());

        if (!Exists(directory.string())) { LogError("The specified directory '{}' doesn't exist!", object.data()); return {}; };

        vector<string> result;
        for (auto &current : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
            if (current.is_regular_file() && Ultra::String::Contains(current.path().filename().string(), token)) {
                result.push_back(current.path().filename().string());
            }
        }
        return result;
    }
    
    // Validate given directory is valid.
    inline static bool ValidatePath(string_view object) {
        return std::filesystem::exists(object.data());
    }
};

class File {
public:
    // Check if a given file exists.
    inline static bool Exists(string_view object) noexcept {
        return std::filesystem::exists(object.data()) && std::filesystem::is_regular_file(object.data());
    }

    // Retrieve the extension of a given file.
    inline static string GetExtension(string_view object) noexcept {
        std::filesystem::path result { object.data() };
        return result.extension().string();
    }

    // Retrieve the name of a given file.
    inline static string GetName(string_view object) noexcept {
        std::filesystem::path result { object.data() };
        return result.stem().string();
    }

    // Retrieve the path of a given file.
    inline static string GetPath(string_view object) noexcept {
        std::filesystem::path result { object.data() };
        return result.parent_path().string();
    }

    // Retrieve the root directory of a given file.
    inline static string GetRoot(string_view object) noexcept {
        std::filesystem::path result { object.data() };
        return result.root_path().string();
    }

    // Load a file as binary data.
    template <typename R = uint32_t>
    inline static vector<R> LoadAsBinary(string_view object) {
        std::ifstream stream(object.data(), std::ios::binary | std::ios::ate);
        if (!stream) { LogError("Error occurred while opening binary file '{}'!", object.data()); return {}; }

        auto size = stream.tellg();
        if (!stream) { LogError("Error occurred while loading binary file '{}'!", object.data()); return {}; }

        stream.seekg(0, std::ios::beg);
        vector<R> buffer(size / sizeof(R));

        stream.read((char *)buffer.data(), size);
        if (stream.fail()) { LogError("Error occurred while reading binary file '{}'!", object.data()); return {}; }

        return buffer;
    }

    // Load a file as string data.
    inline static string LoadAsString(string_view object) {
        std::ifstream stream(object.data());
        if (!stream) { LogError("Error occurred while opening text file '{}'!", object.data()); return {}; }

        string buffer(std::istreambuf_iterator<char>{stream}, {});
        return buffer;
    }

    // Load a file as text data (per line/token).
    inline static vector<string> LoadAsText(string_view object, bool tokenize = false) {
        std::ifstream stream(object.data());
        if (!stream) { LogError("Error occurred while opening text file '{}'!", object.data()); return {}; }

        vector<string> buffer;
        if (tokenize) {
            buffer = {
                std::istream_iterator<string>(stream),
                std::istream_iterator<string>{}
            };
        } else {
            string line;
            while (std::getline(stream, line)) {
                 buffer.push_back(line);
            }
        }
        return buffer;
    }

    // Write binary data to a file.
    template <typename T = uint32_t>
    inline static bool Write(string_view object, const std::vector<T> &data) {
        auto Directory = std::filesystem::path(object.data()).parent_path();
        if (!Directory.empty()) std::filesystem::create_directories(Directory);

        std::ofstream stream(object.data(), std::ios::binary);
        stream.write((char *)data.data(), data.size() * sizeof(T));
        stream.flush();
        return stream.good();
    }

    // Write text data to a file.
    inline static bool Write(string_view object, const std::string &data) {
        auto Directory = std::filesystem::path(object.data()).parent_path();
        if (!Directory.empty()) std::filesystem::create_directories(Directory);

        std::ofstream stream(object.data());
        stream.write(data.data(), data.size());
        return stream.good();
    }
};

void TestFileSystem() {
    // Setup
    const string directory = "Test";
    const string file = "Test.txt";
    const string content = "Hello World!";

    // Test Directory::Exists
    std::filesystem::create_directories(directory);
    //AppAssert(Directory::Exists(test_directory) == true);
    std::filesystem::remove_all(directory);

    // Test Directory::GetFiles
    std::filesystem::create_directories(directory);
    std::ofstream validateFile(directory + "/" + file);
    validateFile << content;
    validateFile.close();

    auto files = Directory::GetFiles(directory, file);
    //AppAssert(files.size() == 1);
    //AppAssert(files[0] == file);
    std::filesystem::remove_all(directory);

    // Test File::Exists
    std::ofstream validateFile2(file);
    validateFile2 << content;
    validateFile2.close();
    //AppAssert(File::Exists(file) == true);
    std::filesystem::remove(file);
}

}
