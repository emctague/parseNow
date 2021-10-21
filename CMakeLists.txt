cmake_minimum_required(VERSION 3.20)
project(QueML)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(parseNow app.cpp lexer.cpp lexer.h spec.cpp spec.h parser.cpp parser.h)
target_link_libraries(parseNow nlohmann_json nlohmann_json::nlohmann_json)
install(TARGETS parseNow)