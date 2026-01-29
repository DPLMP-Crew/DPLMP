#pragma once
#include <string>

enum class LogType {
	INFO,
	WARN,
	ERR,
	DEBUG
};

class Logger {
public:

	Logger(const char* loggerName)
	{
		m_Name = loggerName;
	}

	template <typename... Args>
	void Log(LogType logType, const char* fmt, Args... args) {
		printf("[ ");
		printf(m_Name.c_str());
		printf(" | ");
		std::string logTypeName;
		switch (logType) {
		case LogType::INFO:
			logTypeName = "INFO";
			break;
		case LogType::WARN:
			logTypeName = "WARN";
			break;
		case LogType::ERR:
			logTypeName = "ERROR";
			break;
		case LogType::DEBUG:
			logTypeName = "DEBUG";
			break;
		default:
			logTypeName = "UNK";
			break;
		}
		printf(logTypeName.c_str());
		printf(" ] ");
		printf(fmt, args...);
		printf("\n");
	}

private:
	std::string m_Name;
};