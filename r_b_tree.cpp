#include "r_b_tree.hpp"



auto tree::what_child_is_it(tree_node *center_node) ->int {

    if (center_node == root.get())
        return 0;

    tree_node* father = center_node->up;
    if (father->left.get() == center_node)
        return 1;
    return 2;
}


auto tree::left_rotation(tree_node* center_node) -> tree_node* {

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
    if(center_node->left != nullptr)
        new_left->left = std::move(center_node->left);
    if (center_node->right != nullptr)
        new_left->right = std::move(center_node->right);
    new_left->up = prev_right.get();
    prev_right->left = std::move(new_left);

    if (center_is_such_child == 0) {
        root= std::move(prev_right);

        return root->left.get();
    }

    prev_right->up = father;
    if (center_is_such_child == 1) {
        father->left = std::move(prev_right);
    }    
    else {
        father->right = std::move(prev_right);
    }
        

    return father->left->left.get();
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
    
    return prev;
}

auto tree::remove(int val) -> bool {
    // Код для удаления элемента из дерева

    return true;
}
