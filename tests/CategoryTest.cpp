#include <gtest/gtest.h>
#include <fstream>
#include "../src/Category.hpp"
#include "../src/Transaction.hpp"

// Category
TEST(CategoryTest, Constructor) {
    Category c;
    EXPECT_EQ(c.getName(), "");
    EXPECT_EQ(c.getType(), 'C');
}

TEST(CategoryTest, Constructor_CoThamSo) {
    Category c("An uong", 'C');
    EXPECT_EQ(c.getName(), "An uong");
    EXPECT_EQ(c.getType(), 'C');

    Category c2("Luong", 'T');
    EXPECT_EQ(c2.getName(), "Luong");
    EXPECT_EQ(c2.getType(), 'T');
}

TEST(CategoryTest, SetName) {
    Category c("Cu", 'C');
    c.setName("Moi");
    EXPECT_EQ(c.getName(), "Moi");
}

TEST(CategoryTest, ToString) {
    Category c("An uong", 'C');
    EXPECT_EQ(c.toString(), "An uong|C");

    Category c2("Luong", 'T');
    EXPECT_EQ(c2.toString(), "Luong|T");
}

// CategoryManager
class CategoryManagerTest : public ::testing::Test {
protected:
    CategoryManager cm;
    TransactionManager tm;

    void SetUp() override {
        {
            std::ofstream f("data/test_categories.txt");
            f << "An uong|C\n";
            f << "Di lai|C\n";
            f << "Luong|T\n";
        }
        cm.loadData("data/test_categories.txt");
    }

    void TearDown() override {
        std::remove("data/test_categories.txt");
    }
};

TEST_F(CategoryManagerTest, LoadData_DungSoLuong) {
    EXPECT_EQ(cm.getAll().size(), 3);
}

TEST_F(CategoryManagerTest, LoadData_NoiDungDung) {
    const Category* c = cm.getById(0);
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->getName(), "An uong");
    EXPECT_EQ(c->getType(), 'C');

    const Category* c2 = cm.getById(2);
    ASSERT_NE(c2, nullptr);
    EXPECT_EQ(c2->getName(), "Luong");
    EXPECT_EQ(c2->getType(), 'T');
}

TEST_F(CategoryManagerTest, GetById_NgoaiPhamVi) {
    EXPECT_EQ(cm.getById(-1), nullptr);
    EXPECT_EQ(cm.getById(99), nullptr);
}

TEST_F(CategoryManagerTest, Rename) {
    cm.rename(0, "Thuc pham");
    const Category* c = cm.getById(0);
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->getName(), "Thuc pham");
}

TEST_F(CategoryManagerTest, Rename_IdNgoaiPhamVi) {
    EXPECT_NO_THROW(cm.rename(99, "Test"));
}

TEST_F(CategoryManagerTest, SaveData_VaLoadLai) {
    cm.saveData();
    CategoryManager cm2;
    cm2.loadData("data/test_categories.txt");
    EXPECT_EQ(cm2.getAll().size(), 3);
    EXPECT_EQ(cm2.getById(1)->getName(), "Di lai");
}

TEST_F(CategoryManagerTest, LoadData_BoCauSaiDinhDang) {
    {
        std::ofstream f("data/test_bad_cat.txt");
        f << "HopLe|C\n";
        f << "ThieuType\n";    
    }
    CategoryManager cm2;
    cm2.loadData("data/test_bad_cat.txt");
    EXPECT_EQ(cm2.getAll().size(), 1); 
    std::remove("data/test_bad_cat.txt");
}

TEST_F(CategoryManagerTest, Remove_CoGiaoDich) {
    {
        std::ofstream f("data/test_trans_for_cat.txt");
        f << "2026-06-01|0|100000|C|Test\n";
    }
    TransactionManager tm2;
    tm2.loadData("data/test_trans_for_cat.txt");

    int sizeBefore = cm.getAll().size();
    cm.remove(0, tm2); 
    EXPECT_EQ(cm.getAll().size(), sizeBefore); 
    std::remove("data/test_trans_for_cat.txt");
}
