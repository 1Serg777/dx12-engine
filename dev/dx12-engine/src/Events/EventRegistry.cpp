#include "Events/EventRegistry.h"

namespace dxe
{
	// CallbackStorageBase

	void CallbackStorageBase::ClearAll()
	{
		ClearCallbacks();
		ClearCallbackData();
	}

	// EventRegistry

	void EventRegistry::Tick()
	{
		NotifyAllCallbacks();
		TidyCallbackData();
	}

	void EventRegistry::NotifyAllCallbacks()
	{
		for (auto& storage : callbackStorage)
		{
			if (storage)
				storage->NotifyAll();
		}
	}
	void EventRegistry::TidyCallbackData()
	{
		for (auto& storage : callbackStorage)
		{
			if (storage)
				storage->ClearCallbackData();
		}
	}
}