#include "stdafx.h"
#include "LogAction.h"

void CLogAction::execute()
{
	std::fstream logFile(m_params.logPath, std::ios::out | std::ios::app);
	if (logFile.is_open())
	{
		for (auto log : m_params.logQueue)
		{
			logFile << log.getMsg() << "\n";
		}
		logFile.close();
	}
}
