#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <string>


using namespace std;

// Utility to write node data to a file
void writeNodeToFile(const filesystem::path& filepath, int key, const filesystem::path& left, const filesystem::path& right, int height) 
{
    ofstream file(filepath);
    if (file.is_open()) {
        file << "Key: " << key << endl;
        file << "LeftChildPath: " << (left.empty() ? "NULL" : left.string()) << endl;
        file << "RightChildPath: " << (right.empty() ? "NULL" : right.string()) << endl;
        file << "Height: " << height << endl;
        file.close();
    }
    else {
        cerr << "Error: Unable to write to file " << filepath << endl;
    }
}

// Utility to read node data from a file
bool readNodeFromFile(const filesystem::path& filepath, int& key, filesystem::path& left, filesystem::path& right, int& height) 
{
    if (!filesystem::exists(filepath))
        return false;

    ifstream file(filepath);
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        if (line.find("Key:") == 0) {
            key = stoi(line.substr(5));
        }
        else if (line.find("LeftChildPath:") == 0) {
            string leftPath = line.substr(15);
            left = (leftPath == "NULL") ? filesystem::path() : filesystem::path(leftPath);
        }
        else if (line.find("RightChildPath:") == 0) {
            string rightPath = line.substr(16);
            right = (rightPath == "NULL") ? filesystem::path() : filesystem::path(rightPath);
        }
        else if (line.find("Height:") == 0) {
            height = stoi(line.substr(8));
        }
    }
    file.close();
    return true;
}


class AVLTree {
public:
    filesystem::path rootFile; // Path to the root node's file

    AVLTree() : rootFile("") {}

    void insert(int key)
    {
        rootFile = insertFile(rootFile, key);
    }

private:

    //function to calculate the balance factor
    int calculateBalanceFactor(const filesystem::path& nodeFile)
    {
        if (nodeFile.empty() || !filesystem::exists(nodeFile)) 
            return -1; //or should this be zero??
        return getHeight(getRightChild(nodeFile)) - getHeight(getLeftChild(nodeFile));
    }

    filesystem::path insertFile(const filesystem::path& nodeFile, int key) 
    {
        if (nodeFile.empty() || !filesystem::exists(nodeFile)) {
            // Create a new node file
            filesystem::path newNodeFile = filesystem::path("node" + to_string(key) + ".txt");
            writeNodeToFile(newNodeFile, key, "", "", 1);
            return newNodeFile;
        }

        int currentNodeKey, nodeHeight;
        filesystem::path leftPath, rightPath;
        readNodeFromFile(nodeFile, currentNodeKey, leftPath, rightPath, nodeHeight);

        //find where to insert the file according to BST property
        if (currentNodeKey > key)
            leftPath = insertFile(leftPath, key);
        else if (currentNodeKey < key)
            rightPath = insertFile(rightPath, key);
        else //duplicate keys
        {
            return nodeFile; //no duplicates, change later
        }

        //update heights and rotate
        int leftHeight = getHeight(leftPath);
        int rightHeight = getHeight(rightPath);
        int balanceFactor = rightHeight - leftHeight;
        int maxHeight;
        if (leftHeight > rightHeight)
            maxHeight = leftHeight;
        else
            maxHeight = rightHeight;

        //rotate based on BF
        if (balanceFactor > 1) //right-skewed
        { 
            if (calculateBalanceFactor(rightPath) >= 0) //same sign, single left rotation
                return leftRotate(nodeFile, rightPath);
            else //opposite signs, right-left rotation
            {
                rightPath = rightRotate(rightPath, getLeftChild(rightPath));
                return leftRotate(nodeFile, rightPath);
            }
        }
        else if (balanceFactor < -1) //left-skewed
        { 
            if (calculateBalanceFactor(leftPath) <= 0) //single rights
                return rightRotate(nodeFile, leftPath);
            else //left right rotation
            {
                leftPath = leftRotate(leftPath, getRightChild(leftPath));
                return rightRotate(nodeFile, leftPath);
            }
        }

        //update the current node file
        writeNodeToFile(nodeFile, currentNodeKey, leftPath, rightPath, 1 + maxHeight);
        return nodeFile;
    }

    int getHeight(const filesystem::path& nodeFile) {
        if (nodeFile.empty() || !filesystem::exists(nodeFile)) return 0;

        int key, height;
        filesystem::path left, right;
        readNodeFromFile(nodeFile, key, left, right, height);
        return height;
    }

    int getKey(const filesystem::path& nodeFile) {
        if (nodeFile.empty() || !filesystem::exists(nodeFile)) return 0;

        int key, height;
        filesystem::path left, right;
        readNodeFromFile(nodeFile, key, left, right, height);
        return key;
    }

    filesystem::path getLeftChild(const filesystem::path& nodeFile) {
        int key, height;
        filesystem::path left, right;
        readNodeFromFile(nodeFile, key, left, right, height);
        return left;
    }

    filesystem::path getRightChild(const filesystem::path& nodeFile) {
        int key, height;
        filesystem::path left, right;
        readNodeFromFile(nodeFile, key, left, right, height);
        return right;
    }

    filesystem::path leftRotate(const filesystem::path& xFile, const filesystem::path& yFile) {
        // Perform left rotation and update files
        int xKey, yKey, xHeight, yHeight;
        filesystem::path xLeft, xRight, yLeft, yRight;

        readNodeFromFile(xFile, xKey, xLeft, xRight, xHeight);
        readNodeFromFile(yFile, yKey, yLeft, yRight, yHeight);

        xRight = yLeft;
        yLeft = xFile;

        writeNodeToFile(xFile, xKey, xLeft, xRight, 1 + max(getHeight(xLeft), getHeight(xRight)));
        writeNodeToFile(yFile, yKey, yLeft, yRight, 1 + max(getHeight(yLeft), getHeight(yRight)));

        return yFile;
    }

    filesystem::path rightRotate(const filesystem::path& yFile, const filesystem::path& xFile) {
        // Perform right rotation and update files
        int xKey, yKey, xHeight, yHeight;
        filesystem::path xLeft, xRight, yLeft, yRight;

        readNodeFromFile(xFile, xKey, xLeft, xRight, xHeight);
        readNodeFromFile(yFile, yKey, yLeft, yRight, yHeight);

        yLeft = xRight;
        xRight = yFile;

        writeNodeToFile(xFile, xKey, xLeft, xRight, 1 + max(getHeight(xLeft), getHeight(xRight)));
        writeNodeToFile(yFile, yKey, yLeft, yRight, 1 + max(getHeight(yLeft), getHeight(yRight)));

        return xFile;
    }
};


//class Node {
//public:
//    filesystem::path path;
//    int data;
//    Node* left;
//    Node* right;
//    int height;
//    Node(int d) : data(d), left(nullptr), right(nullptr), height(0) {}
//};
//
//class AVLTree {
//public:
//    Node* root;
//    Node* getRoot() { return root; }
//    AVLTree() : root(nullptr) {}
//
//    void insert(int data) {
//        root = insertion(root, data);
//    }
//    Node* insertion(Node* node, int data) {
//        if (node == nullptr) {
//         //   cout << "Inserting " << " : ";
//            cout << data << endl;
//            return new Node(data);
//
//        }
//
//        if (data < node->data) {
//        //    cout << "Inserting " << data << " to left of " << node->data << endl;
//            node->left = insertion(node->left, data);
//        }
//        else if (data > node->data) {
//        //    cout << "Inserting " << data << " to right of " << node->data << endl;
//            node->right = insertion(node->right, data);
//        }
//        else {
//            // Duplicate data not allowed
//            return node;
//        }
//
//        // Update the height of the current node
//        int maxHeight;
//        if (height(node->left) > height(node->right))
//            maxHeight = height(node->left);
//        else
//            maxHeight = height(node->right);
//        node->height = 1 + maxHeight;
//
//        // Check balance factor 
//        int balance = BF(node);
//
//        // Right-Skewed Case
//        if (balance > 1) {
//            if (BF(node->right) >= 0) {
//                // Rightskew-Rightskew case
//                return leftRotation(node);
//            }
//            else {
//                // Rightrot-Leftrot case
//                node->right = rightRotation(node->right);
//                return leftRotation(node);
//            }
//        }
//
//        // Left-Skewed Case
//        if (balance < -1) {
//            if (BF(node->left) <= 0) {
//                // Leftskew-Leftskew case
//                return rightRotation(node);
//            }
//            else {
//                // Leftrot-Rightrot case
//                node->left = leftRotation(node->left);
//                return rightRotation(node);
//            }
//        }
//
//
//        return node;
//    }
//
//
//    void Delete(int data) {
//        root = deletion(root, data);
//    }
//
//    Node* deletion(Node*& node, int data) {
//        // BST deletion
//        if (node == nullptr) {
//            return node;
//        }
//
//        // Recurse down the tree to find the node to delete
//        if (data < node->data) {
//            node->left = deletion(node->left, data);
//        }
//        else if (data > node->data) {
//            node->right = deletion(node->right, data);
//        }
//        else {
//            // Node with only one child or no child
//            if (node->left == nullptr && node->right == nullptr) {  // No children
//                delete node;
//                return nullptr;
//            }
//            else if (node->left == nullptr) {  // Only right child
//                Node* temp = node->right;
//                delete node;
//                return temp;
//            }
//            else if (node->right == nullptr) {  // Only left child
//                Node* temp = node->left;
//                delete node;
//                return temp;
//            }
//            else {
//                // Node with two children: get the inorder successor (smallest in the right subtree)
//                Node* temp = node->right;
//                while (temp->left != nullptr) {
//                    temp = temp->left;
//                }
//                // Copy the inorder successor's data to this node and delete the successor
//                node->data = temp->data;
//                node->right = deletion(node->right, temp->data);
//            }
//        }
//
//
//        // Update the height of the current node
//        int maxHeight;
//        if (height(node->left) > height(node->right))
//            maxHeight = height(node->left);
//        else
//            maxHeight = height(node->right);
//        node->height = 1 + maxHeight;
//
//        // Check balance factor 
//        int balance = BF(node);
//
//        // Right-skewed cases
//        if (balance > 1) {
//            if (BF(node->right) >= 0) {
//                // Right-Right case
//                node = leftRotation(node);
//            }
//            else {
//                // Right-Left case
//                node->right = rightRotation(node->right);
//                node = leftRotation(node);
//            }
//        }
//        // Left-skewed cases
//        else if (balance < -1) {
//            if (BF(node->left) <= 0) {
//                // Left-Left case
//                node = rightRotation(node);
//            }
//            else {
//                // Left-Right case
//                node->left = leftRotation(node->left);
//                node = rightRotation(node);
//            }
//        }
//
//        return node;
//    }
//
//    int BF(Node* node) {
//        if (node == nullptr)
//            return 0;
//        return height(node->right) - height(node->left);
//    }
//
//    int height(Node* node) {
//        if (node == nullptr) {
//            return -1;
//        }
//        else return node->height;
//    }
//    int max(int a, int b) {
//        if (a > b)
//            return a;
//        else
//            return b;
//    }
//
//    Node* leftRotation(Node* node) {
//        Node* newRoot = node->right;
//        Node* temp = newRoot->left;
//
//        newRoot->left = node;
//        node->right = temp;
//
//        node->height = 1 + max(height(node->left), height(node->right));
//        newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));
//        cout << "rotated left" << endl;
//        return newRoot;
//    }
//
//    Node* rightRotation(Node* node) {
//        Node* newRoot = node->left;
//        Node* temp = newRoot->right;
//
//        newRoot->right = node;
//        node->left = temp;
//
//        node->height = 1 + max(height(node->left), height(node->right));
//        newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));
//        cout << "rotated right" << endl;
//        return newRoot;
//    }
//
//    void inOrder() {
//        cout << "IN : ";
//        inOrderTrav(root);
//        cout << endl;
//    }
//
//    void inOrderTrav(Node* node) {
//        if (node == nullptr)
//            return;
//        {
//            inOrderTrav(node->left);
//            cout << node->data << " ";
//            inOrderTrav(node->right);
//        }
//    }
//    void preOrder() {
//        cout << "PRE : ";
//        preOrderTrav(root);
//        cout << endl;
//    }
//    void preOrderTrav(Node* node) {
//        if (node == nullptr)
//            return;
//        {
//            cout << node->data << " ";
//            preOrderTrav(node->left);
//            preOrderTrav(node->right);
//        }
//    }
//    void postOrder() {
//        cout << "POST : ";
//        postOrderTrav(root);
//        cout << endl;
//    }
//    void postOrderTrav(Node* node) {
//        if (node == nullptr)
//            return;
//        {
//            postOrderTrav(node->left);
//            postOrderTrav(node->right);
//            cout << node->data << " ";
//
//        }
//    }
//
//    //8. Find Smallest and Largest Elements in AVL Tree : Write functions to find the smallest and
//    //    largest elements in an AVL tree.
//    int SmallestVal() {
//        return smallest(root);
//    }
//    int LargestVal() {
//        return largest(root);
//    }
//    int smallest(Node* node) {
//        while (node->left != nullptr) {
//            node = node->left;
//        }
//        return node->data;
//    }
//    int largest(Node* node) {
//        while (node->right != nullptr) {
//            node = node->right;
//        }
//        return node->data;
//    }
//    bool isValid(Node* root) {
//        if (root == nullptr) {
//            return true;
//        }
//        int leftHeight;
//        if (root->left != nullptr) {
//            leftHeight = root->left->height;
//        }
//        else {
//            leftHeight = -1;
//        }
//        int rightHeight;
//        if (root->right != nullptr) {
//            rightHeight = root->right->height;
//        }
//        else {
//            rightHeight = -1;
//        }
//        int hDiff = rightHeight - leftHeight;
//        if (hDiff > 1 || hDiff < -1) {
//            return false;
//        }
//
//        // Recursively check left and right subtrees
//        return isValid(root->left) && isValid(root->right);
//    }
//
//    int totalNodes(Node* root) {
//        if (root == nullptr) {
//            return 0;
//        }
//        return 1 + totalNodes(root->left) + totalNodes(root->right);
//    }
//
//
//
//};