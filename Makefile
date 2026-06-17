CXX = g++
CXXFLAGS = -std=c++14 -pthread

# GGTest
GTEST_DIR = libs/googletest
GTEST_INC = -isystem $(GTEST_DIR)/include -I$(GTEST_DIR)
GTEST_SRC = $(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc


ALL_SRC = $(wildcard src/*.cpp)
SRC_FILES = $(filter-out src/main.cpp, $(ALL_SRC))
APP_MAIN = src/main.cpp

# Thiết lập file test
ifdef TEST_FILE
	TEST_FILES = $(TEST_FILE)
else
	TEST_FILES = $(wildcard tests/*.cpp)
endif

# Tên các file chạy đầu ra
TEST_TARGET = run_tests
APP_TARGET = expense_manager 

all: app

# Lệnh chỉ build chương trình
app: $(SRC_FILES) $(APP_MAIN)
	$(CXX) $(CXXFLAGS) $(SRC_FILES) $(APP_MAIN) -o $(APP_TARGET)

# Lệnh chỉ build file test 
test: $(SRC_FILES) $(TEST_FILES)
	$(CXX) $(CXXFLAGS) $(GTEST_INC) $(GTEST_SRC) $(SRC_FILES) $(TEST_FILES) -o $(TEST_TARGET)

# Lệnh dọn dẹp
clean:
	rm -f $(TEST_TARGET) $(APP_TARGET)