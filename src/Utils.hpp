#pragma once

#include <string>
class Utils {
public:
    static bool isNumber(const std::string& s, bool allowNegative = false);
    static bool isValidDateDMY(const std::string& s);
    static std::string formatMoney(long long amount);
    static bool containsPipe(const std::string& s);
    static std::string dmyToYmd(const std::string& s);
    static std::string ymdToDmy(const std::string& s);
    static std::string getTodayYmd();
    static std::string getTodayDmy();
    static std::string toTwoDigits(int value);

    static std::string readLine(const std::string& prompt);
    static int readInt(const std::string& prompt);
    static long long readLongLong(const std::string& prompt);
    static std::string readNote();
    static std::string readDate();
    static char readType();
};
