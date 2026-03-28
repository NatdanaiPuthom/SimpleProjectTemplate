#include "Console.hpp"
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Simple
{
	Console::Console()
	{
	}

	Console::~Console()
	{
#pragma warning(push)
#pragma warning(disable : 4996)
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);
#pragma warning(pop)
		FreeConsole();
	}

	  void Console::Initialize()
	{
		const char* message = nullptr;

		if (GetConsoleWindow())
		{
			message = "Console already exists; using the existing console.";
			SetTextColor(ConsoleTextColor::Yellow);
		}
#pragma warning(push)
#pragma warning( disable : 4996 )
		else if (AllocConsole())
		{
			message = "Console successfully initialized.";
			SetTextColor(ConsoleTextColor::Green);
		}
		else
		{
			message = "Console initialization failed; using existing console if present.";
			SetTextColor(ConsoleTextColor::Red);
		}

		freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stderr);

		setbuf(stdin, nullptr);
		setbuf(stdout, nullptr);
		setbuf(stderr, nullptr);
#pragma warning( pop )

		SetConsoleTitle(L"Simple, it's just that easy");

		Print(message);
		SetTextColor(ConsoleTextColor::Default);
	}

	void Console::Print(const char* aText, const bool aShouldEndLine)
	{
		std::cout << (aText ? aText : "(null)");

		if (aShouldEndLine)
		{
			std::cout << std::endl;
		}
	}

	void Console::Print(const char* aText, ConsoleTextColor aColor, const bool aShouldEndLine)
	{
		SetTextColor(aColor);
		Print(aText, aShouldEndLine);
		SetTextColor(ConsoleTextColor::Default);
	}

	void Console::SetTextColor(const ConsoleTextColor aColor)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (aColor)
		{
		case ConsoleTextColor::Red:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case ConsoleTextColor::Green:
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case ConsoleTextColor::Blue:
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		case ConsoleTextColor::Yellow:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case ConsoleTextColor::White:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		case ConsoleTextColor::Default:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		default:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		}
	}
}
