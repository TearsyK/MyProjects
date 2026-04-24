#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <string>
#include <ctime>

std::string formatTime(std::time_t rawTime);
std::time_t parseDateTime(const std::string& dateTimeStr);

#endif // TIMEUTILS_H
