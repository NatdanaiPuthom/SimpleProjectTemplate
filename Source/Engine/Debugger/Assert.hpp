#pragma once
#include "Console/Console.hpp"

#ifdef _DEBUG
#include <cassert>
#include <string>
#endif

#ifndef _RETAIL
#include <format>
#endif

namespace Simple
{
	inline void DebugAssert(bool aCondition, const char* aMessage)
	{
#ifndef _RETAIL
		Console::SetTextColor(ConsoleTextColor::Red);
		Console::Print(aMessage);
		Console::SetTextColor(ConsoleTextColor::Default);
#endif

#ifdef _DEBUG
		assert(aCondition && aMessage);
#else
		(void)aCondition;
		(void)aMessage;		
#endif
	}

	inline void DebugAssert(bool aCondition, const char* aMessage, const char* aExpected, const char* aActual)
	{
#ifndef _RETAIL
		const std::string message = std::format("{} <Expected> {} <Actual> {}", aMessage, aExpected, aActual);
		DebugAssert(aCondition, message.c_str());
#endif

#ifdef _RETAIL
		(void)aCondition;
		(void)aMessage;
		(void)aExpected;
		(void)aActual;
#endif
	}
}
