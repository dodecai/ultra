export module Application;

namespace Ultra {

class Application {
public:
    Application() = default;
    virtual ~Application() = default;

    virtual void Create() {};
    virtual void Destroy() {};
    virtual void Update() {};

private:

};

}
