
#include "r_b_tree.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <random>
#include <vector>

//#include <vcruntime.h>

int get_black_h_min(tree_node *center_node) {
  if (center_node == nullptr)
    return 0;
  int left = 0;
  int right = 0;
  if (center_node->left != nullptr) {
    left = get_black_h_min(center_node->left.get());
  }
  if (center_node->right != nullptr) {
    right = get_black_h_min(center_node->right.get());
  }

  return center_node->black + static_cast<int>(fmin(left, right));
}

bool red_under_black(tree_node *center_node) {
  if (center_node == nullptr) {
    return true;
  }
  if (center_node->left == nullptr && center_node->right == nullptr) {
    return true;
  }
  if (center_node->black == 0) {
    if (center_node->left != nullptr && center_node->left->black == 0) {
      std::cout << center_node->value << std::endl;
      return false;
    }
    if (center_node->right != nullptr && center_node->right->black == 0) {
      std::cout << center_node->value << std::endl;
      return false;
    }
  }

  return red_under_black(center_node->left.get()) &&
         red_under_black(center_node->right.get());
}

int get_black_h_max(tree_node *center_node) {
  if (center_node == nullptr)
    return 0;

  int left = 0;
  int right = 0;
  if (center_node->left != nullptr) {
    left = get_black_h_max(center_node->left.get());
  }
  if (center_node->right != nullptr) {
    right = get_black_h_max(center_node->right.get());
  }

  return center_node->black + static_cast<int>(fmax(left, right));
}

void print_tree(tree_node *center_node, int h) {
  if (center_node->left != nullptr) {
    print_tree(center_node->left.get(), h + 1);
  }
  for (int i = 0; i < h; i++)
    std::cout << "     ";
  std::cout << center_node->value << "(" << center_node->black << ")"
            << std::endl;
  if (center_node->right != nullptr) {
    print_tree(center_node->right.get(), h + 1);
  }
}

int main() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);

  auto cout_of_nodes = 30;
  auto cout_of_del = 22;
  int min_value = 10;
  int max_value = 90;
  tree t;
  std::uniform_int_distribution<int> distribution(min_value, max_value);

  std::vector<int> values;
  int next_value = 0;
  int not_correct = true;
  std::cout << "(";
  for (size_t i = 0; i < cout_of_nodes; i++) {
    do {
      next_value = distribution(generator);
      not_correct = false;
      for (size_t j = 0; j < values.size(); j++)
        if (next_value == values[j])
          not_correct = true;

    } while (not_correct);
    values.push_back(next_value);
    t.insert(values[i]);
    std::cout << " " << values[i] << ", ";
  }
  std::cout << ")" << std::endl;
  /*
    std::cout << t.root->value << " black(" << t.root->black << ")" <<
    std::endl; std::cout << t.root->left->value << " black(" <<
    t.root->left->black << ")"
              << std::endl;
    std::cout << t.root->right->value << " black(" << t.root->right->black <<
    ")"
              << std::endl;
    std::cout << t.root->left->left->value << " black("
              << t.root->left->left->black << ")" << std::endl;
  */
  // print_tree(t.root.get(), 0);
  std::cout << "/////" << std::endl;
  if (red_under_black(t.root.get()))
    std::cout << "Yes" << std::endl;
  std::cout << get_black_h_min(t.root.get()) << "  "
            << get_black_h_max(t.root.get()) << std::endl;
  std::cout << "/////////////////////////" << std::endl;

  std::vector<int> deleted_values;
  std::cout << "(";
  for (size_t i = 0; i < cout_of_del; i++) {
    do {
      next_value = distribution(generator);
      not_correct = true;
      for (size_t j = 0; j < values.size(); j++)
        if (next_value == values[j])
          not_correct = false;

      for (size_t j = 0; j < deleted_values.size(); j++)
        if (next_value == deleted_values[j])
          not_correct = true;
    } while (not_correct);

    deleted_values.push_back(next_value);
    t.remove(deleted_values[i]);
    std::cout << " " << deleted_values[i] << ", ";
  }
  std::cout << ")" << std::endl;
  std::cout << get_black_h_min(t.root.get()) << "  "
            << get_black_h_max(t.root.get()) << std::endl;
  if (red_under_black(t.root.get()))
    std::cout << "Yes" << std::endl;

  print_tree(t.root.get(), 0);
  return 0;
}
