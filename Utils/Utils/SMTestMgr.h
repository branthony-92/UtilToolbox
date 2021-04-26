#ifndef SMTESTMGR_H
#define SMTESTMRG_H

#include "SMBaseManager.h"

class CSMTestMgr : public CSMBaseMgr
{
public:
	CSMTestMgr();
	virtual ~CSMTestMgr();

	virtual void init() override;
};
typedef std::shared_ptr<CSMTestMgr> TSMTestMgrPtr;

#endif // !SMTESTMGR_H