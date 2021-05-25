#include "stdafx.h"
#include "TestActions.h"
#include "TestResults.h"

using namespace std::chrono_literals;

void TestActionEmpty::execute()
{
	std::this_thread::sleep_for(5ms);
}

void TestActionSimpleAction::execute()
{
	auto pResult = CAsyncResult::convertTo<TestResultSimple>(m_pResult);

	pResult->setIntData(1);
	pResult->setFloatData(1.0f);
	pResult->setStringData("Result");
	pResult->setBoolData(true);
}

void TestActionComputeFib::execute()
{
	auto pResult = CAsyncResult::convertTo<TestResultComputeFib>(m_pResult);
	auto val = computeFib(m_params.nFib);
	if (m_abortFlag.load()) val = -1;
	pResult->setVal(val);
}

int TestActionComputeFib::computeFib(int nFib)
{	
	if (nFib <= 0 || m_abortFlag.load()) return 0;
	if (nFib == 1) return 1;
	if (nFib == 2) return 1;

	return computeFib(nFib - 1) + computeFib(nFib - 2);
}

void TestActionTimeout::execute()
{
	m_abortFlag = false;
	while (!m_abortFlag.load());
}

void TestActionError::execute()
{
	throw std::exception("UH OH! ERROR!");
}
