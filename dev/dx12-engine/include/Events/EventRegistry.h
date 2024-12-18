#pragma once

#include "EventsCommon.h"

#include <array>
#include <memory>

namespace dxe
{
	class CallbackStorageBase
	{
	public:

		virtual void NotifyAll() = 0;

		virtual void ClearAll();

		virtual void ClearCallbacks() = 0;
		virtual void ClearCallbackData() = 0;
	};

	// This is a storage for "event data" and "event callbacks" of the event of a particular type

	template <typename CallbackData>
	class CallbackStorage : public CallbackStorageBase
	{
	public:

		void NotifyAll() override
		{
			for (auto& data : callbackData)
			{
				for (auto& callback : callbacks)
				{
					callback.Invoke(data);
				}
			}
		}
		void NotifyAll(const CallbackData& callbackData)
		{
			for (auto& callback : callbacks)
			{
				callback.Invoke(callback);
			}
		}

		void ClearCallbacks() override
		{
			callbacks.clear();
		}
		void ClearCallbackData() override
		{
			callbackData.clear();
		}

		std::vector<Callback<CallbackData>> callbacks;
		std::vector<CallbackData> callbackData;
	};

	class EventRegistry
	{
	public:

		void Tick();

		// Callback registration methods

		template<typename CallbackData>
		int RegisterCallback(typename CallbackData::Callable callable)
		{
			CallbackType type = CallbackData::EventType;
			int typeIdx = static_cast<int>(type);

			Callback<CallbackData> callback{ callable };
			typename Callback<CallbackData>::CallbackId callbackId = callback.GetCallbackId();

			if (!callbackStorage[typeIdx])
				CreateCallbackStorage<CallbackData>();

			std::shared_ptr<CallbackStorage<CallbackData>> typedCallbackStorage =
				GetTypedCallbackStorage<CallbackData>();

			typedCallbackStorage->callbacks.push_back(callback);

			return callbackId;
		}

		// Non-delayed notification methods

		template<typename CallbackData>
		void NotifyCallbackEvent(const CallbackData& callbackData)
		{
			CallbackType type = CallbackData::EventType;
			int typeIdx = static_cast<int>(type);

			if (!callbackStorage[typeIdx])
				return;

			std::shared_ptr<CallbackStorage<CallbackData>> typedCallbackStorage =
				GetTypedCallbackStorage<CallbackData>();

			typedCallbackStorage->NotifyAll(callbackData);
		}

		// Delayed notification methods

		template<typename CallbackData>
		void NotifyCallbackEventDelayed(const CallbackData& callbackData)
		{
			CallbackType type = CallbackData::EventType;
			int typeIdx = static_cast<int>(type);

			if (!callbackStorage[typeIdx])
				return;

			std::shared_ptr<CallbackStorage<CallbackData>> typedCallbackStorage =
				GetTypedCallbackStorage<CallbackData>();

			typedCallbackStorage->callbackData.push_back(callbackData);
		}

	private:

		void NotifyAllCallbacks();
		void TidyCallbackData();

		template <typename CallbackData>
		void CreateCallbackStorage()
		{
			CallbackType type = CallbackData::EventType;
			int typeIdx = static_cast<int>(type);

			callbackStorage[typeIdx] = std::make_shared<CallbackStorage<CallbackData>>();
		}

		template <typename CallbackData>
		std::shared_ptr<CallbackStorage<CallbackData>> GetTypedCallbackStorage()
		{
			CallbackType type = CallbackData::EventType;
			int typeIdx = static_cast<int>(type);

			std::shared_ptr<CallbackStorage<CallbackData>> typedCallbackStorage =
				std::static_pointer_cast<CallbackStorage<CallbackData>>(
					callbackStorage[typeIdx]);

			return typedCallbackStorage;
		}

		// All callbacks are indexed by the type of callback in the 'CallbackType' enum.

		static constexpr int8_t CALLBACK_TYPE_COUNT = static_cast<int8_t>(CallbackType::COUNT);

		std::array<std::shared_ptr<CallbackStorageBase>, CALLBACK_TYPE_COUNT> callbackStorage;
	};
}