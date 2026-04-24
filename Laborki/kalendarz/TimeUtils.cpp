#include "TimeUtils.h"
#include <sstream>
#include <iostream>

std::string formatTime(std::time_t rawTime) {
    if (rawTime == 0) {
        return "N/A";
    }
    struct tm timeInfo;
    char buffer[20];
    localtime_s(&timeInfo, &rawTime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return std::string(buffer);
}

std::time_t parseDateTime(const std::string& dateTimeStr) {
    if (dateTimeStr.empty()) {
        return 0;
    }
    int year, month, day, hour, minute;
    char d1, d2, d3;
    std::istringstream iss(dateTimeStr);

    std::string datePart, timePart;
    if (!(iss >> datePart >> timePart)) {
        std::cerr << "[Warning]: Could not parse '" << dateTimeStr << "'. Using 0.\n";
        return 0;
    }

    {
        std::istringstream dateSS(datePart);
        dateSS >> year >> d1 >> month >> d2 >> day;
        if (!dateSS || d1 != '-' || d2 != '-') {
            std::cerr << "[Warning]: Invalid date format '" << datePart << "'. Using 0.\n";
            return 0;
        }
    }
    {
        std::istringstream timeSS(timePart);
        timeSS >> hour >> d3 >> minute;
        if (!timeSS || d3 != ':') {
            std::cerr << "[Warning]: Invalid time format '" << timePart << "'. Using 0.\n";
            return 0;
        }
    }

    struct tm tmStruct = { 0 };
    tmStruct.tm_year = year - 1900;
    tmStruct.tm_mon = month - 1;
    tmStruct.tm_mday = day;
    tmStruct.tm_hour = hour;
    tmStruct.tm_min = minute;
    tmStruct.tm_sec = 0;

    std::time_t result = mktime(&tmStruct);
    if (result == -1) {
        std::cerr << "[Warning]: mktime failed for '" << dateTimeStr << "'. Using 0.\n";
        return 0;
    }
    return result;
}
