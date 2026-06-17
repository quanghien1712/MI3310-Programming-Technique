#include <gtest/gtest.h>
#include <fstream>
#include "../src/Transaction.hpp"
#include "../src/Category.hpp"
#include "../src/Budget.hpp"


//  Transaction
TEST(TransactionTest, Constructor_MacDinh) {
    Transaction t;
    EXPECT_EQ(t.getDate(), "");
    EXPECT_EQ(t.getCategoryId(), 0);
    EXPECT_EQ(t.getAmount(), 0);
    EXPECT_EQ(t.getNote(), "");
    EXPECT_EQ(t.getType(), 'C');
}

TEST(TransactionTest, Constructor_CoThamSo) {
    Transaction t("2026-06-08", 1, 500000LL, "An trua", 'C');
    EXPECT_EQ(t.getDate(), "2026-06-08");
    EXPECT_EQ(t.getCategoryId(), 1);
    EXPECT_EQ(t.getAmount(), 500000LL);
    EXPECT_EQ(t.getNote(), "An trua");
    EXPECT_EQ(t.getType(), 'C');
}

TEST(TransactionTest, Setter) {
    Transaction t;
    t.setDate("2026-01-15");
    t.setCategoryId(3);
    t.setAmount(1000000LL);
    t.setNote("Luong thang 1");
    t.setType('T');

    EXPECT_EQ(t.getDate(), "2026-01-15");
    EXPECT_EQ(t.getCategoryId(), 3);
    EXPECT_EQ(t.getAmount(), 1000000LL);
    EXPECT_EQ(t.getNote(), "Luong thang 1");
    EXPECT_EQ(t.getType(), 'T');
}

TEST(TransactionTest, ToString_DinhDangDung) {
    Transaction t("2026-06-08", 2, 300000LL, "Xang xe", 'C');
    EXPECT_EQ(t.toString(), "2026-06-08|2|300000|C|Xang xe");
}

TEST(TransactionTest, ToString_GhiChuRong) {
    Transaction t("2026-06-08", 0, 0LL, "", 'T');
    EXPECT_EQ(t.toString(), "2026-06-08|0|0|T|");
}


// TransactionManager
class TransactionManagerTest : public ::testing::Test {
protected:
    TransactionManager tm;
    CategoryManager cm;
    BudgetManager bm;

    void SetUp() override {
        {
            std::ofstream f("data/test_transactions.txt");
            f << "2026-06-01|0|500000|C|An trua\n";
            f << "2026-06-02|1|2000000|T|Luong\n";
            f << "2026-06-03|0|100000|C|Ca phe\n";
        }
        {
            std::ofstream f("data/test_categories.txt");
            f << "An uong|C\n";
            f << "Luong|T\n";
        }
        cm.loadData("data/test_categories.txt");
        tm.loadData("data/test_transactions.txt");
    }

    void TearDown() override {
        std::remove("data/test_transactions.txt");
        std::remove("data/test_categories.txt");
    }
};

TEST_F(TransactionManagerTest, LoadData_DungSoLuong) {
    EXPECT_EQ(tm.getAll().size(), 3);
}

TEST_F(TransactionManagerTest, LoadData) {
    const Transaction& t = tm.getAll()[0];
    EXPECT_EQ(t.getDate(), "2026-06-01");
    EXPECT_EQ(t.getCategoryId(), 0);
    EXPECT_EQ(t.getAmount(), 500000LL);
    EXPECT_EQ(t.getNote(), "An trua");
    EXPECT_EQ(t.getType(), 'C');
}

TEST_F(TransactionManagerTest, GetById_HopLe) {
    Transaction* t = tm.getById(0);
    ASSERT_NE(t, nullptr);
    EXPECT_EQ(t->getAmount(), 500000LL);
}

TEST_F(TransactionManagerTest, GetById_NgoaiPhamVi) {
    EXPECT_EQ(tm.getById(-1), nullptr);
    EXPECT_EQ(tm.getById(99), nullptr);
}

TEST_F(TransactionManagerTest, HasCategory_TonTai) {
    EXPECT_TRUE(tm.hasCategory(0));
    EXPECT_TRUE(tm.hasCategory(1));
}

TEST_F(TransactionManagerTest, HasCategory_KhongTonTai) {
    EXPECT_FALSE(tm.hasCategory(99));
}

TEST_F(TransactionManagerTest, CurrentBalance) {
    EXPECT_EQ(tm.currentBalance(), 1400000LL);
}

TEST_F(TransactionManagerTest, GetSpent) {
    EXPECT_EQ(tm.getSpent(0, 6, 2026), 600000LL);
}

TEST_F(TransactionManagerTest, GetSpent_ThangKhac) {
    EXPECT_EQ(tm.getSpent(0, 5, 2026), 0LL);
    EXPECT_EQ(tm.getSpent(0, 6, 2025), 0LL);
}

TEST_F(TransactionManagerTest, GetSpent_ChiTinhLoaiChi) {
    EXPECT_EQ(tm.getSpent(1, 6, 2026), 0LL);
}

TEST_F(TransactionManagerTest, RemoveTransaction_HopLe) {
    tm.removeTransaction(0);
    EXPECT_EQ(tm.getAll().size(), 2);
}

TEST_F(TransactionManagerTest, SaveData_VaLoadLai) {
    tm.saveData();
    TransactionManager tm2;
    tm2.loadData("data/test_transactions.txt");
    EXPECT_EQ(tm2.getAll().size(), 3);
    EXPECT_EQ(tm2.getAll()[1].getAmount(), 2000000LL);
}

TEST_F(TransactionManagerTest, LoadData_SaiDinhDang) {
    {
        std::ofstream f("data/test_bad_tx.txt");
        f << "2026-06-01|0|500000|C|HopLe\n";
        f << "dong_sai_hoan_toan\n";
        f << "|abc|xyz|C|No type\n";
    }
    TransactionManager tm2;
    tm2.loadData("data/test_bad_tx.txt");

    EXPECT_EQ(tm2.getAll().size(), 1);
    std::remove("data/test_bad_tx.txt");
}

TEST_F(TransactionManagerTest, CurrentBalance_KhiKhongCoGiaoDich) {
    TransactionManager empty;
    EXPECT_EQ(empty.currentBalance(), 0LL);
}
