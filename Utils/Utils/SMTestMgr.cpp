#include "stdafx.h"
#include "SMTestMgr.h"
#include "TestContext.h"
#include "TestSM.h"

CSMTestMgr::CSMTestMgr()
{
}

CSMTestMgr::~CSMTestMgr()
{
}

void CSMTestMgr::init()
{
	auto pCtx = std::make_shared<CTestContext>();
	auto pSM  = std::make_shared<CTestSM>(pCtx);

	setSM(pSM);
	setContext(pCtx);
}
