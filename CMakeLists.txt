cmake_minimum_required(VERSION 3.16)

# Tên dự án
project(PathFinding LANGUAGES CXX)

# Cấu hình tiêu chuẩn C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Đường dẫn tới Qt5 (Cập nhật chính xác đường dẫn của bạn nếu cần)
list(APPEND CMAKE_PREFIX_PATH "/home/dihnhuunam/Qt5.13.2/5.13.2/gcc_64/lib/cmake/Qt5")

# Bật các tính năng tự động của Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Tìm các gói Qt5 cần thiết
find_package(Qt5 COMPONENTS Widgets Quick QuickWidgets Gui Location Positioning REQUIRED)

# Thêm các file nguồn của dự án
add_executable(PathFinding
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Resources.qrc
)

# Liên kết thư viện Qt5 với executable
target_link_libraries(PathFinding PRIVATE
    Qt5::Widgets
    Qt5::Gui
    Qt5::Quick
    Qt5::QuickWidgets
    Qt5::Location
    Qt5::Positioning
)

# Đảm bảo thư mục output luôn sạch
set_target_properties(PathFinding PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
