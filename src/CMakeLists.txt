cmake_minimum_required(VERSION 3.10.0)

#add_executable(proj_test test.cpp lib.cpp)
add_executable (RedBlackTree_Test "test.cpp" "r_b_tree.hpp" "r_b_tree.cpp")
# ^ Взять test.cpp и lib.cpp, собрать вместе, назвать proj_test (proj_test.exe)
target_link_libraries(RedBlackTree_Test PUBLIC doctest::doctest default CONAN_PKG::eigen)
# ^ Использовать в proj_test библиотеки doctest::doctest и default
doctest_discover_tests(RedBlackTree_Test)
# ^ Прочитать из proj_test список тестов

add_executable(RedBlackTree "main.cpp" "r_b_tree.hpp" "r_b_tree.cpp")
target_link_libraries(RedBlackTree PUBLIC default CONAN_PKG::fmt CONAN_PKG::eigen)

add_executable(RedBlackTree_Bench "bench.cpp" "r_b_tree.hpp" "r_b_tree.cpp")
target_link_libraries(RedBlackTree_Bench PUBLIC default CONAN_PKG::fmt CONAN_PKG::benchmark CONAN_PKG::eigen)