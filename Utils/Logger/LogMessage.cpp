#include "stdafx.h"
#include "LogMessage.h"

CLogMessage::CLogMessage(Severity sev, std::string msg)
	: m_message(msg)
	, m_severity(sev)
	, m_timeStamp()
{
	m_timeStamp = std::chrono::system_clock::now();
}

CLogMessage::~CLogMessage()
{
}
