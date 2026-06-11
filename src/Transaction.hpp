#pragma once

#include <string>
#include "DynamicArray.hpp"

class CategoryManager;
class BudgetManager;

class Transaction {
    private:
        std::string date;
        int categoryId;
        long long amount;
        std::string note;
        char type;
    public:
        Transaction();
        Transaction(const std::string& date, int categoryId,
                    long long amount, const std::string& note, char type);

        const std::string& getDate() const;
        int getCategoryId() const;
        long long getAmount() const;
        const std::string& getNote() const;
        char getType() const;

        void setDate(const std::string& value);
        void setCategoryId(int value);
        void setAmount(long long value);
        void setNote(const std::string& value);
        void setType(char value);

        std::string toString() const;
};

class TransactionManager {
    private:
        DynamicArray<Transaction> transactions;
        std::string dataFile;
    public:
        TransactionManager();

        void loadData(const std::string& fileName);
        void saveData() const;

        void addTransaction(const CategoryManager& categoryManager, const BudgetManager& budgetManager, char type);
        void editTransaction(int id, const CategoryManager& categoryManager, const BudgetManager& budgetManager);
        void removeTransaction(int id);

        void display(const CategoryManager& categoryManager) const;

        Transaction* getById(int id);
        bool hasCategory(int categoryId) const;
        long long currentBalance() const;
        long long getSpent(int categoryId, int month, int year) const;
        
        const DynamicArray<Transaction>& getAll() const;
};
