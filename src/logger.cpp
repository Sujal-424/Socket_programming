#include "logger.hpp"

#include <unistd.h>

#include <iostream>
#include <json.hpp>
#include <sstream>

#include "colorprint.hpp"
#include "datetime.hpp"
#include "strutils.hpp"

Logger::Logger(Logger::Level level)
    : level_(level),
      stream_(std::cout),
      isaTTY_(isatty(fileno(stdout))) {}

Logger::Logger(Logger::Level level, std::ofstream& file)
    : level_(level),
      stream_(file),
      isaTTY_(false) {}

void Logger::info(const std::string& message,
                  const std::string& action,
                  int messageCode,
                  const std::unordered_map<std::string, std::string>& details) {
    if (level_ <= Level::Info) {
        log(Level::Info, message, action, messageCode, details);
    }
}

void Logger::warn(const std::string& message,
                  const std::string& action,
                  int messageCode,
                  const std::unordered_map<std::string, std::string>& details) {
    if (level_ <= Level::Warning) {
        log(Level::Warning, message, action, messageCode, details);
    }
}

void Logger::error(const std::string& message,
                   const std::string& action,
                   int messageCode,
                   const std::unordered_map<std::string, std::string>& details) {
    if (level_ <= Level::Error) {
        log(Level::Error, message, action, messageCode, details);
    }
}

void Logger::log(Level level,
                 const std::string& message,
                 const std::string& action,
                 int messageCode,
                 const std::unordered_map<std::string, std::string>& details) {
    if (!isaTTY_) {
        logJson(level, message, action, messageCode, details);
    }
    else {
        stream_ << levelToString(level) << ' ';
        if (messageCode != -1) {
            stream_ << '{' << messageCode << "} ";
        }
        stream_ << message << std::endl;
    }
}

void Logger::logJson(Level level,
                     const std::string& message,
                     const std::string& action,
                     int messageCode,
                     const std::unordered_map<std::string, std::string>& details) {
    nlohmann::json json;
    json["level"] = levelToString(level, true);
    json["message"] = message;
    if (messageCode != -1) {
        json["messageCode"] = messageCode;
    }
    if (!action.empty()) {
        json["action"] = action;
    }
    for (const auto& detail : details) {
        json[detail.first] = detail.second;
    }
    json["timestamp"] = DateTime::toStr(DateTime::getDateTime());
    json["serverDate"] = DateTime::toStr(DateTime::getServerDate());
    stream_ << json.dump(4) << std::endl;
}

std::string Logger::levelToString(Level level, bool isJson) {
    static std::unordered_map<Level, std::pair<std::string, Color>> levels = {
        {Level::Info, {"info", Color::GRN}},
        {Level::Warning, {"warn", Color::YEL}},
        {Level::Error, {"error", Color::RED}},
    };
    std::stringstream ss;
    if (isJson) {
        ss << levels[level].first;
    }
    else {
        ss << levels[level].second << '[' << strutils::toupper(levels[level].first) << ']' << Color::RST;
    }
    return ss.str();
}
