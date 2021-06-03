# CMakeList.txt: проект CMake для RedBlackTree; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("RedBlackTree")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (RedBlackTree "RedBlackTree.cpp" "RedBlackTree.h" "r_b_tree.hpp" "r_b_tree.cpp")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
