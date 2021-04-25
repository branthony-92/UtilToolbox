#include "stdafx.h"
#include "SMBase.h"

#ifdef _DEBUG
#define CONSOLE_LOG(str) std::cout << str << std::endl
#else
#define CONSOLE_LOG(str)
#endif // !_DEBUG


namespace Constants {
	constexpr uint32_t c_defaultInterval = 5;
}

CSMBase::CSMBase(TContextPtr pCtx, uint32_t id, std::string name)
	: CSMState(id, name)
	, m_ticThread()
	, m_ticIntervalMS(Constants::c_defaultInterval)
	, m_smEnabled(false)
	, m_finalStateReached(false)
	, m_pContext(pCtx)
	, m_pCurrentState(nullptr)
	, m_pErrorState(nullptr)
{
}

CSMBase::~CSMBase()
{
	setSMEnabled(false);
	if (m_ticThread.joinable())
	{
		m_ticThread.join();
	}
}

void CSMBase::startSM(TStatePtr pInitialState)
{
	if (pInitialState)
	{
		m_pCurrentState = pInitialState;
	}
	setSMDone(false);
	setSMEnabled(true);
	m_ticThread = std::thread( [&]()
	{
		CONSOLE_LOG("Starting SM Loop");
		while (getSMEnabled())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(m_ticIntervalMS.load()));
			onTic();
		}
		setSMDone(true);
		CONSOLE_LOG("Ending SM Loop");
	});
}

void CSMBase::onTic()
{
	if (!m_pCurrentState)
	{
		// kill the SM and/or throw an error
		throw std::runtime_error("No state detected");
	}

	if (!m_pContext)
	{
		throw std::runtime_error("Context pointer has expired");
	}

	// first get the next event if there is one
	auto pEvent = m_pContext->getNextEvent();

	// if we have an event queued then we can do the appropriate transition
	if (pEvent)
	{
		auto pNextState = getNextState(pEvent->c_eventID);
		if (!pNextState)
		{
			throw std::runtime_error("No state detected");
		}

		// exit current state and enter the next
		bool isOk = true;

		std::ostringstream ossExit;
		ossExit << "Leaving State " << m_pCurrentState->c_stateName;
		CONSOLE_LOG(ossExit.str());

		isOk &= m_pCurrentState->exitState(m_pContext, pEvent);

		pNextState->resetTimer();

		std::ostringstream ossEnter;
		ossEnter << "Entering State " << pNextState->c_stateName;
		CONSOLE_LOG(ossEnter.str());

		isOk &= pNextState->enterState(m_pContext, pEvent);

		setNextState(pNextState);
	}
	else
	{
		std::ostringstream ossExit;
		ossExit << "Ticking State " << m_pCurrentState->c_stateName;
		CONSOLE_LOG(ossExit.str());

		bool isOk = m_pCurrentState->ticState(m_pContext);
		
		if (m_pCurrentState->getIsFinalState())
		{
			CONSOLE_LOG("Final state reached");
			setSMEnabled(false);
		}	
	}
}

void CSMBase::setNextState(TStatePtr pState)
{
	m_pCurrentState = pState;
}

TStatePtr CSMBase::getNextState(uint32_t eventID)
{
	if (m_stateTable.empty()) return nullptr;

	uint32_t targetStateID = -1;
	TStatePtr pNextState = nullptr;

	if (m_pCurrentState)
	{
		targetStateID = m_pCurrentState->findTargetState(eventID);
	}
	if (targetStateID < m_stateTable.size())
	{
		pNextState = m_stateTable[targetStateID];
	}
	return pNextState;
}
