#include "stdafx.h"

#include "LogAction.h"
#include "Logger.h"

void CLogAction::execute(CActionContext* pCtx)
{
	std::fstream logFile;
	auto pLogger = dynamic_cast<CLogger*>(pCtx);

	auto logs = pLogger->getLogs();

	if (logs.empty()) return;

	auto writeMode = pLogger->getWriteMode();
	auto filePath = pLogger->getFilePath();

	switch (writeMode)
	{
		case WriteMode::APPEND:   
			logFile.open(filePath, std::ios::out | std::ios::app);
			break;
		case WriteMode::TRUNCATE: 
			logFile.open(filePath, std::ios::out | std::ios::trunc);
			break;
		case WriteMode::NO_WRITE:
			return;
		default: 
			break;
	}

	if (logFile.is_open())
	{
		for (auto log : logs)
		{
			logFile << log.getMsg() << "\n";
		}
		logFile.close();
	}
}
