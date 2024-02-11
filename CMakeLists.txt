cmake_minimum_required(VERSION 3.10)

project(chat_server VERSION 1.0.0)

# Add your source files and include directories
file(GLOB SOURCES "src/*.cpp")
include_directories(inc)

# Find the SQLite3 package
find_package(SQLite3 REQUIRED)

# Create the executable
add_executable(chat_server.exe ${SOURCES})

# Link against SQLite3
target_link_libraries(chat_server.exe SQLite::SQLite3)

# Set C++ standard and include pthread, sqlite3, gtest, etc.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lsqlite3 -lgtest")

# Install the executable
install(TARGETS chat_server.exe DESTINATION bin)

# CPack configuration
set(CPACK_PACKAGE_NAME "chat_server")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "ZIP")  # Change this according to your preference

include(CPack)
