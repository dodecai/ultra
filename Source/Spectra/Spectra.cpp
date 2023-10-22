import <Settings.h>;
import <Ultra/EntryPoint.h>;
import Ultra;
import Ultra.Spectra.Designer;

namespace Ultra {

using namespace Ultra::UI;

// Application
class Spectra: public Application {
public:
    // Constructors and Destructor
    Spectra(const ApplicationProperties &properties): Application(properties) {
        AssetManager::Instance().Load();
    }
    ~Spectra() = default;

    // Methods
    void Create() {
        PushLayer(new Designer());
        mRenderer = Renderer::Create();
    }

    void Destroy() {
    }

    void Update(Timestamp deltaTime) {
        HmGui::ShowDemo();
        mRenderer->RenderFrame();
        HmGui::Update(deltaTime);
        HmGui::Draw();
    }

    // Styles
    string AsciiLogo() override {
        return 1 + R"(
      ____ ___.__   __                     _________                    __                 
     |    |   \  |_/  |_____________      /   _____/_____   ____  _____/  |_____________   
     |    |   /  |\   __\_  __ \__  \     \_____  \\____ \_/ __ \/ ___\   __\_  __ \__  \  
     |    |  /|  |_|  |  |  | \// __ \_   /        \  |_> >  ___|  \___|  |  |  | \// __ \_
     |______/ |____/__|  |__|  (____  /  /_______  /   __/ \___  >___  >__|  |__|  (____  /
                                    \/           \/|__|        \/    \/                 \/ )";
    }

private:
    Scope<Renderer> mRenderer;
};

// Application Entry-Point
Application *CreateApplication() {
    return new Spectra({ "Spectra", "1280x1024", GraphicsAPI::OpenGL });
}

}
