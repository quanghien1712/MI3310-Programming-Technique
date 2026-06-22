# Ứng Dụng Quản Lý Chi Tiêu Cá Nhân (Personal Expense Manager)
Bài tập nhóm môn Kỹ Thuật Lập Trình - MI3310: Thiết kế Ứng dụng Quản lý chi tiêu cá nhân bằng C++ 

## Các tính năng

- **Quản lý ví/tài khoản:** Theo dõi số dư tài chính cá nhân một cách chính xác dựa trên lịch sử giao dịch.
- **Ghi chép giao dịch:** 
  - Thêm, Sửa, Xóa các khoản THU và CHI.
  - Phân loại giao dịch theo các danh mục được định nghĩa sẵn hoặc do người dùng tạo.
- **Quản lý ngân sách:** Thiết lập hạn mức chi tiêu cho từng tháng. Cảnh báo/hiển thị trạng thái ngân sách và phần trăm đã chi tiêu.
- **Thống kê & Báo cáo:** 
  - Tổng hợp thu chi theo tháng và theo năm.
  - Hiển thị tỷ lệ chi tiêu trực quan qua báo cáo biểu đồ text.
- **Quản lý danh mục:** Thêm, sửa tên, xóa các danh mục thu/chi để phù hợp với nhu cầu cá nhân.
- **Lưu trữ dữ liệu:** Đọc và ghi dữ liệu tự động định kỳ vào file text.

---

## Thiết kế / Kiến trúc

Chương trình được thiết kế theo hướng module hóa và lập trình hướng đối tượng (OOP) cơ bản trong C++, với việc phân tách trách nhiệm rõ ràng:

- **Lớp thực thể (Entity Layer):** Các class đại diện cho các đối tượng trong thực tế như `Transaction` (Giao dịch), `Category` (Danh mục), `Budget` (Ngân sách), `Report` (Báo cáo).
- **Lớp quản lý (Manager Layer):** Các manager class như `TransactionManager`, `CategoryManager`, `BudgetManager` đóng vai trò quản lý danh sách các thực thể, xử lý logic (thêm, sửa, xóa, tìm kiếm) và đảm nhiệm việc Load/Save dữ liệu vào File hệ thống.
- **Lớp tiện ích (Utility/Helper Layer):** Lớp `Utils` chứa các hàm tiện ích dùng chung (format tiền tệ, xử lý thời gian, đọc input an toàn). File `DynamicArray.hpp` cung cấp cấu trúc dữ liệu mảng động tự cài đặt bằng template để thay thế cho `std::vector`, đáp ứng yêu cầu kỹ thuật của môn học.
- **Giao diện dòng lệnh (CLI):** Hàm `main()` và các hàm menu đóng vai trò như router, điều hướng luồng chương trình và tương tác với người dùng qua console.

---

## Cấu trúc thư mục

```text
KTLT/
├── data/                    # Thư mục chứa dữ liệu lưu trữ cố định
│   ├── budgets.txt          # Lưu thông tin ngân sách hàng tháng
│   ├── categories.txt       # Lưu danh sách các danh mục thu/chi
│   └── transactions.txt     # Lưu chi tiết lịch sử giao dịch
│
├── src/                     # Thư mục chứa toàn bộ mã nguồn
│   ├── Budget.hpp / .cpp    # Xử lý logic và thực thể Ngân sách
│   ├── Category.hpp / .cpp  # Xử lý logic và thực thể Danh mục
│   ├── DynamicArray.hpp     # Template mảng động tự cài đặt (Generic Array)
│   ├── Report.hpp / .cpp    # Xử lý logic tạo báo cáo và thống kê
│   ├── Transaction.hpp / .cpp # Xử lý logic và thực thể Giao dịch
│   ├── Utils.hpp / .cpp     # Cung cấp các hàm tiện ích dùng chung
│   └── main.cpp             # Điểm đầu vào (Entry point), chứa UI Menu
│
│── libs/ # Chứa framework GGTest để unit test
│
│── tests/ # Chứa các mã nguồn unit test
│   ├── BudgetTest.cpp        # Test Budget, BudgetManager
│   ├── CategoryTest.cpp      # Test Category, CategoryManager
│   ├── DynamicArrayTest.cpp  # Test DynamicArray    
│   ├── ReportTest.cpp        # Test Report, ReportManager
│   └── UtilsTest.cpp         # Test Utils
│
│── CMakeLists.txt           # File cấu hình biên dịch hệ thống
│
└── README.md                # Tài liệu mô tả dự án`
```
---

## Yêu cầu tiền quyết (Prerequisites)

Dự án sử dụng **CMake** làm hệ thống quản lý biên dịch tự động. Để chạy dự án, máy tính của bạn cần có:

1. **Trình biên dịch C++:** Hỗ trợ tiêu chuẩn C++14 trở lên (GCC, Clang, hoặc MSVC).
2. **CMake (>= 3.10):** Công cụ sinh mã tự động.

## Hướng dẫn Biên dịch và Chạy (Sử dụng CMake)

### Bước 1: Sao chép dự án (Clone Repository)
Mở Terminal / Command Prompt và chạy lệnh:
```bash
git clone https://github.com/quanghien1712/MI3310-Programming-Technique.git
cd MI3310-Programming-Technique
```

### Bước 2: Khởi tạo cấu hình CMake
```bash
mkdir build
cd build

# 1. Dành cho macOS, Linux, hoặc Windows sử dụng Visual Studio:
cmake ..

# 2. Dành riêng cho Windows sử dụng bộ công cụ MSYS2/MinGW:
cmake .. -G "Unix Makefiles"
```
### Bước 3: Biên dịch mã nguồn (Build)
```bash
# Biên dịch ứng dụng chính
cmake --build .

# (Tùy chọn) Biên dịch gộp toàn bộ Unit Test
cmake --build . --target build_test
```


---

### Bước 4: Chạy chương trình (Run)

LƯU Ý: Ứng dụng cần truy cập vào thư mục data/. Do đó, sau khi biên dịch xong, bạn bắt buộc phải lùi ra thư mục gốc trước khi chạy lệnh.

```bash
# Lùi ra ngoài thư mục gốc:
cd ..
```
* **Chạy ứng dụng chính:**
```bash
# Trên Linux/macOS
./expense_manager

# Trên Windows: 
.\expense_manager.exe
```
* **Chạy Unit Test**
```bash
# Trên Linux/macOS
./run_tests

# Trên Windows: 
.\run_tests.exe
```
