#include "UnitTest/pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Simple;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:

		TEST_METHOD(Constructor)
		{
			Assert::AreEqual(1, 1);
		}

		TEST_METHOD(Destructor)
		{
			Assert::AreEqual(1, 1);
		}

		TEST_METHOD(AssignConstructor)
		{
			Assert::AreEqual(1, 1);
		}

		TEST_METHOD(MoveConstructor)
		{
			Assert::AreEqual(1, 1);
		}

		TEST_METHOD(AssignOperator)
		{
			Assert::AreEqual(1, 1);
		}

		TEST_METHOD(MoveOperator)
		{
			Assert::AreEqual(1, 1);
		}
	};
}
