#pragma once

namespace Simple
{
	enum class ConsoleTextColor
	{
		White,
		Red,
		Green,
		Blue,
		Yellow,
		Default
	};

	class Console final
	{
	public:
		Console();
		~Console();

		void Initialize();

		Console(const Console&) = delete;
		Console& operator=(const Console&) = delete;

		Console(Console&&) = delete;
		Console& operator=(Console&&) = delete;

		static void Print(const char* aText, const bool aShouldEndLine = true);
		static void Print(const char* aText, ConsoleTextColor aColor, const bool aShouldEndLine = true);
		static void SetTextColor(const ConsoleTextColor aColor);
	};
}
