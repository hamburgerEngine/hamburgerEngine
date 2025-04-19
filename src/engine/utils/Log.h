#pragma once
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

class Log {
public:
    static Log& getInstance() {
        static Log instance;
        return instance;
    }

    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void debug(const std::string& message);

private:
    Log();
    ~Log();

    void write(const std::string& level, const std::string& message);
    std::string getTimestamp();
    std::string getLogPath();

    std::ofstream logFile;
    std::string logPath;
}; 