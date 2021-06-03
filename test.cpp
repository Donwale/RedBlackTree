// RedBlackTree.cpp: определяет точку входа для приложения.
//

#include "RedBlackTree.h"

int main() {

  tree t;
  t.insert(7);
  t.insert(5);
  t.insert(3);
  t.insert(6);
  t.insert(4);
  t.left_rotation(t.root->left.get());

  std::cout << std::endl;
  std::cout << t.root->left->left->left->right->value << std::endl;
  std::cout << t.root->left->left->left->value << std::endl;
  std::cout << t.root->left->left->value << std::endl;
  std::cout << t.root->left->value << std::endl;
  std::cout << t.root->value << std::endl;

  return 0;
}
