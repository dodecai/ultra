#pragma once
#pragma warning(push)
#pragma warning(disable: 4930)

#include "Omnia/Omnia.pch"

namespace Omnia {

/**
 * @brief EventData Interface and EventPriority
*/
enum class EventPriority {
	// HowTo # Extend EventPriority: typedef DeriveEnum<EventPriorityExtended, EventPriority> EventPriorityX;
	Null		= 0x00,
	Low			= 0x01,
	Normal		= 0x05,
	High		= 0x09,
	Realtime	= 0x0D,
	Undefined	= 0xFF,
};

struct IEventData {
	std::string Event = "";
	EventPriority Priority = EventPriority::Null;

	IEventData() = default;
	IEventData(const std::string_view &event, EventPriority priority = EventPriority::Normal):
		Event {event},
		Priority {priority} {};
	virtual ~IEventData() = default;
};

/**
* @brief This interface holds the message data
*/
class IMessage {
	IEventData Data;

public:
	IMessage(const IEventData &data): Data {data} {};
	IMessage(const std::string &data, EventPriority priority = EventPriority::Normal) {
		Data.Event = data;
		Data.Priority = priority;
	};
	virtual ~IMessage() = default;
	
	const IEventData &GetData() { return Data; }
};


/**
* @brief Observer: Notifies all observers
*/
class Observer {
	std::shared_ptr<void> Object = nullptr;

public:
	Observer(std::shared_ptr<void> &&object): Object(std::move(object)) {};
	void Destroy() { Object = nullptr; };
};

/**
* @brief Subject: The subject to subscribe
*/
template<typename ... Arguments>
class Subject {
	using Callback = function<void(Arguments ...)>;

	mutex Mutex;
	vector<weak_ptr<Callback>> Callbacks;

public:
	Subject() = default;
	virtual ~Subject() = default;

	Observer &operator+=(Callback &&callback) {
		return Subscribe(callback);
	};
	Observer &operator-=(const Callback callback) {
		return Observer{};
	};


	Observer Subscribe(const Callback &callback) {
		std::shared_ptr<Callback> sharedCallback = std::make_shared<Callback>(callback);
		const std::lock_guard<std::mutex> lock(mutex);
		Callbacks.push_back(sharedCallback);
		return Observer(std::move(sharedCallback));
	}
	Observer Subscribe(Callback &&callback) {
		std::shared_ptr<Callback> sharedCallback = std::make_shared<Callback>(std::move(callback));
		const std::lock_guard<std::mutex> lock(mutex);
		Callbacks.push_back(sharedCallback);
		return Observer(std::move(sharedCallback));
	};
	
	void Unsubscribe(Observer observer) {
		const std::lock_guard<std::mutex> lock(mutex);
		Callbacks.erase(std::remove_if(Callbacks.begin(), Callbacks.end(), observer));
	}
	void Unsibscribe() {
		//std::remove(Receivers.begin(), Receivers.end(), receiver);
	}


	void Publish(Arguments ... arguments) {
		std::vector<std::shared_ptr<Callback>> callbacks;
		std::vector<std::weak_ptr<Callback>> validCallbacks;

		{
			const std::lock_guard<std::mutex> lock(mutex);
			validCallbacks.reserve(Callbacks.size());
			for (auto& callback : Callbacks) {
				if (auto sharedFn = callback.lock()) {
					callbacks.push_back(std::move(sharedFn));
					validCallbacks.push_back(std::move(callback));
				}
			}
			Callbacks = std::move(validCallbacks);
		}

		for (const auto& callback : callbacks) {
			(*callback)(arguments ...);
		}
	};
	

	bool Empty() const {
		return Callbacks.empty();
	}
};

}

#pragma warning(pop)
