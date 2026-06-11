#include "Report.hpp"

#include <iostream>
#include <iomanip>

#include "Transaction.hpp"
#include "Category.hpp"
#include "Budget.hpp"
#include "Utils.hpp"

using namespace std;

Report::Report(const TransactionManager& transactionManager, const CategoryManager& categoryManager,
               const BudgetManager& budgetManager)
    : transactionManager(transactionManager), categoryManager(categoryManager), budgetManager(budgetManager) {}

void Report::summaryByMonth(int month, int year) const {
    long long income = 0;
    long long expense = 0;
    int count = 0;
    const DynamicArray<Transaction>& transactions = transactionManager.getAll();
    for (int i = 0; i < transactions.size(); ++i) {
        const string& date = transactions[i].getDate();
        int tMonth = stoi(date.substr(5, 2));
        int tYear = stoi(date.substr(0, 4));
        if (tMonth == month && tYear == year) {
            count++;
            if (transactions[i].getType() == 'T') {
                income += transactions[i].getAmount();
            } else {
                expense += transactions[i].getAmount();
            }
        }
    }
    cout << endl;
    cout << "=== KET QUA THONG KE - THANG " << month << "/" << year << " ===" << endl;
    cout << "- Tong so giao dich: " << count << endl;
    cout << "- Tong THU: + " << Utils::formatMoney(income) << " VND" << endl;
    cout << "- Tong CHI: - " << Utils::formatMoney(expense) << " VND" << endl;

    DynamicArray<Budget> exceeded = budgetManager.checkExceeded(transactionManager, month, year);
    cout << "=== CAC KHOAN VUOT NGAN SACH TRONG THANG ===" << endl;
    for (int i = 0; i < exceeded.size(); ++i) {
        long long spent = transactionManager.getSpent(exceeded[i].getCategoryId(), month, year);
        long long limit = exceeded[i].getLimitAmount();
        const Category* category = categoryManager.getById(exceeded[i].getCategoryId());
        cout << i + 1 << ". " << category->getName() << ": Vuot " 
             << Utils::formatMoney(spent - limit) << " VND" << endl;
    }
}

void Report::summaryByYear(int year) const {
    long long income = 0;
    long long expense = 0;
    int count = 0;
    const DynamicArray<Transaction>& transactions = transactionManager.getAll();
    for (int i = 0; i < transactions.size(); ++i) {
        const string& date = transactions[i].getDate();
        int tYear = stoi(date.substr(0, 4));
        if (tYear != year) continue;

        count++;
        if (transactions[i].getType() == 'T') {
            income += transactions[i].getAmount();
        } else {
            expense += transactions[i].getAmount();
        }
    }
    cout << endl;
    cout << "=== KET QUA THONG KE - NAM " << year << " ===" << endl;
    cout << "- Tong so giao dich: " << count << endl;
    cout << "- Tong THU: + " << Utils::formatMoney(income) << " VND" << endl;
    cout << "- Tong CHI: - " << Utils::formatMoney(expense) << " VND" << endl;
}

void Report::printBarChart(int month, int year) const {
    const DynamicArray<Category>& categories = categoryManager.getAll();
    if (categories.isEmpty()) {
        cout << "Chua co danh muc" << endl;
        return;
    }

    long long totalExpense = 0;
    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].getType() == 'C') {
            totalExpense += transactionManager.getSpent(i, month, year);
        }
    }

    cout << "=== BIEU DO TY LE CHI TIEU THANG " << month << "/" << year << " ===" << endl;
    cout << "Tong chi: " << Utils::formatMoney(totalExpense) << " VND" << endl;
    cout << endl;

    if (totalExpense == 0) return;
    const int barWidth = 25;
    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].getType() != 'C') continue;

        long long spent = transactionManager.getSpent(i, month, year);
        double percent = (spent * 100.0) / totalExpense;
        int filled = ((int)(percent) * barWidth) / 100;
        cout << categories[i].getName() << " (" << fixed << setprecision(2) << percent << "%) : ";
        for (int j = 0; j < barWidth; ++j) {
            cout << (j < filled ? '#' : ' ');
        }
        cout << " (" << Utils::formatMoney(spent) << " VND)" << endl;
    }
}