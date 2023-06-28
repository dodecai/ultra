#include <Settings.h>
#include <Ultra/EntryPoint.h>

import Ultra;

namespace Ultra {

// Application
class Spectra: public Application {
public:
    // Constructors and Destructor
    Spectra(const ApplicationProperties &properties): Application(properties) {}
    ~Spectra() = default;

    // Methods
    void Create() {}
    void Destroy() {}
    void Update([[maybe_unused]] Timestamp deltaTime) {}

    // Styles
    string AsciiLogo() {
        return 1 + R"(
      ____ ___.__   __                     _________                    __                 
     |    |   \  |_/  |_____________      /   _____/_____   ____  _____/  |_____________   
     |    |   /  |\   __\_  __ \__  \     \_____  \\____ \_/ __ \/ ___\   __\_  __ \__  \  
     |    |  /|  |_|  |  |  | \// __ \_   /        \  |_> >  ___|  \___|  |  |  | \// __ \_
     |______/ |____/__|  |__|  (____  /  /_______  /   __/ \___  >___  >__|  |__|  (____  /
                                    \/           \/|__|        \/    \/                 \/ )";
    }
};

// Application Entry-Point
Application *CreateApplication() {
    return new Spectra({ "Spectra", "1280x1024" });
}

}
