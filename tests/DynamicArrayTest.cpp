#include <gtest/gtest.h>
#include "../src/DynamicArray.hpp"

// Khởi tạo
TEST(DynamicArrayTest, KhoiTaoMangRong) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.size(), 0);
    EXPECT_TRUE(arr.isEmpty());
}
 
// Add
TEST(DynamicArrayTest, Add_PhanTuDuocLuu) {
    DynamicArray<int> arr;
    arr.add(10);
    arr.add(20);
    arr.add(30);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}
 
TEST(DynamicArrayTest, Add_TuDongMoRong) {
    DynamicArray<int> arr;
    for (int i = 0; i < 1000; ++i) {
        arr.add(i);
    }
    EXPECT_EQ(arr.size(), 1000);
    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(arr[i], i);
    }
}
 
TEST(DynamicArrayTest, Add_KiemTraIsEmpty) {
    DynamicArray<int> arr;
    EXPECT_TRUE(arr.isEmpty());
    arr.add(1);
    EXPECT_FALSE(arr.isEmpty());
}

// Remove_At
TEST(DynamicArrayTest, RemoveAt_ViTriHopLe) {
    DynamicArray<int> arr;
    arr.add(10);
    arr.add(20);
    arr.add(30);
    arr.removeAt(1);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 30);
}
 
TEST(DynamicArrayTest, RemoveAt_PhanTuDauTien) {
    DynamicArray<int> arr;
    arr.add(10);
    arr.add(20);
    arr.removeAt(0);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 20);
}
 
TEST(DynamicArrayTest, RemoveAt_PhanTuCuoi) {
    DynamicArray<int> arr;
    arr.add(10);
    arr.add(20);
    arr.removeAt(1);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 10);
}
 
TEST(DynamicArrayTest, RemoveAt_ViTriNgoaiPhamVi) {
    DynamicArray<int> arr;
    arr.add(10);
    EXPECT_THROW(arr.removeAt(5), std::out_of_range);
    EXPECT_THROW(arr.removeAt(-1), std::out_of_range);
}
 
// operator[]
TEST(DynamicArrayTest, LayPhanTu_ViTriNgoaiPhamVi) {
    DynamicArray<int> arr;
    EXPECT_THROW(arr[0], std::out_of_range);
    arr.add(1);
    EXPECT_THROW(arr[1], std::out_of_range);
    EXPECT_THROW(arr[-1], std::out_of_range);
}
 
TEST(DynamicArrayTest, LayPhanTu_CoTheSuaDoi) {
    DynamicArray<int> arr;
    arr.add(10);
    arr[0] = 99;
    EXPECT_EQ(arr[0], 99);
}
 
// clear
TEST(DynamicArrayTest, Clear_XoaHetPhanTu) {
    DynamicArray<int> arr;
    arr.add(1);
    arr.add(2);
    arr.add(3);
    arr.clear();
    EXPECT_EQ(arr.size(), 0);
    EXPECT_TRUE(arr.isEmpty());
}
 
// Copy Constructor, Operator =
TEST(DynamicArrayTest, CopyConstructor) {
    DynamicArray<int> original;
    original.add(1);
    original.add(2);
 
    DynamicArray<int> copy(original);
    EXPECT_EQ(copy.size(), 2);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);

    original[0] = 99;
    EXPECT_EQ(copy[0], 1);
}
 
TEST(DynamicArrayTest, OperatorAssignment) {
    DynamicArray<int> a;
    a.add(10);
    a.add(20);
 
    DynamicArray<int> b;
    b = a;
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], 10);
 
    a[0] = 99;
    EXPECT_EQ(b[0], 10);
}
 
TEST(DynamicArrayTest, OperatorAssignment_TuGanChinhNo) {
    DynamicArray<int> arr;
    arr.add(42);
    arr = arr;
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 42);
}
 
// Kiểu dữ liệu khác
TEST(DynamicArrayTest, KieuString) {
    DynamicArray<std::string> arr;
    arr.add("hello");
    arr.add("world");
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], "hello");
    EXPECT_EQ(arr[1], "world");
}
