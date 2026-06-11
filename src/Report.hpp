#pragma once

#include <string>

class TransactionManager;
class CategoryManager;
class BudgetManager;

class Report {
    private:
        const TransactionManager& transactionManager;
        const CategoryManager& categoryManager;
        const BudgetManager& budgetManager;
    public:
        Report(const TransactionManager& transactionManager, const CategoryManager& categoryManager,
                     const BudgetManager& budgetManager);

        void summaryByMonth(int month, int year) const;
        void summaryByYear(int year) const;
        
        void printBarChart(int month, int year) const;
};
