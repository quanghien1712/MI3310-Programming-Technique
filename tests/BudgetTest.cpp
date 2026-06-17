#include <gtest/gtest.h>
#include <fstream>
#include "../src/Budget.hpp"
#include "../src/Transaction.hpp"
#include "../src/Category.hpp"

// Budget
TEST(BudgetTest, Constructor) {
    Budget b;
    EXPECT_EQ(b.getCategoryId(), 0);
    EXPECT_EQ(b.getMonth(), 0);
    EXPECT_EQ(b.getYear(), 0);
    EXPECT_EQ(b.getLimitAmount(), 0);
}

TEST(BudgetTest, Constructor_CoThamSo) {
    Budget b(1, 6, 2026, 2000000LL);
    EXPECT_EQ(b.getCategoryId(), 1);
    EXPECT_EQ(b.getMonth(), 6);
    EXPECT_EQ(b.getYear(), 2026);
    EXPECT_EQ(b.getLimitAmount(), 2000000LL);
}

TEST(BudgetTest, SetLimitAmount) {
    Budget b(0, 6, 2026, 1000000LL);
    b.setLimitAmount(3000000LL);
    EXPECT_EQ(b.getLimitAmount(), 3000000LL);
}

TEST(BudgetTest, ToString) {
    Budget b(2, 6, 2026, 500000LL);
    EXPECT_EQ(b.toString(), "2|6|2026|500000");
}

//  BudgetManager
class BudgetManagerTest : public ::testing::Test {
protected:
    BudgetManager bm;
    TransactionManager tm;
    CategoryManager cm;

    void SetUp() override {
        {
            std::ofstream f("data/test_budgets.txt");
            f << "0|6|2026|2000000\n";
            f << "1|6|2026|1000000\n";
            f << "0|5|2026|1500000\n";
        }
        bm.loadData("data/test_budgets.txt");

        {
            std::ofstream f("data/test_tx_budget.txt");
            f << "2026-06-01|0|800000|C|Hang 1\n";
            f << "2026-06-10|0|700000|C|Hang 2\n";  
            f << "2026-06-05|1|1200000|C|Di lai\n";
        }
        tm.loadData("data/test_tx_budget.txt");
    }

    void TearDown() override {
        std::remove("data/test_budgets.txt");
        std::remove("data/test_tx_budget.txt");
    }
};

TEST_F(BudgetManagerTest, LoadData) {
    EXPECT_EQ(bm.getAll().size(), 3);
}

TEST_F(BudgetManagerTest, GetLimit) {
    EXPECT_EQ(bm.getLimit(0, 6, 2026), 2000000LL);
    EXPECT_EQ(bm.getLimit(1, 6, 2026), 1000000LL);
    EXPECT_EQ(bm.getLimit(0, 5, 2026), 1500000LL);
}

TEST_F(BudgetManagerTest, GetLimit_KhongTimThay) {
    EXPECT_EQ(bm.getLimit(99, 6, 2026), -1LL);
    EXPECT_EQ(bm.getLimit(0, 1, 2026), -1LL);
}

TEST_F(BudgetManagerTest, SetLimit_ThemMoi) {
    bm.setLimit(2, 6, 2026, 500000LL);
    EXPECT_EQ(bm.getLimit(2, 6, 2026), 500000LL);
}

TEST_F(BudgetManagerTest, SetLimit_CapNhatHienCo) {
    bm.setLimit(0, 6, 2026, 3000000LL);
    EXPECT_EQ(bm.getLimit(0, 6, 2026), 3000000LL);
    EXPECT_EQ(bm.getAll().size(), 3);
}

TEST_F(BudgetManagerTest, CheckExceeded) {
    DynamicArray<Budget> exceeded = bm.checkExceeded(tm, 6, 2026);
    EXPECT_EQ(exceeded.size(), 1);
    EXPECT_EQ(exceeded[0].getCategoryId(), 1);
}

TEST_F(BudgetManagerTest, CheckExceeded_KhongCoDanhMucVuot) {
    DynamicArray<Budget> exceeded = bm.checkExceeded(tm, 5, 2026);
    EXPECT_EQ(exceeded.size(), 0);
}

TEST_F(BudgetManagerTest, CheckExceeded_ThangKhac) {
    DynamicArray<Budget> exceeded = bm.checkExceeded(tm, 1, 2025);
    EXPECT_EQ(exceeded.size(), 0);
}

TEST_F(BudgetManagerTest, SaveData_VaLoadLai) {
    bm.saveData();
    BudgetManager bm2;
    bm2.loadData("data/test_budgets.txt");
    EXPECT_EQ(bm2.getAll().size(), 3);
    EXPECT_EQ(bm2.getLimit(0, 6, 2026), 2000000LL);
}

TEST_F(BudgetManagerTest, LoadData_SaiDinhDang) {
    {
        std::ofstream f("data/test_bad_budget.txt");
        f << "0|6|2026|2000000\n";       
        f << "abc|6|2026|1000000\n";     
        f << "0|xyz|2026|500000\n";      
        f << "0|6|abc|500000\n";         
        f << "0|6|2026|notnum\n";        
    }
    BudgetManager bm2;
    bm2.loadData("data/test_bad_budget.txt");
    EXPECT_EQ(bm2.getAll().size(), 1);
    std::remove("data/test_bad_budget.txt");
}
