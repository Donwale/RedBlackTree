/*
#include "catch.hpp"

TEST_CASE("Something works") { REQUIRE(3 == 3); }

*/
#include "r_b_tree.hpp"
#include <chrono>
#include <doctest.h>
#include <numeric>
#include <random>
#include <vector>

/**
  Возвращает минимальную черную высоту данной вершины(эта высота должна быть
  равной максимальной)

  \param center_node -- указатель на данную вершину
  */
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
/**
  Возвращает максимальную черную высоту данной вершины(эта высота должна быть
  равной минимальной)

  \param center_node -- указатель на данную вершину
  */
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

/**
  Возвращает false, если данная вершина красная и она имеет красных потомков

  \param center_node -- указатель на данную вершину
  */
bool red_under_black(tree_node *center_node) {

  if (center_node == nullptr) {
    return true;
  }
  if (center_node->left == nullptr && center_node->right == nullptr) {
    return true;
  }
  if (center_node->black == 0) {
    if (center_node->left != nullptr && center_node->left->black == 0) {
      return false;
    }
    if (center_node->right != nullptr && center_node->right->black == 0) {
      return false;
    }
  }

  return red_under_black(center_node->left.get()) &&
         red_under_black(center_node->right.get());
}

///////////////////Заимствовано(начало)
void check_tree(tree_node const &t) {
  if (t.left) {
    CHECK(t.left->value <= t.value);
    check_tree(*t.left);
  }
  if (t.right) {
    CHECK(t.right->value >= t.value);
    check_tree(*t.right);
  }
}

void tree_to_vector(tree_node const &t, std::vector<int> &res) {
  if (t.left)
    tree_to_vector(*t.left, res);
  res.push_back(t.value);
  if (t.right)
    tree_to_vector(*t.right, res);
}

auto tree_to_vector(tree const &t) {
  std::vector<int> result;
  if (t.root)
    tree_to_vector(*t.root, result);
  return result;
}

TEST_CASE("Can insert many values") {
  tree t;
  for (int i = 0; i < 17; ++i)
    t.insert(i * 5 % 17);
  REQUIRE(t.root);
  check_tree(*t.root);
  std::vector<int> correct(17);
  std::iota(correct.begin(), correct.end(), 0);
  REQUIRE(tree_to_vector(t) == correct);
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
}

TEST_CASE("Can remove many values") {
  tree t;
  t.insert(3);
  t.insert(1);
  t.insert(2);
  t.insert(4);
  t.insert(5);
  t.remove(3);
  t.remove(5);
  t.insert(10);
  REQUIRE(t.root);
  check_tree(*t.root);
  REQUIRE(tree_to_vector(t) == std::vector{1, 2, 4, 10});
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
}

TEST_CASE("correct return values") {
  tree t;
  auto r = t.insert(3);
  REQUIRE(r);
  REQUIRE(r->value == 3);
  r = t.insert(3);
  REQUIRE(r);
  REQUIRE(r->value == 3);
  check_tree(*t.root);
  REQUIRE(t.remove(3));
  REQUIRE(!t.remove(3));
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
}

constexpr auto TEST_RANGE_MAX = 10;
TEST_CASE("insert many vals and remove elements with only right subtree "
          "(custom range)") {
  tree t;
  std::vector<int> correct(TEST_RANGE_MAX);
  std::iota(correct.begin(), correct.end(), 0);
  for (int i = 0; i < correct.size(); ++i) {
    auto val = static_cast<int>(correct.size()) / 2 +
               ((i % 2) * 2 - 1) * ((i + 1) / 2) - 1;
    auto r = t.insert(val);
    REQUIRE(r);
    REQUIRE(r->value == val);
  }
  REQUIRE(tree_to_vector(t) == correct);
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
  for (int i = 0; i < TEST_RANGE_MAX; ++i) {
    REQUIRE(t.root);
    check_tree(*t.root);
    // CHECK(
    //    t.remove(i)); // throws exception read access violation on bad
    //    solution.
    // dont know how to catch with REQUIRE statement
    bool check;
    CHECK_NOTHROW(([&] { check = t.remove(i); })());
    CHECK(check);
  }
  CHECK(!t.root);
}

TEST_CASE("insert many same values") {
  tree t;
  t.insert(2);
  t.insert(3);
  t.insert(2);
  t.insert(3);
  t.insert(2);
  check_tree(*t.root);
  /* REQUIRE(tree_to_vector(t) == std::vector{2, 2, 2, 3, 3}); */
  REQUIRE(t.remove(2));
  REQUIRE(!t.remove(2));
  REQUIRE(t.root);
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
  /* CHECK(tree_to_vector(t) == std::vector{3, 3}); */
  CHECK(t.remove(3));
  CHECK(!t.root);
}

TEST_CASE("insert, remove then insert again "
          "(custom range)") {
  tree t;
  std::vector<int> correct(TEST_RANGE_MAX);
  std::iota(correct.begin(), correct.end(), 0);
  for (int i = 0; i < correct.size(); ++i) {
    auto val = static_cast<int>(correct.size()) / 2 +
               ((i % 2) * 2 - 1) * ((i + 1) / 2) - 1;
    auto r = t.insert(val);
    REQUIRE(r);
    REQUIRE(r->value == val);
  }
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
  REQUIRE(tree_to_vector(t) == correct);
  for (int i = 0; i < TEST_RANGE_MAX; ++i) {
    REQUIRE(t.root);
    check_tree(*t.root);
    CHECK(
        t.remove(i)); // throws exception read access violation on bad solution.
                      // dont know how to catch with REQUIRE statement
    // bool check;
    // REQUIRE_NOTHROW(([&]{
    //  check = t.remove(i);})());
    // CHECK(check);
  }
  CHECK(!t.root);
  for (int i = 0; i < correct.size(); ++i) {
    auto val = static_cast<int>(correct.size()) / 2 +
               ((i % 2) * 2 - 1) * ((i + 1) / 2) - 1;
    auto r = t.insert(val);
    REQUIRE(r);
    REQUIRE(r->value == val);
  }
}

TEST_CASE("insert and remove magic values") {
  tree t;
  t.insert(5);
  t.insert(3);
  t.insert(7);
  t.insert(1);
  t.insert(4);
  t.insert(2);
  check_tree(*t.root);
  /* CHECK(tree_to_vector(t) == std::vector{1, 2, 3, 4, 5, 7}); */
  REQUIRE(t.remove(3));
  /* CHECK(tree_to_vector(t) == std::vector{1, 2, 4, 5, 7}); */
  CHECK(t.remove(5));
  /* CHECK(tree_to_vector(t) == std::vector{1, 2, 4, 7}); */
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
  CHECK(t.root);
}
///////////////////Заимствовано(конец)

int max_insert_value = 100000;
int max_delete_value = 90000;
TEST_CASE("insert and remove a lot of values in ascending order") {
  tree t;
  for (int i = 0; i < max_insert_value; i++) {
    t.insert(i);
  }
  check_tree(*t.root);
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
  for (int i = 0; i < max_delete_value; i++) {
    t.remove(i);
  }
  check_tree(*t.root);
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
}

int cout_of_random = 1000;
TEST_CASE("insert and remove a lot of random values") {
  tree t;
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(0, cout_of_random + 10);

  int deleted_count = static_cast<int>(cout_of_random * 0.9);

  std::vector<int> values;
  int next_value = 0;
  int not_correct = true;
  for (size_t i = 0; i < cout_of_random; i++) {
    do {
      next_value = distribution(generator);
      not_correct = false;
      for (size_t j = 0; j < values.size(); j++)
        if (next_value == values[j])
          not_correct = true;

    } while (not_correct);
    values.push_back(next_value);
  }

  std::vector<int> deleted_values;

  for (size_t i = 0; i < deleted_count; i++) {
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
  }

  for (int i = 0; i < values.size(); i++) {
    t.insert(values[i]);
  }
  check_tree(*t.root);
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
  for (int i = 0; i < deleted_values.size(); i++) {
    t.remove(deleted_values[i]);
  }
  check_tree(*t.root);
  REQUIRE(get_black_h_max(t.root.get()) == get_black_h_min(t.root.get()));
  REQUIRE(red_under_black(t.root.get()));
}