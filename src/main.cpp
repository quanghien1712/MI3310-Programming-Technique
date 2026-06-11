#include <iostream>

#include "Category.hpp"
#include "Transaction.hpp"
#include "Budget.hpp"
#include "Report.hpp"
#include "Utils.hpp"

using namespace std;

static void showMainMenu(const TransactionManager& transactionManager) {
    string today = Utils::getTodayDmy();
    long long balance = transactionManager.currentBalance();
    cout << endl;
    cout << "=========================================================" << endl;
    cout << "          UNG DUNG QUAN LY CHI TIEU CA NHAN" << endl;
    cout << "=========================================================" << endl;
    cout << "Xin chao, Tran Quang Minh - MI1 - K69" << endl; 
    cout << "[Hom nay: " << today << "]" << endl;
    cout << "[So du hien tai: " << (balance >= 0 ? "+ " : "- ")
         << Utils::formatMoney(balance >= 0 ? balance : -balance) << " VND]" << endl;
    cout << endl;
    cout << "1. Quan ly Giao dich" << endl;
    cout << "2. Quan ly Ngan sach" << endl;
    cout << "3. Bao cao & Thong ke" << endl;
    cout << "4. Cai dat Danh muc" << endl;
    cout << "0. Thoat ung dung" << endl;
    cout << "=========================================================" << endl;
}

static bool parseMonthYear(const string& input, int& month, int& year) {
    if (input.size() != 7 || input[2] != '/') {
        return false;
    }
    string m = input.substr(0, 2);
    string y = input.substr(3, 4);
    if (!Utils::isNumber(m) || !Utils::isNumber(y)) {
        return false;
    }
    month = stoi(m);
    year = stoi(y);
    return month >= 1 && month <= 12 && year > 0;
}

static void menuTransaction(TransactionManager& transactionManager,
                CategoryManager& categoryManager, BudgetManager& budgetManager) {
    int choice = -1;
    while (choice != 0) {
        cout << endl;
        cout << "=== QUAN LY GIAO DICH ===" << endl;
        cout << "1. Them khoan CHI moi" << endl;
        cout << "2. Them khoan THU moi" << endl;
        cout << "3. Xem danh sach giao dich" << endl;
        cout << "4. Sua giao dich" << endl;
        cout << "5. Xoa giao dich" << endl;
        cout << "0. Quay lai Menu chinh" << endl;
        choice = Utils::readInt("-> Nhap lua chon: ");
        switch (choice) {
            case 1:
                transactionManager.addTransaction(categoryManager, budgetManager, 'C');
                break;
            case 2:
                transactionManager.addTransaction(categoryManager, budgetManager, 'T');
                break;
            case 3:
                transactionManager.display(categoryManager);
                break;
            case 4: {
                transactionManager.display(categoryManager);
                int id = Utils::readInt("-> Nhap ID giao dich muon sua: ");
                transactionManager.editTransaction(id, categoryManager, budgetManager);
                break;
            }
            case 5: {
                transactionManager.display(categoryManager);
                int id = Utils::readInt("-> Nhap ID giao dich muon xoa: ");
                transactionManager.removeTransaction(id);
                break;
            }
            case 0:
                break;
            default:
                cout << "Lua chon khong hop le" << endl;
        }
    }
}

static void menuBudget(BudgetManager& budgetManager, CategoryManager& categoryManager,
                       TransactionManager& transactionManager) {
    string todayYmd = Utils::getTodayYmd();
    int currentMonth = stoi(todayYmd.substr(5, 2));
    int currentYear = stoi(todayYmd.substr(0, 4));

    int choice = -1;
    while (choice != 0) {
        cout << endl;
        cout << "=== QUAN LY NGAN SACH - THANG " << currentMonth << "/" << currentYear << " ===" << endl;
        cout << "1. Thiet lap / Cap nhat ngan sach" << endl;
        cout << "2. Xem trang thai ngan sach hien tai" << endl;
        cout << "0. Quay lai Menu chinh" << endl;
        choice = Utils::readInt("-> Nhap lua chon: ");
        switch (choice) {
            case 1:
                budgetManager.updateBudget(categoryManager, currentMonth, currentYear);
                break;
            case 2:
                budgetManager.showBudgetStatus(categoryManager, transactionManager,
                                               currentMonth, currentYear);
                break;
            case 0:
                break;
            default:
                cout << "Lua chon khong hop le" << endl;
                break;
        }
    }
}

static void menuReport(Report& report, CategoryManager& categoryManager) {
    int choice = -1;
    while (choice != 0) {
        cout << endl;
        cout << "=== BAO CAO & THONG KE ===" << endl;
        cout << "1. Thong ke theo Thang" << endl;
        cout << "2. Thong ke theo Nam" << endl;
        cout << "3. Bao cao ty le chi tieu" << endl;
        cout << "0. Quay lai Menu chinh" << endl;
        choice = Utils::readInt("-> Nhap lua chon: ");
        switch (choice) {
            case 1: {
                string input = Utils::readLine("-> Nhap thang/nam (MM/YYYY): ");
                int month = 0;
                int year = 0;
                if (!parseMonthYear(input, month, year)) {
                    cout << "Thang/nam khong hop le" << endl;
                    break;
                }
                report.summaryByMonth(month, year);
                Utils::readLine("Nhan phim bat ky de quay lai...");
                break;
            }
            case 2: {
                int year = Utils::readInt("-> Nhap nam: ");
                report.summaryByYear(year);
                Utils::readLine("Nhan phim bat ky de quay lai...");
                break;
            }
            case 3: {
                string input = Utils::readLine("-> Nhap thang/nam (MM/YYYY): ");
                int month = 0;
                int year = 0;
                if (!parseMonthYear(input, month, year)) {
                    cout << "Thang/nam khong hop le" << endl;
                    break;
                }
                report.printBarChart(month, year);
                Utils::readLine("Nhan phim bat ky de quay lai...");
                break;
            }
            case 0:
                break;
            default:
                cout << "Lua chon khong hop le" << endl;
                break;
        }
    }
}

static void menuCategory(CategoryManager& categoryManager, TransactionManager& transactionManager) {
    int choice = -1;
    while (choice != 0) {
        cout << endl;
        cout << "=== CAI DAT DANH MUC ===" << endl;
        cout << "1. Xem danh sach danh muc" << endl;
        cout << "2. Them danh muc moi" << endl;
        cout << "3. Sua ten danh muc" << endl;
        cout << "4. Xoa danh muc" << endl;
        cout << "0. Quay lai Menu chinh" << endl;
        choice = Utils::readInt("-> Nhap lua chon: ");
        switch (choice) {
            case 1:
                categoryManager.display();
                break;
            case 2:
                categoryManager.addCategory();
                break;
            case 3: {
                int id = Utils::readInt("-> Nhap ID danh muc: ");
                string newName = Utils::readLine("-> Ten moi: ");
                categoryManager.rename(id, newName);
                break;
            }
            case 4: {
                int id = Utils::readInt("-> Nhap ID danh muc muon xoa: ");
                categoryManager.remove(id, transactionManager);
                break;
            }
            case 0:
                break;
            default:
                cout << "Lua chon khong hop le" << endl;
                break;
        }
    }
}

int main() {
    CategoryManager categoryManager;
    TransactionManager transactionManager;
    BudgetManager budgetManager;

    categoryManager.loadData("../data/categories.txt");
    transactionManager.loadData("../data/transactions.txt");
    budgetManager.loadData("../data/budgets.txt");

    Report report(transactionManager, categoryManager, budgetManager);

    int choice = -1;
    while (choice != 0) {
        showMainMenu(transactionManager);
        choice = Utils::readInt("-> Nhap lua chon: ");
        switch (choice) {
            case 1:
                menuTransaction(transactionManager, categoryManager, budgetManager);
                break;
            case 2:
                menuBudget(budgetManager, categoryManager, transactionManager);
                break;
            case 3:
                menuReport(report, categoryManager);
                break;
            case 4:
                menuCategory(categoryManager, transactionManager);
                break;
            case 0:
                break;
            default:
                cout << "Lua chon khong hop le" << endl;
                break;
        }
    }
    categoryManager.saveData();
    transactionManager.saveData();
    budgetManager.saveData();
    return 0;
}
