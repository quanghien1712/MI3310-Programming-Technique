#include "Transaction.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include "Utils.hpp"
#include "Category.hpp"
#include "Budget.hpp"

using namespace std;

Transaction::Transaction()
    : date(""), categoryId(0), amount(0), note(""), type('C') {}

Transaction::Transaction(const string& date, int categoryId, long long amount, const string& note, char type)
    : date(date), categoryId(categoryId), amount(amount), note(note), type(type) {}

const string& Transaction::getDate() const {
    return date;
}

int Transaction::getCategoryId() const {
    return categoryId;
}

long long Transaction::getAmount() const {
    return amount;
}

const string& Transaction::getNote() const {
    return note;
}

char Transaction::getType() const {
    return type;
}

void Transaction::setDate(const string& value) {
    date = value;
}

void Transaction::setCategoryId(int value) {
    categoryId = value;
}

void Transaction::setAmount(long long value) {
    amount = value;
}

void Transaction::setNote(const string& value) {
    note = value;
}

void Transaction::setType(char value) {
    type = value;
}

string Transaction::toString() const {
    return date + "|" + to_string(categoryId) + "|" +
        to_string(amount) + "|" + type + "|" + note;
}

TransactionManager::TransactionManager() : dataFile("") {}

void TransactionManager::loadData(const string& fileName) {
    dataFile = fileName;
    transactions.clear();

    ifstream input(fileName);
    if (!input) return;

    string line;
    string date, note;
    int categoryId;
    long long amount;
    char type;
    while (getline(input, line)) {
        stringstream ss(line);
        string tmp;

        getline(ss, date, '|');

        getline(ss, tmp, '|');
        if (tmp.empty() || !Utils::isNumber(tmp)) {
            continue;
        }
        categoryId = stoi(tmp);

        getline(ss, tmp, '|');
        if (tmp.empty() || !Utils::isNumber(tmp, true)) {
            continue;
        }
        amount = stoll(tmp);

        getline(ss, tmp, '|');
        if (tmp.empty()) {
            continue;
        }
        type = tmp[0];

        getline(ss, note);

        transactions.add(Transaction(date, categoryId, amount, note, type));
    }
    input.close();
}

void TransactionManager::saveData() const {
    ofstream output(dataFile);
    if (!output) return;
    
    for (int i = 0; i < transactions.size(); ++i) {
        output << transactions[i].toString() << endl;
    }

    output.close();
}

void TransactionManager::addTransaction(const CategoryManager& categoryManager, 
                                    const BudgetManager& budgetManager, char type) {
    const DynamicArray<Category>& categories = categoryManager.getAll();
    bool hasCategory = false;
    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].getType() == type) {
            hasCategory = true;
            break;
        }
    }
    if (!hasCategory) {
        cout << "Chua co danh muc " << (type == 'C' ? "CHI" : "THU") << endl;
        return;
    }
    cout << endl;
    cout << "== THEM KHOAN " << (type == 'C' ? "CHI" : "THU") << " ===" << endl;
    long long amount = Utils::readLongLong("-> Nhap so tien (VND): ");

    int categoryId = categoryManager.categoryOptions(type);
    const Category* category = categoryManager.getById(categoryId);

    string note = Utils::readNote();
    string date = Utils::readDate();

    transactions.add(Transaction(date, categoryId, amount, note, type));
    
    if (type == 'C') {
        cout << "Da luu thanh cong!" << endl;
        cout << "Giao dich: - " << Utils::formatMoney(amount) << " VND | Danh muc: "
             << category->getName() << " | Ghi chu: \"" << note << "\"" << endl;

        int month = stoi(date.substr(5, 2));
        int year = stoi(date.substr(0, 4));
        long long limit = budgetManager.getLimit(categoryId, month, year);
        long long spent = getSpent(categoryId, month, year);
        long long remain = limit - spent;
        if (limit > 0 && remain < 0) {
            cout << "CANH BAO: Ban da chi " << Utils::formatMoney(spent) << " / "
                    << Utils::formatMoney(limit) << " VND cho \"" << category->getName()
                    << "\" trong thang nay" << endl;
            cout << "Da vuot " << Utils::formatMoney(-remain) << " VND" << endl;
        }
    } else {
        long long balance = currentBalance();
        cout << "Da luu thanh cong! So du hien tai tang len: ";
        if (balance >= 0) {
            cout << "+ " << Utils::formatMoney(balance) << " VND" << endl;
        } else {
            cout << "- " << Utils::formatMoney(-balance) << " VND" << endl;
        }
    }
    
}

void TransactionManager::editTransaction(int id, const CategoryManager& categoryManager, const BudgetManager& budgetManager) {
    Transaction* transaction = getById(id);
    if (transaction == nullptr) {
        cout << "Khong tim thay giao dich" << endl;
        return;
    }
    cout << endl;
    cout << "=== Sua giao dich ===" << endl;
    while (true) {
        const Category* category = categoryManager.getById(transaction->getCategoryId());
        string categoryName = category->getName();
        string date = Utils::ymdToDmy(transaction->getDate());
        char type = transaction->getType();
        string amount = Utils::formatMoney(transaction->getAmount());
        string note = transaction->getNote();

        cout << "Thong tin giao dich:" << endl;
        cout << "1. Ngay: " << date << endl;
        cout << "2. Loai: " << (type == 'C' ? "CHI" : "THU") << endl;
        cout << "3. So tien: " << amount << endl;
        cout << "4. Danh muc: " << categoryName << endl;
        cout << "5. Ghi chu: " << note << endl;
        cout << "0. Luu va thoat" << endl;

        int choice = Utils::readInt("-> Nhap lua chon: ");
        if (choice == 0) break;
        switch (choice) {
            case 1: {
                string newDate = Utils::readDate();
                transaction->setDate(newDate);
                break;
            }
            case 2: {
                char newType = Utils::readType();
                transaction->setType(newType);
                break;
            }
            case 3: {
                long long amount = Utils::readLongLong("-> Nhap so tien: ");
                transaction->setAmount(amount);
                break;
            }
            case 4: {
                int categoryId = categoryManager.categoryOptions(transaction->getType());
                transaction->setCategoryId(categoryId);
                break;
            }
            case 5: {
                string newNote = Utils::readNote();
                transaction->setNote(newNote);
                break;
            }
            default:
                cout << "Option khong hop le" << endl;
        }
    }

    if (transaction->getType() == 'C') {
        int month = stoi(transaction->getDate().substr(5, 2));
        int year = stoi(transaction->getDate().substr(0, 4));
        long long limit = budgetManager.getLimit(transaction->getCategoryId(), month, year);
        long long spent = getSpent(transaction->getCategoryId(), month, year);
        long long remain = limit - spent;
        if (limit > 0 && spent > limit) {
            cout << "CANH BAO: Vuot ngan sach " << Utils::formatMoney(-remain) << "VND" << endl;
        }
    }
    cout << "Da cap nhat giao dich" << endl;
}

void TransactionManager::removeTransaction(int id) {
    if (id < 0 || id >= transactions.size()) {
        cout << "Khong ton tai giao dich" << endl;
    }
    transactions.removeAt(id);
}

void TransactionManager::display(const CategoryManager& categoryManager) const {
    cout << endl;
    cout << "=== LICH SU GIAO DICH GAN DAY ===" << endl;
    if (transactions.isEmpty()) {
        cout << "Khong co giao dich" << endl;
        return;
    }
    cout << endl;
    cout << "ID  | Ngay | Loai | Danh muc | So tien (VND) | Ghi chu" << endl;
    cout << "-------------------------------------------------------------" << endl;
    for (int i = transactions.size() - 1; i >= 0; --i) {
        const Category* category = categoryManager.getById(transactions[i].getCategoryId());
        string name = (category != nullptr) ? category->getName() : "Khac";
        string date = Utils::ymdToDmy(transactions[i].getDate());
        string sign = (transactions[i].getType() == 'T') ? "+ " : "- ";
        string type = (transactions[i].getType() == 'T') ? "THU" : "CHI";

        cout << i << " | " << date << " | " << type << " | " << name 
             << " | " << sign << Utils::formatMoney(transactions[i].getAmount())
             << " | " << transactions[i].getNote() << endl;
    }
}

Transaction* TransactionManager::getById(int id) {
    if (id < 0 || id >= transactions.size()) {
        return nullptr;
    }
    return &transactions[id];
}

bool TransactionManager::hasCategory(int categoryId) const {
    for (int i = 0; i < transactions.size(); ++i) {
        if (transactions[i].getCategoryId() == categoryId) {
            return true;
        }
    }
    return false;
}

long long TransactionManager::currentBalance() const {
    long long income = 0;
    long long expense = 0;
    for (int i = 0; i < transactions.size(); ++i) {
        if (transactions[i].getType() == 'T') {
            income += transactions[i].getAmount();
        } else {
            expense += transactions[i].getAmount();
        }
    }
    return income - expense;
}

long long TransactionManager::getSpent(int categoryId, int month, int year) const {
    long long total = 0;
    for (int i = 0; i < transactions.size(); ++i) {
        if (transactions[i].getCategoryId() != categoryId || transactions[i].getType() != 'C') {
            continue;
        }
        const string& date = transactions[i].getDate();
        if (date.size() != 10) {
            continue;
        }
        int tMonth = stoi(date.substr(5, 2));
        int tYear = stoi(date.substr(0, 4));
        if (tMonth == month && tYear == year) {
            total += transactions[i].getAmount();
        }
    }
    return total;
}

const DynamicArray<Transaction>& TransactionManager::getAll() const {
    return transactions;
}
