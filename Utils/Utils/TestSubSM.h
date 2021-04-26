#ifndef TESTSUBSM_H
#define TESTSUBSM_H

#include "SMBase.h"

class CTestSubSM : public CSMBase
{
public:
	CTestSubSM(TContextPtr pCtx = nullptr);
	virtual ~CTestSubSM();

	virtual void initSM();

	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

	virtual void initTransitionTable() override;
};

#endif // !TESTSUBSM_H