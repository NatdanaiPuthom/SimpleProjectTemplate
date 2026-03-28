#include "Engine/Debugger/Console/Console.hpp"
#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"

using namespace Simple;

int main()
{
	Console console;
	console.Initialize();

	Console::Print("HelloWorld");

	return 0;
}
