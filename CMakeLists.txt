cmake_minimum_required(VERSION 3.22)

project(leaderboards VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23 CACHE STRING "")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/permissive- /W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-c++17-extensions)
endif()


# ------------------------------------------------------------------------------

add_executable(
        main_app
        src/main.cpp)

find_package(libpqxx CONFIG REQUIRED)
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(main_app PRIVATE libpqxx::pqxx)
target_link_libraries(main_app PRIVATE Drogon::Drogon)

# ------------------------------------------------------------------------------

#find_package(GTest CONFIG REQUIRED)
#enable_testing()
#
#add_executable(
#        scenarios_test
#        test/test_scenarios.cpp)
#target_link_libraries(scenarios_test PRIVATE GTest::gtest GTest::gtest_main)
#target_include_directories(scenarios_test PRIVATE src)
#add_test(scenarios_test scenarios_test)
