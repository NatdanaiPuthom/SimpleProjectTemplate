#pragma once
#include "StackTrace.hpp"

#include <mutex>
#include <unordered_map>

namespace Simple
{
	class SimpleMemoryTracker;
	class SimpleMemoryTrackerWrapper;

	struct MemoryTrackingSettings final
	{
		bool shouldStoreStackTraces;
		bool shouldTrackAllAllocations;
	};

	struct Allocation final
	{
		size_t size = 0;
		StackTrace stackTrace;
	};

	class SimpleMemoryTracker final
	{
		friend class Simple::SimpleMemoryTrackerWrapper;
	private:
		static void Initialize();
		static void Destory();

		static int AllocHook(int aAllocType, void* aUserData, size_t aSize, int aBlockType, long aRequestNumber, const unsigned char*, int);
		static void PrintTopLeaks(std::unordered_map<StackTrace, int>& aMap);
	private:
		inline static std::mutex myStaticAllocationMapMutex{};
		inline static std::unordered_map<StackTrace, int> myStaticStackTraceToAllocationCount{};
		inline static std::unordered_map<long, Allocation> myStaticAllocationMap{};
		inline static int myStaticTotalAllocationCount = 0;
		inline static thread_local bool myStaticIsAllocationInProgress = false;
		inline static MemoryTrackingSettings myStaticMemoryTrackingSettings = { false, true };
	};

	class SimpleMemoryTrackerWrapper final
	{
		friend class SimpleMemoryTracker;
	public:
		inline SimpleMemoryTrackerWrapper()
		{
			Simple::SimpleMemoryTracker::Initialize();
		}

		inline ~SimpleMemoryTrackerWrapper()
		{
			SimpleMemoryTracker::Destory();
		}

	private:
		inline static bool myShouldActive = true;
	};

	class SimpleMemoryTrackerWrapperInitializer final
	{
	private:
		inline static SimpleMemoryTrackerWrapper myStaticMemoryTrackerWrapper;
	};
}