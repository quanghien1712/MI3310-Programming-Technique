#include "Budget.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

#include "Utils.hpp"
#include "Transaction.hpp"
#include "Category.hpp"

using namespace std;

Budget::Budget() : categoryId(0), month(0), year(0), limitAmount(0) {}

Budget::Budget(int categoryId, int month, int year, long long limit)
    : categoryId(categoryId), month(month), year(year), limitAmount(limit) {}

int Budget::getCategoryId() const {
    return categoryId;
}

int Budget::getMonth() const {
    return month;
}

int Budget::getYear() const {
    return year;
}

long long Budget::getLimitAmount() const {
    return limitAmount;
}

void Budget::setLimitAmount(long long value) {
    limitAmount = value;
}

string Budget::toString() const {
    return to_string(categoryId) + "|" + to_string(month) + "|" +
        to_string(year) + "|" + to_string(limitAmount);
}

BudgetManager::BudgetManager() : dataFile("") {}

void BudgetManager::loadData(const string& fileName) {
    dataFile = fileName;
    budgets.clear();

    ifstream input(fileName);
    if (!input) return;

    string line;
    int categoryId, month, year;
    long long limitAmount;
    while (getline(input, line)) {
        stringstream ss(line);
        string tmp;
        getline(ss, tmp, '|');
        if (tmp.empty() || !Utils::isNumber(tmp)) {
            continue;
        }
        categoryId = stoi(tmp);

        getline(ss, tmp, '|');
        if (tmp.empty() || !Utils::isNumber(tmp)) {
            continue;
        }
        month = stoi(tmp);

        getline(ss, tmp, '|');
        if (tmp.empty() || !Utils::isNumber(tmp)) {
            continue;
        }
        year = stoi(tmp);

        getline(ss, tmp, '|');
        if (tmp.empty() || !Utils::isNumber(tmp, true)) {
            continue;
        }
        limitAmount = stoll(tmp);

        budgets.add(Budget(categoryId, month, year, limitAmount));
    }
    input.close();
}

void BudgetManager::saveData() const {
    ofstream output(dataFile);
    if (!output) return;
    
    for (int i = 0; i < budgets.size(); ++i) {
        output << budgets[i].toString() << endl;
    }

    output.close();
}

void BudgetManager::updateBudget(CategoryManager& categoryManager, int month, int year) {
    cout << endl;
    cout << "=== THIET LAP NGAN SACH ===" << endl;
    cout << "Danh sach cac danh muc CHI:" << endl;
    const DynamicArray<Category>& categories = categoryManager.getAll();
    bool hasExpense = false;
    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].getType() != 'C') continue;
        hasExpense = true;
        long long limit = getLimit(i, month, year);
        cout << i << ". " << categories[i].getName() << " (Hien tai: "
             << (limit >= 0 ? Utils::formatMoney(limit) : "Chua dat han muc") << " VND)" << endl;
    }
    if (!hasExpense) {
        cout << "Chua co danh muc CHI" << endl;
        return;
    }
    int categoryId = categoryManager.categoryOptions('C');
    long long limit = Utils::readLongLong("-> Nhap muc ngan sach moi (VND): ");
    setLimit(categoryId, month, year, limit);
    cout << "Da cap nhat ngan sach thanh cong" << endl;
}

void BudgetManager::showBudgetStatus(const CategoryManager& categoryManager,
                    const TransactionManager& transactionManager, int month, int year) const {
    cout << endl;
    cout << "=== TRANG THAI NGAN SACH HIEN TAI ===" << endl;
    cout << "Danh muc  | Da chi  | Ngan sach | Trang thai" << endl;
    cout << "----------------------------------------------------------------" << endl;
    const DynamicArray<Category>& categories = categoryManager.getAll();
    bool hasExpense = false;
    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].getType() != 'C') continue;
        hasExpense = true;
        long long spent = transactionManager.getSpent(i, month, year);
        long long limit = getLimit(i, month, year);
        float percent = 0;
        if (limit > 0) {
            percent = (spent * 100.0f) / limit;
        }
        int barWidth = 10;
        int filled = (limit > 0) ? ((int)percent * barWidth) / 100 : 0;
        string status;
        if (limit <= 0) {
            status = "Chua dat";
        } else if (percent >= 100) {
            status = "VUOT " + Utils::formatMoney(spent - limit);
        } else if (percent >= 80) {
            status = "Sap vuot";
        } else {
            status = "An toan";
        }
        cout << categories[i].getName() << " | " << Utils::formatMoney(spent) << " | "
             << (limit > 0 ? Utils::formatMoney(limit) : "Chua dat") << " | [";
        for (int j = 0; j < barWidth; ++j) {
            cout << (j < filled ? '=' : '-');
        }
        cout << "] " << percent << "% (" << status << ")" << endl;
    }
    if (!hasExpense) {
        cout << "Chua co danh muc CHI" << endl;
    }
}

void BudgetManager::setLimit(int categoryId, int month, int year, long long limit) {
    for (int i = 0; i < budgets.size(); ++i) {
        if (budgets[i].getCategoryId() == categoryId 
            && budgets[i].getMonth() == month 
            && budgets[i].getYear() == year) {
            budgets[i].setLimitAmount(limit);
            cout << "Da cap nhat ngan sach" << endl;
            return;
        }
    }
    budgets.add(Budget(categoryId, month, year, limit));
    cout << "Da them ngan sach" << endl;
}

long long BudgetManager::getLimit(int categoryId, int month, int year) const {
    for (int i = 0; i < budgets.size(); ++i) {
        if (budgets[i].getCategoryId() == categoryId
            && budgets[i].getMonth() == month
            && budgets[i].getYear() == year) {
            return budgets[i].getLimitAmount();
        }
    }
    return -1;
}

void BudgetManager::listByMonth(const CategoryManager& categoryManager, int month, int year) const {
    bool found = false;
    cout << "Danh muc | Thang/Nam | Han muc " << endl;
    for (int i = 0; i < budgets.size(); ++i) {
        if (budgets[i].getMonth() == month && budgets[i].getYear() == year) {
            const Category* category = categoryManager.getById(budgets[i].getCategoryId());
            cout << category->getName() << " | " << month << " / " << year << " | " 
                 << budgets[i].getLimitAmount() << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Khong co ngan sach cho thang nay" << endl;
    }
}

DynamicArray<Budget> BudgetManager::checkExceeded(const TransactionManager& transactionManager,
                                                 int month, int year) const {
    DynamicArray<Budget> exceeded;
    for (int i = 0; i < budgets.size(); ++i) {
        if (budgets[i].getMonth() != month || budgets[i].getYear() != year) {
            continue;
        }
        long long spent = transactionManager.getSpent(budgets[i].getCategoryId(), month, year);
        if (spent > budgets[i].getLimitAmount()) {
            exceeded.add(budgets[i]);
        }
    }
    return exceeded;
}

const DynamicArray<Budget>& BudgetManager::getAll() const {
    return budgets;
}
