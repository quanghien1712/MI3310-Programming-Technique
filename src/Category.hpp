#pragma once

#include <string>
#include "DynamicArray.hpp"

class TransactionManager;

class Category {
    private:
        std::string name;
        char type;// Thu là 'T', Chi là 'C'
    public: 
        Category();
        Category(const std::string& name, char type);

        const std::string& getName() const;
        char getType() const;

        void setName(const std::string& newName);

        void display() const;
        std::string toString() const;
};

class CategoryManager {
    private:
        DynamicArray<Category> categories;
        std::string dataFile;
    public:
        CategoryManager();

        void loadData(const std::string& fileName);
        void saveData() const;

        void addCategory();
        const Category* getById(int id) const;
        void rename(int id, const std::string& newName);
        void remove(int id, const TransactionManager& transactionManager);

        void display() const;
        
        int categoryOptions(char type) const;
        const DynamicArray<Category>& getAll() const;
};