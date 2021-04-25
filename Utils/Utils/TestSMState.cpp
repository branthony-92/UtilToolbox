#include "stdafx.h"
#include "TestSMState.h"

CSMTestState::CSMTestState(StateID id, std::string name, bool isFinal)
	: CSMState(static_cast<unsigned int>(id), name, isFinal)
{
}

CSMTestState::~CSMTestState()
{
}

std::shared_ptr<CSMTestState> CSMTestState::createState(StateID id)
{
	std::shared_ptr<CSMTestState> pState = nullptr;
	switch (id)
	{
	case CSMTestState::StateID::eStateIdle:
		pState = std::make_shared<CSMTestStateIdle>();
		break;
	case CSMTestState::StateID::eStateOne:
		pState = std::make_shared<CSMTestStateOne>();
		break;
	case CSMTestState::StateID::eStateTwo:
		pState = std::make_shared<CSMTestStateTwo>();
		break;
	case CSMTestState::StateID::eStateThree:
		pState = std::make_shared<CSMTestStateThree>();
		break;
	case CSMTestState::StateID::eStateDone:
		pState = std::make_shared<CSMTestStateDone>();
		break;
	case CSMTestState::StateID::eStateError:
		pState = std::make_shared<CSMTestStateError>();
		break;
	default:
		break;
	}
    return pState;
}
