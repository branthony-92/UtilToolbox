#ifndef TESTSM_H
#define TESTSM_H

#include "SMBase.h"

class CTestSM : public CSMBase
{
public:
	CTestSM(TContextPtr pCtx);
	virtual ~CTestSM();

	void initSM();
};
typedef std::shared_ptr<CTestSM> TTestSMPtr;
#endif // !TESTSM_H