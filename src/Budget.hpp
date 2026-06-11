#pragma once

#include <string>
#include "DynamicArray.hpp"

class TransactionManager;
class CategoryManager;

class Budget {
    private:
        int categoryId;
        int month;
        int year;
        long long limitAmount;
    public:
        Budget();
        Budget(int categoryId, int month, int year, long long limit);

        int getCategoryId() const;
        int getMonth() const;
        int getYear() const;
        long long getLimitAmount() const;

        void setLimitAmount(long long value);

        std::string toString() const;
};

class BudgetManager {
    private:
        DynamicArray<Budget> budgets;
        std::string dataFile;
    public:
        BudgetManager();

        void loadData(const std::string& fileName);
        void saveData() const;

        void updateBudget(CategoryManager& categoryManager, int month, int year);
        void showBudgetStatus(const CategoryManager& categoryManager,
                    const TransactionManager& transactionManager, int month, int year) const;

        void setLimit(int categoryId, int month, int year, long long limit);
        long long getLimit(int categoryId, int month, int year) const;
        void listByMonth(const CategoryManager& categoryManager, int month, int year) const;
        
        DynamicArray<Budget> checkExceeded(const TransactionManager& transactionManager,
                                            int month, int year) const;

        const DynamicArray<Budget>& getAll() const;
};
