#include "Log.h"
#include <iostream>
#ifdef __SWITCH__
#include <switch.h>
#endif

Log::Log() {
    logPath = getLogPath();
    std::filesystem::create_directories(std::filesystem::path(logPath).parent_path());
    logFile.open(logPath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << logPath << std::endl;
    }
    info("Log system initialized");
}

Log::~Log() {
    if (logFile.is_open()) {
        info("Log system shutting down");
        logFile.close();
    }
}

void Log::info(const std::string& message) {
    write("INFO", message);
}

void Log::warning(const std::string& message) {
    write("WARNING", message);
}

void Log::error(const std::string& message) {
    write("ERROR", message);
}

void Log::debug(const std::string& message) {
    write("DEBUG", message);
}

void Log::write(const std::string& level, const std::string& message) {
    if (logFile.is_open()) {
        logFile << "[" << getTimestamp() << "] [" << level << "] " << message << std::endl;
        logFile.flush();
    }
}

std::string Log::getTimestamp() {
    auto now = std::time(nullptr);
    auto tm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Log::getLogPath() {
    #ifdef __SWITCH__
    return "sdmc:/switch/hamburgerEngine/logs/hamburger-engine.log";
    #else
    return "logs/hamburger-engine.log";
    #endif
} 