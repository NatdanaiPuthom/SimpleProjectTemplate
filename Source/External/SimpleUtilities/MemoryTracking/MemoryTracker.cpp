#include "MemoryTracker.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

typedef struct _SimpleCrtMemBlockHeader
{
	struct _SimpleCrtMemBlockHeader* pBlockHeaderNext;
	struct _SimpleCrtMemBlockHeader* pBlockHeaderPrev;
	char* szFileName;
	int                         nLine;
#ifdef _WIN64
	/* These items are reversed on Win64 to eliminate gaps in the struct
	 * and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
	 * maintained in the debug heap.
	 */
	int                         nBlockUse;
	size_t                      nDataSize;
#else 
	size_t                      nDataSize;
	int                         nBlockUse;
#endif
	long                        lRequest;
	unsigned char               gap[4];
} _SimpleCrtMemBlockHeader;


namespace Simple
{
	char localBuffer[255];

#ifdef _DEBUG
	int SimpleMemoryTracker::AllocHook(int aAllocType, void* aUserData, size_t aSize, int aBlockType, long aRequestNumber, const unsigned char*, int)
	{
		if (aBlockType == _CRT_BLOCK || myStaticIsAllocationInProgress)
			return 1;

		// Do not track allocations by the memory tracker itself
		myStaticIsAllocationInProgress = true;

		std::lock_guard<std::mutex> guard(myStaticAllocationMapMutex);

		bool alloc = false;
		bool free = false;

		switch (aAllocType)
		{
		case _HOOK_ALLOC:
			alloc = true;
			break;
		case _HOOK_REALLOC:
			alloc = true;
			free = true;
			break;
		case _HOOK_FREE:
			free = true;
			break;
		}

		if (alloc)
		{
			Allocation& entry = myStaticAllocationMap[aRequestNumber];
			entry.size = aSize;

			if (myStaticMemoryTrackingSettings.shouldStoreStackTraces == true)
			{
				entry.stackTrace = StackTrace::CaptureStackTrace(1);
				myStaticStackTraceToAllocationCount[entry.stackTrace]++;
			}

			SimpleMemoryTracker::myStaticTotalAllocationCount++;
		}

		if (free)
		{
			auto* pHead = reinterpret_cast<_SimpleCrtMemBlockHeader*>(reinterpret_cast<uint8_t*>(aUserData) - sizeof(_SimpleCrtMemBlockHeader));

			long freeRequestNumber = pHead->lRequest;

			myStaticStackTraceToAllocationCount.erase(myStaticAllocationMap[freeRequestNumber].stackTrace);
			myStaticAllocationMap.erase(freeRequestNumber);
		}

		myStaticIsAllocationInProgress = false;
		return 1;
	}

	void SimpleMemoryTracker::Initialize()
	{
		_CrtSetAllocHook(&SimpleMemoryTracker::AllocHook);
	}

	void SimpleMemoryTracker::PrintTopLeaks(std::unordered_map<StackTrace, int>& aMap)
	{
		std::vector<std::pair<StackTrace, int>> pairs;

		for (const auto& p : aMap)
		{
			pairs.push_back(p);
		}

		std::sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b) {return a.second > b.second; });

		OutputDebugStringA("================================================================================\n");
		int i = 0;
		for (const auto& p : pairs)
		{
			if (i >= 5)
				break;

			char buffer[100];
			sprintf_s(buffer, "Count: %d\n, Stack Trace:", p.second);
			OutputDebugStringA(buffer);
			p.first.Print();

			OutputDebugStringA("================================================================================\n");

			i++;
		}
	}

	void SimpleMemoryTracker::Destory()
	{
		if (SimpleMemoryTrackerWrapper::myShouldActive == false)
		{
			return;
		}

		std::lock_guard<std::mutex> guard(myStaticAllocationMapMutex);

		_CrtSetAllocHook(nullptr);

		if (myStaticAllocationMap.size() == 0)
		{
			if (myStaticMemoryTrackingSettings.shouldTrackAllAllocations)
			{
				OutputDebugStringA("================================================================================\n");
				char buffer[100];
				sprintf_s(buffer, "== Total Allocation Count: %d\n", myStaticTotalAllocationCount);
				OutputDebugStringA(buffer);
			}

			OutputDebugStringA("================================================================================\n");
			OutputDebugStringA("== No memory leaks found! \n");
			OutputDebugStringA("================================================================================\n");
		}
		else
		{
			if (myStaticMemoryTrackingSettings.shouldStoreStackTraces)
			{
				OutputDebugStringA("================================================================================\n");
				std::unordered_map<StackTrace, int> stackTraceToLeakCountMap;

				for (const auto& p : myStaticAllocationMap)
				{
					stackTraceToLeakCountMap[p.second.stackTrace]++;
				}

				OutputDebugStringA("== Top 5 Leaks: \n");
				PrintTopLeaks(stackTraceToLeakCountMap);
			}

			if (myStaticMemoryTrackingSettings.shouldTrackAllAllocations)
			{
				OutputDebugStringA("================================================================================\n");
				char buffer[100];
				sprintf_s(buffer, "== Total Allocation Count: %d\n", myStaticTotalAllocationCount);
				OutputDebugStringA(buffer);
			}

			OutputDebugStringA("================================================================================\n");
			char buffer[100];
			sprintf_s(buffer, "== Total Number of Memory Leaks: %d\n", (int)myStaticAllocationMap.size());
			OutputDebugStringA(buffer);
			OutputDebugStringA("================================================================================\n");
			OutputDebugStringA("== For more details modify SimpleTracker::MemoryTrackingSettings\n");
			OutputDebugStringA("== SimpleTracker::MemoryTrackingSettings({ StoreStackTraces(Very slow, gives precise info), TrackAllAllocations(decently fast, lots of spam) });\n");
			OutputDebugStringA("================================================================================\n");
		}

		myStaticAllocationMap.clear();
		myStaticStackTraceToAllocationCount.clear();
	}
#else 
	void SimpleMemoryTracker::Initialize() {};
	void SimpleMemoryTracker::Destory() {};
#endif 
}