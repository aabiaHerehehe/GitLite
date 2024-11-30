#pragma once
#include "GlobalFunctions.h"
using namespace std;



// Node structure for the tree
struct Node {
    int key;
    char* filepath;
    char* leftchildpath;
    char* rightchildpath;
    int height;
    Node* left;
    Node* right;

    Node(int k, const char* fp) {
        key = k;
        filepath = new char[stringLength(fp) + 1];
        copyIn(filepath, fp);
        //   lineNumber = ln;
        left = right = nullptr;
    }
    Node(int k) {
        this->key = k;
    }

    ~Node() {
        delete[] filepath;
    }
};
class AVLTree {
public:
    Node* root;
    Node* getRoot() { return root; }
    AVLTree() : root(nullptr) {}

    void insert(int data) {
        root = insertion(root, data);
    }

    Node* insertion(Node* node, int key) {
        if (node == nullptr) {
            cout << "Inserting " << " : ";
            cout << key << endl;
            return new Node(key);

        }

        if (key < node->key) {
            cout << "Inserting " << key << " to left of " << node->key << endl;
            node->left = insertion(node->left, key);
        }
        else if (key > node->key) {
            cout << "Inserting " << key << " to right of " << node->key << endl;
            node->right = insertion(node->right, key);
        }
        else {
            // Duplicate data not allowed
            return node;
        }

        // Update the height of the current node
        int maxHeight;
        if (height(node->left) > height(node->right))
            maxHeight = height(node->left);
        else
            maxHeight = height(node->right);
        node->height = 1 + maxHeight;

        // Check balance factor 
        int balance = BF(node);

        // Right-Skewed Case
        if (balance > 1) {
            if (BF(node->right) >= 0) {
                // Rightskew-Rightskew case
                return leftRotation(node);
            }
            else {
                // Rightrot-Leftrot case
                node->right = rightRotation(node->right);
                return leftRotation(node);
            }
        }

        // Left-Skewed Case
        if (balance < -1) {
            if (BF(node->left) <= 0) {
                // Leftskew-Leftskew case
                return rightRotation(node);
            }
            else {
                // Leftrot-Rightrot case
                node->left = leftRotation(node->left);
                return rightRotation(node);
            }
        }


        return node;
    }


    void Delete(int data) {
        root = deletion(root, data);
    }

    Node* deletion(Node*& node, int data) {
        // BST deletion
        if (node == nullptr) {
            return node;
        }

        // Recurse down the tree to find the node to delete
        if (data < node->key) {
            node->left = deletion(node->left, data);
        }
        else if (data > node->key) {
            node->right = deletion(node->right, data);
        }
        else {
            // Node with only one child or no child
            if (node->left == nullptr && node->right == nullptr) {  // No children
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr) {  // Only right child
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {  // Only left child
                Node* temp = node->left;
                delete node;
                return temp;
            }
            else {
                // Node with two children: get the inorder successor (smallest in the right subtree)
                Node* temp = node->right;
                while (temp->left != nullptr) {
                    temp = temp->left;
                }
                // Copy the inorder successor's data to this node and delete the successor
                node->key = temp->key;
                node->right = deletion(node->right, temp->key);
            }
        }


        // Update the height of the current node
        int maxHeight;
        if (height(node->left) > height(node->right))
            maxHeight = height(node->left);
        else
            maxHeight = height(node->right);
        node->height = 1 + maxHeight;

        // Check balance factor 
        int balance = BF(node);

        // Right-skewed cases
        if (balance > 1) {
            if (BF(node->right) >= 0) {
                // Right-Right case
                node = leftRotation(node);
            }
            else {
                // Right-Left case
                node->right = rightRotation(node->right);
                node = leftRotation(node);
            }
        }
        // Left-skewed cases
        else if (balance < -1) {
            if (BF(node->left) <= 0) {
                // Left-Left case
                node = rightRotation(node);
            }
            else {
                // Left-Right case
                node->left = leftRotation(node->left);
                node = rightRotation(node);
            }
        }

        return node;
    }

    int BF(Node* node) {
        if (node == nullptr)
            return 0;
        return height(node->right) - height(node->left);
    }

    int height(Node* node) {
        if (node == nullptr) {
            return -1;
        }
        else return node->height;
    }
    int max(int a, int b) {
        if (a > b)
            return a;
        else
            return b;
    }

    Node* leftRotation(Node* node) {
        Node* newRoot = node->right;
        Node* temp = newRoot->left;

        newRoot->left = node;
        node->right = temp;

        node->height = 1 + max(height(node->left), height(node->right));
        newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));
        cout << "rotated left" << endl;
        return newRoot;
    }

    Node* rightRotation(Node* node) {
        Node* newRoot = node->left;
        Node* temp = newRoot->right;

        newRoot->right = node;
        node->left = temp;

        node->height = 1 + max(height(node->left), height(node->right));
        newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));
        cout << "rotated right" << endl;
        return newRoot;
    }

    void inOrder() {
        cout << "IN : ";
        inOrderTrav(root);
        cout << endl;
    }

    void inOrderTrav(Node* node) {
        if (node == nullptr)
            return;
        {
            inOrderTrav(node->left);
            cout << node->key << " ";
            inOrderTrav(node->right);
        }
    }
    void preOrder() {
        cout << "PRE : ";
        preOrderTrav(root);
        cout << endl;
    }
    void preOrderTrav(Node* node) {
        if (node == nullptr)
            return;
        {
            cout << node->key << " ";
            preOrderTrav(node->left);
            preOrderTrav(node->right);
        }
    }
    void postOrder() {
        cout << "POST : ";
        postOrderTrav(root);
        cout << endl;
    }
    void postOrderTrav(Node* node) {
        if (node == nullptr)
            return;
        {
            postOrderTrav(node->left);
            postOrderTrav(node->right);
            cout << node->key << " ";

        }
    }

    //8. Find Smallest and Largest Elements in AVL Tree : Write functions to find the smallest and
    //    largest elements in an AVL tree.
    int SmallestVal() {
        return smallest(root);
    }
    int LargestVal() {
        return largest(root);
    }
    int smallest(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node->key;
    }
    int largest(Node* node) {
        while (node->right != nullptr) {
            node = node->right;
        }
        return node->key;
    }
    bool isValid(Node* root) {
        if (root == nullptr) {
            return true;
        }
        int leftHeight;
        if (root->left != nullptr) {
            leftHeight = root->left->height;
        }
        else {
            leftHeight = -1;
        }
        int rightHeight;
        if (root->right != nullptr) {
            rightHeight = root->right->height;
        }
        else {
            rightHeight = -1;
        }
        int hDiff = rightHeight - leftHeight;
        if (hDiff > 1 || hDiff < -1) {
            return false;
        }

        // Recursively check left and right subtrees
        return isValid(root->left) && isValid(root->right);
    }

    int totalNodes(Node* root) {
        if (root == nullptr) {
            return 0;
        }
        return 1 + totalNodes(root->left) + totalNodes(root->right);
    }
};
