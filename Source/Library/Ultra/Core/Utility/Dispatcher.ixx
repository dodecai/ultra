﻿///
/// @brief Event Dispatcher
/// Base: https://github.com/skypjack/entt/blob/master/src/entt/signal/dispatcher.hpp
///
export module Ultra.Core.Dispatcher;

import Ultra.Core.Signal;

import <entt/entt.hpp>;

namespace {

struct basic_dispatcher_handler {
    virtual ~basic_dispatcher_handler() = default;
    virtual void publish() = 0;
    virtual void disconnect(void *) = 0;
    virtual void clear() noexcept = 0;
    virtual std::size_t size() const noexcept = 0;
};

template<typename Type, typename Allocator>
class dispatcher_handler final: public basic_dispatcher_handler {
    static_assert(std::is_same_v<Type, std::decay_t<Type>>, "Invalid type");

    using alloc_traits = std::allocator_traits<Allocator>;
    using signal_type = Ultra::Signal<void(Type &), Allocator>;
    using container_type = std::vector<Type, typename alloc_traits::template rebind_alloc<Type>>;

public:
    using allocator_type = Allocator;

    dispatcher_handler(const allocator_type &allocator)
        : signal { allocator },
        events { allocator } {
    }

    void publish() override {
        const auto length = events.size();

        for (std::size_t pos {}; pos < length; ++pos) {
            signal.publish(events[pos]);
        }

        events.erase(events.cbegin(), events.cbegin() + length);
    }

    void disconnect(void *instance) override {
        bucket().disconnect(instance);
    }

    void clear() noexcept override {
        events.clear();
    }

    [[nodiscard]] auto bucket() noexcept {
        return typename signal_type::sink_type { signal };
    }

    void trigger(Type event) {
        signal.publish(event);
    }

    template<typename... Args>
    void enqueue(Args &&...args) {
        if constexpr (std::is_aggregate_v<Type>) {
            events.push_back(Type { std::forward<Args>(args)... });
        } else {
            events.emplace_back(std::forward<Args>(args)...);
        }
    }

    std::size_t size() const noexcept override {
        return events.size();
    }

private:
    signal_type signal;
    container_type events;
};

}

export namespace Ultra {

///
/// @brief Basic dispatcher implementation.
/// 
/// A dispatcher can be used either to trigger an immediate event or to enqueue
/// events to be published all together once per tick.<br/>
/// Listeners are provided in the form of member functions. For each event of
/// type `Type`, listeners are such that they can be invoked with an argument of
/// type `Type &`, no matter what the return type is.
/// 
/// The dispatcher creates instances of the `sigh` class internally. Refer to the
/// documentation of the latter for more details.
/// 
/// @tparam Allocator Type of allocator used to manage memory and elements.
///
template<typename Allocator>
class BasicDispatcher {
    template<typename Type>
    using handler_type = dispatcher_handler<Type, Allocator>;

    using key_type = entt::id_type;
    // std::shared_ptr because of its type erased allocator which is useful here
    using mapped_type = std::shared_ptr<basic_dispatcher_handler>;

    using alloc_traits = std::allocator_traits<Allocator>;
    using container_allocator = typename alloc_traits::template rebind_alloc<std::pair<const key_type, mapped_type>>;
    using container_type = entt::dense_map<key_type, mapped_type, entt::identity, std::equal_to<key_type>, container_allocator>;

    template<typename Type>
    [[nodiscard]] handler_type<Type> &assure(const entt::id_type id) {
        static_assert(std::is_same_v<Type, std::decay_t<Type>>, "Non-decayed types not allowed");
        auto &&ptr = pools.first()[id];

        if (!ptr) {
            const auto &allocator = get_allocator();
            ptr = std::allocate_shared<handler_type<Type>>(allocator, allocator);
        }

        return static_cast<handler_type<Type> &>(*ptr);
    }

    template<typename Type>
    [[nodiscard]] const handler_type<Type> *assure(const entt::id_type id) const {
        static_assert(std::is_same_v<Type, std::decay_t<Type>>, "Non-decayed types not allowed");

        if (auto it = pools.first().find(id); it != pools.first().cend()) {
            return static_cast<const handler_type<Type> *>(it->second.get());
        }

        return nullptr;
    }

public:
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;

    /*! @brief Default constructor. */
    BasicDispatcher()
        : BasicDispatcher { allocator_type{} } {}

    /**
     * @brief Constructs a dispatcher with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit BasicDispatcher(const allocator_type &allocator)
        : pools { allocator, allocator } {}

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    BasicDispatcher(BasicDispatcher &&other) noexcept
        : pools { std::move(other.pools) } {}

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    BasicDispatcher(BasicDispatcher &&other, const allocator_type &allocator) noexcept
        : pools { container_type{std::move(other.pools.first()), allocator}, allocator } {
        ENTT_ASSERT(alloc_traits::is_always_equal::value || pools.second() == other.pools.second(), "Copying a dispatcher is not allowed");
    }

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This dispatcher.
     */
    BasicDispatcher &operator=(BasicDispatcher &&other) noexcept {
        ENTT_ASSERT(alloc_traits::is_always_equal::value || pools.second() == other.pools.second(), "Copying a dispatcher is not allowed");

        pools = std::move(other.pools);
        return *this;
    }

    /**
     * @brief Exchanges the contents with those of a given dispatcher.
     * @param other Dispatcher to exchange the content with.
     */
    void swap(BasicDispatcher &other) {
        using std::swap;
        swap(pools, other.pools);
    }

    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return pools.second();
    }

    /**
     * @brief Returns the number of pending events for a given type.
     * @tparam Type Type of event for which to return the count.
     * @param id Name used to map the event queue within the dispatcher.
     * @return The number of pending events for the given type.
     */
    template<typename Type>
    size_type size(const entt::id_type id = entt::type_hash<Type>::value()) const noexcept {
        const auto *cpool = assure<std::decay_t<Type>>(id);
        return cpool ? cpool->size() : 0u;
    }

    /**
     * @brief Returns the total number of pending events.
     * @return The total number of pending events.
     */
    size_type size() const noexcept {
        size_type count {};

        for (auto &&cpool : pools.first()) {
            count += cpool.second->size();
        }

        return count;
    }

    /**
     * @brief Returns a sink object for the given event and queue.
     *
     * A sink is an opaque object used to connect listeners to events.
     *
     * The function type for a listener is _compatible_ with:
     *
     * @code{.cpp}
     * void(Type &);
     * @endcode
     *
     * The order of invocation of the listeners isn't guaranteed.
     *
     * @sa sink
     *
     * @tparam Type Type of event of which to get the sink.
     * @param id Name used to map the event queue within the dispatcher.
     * @return A temporary sink object.
     */
    template<typename Type>
    [[nodiscard]] auto sink(const entt::id_type id = entt::type_hash<Type>::value()) {
        return assure<Type>(id).bucket();
    }

    /**
     * @brief Triggers an immediate event of a given type.
     * @tparam Type Type of event to trigger.
     * @param value An instance of the given type of event.
     */
    template<typename Type>
    void trigger(Type &&value = {}) {
        trigger(entt::type_hash<std::decay_t<Type>>::value(), std::forward<Type>(value));
    }

    /**
     * @brief Triggers an immediate event on a queue of a given type.
     * @tparam Type Type of event to trigger.
     * @param value An instance of the given type of event.
     * @param id Name used to map the event queue within the dispatcher.
     */
    template<typename Type>
    void trigger(const entt::id_type id, Type &&value = {}) {
        assure<std::decay_t<Type>>(id).trigger(std::forward<Type>(value));
    }

    /**
     * @brief Enqueues an event of the given type.
     * @tparam Type Type of event to enqueue.
     * @tparam Args Types of arguments to use to construct the event.
     * @param args Arguments to use to construct the event.
     */
    template<typename Type, typename... Args>
    void enqueue(Args &&...args) {
        enqueue_hint<Type>(entt::type_hash<Type>::value(), std::forward<Args>(args)...);
    }

    /**
     * @brief Enqueues an event of the given type.
     * @tparam Type Type of event to enqueue.
     * @param value An instance of the given type of event.
     */
    template<typename Type>
    void enqueue(Type &&value) {
        enqueue_hint(entt::type_hash<std::decay_t<Type>>::value(), std::forward<Type>(value));
    }

    /**
     * @brief Enqueues an event of the given type.
     * @tparam Type Type of event to enqueue.
     * @tparam Args Types of arguments to use to construct the event.
     * @param id Name used to map the event queue within the dispatcher.
     * @param args Arguments to use to construct the event.
     */
    template<typename Type, typename... Args>
    void enqueue_hint(const entt::id_type id, Args &&...args) {
        assure<Type>(id).enqueue(std::forward<Args>(args)...);
    }

    /**
     * @brief Enqueues an event of the given type.
     * @tparam Type Type of event to enqueue.
     * @param id Name used to map the event queue within the dispatcher.
     * @param value An instance of the given type of event.
     */
    template<typename Type>
    void enqueue_hint(const entt::id_type id, Type &&value) {
        assure<std::decay_t<Type>>(id).enqueue(std::forward<Type>(value));
    }

    /**
     * @brief Utility function to disconnect everything related to a given value
     * or instance from a dispatcher.
     * @tparam Type Type of class or type of payload.
     * @param value_or_instance A valid object that fits the purpose.
     */
    template<typename Type>
    void disconnect(Type &value_or_instance) {
        disconnect(&value_or_instance);
    }

    /**
     * @brief Utility function to disconnect everything related to a given value
     * or instance from a dispatcher.
     * @tparam Type Type of class or type of payload.
     * @param value_or_instance A valid object that fits the purpose.
     */
    template<typename Type>
    void disconnect(Type *value_or_instance) {
        for (auto &&cpool : pools.first()) {
            cpool.second->disconnect(value_or_instance);
        }
    }

    /**
     * @brief Discards all the events stored so far in a given queue.
     * @tparam Type Type of event to discard.
     * @param id Name used to map the event queue within the dispatcher.
     */
    template<typename Type>
    void clear(const entt::id_type id = entt::type_hash<Type>::value()) {
        assure<Type>(id).clear();
    }

    /*! @brief Discards all the events queued so far. */
    void clear() noexcept {
        for (auto &&cpool : pools.first()) {
            cpool.second->clear();
        }
    }

    /**
     * @brief Delivers all the pending events of a given queue.
     * @tparam Type Type of event to send.
     * @param id Name used to map the event queue within the dispatcher.
     */
    template<typename Type>
    void update(const entt::id_type id = entt::type_hash<Type>::value()) {
        assure<Type>(id).publish();
    }

    /*! @brief Delivers all the pending events. */
    void update() const {
        for (auto &&cpool : pools.first()) {
            cpool.second->publish();
        }
    }

private:
    entt::compressed_pair<container_type, allocator_type> pools;
};

template<typename = std::allocator<void>>
class BasicDispatcher;

/// @brief Alias declaration for the most common use case.
using Dispatcher = BasicDispatcher<>;


// ToDo: Merge

//// Dispatcher
//template <typename>
//class Dispatcher;
//
//template<typename R, typename ...Arguments>
//class Dispatcher<R(Arguments...)> {
//    using Callback = std::function<R(Arguments ...)>;
//    using Callbacks = std::unordered_map<size_t, unique_ptr<Callback>>;
//
//public:
//    // Default
//    Dispatcher() noexcept = default;
//    Dispatcher(const Dispatcher &) = delete;
//    Dispatcher(Dispatcher &&) = delete;
//    ~Dispatcher() { mCallbacks.clear(); };
//
//    // Operators
//    bool Empty() const {
//        return mCallbacks.empty();
//    }
//
//    void operator=(Dispatcher &) = delete;
//    operator bool() {
//        return Empty();
//    }
//
//    R operator()(Arguments &&...arguments) {
//        return Publish(std::forward<Arguments>(arguments)...);
//    }
//
//    template <typename T>
//    Dispatcher &operator+=(T &&callback) {
//        Attach(std::forward<Callback>(callback));
//        return *this;
//    }
//
//    template <typename T>
//    Dispatcher &operator-=(T &&callback) {
//        Detach(std::forward<Callback>(callback));
//        return *this;
//    }
//
//private:
//    // Methods
//    template <typename T>
//    void Attach(T &&callback) {
//        std::unique_lock<std::mutex> lock(mMutex);
//        auto key = std::type_index(callback.target_type()).hash_code();
//
//        auto name = std::type_index(callback.target_type()).name();
//        logger << name << " | " << key << std::endl;
//        
//        mCallbacks.emplace(key, std::make_unique<Callback>(std::forward<Callback>(callback)));
//
//    }
//
//    template <typename T>
//    void Detach(T &&callback) {
//        std::unique_lock<std::mutex> lock(mMutex);
//        auto key = std::type_index(callback.target_type()).hash_code();
//
//        auto it = mCallbacks.find(key);
//        if (it != mCallbacks.end()) {
//            mCallbacks.erase(it);
//        }
//    }
//
//    R Publish(Arguments &&...arguments) {
//        std::unique_lock<std::mutex> lock(mMutex);
//        if constexpr (std::is_same_v<R, bool>) {
//            bool handled = false;
//            for (auto &callback : mCallbacks) {
//                if ((*callback.second)) {
//                    handled = (*callback.second)(arguments ...);
//                    if (handled) { return true; }
//                } else {
//                    mCallbacks.erase(callback.first);
//                }
//            }
//            return false;
//        } else {
//            for (auto &callback : mCallbacks) {
//                if ((*callback.second)) {
//                    (*callback.second)(arguments ...);
//                } else {
//                    mCallbacks.erase(callback.first);
//                }
//            }
//        }
//    }
//
//private:
//    // Properties
//    Callbacks mCallbacks = {};
//    std::mutex mMutex = {};
//};

}
