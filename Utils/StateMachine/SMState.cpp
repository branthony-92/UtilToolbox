#include "stdafx.h"
#include "SMState.h"

constexpr long long c_defaultStateTimeout = 30000; // 3000 ms = 30 seconds

CSMState::CSMState(uint32_t id, std::string name, bool isFinalState)
	: c_stateName(name)
	, c_stateID(id)
	, m_transitions()
	, m_startTime()
	, m_stateTimeout(c_defaultStateTimeout)
	, m_isFinalState(isFinalState)
{
}


CSMState::~CSMState()
{
}

void CSMState::addTransition(uint32_t eventID, uint32_t targetID)
{
	m_transitions.insert_or_assign(eventID, targetID);
}

uint32_t CSMState::findTargetState(uint32_t eventID)
{
	auto targetStateIter = m_transitions.find(eventID);
	if (targetStateIter != m_transitions.end())
	{
		return targetStateIter->second;
	}
	return -1;
}

bool CSMState::isTimedOut() const
{
	auto now = std::chrono::high_resolution_clock::now();
	auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_startTime).count();

	return dt > m_stateTimeout;
}