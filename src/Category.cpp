#include "Category.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Utils.hpp"
#include "Transaction.hpp"

using namespace std;

Category::Category()
    : name(""), type('C') {}

Category::Category(const string& name, char type)
    : name(name), type(type) {}

const string& Category::getName() const {
    return name;
}

char Category::getType() const {
    return type;
}

void Category::setName(const string& newName){
    this->name = newName;
}

void Category::display() const {
    cout << name << " | " << (type == 'T' ? "THU" : "CHI");
}

string Category::toString() const {
    return name + "|" + type;
}

CategoryManager::CategoryManager() : dataFile("") {}

void CategoryManager::loadData(const string& fileName) {
    dataFile = fileName;
    categories.clear();

    ifstream input(fileName);
    if (!input) return;

    string line;
    string name;
    char type;
    while (getline(input, line)) {
        stringstream ss(line);
        string tmp;

        getline(ss, name, '|');
        getline(ss, tmp, '|');
        if (tmp.empty()) {
            continue;
        }
        type = tmp[0];

        categories.add(Category(name, type));
    }
    input.close();
}

void CategoryManager::saveData() const {
    ofstream output(dataFile);
    if (!output) return;
    
    for (int i = 0; i < categories.size(); ++i) {
        output << categories[i].toString() << endl;
    }

    output.close();
}

void CategoryManager::addCategory() {
    cout << endl;
    cout << "=== THEM DANH MUC MOI ===" << endl;
    string name = Utils::readLine("-> Ten danh muc: ");

    char type = Utils::readType();
    categories.add(Category(name, type));
    cout << "Them thanh cong danh muc \"" << name << "\" vao nhom "
         << (type == 'T' ? "THU" : "CHI") << endl;
}

const Category* CategoryManager::getById(int id) const {
    if (id < 0 || id >= categories.size()) {
        return nullptr;
    }
    return &categories[id];
}

void CategoryManager::rename(int id, const string& newName) {
    if (id < 0 || id >= categories.size()) {
        cout << "Khong tim thay danh muc" << endl;
        return;
    }
    if (newName.empty()) {
        cout << "Ten khong duoc rong" << endl;
        return;
    }
    categories[id].setName(newName);
    cout << "Da cap nhat ten danh muc" << endl;
}

void CategoryManager::remove(int id, const TransactionManager& transManager) {
    if (id < 0 || id >= categories.size()) {
        cout << "Khong tim thay danh muc" << endl;
        return;
    }

    if (transManager.hasCategory(id)) {
        cout << "Khong the xoa danh muc dang co giao dich" << endl;
        return;
    }
    
    string confirm = Utils::readLine("Xac nhan xoa? (Y/N): ");
    if (confirm != "Y" && confirm != "y") {
        cout << "Da huy thao tac" << endl;
        return;
    }
    categories.removeAt(id);
    cout << "Da xoa danh muc" << endl;
}

void CategoryManager::display() const {
    cout << endl;
    cout << "=== DANH SACH DANH MUC ===" << endl;
    if (categories.isEmpty()) {
        cout << "Chua co danh muc" << endl;
        return;
    }
    cout << "ID | Ten | Loai" << endl;
    cout << "----------------------------------" << endl;
    for (int i = 0; i < categories.size(); ++i) {
        cout << i << " | ";
        categories[i].display();
        cout << endl;
    }
    cout << endl;
}

int CategoryManager::categoryOptions(char type) const{
    string options;
    bool first = true;
    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].getType() != type) {
            continue;
        }
        if (!first) {
            options += ", ";
        }
        options += to_string(i) + "-" + categories[i].getName();
        first = false;
    }

    const Category* category = nullptr;
    int categoryId = 0;
    while (true) {
        categoryId = Utils::readInt("-> Chon danh muc (" + options + "): ");
        category = getById(categoryId);
        if (category != nullptr && category->getType() == type) {
            break;
        }
        cout << "Danh muc khong hop le" << endl;
    }
    return categoryId;
}

const DynamicArray<Category>& CategoryManager::getAll() const {
    return categories;
}
