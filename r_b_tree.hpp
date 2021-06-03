#include <memory>

struct tree_node {
  int value;
  int black;
  tree_node *up;
  std::unique_ptr<tree_node> left, right;

  
};

struct tree {
  std::unique_ptr<tree_node> root;
  

  auto insert(int val) -> tree_node *;
  auto remove(int val) -> bool;
  auto left_rotation(tree_node* center_node) ->tree_node*;//
  auto right_rotation(tree_node *) -> void;//
  auto get_color(tree_node *)->int;//
  auto get_left_uncle(tree_node *)->tree_node *;//
  auto what_child_is_it(tree_node* center_node)->int;

  
};