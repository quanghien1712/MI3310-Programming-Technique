#include "Utils.hpp"

#include <iostream>
#include <ctime>

using namespace std;

bool Utils::isNumber(const string& s, bool allowNegative) {
    if (s.empty()) return false;

    if (!allowNegative && s[0] == '-') {
        return false;
    }
    
    try {
        size_t pos;
        stoll(s, &pos);

        while (pos < s.size() && isspace(s[pos])) {
            ++pos;
        }
        return pos == s.size();

    } catch (const invalid_argument& e) {
        return false;
    } catch (const out_of_range& e) {
        return false;
    }
}

bool Utils::isValidDateDMY(const string& s) {
    if (s.size() != 10) {
        return false;
    }
    if (s[2] != '/' || s[5] != '/') {
        return false;
    }
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) {
            continue;
        }
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    int day = stoi(s.substr(0, 2));
    int month = stoi(s.substr(3, 2));
    int year = stoi(s.substr(6, 4));
    if (month < 1 || month > 12) {
        return false;
    }
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeapYear) {
        daysInMonth[1] = 29;
    }
    if (day < 1 || day > daysInMonth[month - 1]) {
        return false;
    }
    return true;
}

string Utils::formatMoney(long long amount) {
    bool negative = amount < 0;
    amount = negative ? -amount : amount;
    string s = to_string(amount);
    int count = 0;
    for (int i = s.size() - 1; i >= 0; --i) {
        count++;
        if (count == 3 && i > 0) {
            s.insert(s.begin() + i, ',');
            count = 0;
        }
    }
    if (negative) {
        s.insert(s.begin(), '-');
    }
    return s;
}

bool Utils::containsPipe(const string& s) {
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '|') {
            return true;
        }
    }
    return false;
}

string Utils::dmyToYmd(const string& s) {
    return s.substr(6, 4) + "-" + s.substr(3, 2) + "-" + s.substr(0, 2);
}

string Utils::ymdToDmy(const string& s) {
    if (s.size() != 10) {
        return s;
    }
    return s.substr(8, 2) + "/" + s.substr(5, 2) + "/" + s.substr(0, 4);
}

string Utils::getTodayYmd() {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    int year = local->tm_year + 1900;
    int month = local->tm_mon + 1;
    int day = local->tm_mday;
    return to_string(year) + "-" + toTwoDigits(month) + "-" + toTwoDigits(day);
}

string Utils::getTodayDmy() {
    return ymdToDmy(getTodayYmd());
}

string Utils::toTwoDigits(int value) {
    if (value < 10) {
        return "0" + to_string(value);
    }
    return to_string(value);
}

string Utils::readLine(const string& prompt) {
    string line;
    cout << prompt;
    getline(cin, line);
    return line;
}

int Utils::readInt(const string& prompt) {
    while (true) {
        string line = readLine(prompt);
        if (isNumber(line, false)) {
            return stoi(line);
        }
        cout << "Nhap so khong hop le" << endl;
    }
}

long long Utils::readLongLong(const string& prompt) {
    string line;
    while (true) {
        line = readLine(prompt);
        if (isNumber(line, false)) {
            return stoll(line);
        }
        cout << "Nhap so khong hop le" << endl;
    }
}

string Utils::readNote() {
    string noteInput;
    while (true) {
        noteInput = Utils::readLine("-> Nhap ghi chu (Enter de bo qua): ");
        if (!containsPipe(noteInput)) {
            return noteInput;
        }
        cout << "Ghi chu khong duoc chua ky tu '|'" << endl;
    }
}

string Utils::readDate() {
    string dateInput;
    while (true) {
        dateInput = readLine("-> Nhap ngay (DD/MM/YYYY) [Mac dinh: Hom nay]: ");
        if (dateInput.empty()) {
            dateInput = getTodayDmy();
        }

        if (isValidDateDMY(dateInput)) {
            return dmyToYmd(dateInput);
        }
        cout << "Ngay khong hop le" << endl;
    }
}

char Utils::readType() {
    while (true) {
        int tmp = Utils::readInt("-> Phan loai (1-THU, 2-CHI): ");
        if (tmp == 1) {
            return 'T';
        }
        if (tmp == 2) {
            return 'C';
        }
        cout << "Phan loai khong hop le" << endl;
    }
}

