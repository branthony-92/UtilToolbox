#include "stdafx.h"
#include "SMBaseManager.h"

#include "SMBase.h"
#include "SMContext.h"

using namespace std::chrono_literals;

CSMBaseMgr::CSMBaseMgr(std::string name, uint32_t timeout)
	: m_stateMachines()
	, m_enabled(false)
	, c_name(name)
	, c_ticTimeout(timeout)
{
}

CSMBaseMgr::~CSMBaseMgr()
{
	resetStateMachines();
}

void CSMBaseMgr::startStateMachines()
{
	for (auto& SM : m_stateMachines)
	{
		// init each of our state machines
		auto pSM = SM.second;
		if (pSM)
		{
			pSM->initSM();
		}
	}

	stopStateMachines();
	m_ticThread = std::thread([&]{
		m_enabled = true;
		while (m_enabled)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(c_ticTimeout));
			for (auto& SM : m_stateMachines)
			{
				// tic each of our state machines
				auto pSM = SM.second;
				if (pSM && pSM->getSMEnabled())
				{
					SM.second->onTic();
				}
			}
		}
	});
}

void CSMBaseMgr::stopStateMachines()
{
	m_enabled = false;
	if (m_ticThread.joinable()) m_ticThread.join();
}

void CSMBaseMgr::registerStateMachine(SMPtr pSM)
{
	auto name = pSM->c_stateName;
	m_stateMachines.insert_or_assign(name, pSM);
}



void CSMBaseMgr::resetStateMachines()
{
	stopStateMachines();
	m_stateMachines.clear();
}
