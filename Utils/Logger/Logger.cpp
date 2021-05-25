#include "stdafx.h"
#include "Logger.h"

using namespace std::chrono_literals;

namespace {
	const std::map<Severity, std::string> c_sevReflectionMap = {
		{ Severity::Info,	  " [INFO] "  },
		{ Severity::Debug,    " [DEBUG] " },
		{ Severity::Warning,  " [WARN] "  },
		{ Severity::ErrorL1,  " [ERR1] "  },
		{ Severity::ErrorL2,  " [ERR2] "  },
		{ Severity::Fatal,    " [FATAL] " }
	};
}

CLogger::CLogger(std::string logPath)
	: m_logName("")
	, m_logPath(logPath)
	, m_logs()
	, m_logLevel(Severity::Normal)
    , m_logThread()
    , m_writeEnabled(false)
    , m_mutex()
	, m_pLogAction(nullptr)
{
	m_pLogAction = std::make_unique<CLogAction>();
}

CLogger::~CLogger()
{
	writeChunk();
	auto pPromise = m_pLogAction->getPromise();
	if (pPromise) pPromise->waitForResult();
}

void CLogger::logMsg(CLogMessage log)
{
	if (!m_writeEnabled.load() || log.getSev() > m_logLevel)
	{
		return;
	}

	auto logStr = constructMessageString(log);
	log.setMsg(logStr);
	m_logs.push_back(log);

	if (m_logs.size() >= 10)
	{
		writeChunk();
	}
}

void CLogger::writeChunk(int chunkSize)
{
	auto pPromise = m_pLogAction->getPromise();
	if (pPromise) pPromise->waitForResult();

	std::ostringstream oss;
	auto now = std::chrono::system_clock::now();
	oss << m_logPath << m_logName << ".txt";

	m_pLogAction->m_params.logPath = oss.str();
	m_pLogAction->m_params.logQueue = m_logs;

	auto promise = m_pLogAction->run();

	m_logs.clear();
}

void CLogger::purgeMessages()
{
	TLock lock(m_mutex);
	m_logs.clear();
}

std::string CLogger::getTimePointString(TimePoint timePoint)
{
	// todo - determine local
	std::time_t t = std::chrono::system_clock::to_time_t(timePoint);
	std::string ts = std::ctime(&t);
	ts.resize(ts.size() - 1);
	for (auto& c : ts)
	{
		if (c == ' ') c = '_';
	}
	return ts;
}

std::string CLogger::constructMessageString(CLogMessage log)
{
	std::ostringstream oss;
	oss << getTimePointString(log.getTimestamp()) << c_sevReflectionMap.at(log.getSev());
	oss << log.getMsg();
	return oss.str();
}