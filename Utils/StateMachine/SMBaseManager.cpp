#include "stdafx.h"
#include "SMBaseManager.h"

#include "SMBase.h"
#include "SMContext.h"

CSMBaseMgr::CSMBaseMgr(std::shared_ptr<CSMBase> pSM)
	: m_pSM(pSM)
	, m_pCtx(nullptr)
{
	if (pSM)
	{
		m_pCtx = m_pSM->getContext();
	}
}

CSMBaseMgr::~CSMBaseMgr()
{
}

void CSMBaseMgr::reset()
{
	if (m_pSM)
	{
		m_pSM->stopSM();
	}

	m_pSM  = nullptr;
	m_pCtx = nullptr;
}
