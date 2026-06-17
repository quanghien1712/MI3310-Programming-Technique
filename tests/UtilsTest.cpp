#include <gtest/gtest.h>
#include "../src/Utils.hpp"

// isNumber 
TEST(UtilsTest, IsNumber_SoDuong_HopLe) {
    EXPECT_TRUE(Utils::isNumber("0"));
    EXPECT_TRUE(Utils::isNumber("123"));
    EXPECT_TRUE(Utils::isNumber("9999999999"));
}

TEST(UtilsTest, IsNumber_SoAm_KhiKhongChoPhep) {
    EXPECT_FALSE(Utils::isNumber("-1"));
    EXPECT_FALSE(Utils::isNumber("-100"));
}

TEST(UtilsTest, IsNumber_SoAm) {
    EXPECT_TRUE(Utils::isNumber("-1",  true));
    EXPECT_TRUE(Utils::isNumber("-100", true));
}

TEST(UtilsTest, IsNumber_ChuoiRong) {
    EXPECT_FALSE(Utils::isNumber(""));
}

TEST(UtilsTest, IsNumber_ChuoiChuCai) {
    EXPECT_FALSE(Utils::isNumber("abc"));
    EXPECT_FALSE(Utils::isNumber("12a3"));
    EXPECT_FALSE(Utils::isNumber("12.5"));
}

TEST(UtilsTest, IsNumber_CoChuoiTrang) {
    EXPECT_TRUE(Utils::isNumber(" 123")); 
    EXPECT_FALSE(Utils::isNumber("   "));
}

// isValidDateDMY
TEST(UtilsTest, IsValidDateDMY_NgayHopLe) {
    EXPECT_TRUE(Utils::isValidDateDMY("01/01/2024"));
    EXPECT_TRUE(Utils::isValidDateDMY("31/12/2023"));
    EXPECT_TRUE(Utils::isValidDateDMY("15/06/2026"));
}

TEST(UtilsTest, IsValidDateDMY_NamNhuan) {
    EXPECT_TRUE(Utils::isValidDateDMY("29/02/2024")); 
    EXPECT_FALSE(Utils::isValidDateDMY("29/02/2023")); 
}

TEST(UtilsTest, IsValidDateDMY_Thang30Ngay) {
    EXPECT_TRUE(Utils::isValidDateDMY("30/04/2024"));
    EXPECT_FALSE(Utils::isValidDateDMY("31/04/2024")); 
}

TEST(UtilsTest, IsValidDateDMY_NgayKhongHopLe) {
    EXPECT_FALSE(Utils::isValidDateDMY("00/01/2024")); 
    EXPECT_FALSE(Utils::isValidDateDMY("32/01/2024")); 
    EXPECT_FALSE(Utils::isValidDateDMY("01/00/2024")); 
    EXPECT_FALSE(Utils::isValidDateDMY("01/13/2024")); 
}

TEST(UtilsTest, IsValidDateDMY_DinhDangSai) {
    EXPECT_FALSE(Utils::isValidDateDMY("1/1/2024"));      
    EXPECT_FALSE(Utils::isValidDateDMY("01-01-2024"));    
    EXPECT_FALSE(Utils::isValidDateDMY("2024/01/01"));    
    EXPECT_FALSE(Utils::isValidDateDMY(""));              
    EXPECT_FALSE(Utils::isValidDateDMY("01/01/24"));      
}

// formatMoney
TEST(UtilsTest, FormatMoney_NhoHon1000) {
    EXPECT_EQ(Utils::formatMoney(0), "0");
    EXPECT_EQ(Utils::formatMoney(999), "999");
}

TEST(UtilsTest, FormatMoney_Hang1000) {
    EXPECT_EQ(Utils::formatMoney(1000), "1,000");
    EXPECT_EQ(Utils::formatMoney(1500), "1,500");
}

TEST(UtilsTest, FormatMoney_Hang1Trieu) {
    EXPECT_EQ(Utils::formatMoney(1000000), "1,000,000");
    EXPECT_EQ(Utils::formatMoney(1500000), "1,500,000");
}

TEST(UtilsTest, FormatMoney_SoAm) {
    EXPECT_EQ(Utils::formatMoney(-1000), "-1,000");
    EXPECT_EQ(Utils::formatMoney(-1000000), "-1,000,000");
}

TEST(UtilsTest, FormatMoney_LonHon1Ty) {
    EXPECT_EQ(Utils::formatMoney(1000000000LL), "1,000,000,000");
}

// containsPipe

TEST(UtilsTest, ContainsPipe_CoChuoiPipe) {
    EXPECT_TRUE(Utils::containsPipe("hello|world"));
    EXPECT_TRUE(Utils::containsPipe("|"));
    EXPECT_TRUE(Utils::containsPipe("a|b|c"));
}

TEST(UtilsTest, ContainsPipe_KhongCoPipe) {
    EXPECT_FALSE(Utils::containsPipe("hello"));
    EXPECT_FALSE(Utils::containsPipe(""));
    EXPECT_FALSE(Utils::containsPipe("abc 123"));
}

// dmyToYmd
TEST(UtilsTest, DmyToYmd) {
    EXPECT_EQ(Utils::dmyToYmd("08/06/2026"), "2026-06-08");
    EXPECT_EQ(Utils::dmyToYmd("01/01/2000"), "2000-01-01");
    EXPECT_EQ(Utils::dmyToYmd("31/12/1999"), "1999-12-31");
}

// ymdToDmy
TEST(UtilsTest, YmdToDmy) {
    EXPECT_EQ(Utils::ymdToDmy("2026-06-08"), "08/06/2026");
    EXPECT_EQ(Utils::ymdToDmy("2000-01-01"), "01/01/2000");
    EXPECT_EQ(Utils::ymdToDmy("1999-12-31"), "31/12/1999");
}

TEST(UtilsTest, YmdToDmy_ChuoiNganHon10KyTu) {
    EXPECT_EQ(Utils::ymdToDmy("2026-06"), "2026-06");
}

TEST(UtilsTest, ChuyenDoiNguocChieu_DmyYmdDmy) {
    std::string original = "15/06/2026";
    EXPECT_EQ(Utils::ymdToDmy(Utils::dmyToYmd(original)), original);
}

TEST(UtilsTest, ChuyenDoiNguocChieu_YmdDmyYmd) {
    std::string original = "2026-06-15";
    EXPECT_EQ(Utils::dmyToYmd(Utils::ymdToDmy(original)), original);
}

// toTwoDigits
TEST(UtilsTest, ToTwoDigits_SoNhoHon10) {
    EXPECT_EQ(Utils::toTwoDigits(1), "01");
    EXPECT_EQ(Utils::toTwoDigits(9), "09");
}

TEST(UtilsTest, ToTwoDigits_SoLonHonHoacBang10) {
    EXPECT_EQ(Utils::toTwoDigits(10), "10");
    EXPECT_EQ(Utils::toTwoDigits(12), "12");
}
