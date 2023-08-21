export module Ultra.Core.Object;

import <exception>;
import <mutex>;

///
/// @brief Contains classes for managing object behavior and immutability.
/// They can be used as base classes to ensure specific behaviors.
/// @note The classes here are useful for controlling copying, moving, and modification of objects in the application.
///

export namespace Ultra {

///
/// @brief Removes the move constructer/operator from a derived class.
///
class CopyableObject {
protected:
    CopyableObject() = default;
    virtual ~CopyableObject() = default;

private:
    CopyableObject(CopyableObject &&) noexcept = delete;
    CopyableObject &operator=(CopyableObject &&) noexcept = delete;
};

///
/// @brief Removes the copy constructer/operator from derived class.
///
class MoveableObject {
protected:
    MoveableObject() = default;
    virtual ~MoveableObject() = default;

private:
    MoveableObject(const MoveableObject &) = delete;
    MoveableObject &operator=(const MoveableObject &) = delete;
};

///
/// @brief Removes the copy and move constructer/operator from derived class.
///
class SteadyObject {
protected:
    SteadyObject() = default;
    virtual ~SteadyObject() = default;

private:
    SteadyObject(const SteadyObject &) = delete;
    SteadyObject(SteadyObject &&) noexcept = default;
    SteadyObject &operator=(const SteadyObject &) = delete;
    SteadyObject &operator=(SteadyObject &&) noexcept = default;
};


///
/// @brief Restricts access to certain methods or properties of objects.
///
class AccessControlledObject {
protected:
    AccessControlledObject() = default;
    virtual ~AccessControlledObject() = default;

    void Restricted() {
        if (!mAllowedAccess) throw std::runtime_error("Access not allowed.");
    }

private:
    bool mAllowedAccess {};
};

///
/// @brief Facilitates object configuration by providing preset options.
/// @note Left for future use.
///
class ConfigurationObject {
protected:
    ConfigurationObject() = default;
    virtual ~ConfigurationObject() = default;
};

///
/// @brief Provides a template for objects that can be serialized and deserialized.
/// @tparam T The data type that represents the serialized form of the object.
/// 
/// @example
/// class ExampleClass : public SerializableObject<string> {
/// public:
///     string Serialize() override {
///         // Implement serialization logic to convert object data to a string
///     }
/// 
///     void Deserialize(const string &data) override {
///         // Implement deserialization logic to restore object data from the string
///     }
/// };
///
template <typename T>
class SerializableObject {
public:
    SerializableObject() = default;
    virtual ~SerializableObject() = default;

    virtual T Serialize() = 0;
    virtual void Deserialize(const T &data) = 0;
};

///
/// @brief Ensures thread-safe usage of objects through locking mechanisms.
/// 
/// @example
/// ThreadSafeObject::LockGuard lock(*this);
///
class ThreadSafeObject {
protected:
    ThreadSafeObject() = default;
    virtual ~ThreadSafeObject() = default;

    class LockGuard {
    public:
        LockGuard(ThreadSafeObject &object): mObject(object) { mObject.Lock(); }
        ~LockGuard() { mObject.Unlock(); }

    private:
        ThreadSafeObject &mObject;
    };

    void Lock() {
        mMutex.lock();
    }
    void Unlock() {
        mMutex.unlock();
    }

private:
    std::mutex mMutex;
};

}
