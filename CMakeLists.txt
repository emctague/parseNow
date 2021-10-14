cmake_minimum_required(VERSION 3.20)
project(QueML)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(app app.cpp lexer.cpp lexer.h spec.cpp spec.h parser.cpp parser.h)
target_link_libraries(app nlohmann_json nlohmann_json::nlohmann_json)