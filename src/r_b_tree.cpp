#include "r_b_tree.hpp"
#include <cstddef>

auto tree::what_child_is_it(tree_node *center_node) -> int {

  if (center_node == root.get())
    return 0;

  tree_node *father = center_node->up;
  if (father->left.get() == center_node)
    return 1;
  return 2;
}

auto tree::get_color(tree_node *center_node) -> int {
  if (center_node == nullptr) {
    return 1;
  }
  return center_node->black;
}

auto tree::left_rotation(tree_node *center_node) -> tree_node * {

  auto center_is_such_child = what_child_is_it(center_node);
  auto prev_right = std::move(center_node->right);
  tree_node *father = center_node->up;

  if (prev_right->left != nullptr) {
    center_node->right = std::move(prev_right->left);
    center_node->right->up = center_node;
  }

  auto new_left = std::make_unique<tree_node>();
  new_left->value = center_node->value;
  new_left->black = center_node->black;
  if (center_node->left != nullptr) {
    new_left->left = std::move(center_node->left);
    new_left->left->up = new_left.get();
  }

  if (center_node->right != nullptr) {
    new_left->right = std::move(center_node->right);
    new_left->right->up = new_left.get();
  }

  new_left->up = prev_right.get();
  prev_right->left = std::move(new_left);
  if (center_is_such_child == 0) {
    root = std::move(prev_right);

    return root->left.get();
  }

  prev_right->up = father;
  if (center_is_such_child == 1) {
    father->left = std::move(prev_right);
    return father->left->left.get();
  }

  father->right = std::move(prev_right);
  return father->right->left.get();
}

auto tree::right_rotation(tree_node *center_node) -> tree_node * {

  auto center_is_such_child = what_child_is_it(center_node);
  auto prev_left = std::move(center_node->left);
  tree_node *father = center_node->up;

  if (prev_left->right != nullptr) {
    center_node->left = std::move(prev_left->right);
    center_node->left->up = center_node;
  }

  auto new_right = std::make_unique<tree_node>();
  new_right->value = center_node->value;
  new_right->black = center_node->black;
  if (center_node->left != nullptr) {
    new_right->left = std::move(center_node->left);
    new_right->left->up = new_right.get();
  }

  if (center_node->right != nullptr) {
    new_right->right = std::move(center_node->right);
    new_right->right->up = new_right.get();
  }

  new_right->up = prev_left.get();
  prev_left->right = std::move(new_right);

  if (center_is_such_child == 0) {
    root = std::move(prev_left);

    return root->right.get();
  }

  prev_left->up = father;
  if (center_is_such_child == 1) {
    father->left = std::move(prev_left);
    return father->left->right.get();
  }

  father->right = std::move(prev_left);
  return father->right->right.get();
}

auto tree::insert(int val) -> tree_node * {
  if (root == nullptr) {
    auto new_node = std::make_unique<tree_node>();
    new_node->value = val;
    new_node->black = 1;
    root = std::move(new_node);
    return root.get();
  }

  tree_node *prev = root.get();
  tree_node *temp = root.get();
  while (temp != nullptr && temp->value != val) {
    prev = temp;
    if (temp->value > val) {
      temp = temp->left.get();
    } else if (temp->value < val) {
      temp = temp->right.get();
    }
  }

  if (temp != nullptr) {
    return temp;
  }

  auto new_item = std::make_unique<tree_node>();
  new_item->value = val;
  if (prev->value > val) {
    prev->left = std::move(new_item);
    prev->left->up = prev;
    prev = prev->left.get();
  }
  if (prev->value < val) {
    prev->right = std::move(new_item);
    prev->right->up = prev;
    prev = prev->right.get();
  }
  prev->black = 0;

  balance_insert(prev);
  return prev;
}

auto tree::balance_insert(tree_node *center_node) -> tree_node * {

  if (center_node == root.get()) {
    center_node->black = 1;
    return center_node;
  }

  if (center_node->up->black == 1) {
    return center_node;
  }
  auto father_is_such_child = what_child_is_it(center_node->up);
  auto grandpa = center_node->up->up;
  auto uncle_black = 0;

  if (father_is_such_child == 1) {
    uncle_black = get_color(grandpa->right.get());
  } else {
    uncle_black = get_color(grandpa->left.get());
  }

  if (uncle_black == 0) {
    grandpa->black = 0;
    grandpa->left->black = 1;
    grandpa->right->black = 1;
    balance_insert(grandpa);
    return center_node;
  }

  if (what_child_is_it(center_node) == 2 && father_is_such_child == 1)
    left_rotation(center_node->up);

  if (what_child_is_it(center_node) == 1 && father_is_such_child == 2)
    right_rotation(center_node->up);

  if (father_is_such_child == 1)
    grandpa = right_rotation(grandpa)->up;

  if (father_is_such_child == 2)
    grandpa = left_rotation(grandpa)->up;

  grandpa->black = 1;
  grandpa->left->black = 0;
  grandpa->right->black = 0;

  return center_node;
}

auto tree::remove(int val) -> bool {

  if (this->root == nullptr) {
    return false;
  }
  tree_node *temp = this->root.get();
  tree_node *prev = this->root.get();

  while (temp != nullptr && temp->value != val) {
    prev = temp;
    if (temp->value > val) {
      temp = temp->left.get();
    } else if (temp->value < val) {
      temp = temp->right.get();
    }
  }

  if (temp == nullptr) {
    return false;
  }
  tree_node *mark_del = temp;

  auto child_count = 0;
  if (mark_del->left) {
    child_count++;
  }
  if (mark_del->right) {
    child_count++;
  }

  if (child_count == 2) {
    mark_del = temp->right.get();
    while (mark_del->left != nullptr)
      mark_del = mark_del->left.get();
    temp->value = mark_del->value;
  }

  mark_del = balance_remove(mark_del);
  prev = mark_del->up;
  auto deleted_is_such_child = what_child_is_it(mark_del);

  if (mark_del->left == nullptr && mark_del->right == nullptr) {
    if (mark_del == root.get()) {
      root = nullptr;
      return true;
    }
    if (deleted_is_such_child == 1)
      prev->left = nullptr;
    if (deleted_is_such_child == 2)
      prev->right = nullptr;

    return true;
  }

  if (mark_del == root.get() && mark_del->left != nullptr) {
    root = std::move(mark_del->left);
    root->black = 1;
    root->up = nullptr;
    return true;
  }
  if (mark_del == root.get() && mark_del->right != nullptr) {
    root = std::move(mark_del->right);
    root->black = 1;
    root->up = nullptr;
    return true;
  }

  if (mark_del->left != nullptr && deleted_is_such_child == 1) {
    prev->left = std::move(mark_del->left);
    prev->left->up = prev;
    return true;
  }
  if (mark_del->left != nullptr && deleted_is_such_child == 2) {
    prev->right = std::move(mark_del->left);
    prev->right->up = prev;
    return true;
  }

  if (mark_del->right != nullptr && deleted_is_such_child == 1) {
    prev->left = std::move(mark_del->right);
    prev->left->up = prev;
    return true;
  }
  if (mark_del->right != nullptr && deleted_is_such_child == 2) {
    prev->right = std::move(mark_del->right);
    prev->right->up = prev;
  }

  return true;
}

auto tree::balance_remove(tree_node *center_node) -> tree_node * {

  if (center_node == root.get())
    return center_node;

  if (center_node->black == 0)
    return center_node;
  auto center_is_such_child = what_child_is_it(center_node);
  auto brothers_color = 0;

  if (center_is_such_child == 1)
    brothers_color = get_color(center_node->up->right.get());
  if (center_is_such_child == 2)
    brothers_color = get_color(center_node->up->left.get());

  if (brothers_color == 0 && center_is_such_child == 1) {
    left_rotation(center_node->up);
  }
  if (brothers_color == 0 && center_is_such_child == 2) {
    right_rotation(center_node->up);
  }

  if (brothers_color == 0) {
    center_node->up->black = 0;
    center_node->up->up->black = 1;
  }

  auto left_nephew = 0;
  auto right_nephew = 0;

  if (center_is_such_child == 1) {
    left_nephew = get_color(center_node->up->right->left.get());
    right_nephew = get_color(center_node->up->right->right.get());
  }

  if (center_is_such_child == 2) {
    left_nephew = get_color(center_node->up->left->left.get());
    right_nephew = get_color(center_node->up->left->right.get());
  }

  if (left_nephew == 1 && right_nephew == 1) {
    auto father_color = center_node->up->black;
    center_node->up->black = 1;

    if (center_is_such_child == 1)
      center_node->up->right->black = 0;

    if (center_is_such_child == 2)
      center_node->up->left->black = 0;

    if (father_color == 1)
      balance_remove(center_node->up);
    return center_node;
  }

  if (center_is_such_child == 1 && right_nephew == 1) {
    right_rotation(center_node->up->right.get());
    center_node->up->right->black = 1;
    center_node->up->right->right->black = 0;
  }

  if (center_is_such_child == 2 && left_nephew == 1) {
    left_rotation(center_node->up->left.get());
    center_node->up->left->black = 1;
    center_node->up->left->left->black = 0;
  }

  if (center_is_such_child == 1)
    center_node = left_rotation(center_node->up)->left.get();

  if (center_is_such_child == 2)
    center_node = right_rotation(center_node->up)->right.get();

  auto father_color = center_node->up->black;
  center_node->up->up->left->black = 1;
  center_node->up->up->right->black = 1;
  center_node->up->up->black = father_color;

  return center_node;
}
