#include "stdafx.h"
#include "TestActions.h"
#include "TestResults.h"

using namespace std::chrono_literals;

void TestActionEmpty::execute()
{
	std::this_thread::sleep_for(10ms);
}

void TestActionSimpleAction::execute()
{
	auto pResult = CAsyncResult::convertTo<TestResultSimple>(m_pResult);

	pResult->setIntData(1);
	pResult->setFloatData(1.0f);
	pResult->setStringData("Result");
	pResult->setBoolData(true);
}

void TestActionTimeout::execute()
{
	m_abortFlag = false;
	while (!m_abortFlag.load())
	{
		// loop until aborted
		std::this_thread::sleep_for(5ms);
	};
}

void TestActionError::execute()
{
	throw std::exception("UH OH! ERROR!");
}
