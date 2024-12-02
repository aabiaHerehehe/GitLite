#include "GlobalFunctions.h"
using namespace std;

//write node data to a file
void writeNodeToFile(const filesystem::path& filepath, int key, const filesystem::path& left, const filesystem::path& right, int height) 
{
    ofstream file(filepath);
    if (file.is_open())
    {
        file << "Key: " << key << endl;
        file << "LeftChildPath: " << (left.empty() ? "NULL" : left.string()) << endl;
        file << "RightChildPath: " << (right.empty() ? "NULL" : right.string()) << endl;
        file << "Height: " << height << endl;
        file.close();
    }
    else
        cout << "Unable to write to file " << filepath << endl;
}

bool readNodeFromFile(const filesystem::path& filepath, int& key, filesystem::path& left, filesystem::path& right, int& height) 
{
    if (!filesystem::exists(filepath)) 
        return false;

    ifstream file(filepath);
    if (!file.is_open()) 
        return false;

    char line[256];
    while (file.getline(line, sizeof(line))) 
    {
        char temp[256];
        if (findSubstring(line, "Key:") == 0)
        {
            extractSubstring(temp, line, 5, stringLength(line) - 5);
            key = convertStringToInt(temp);
        }
        else if (findSubstring(line, "LeftChildPath:") == 0)
        {
            extractSubstring(temp, line, 15, stringLength(line) - 15);
            left = (compareStrings(temp, "NULL") == 0) ? filesystem::path() : filesystem::path(temp);
        }
        else if (findSubstring(line, "RightChildPath:") == 0) 
        {
            extractSubstring(temp, line, 16, stringLength(line) - 16);
            right = (compareStrings(temp, "NULL") == 0) ? filesystem::path() : filesystem::path(temp);
        }
        else if (findSubstring(line, "Height:") == 0) 
        {
            extractSubstring(temp, line, 8, stringLength(line) - 8);
            height = convertStringToInt(temp);
        }
    }
    file.close();
    return true;
}


class AVLTree 
{
public:
    filesystem::path rootFile; // Path to the root node's file

    AVLTree() : rootFile("") {}
    void insert(int key, char * originalValue)
    {
        rootFile = insertFile(rootFile, key, originalValue);
    }

private:

    //function to calculate the balance factor
    int calculateBalanceFactor(const filesystem::path& nodeFile)
    {
        if (nodeFile.empty() || !filesystem::exists(nodeFile)) 
            return -1; //or should this be zero??
        return getHeight(getRightChild(nodeFile)) - getHeight(getLeftChild(nodeFile));
    }

    filesystem::path insertFile(const filesystem::path& nodeFile, int key, char * originalValue) 
    {
        if (nodeFile.empty() || !filesystem::exists(nodeFile)) 
        {
            //create file node
            filesystem::path newNodeFile = convertToFilesystemPath(originalValue);
            writeNodeToFile(newNodeFile, key, "", "", 1);
            return newNodeFile;
        }

        int currentNodeKey, nodeHeight;
        filesystem::path leftPath, rightPath;
        readNodeFromFile(nodeFile, currentNodeKey, leftPath, rightPath, nodeHeight);

        //find where to insert the file according to BST property
        if (currentNodeKey > key)
            leftPath = insertFile(leftPath, key, originalValue);
        else if (currentNodeKey < key)
            rightPath = insertFile(rightPath, key, originalValue);
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

    int getHeight(const filesystem::path& nodeFile)
    {
        if (nodeFile.empty() || !filesystem::exists(nodeFile)) 
            return 0;

        int key, height;
        filesystem::path left, right;
        readNodeFromFile(nodeFile, key, left, right, height);
        return height;
    }

    int getKey(const filesystem::path& nodeFile)
    {
        if (nodeFile.empty() || !filesystem::exists(nodeFile)) 
            return 0;

        int key, height;
        filesystem::path left, right;
        readNodeFromFile(nodeFile, key, left, right, height);
        return key;
    }

    filesystem::path getLeftChild(const filesystem::path& nodeFile) 
    {
        int key, height;
        filesystem::path left, right;
        readNodeFromFile(nodeFile, key, left, right, height);
        return left;
    }

    filesystem::path getRightChild(const filesystem::path& nodeFile)
    {
        int key, height;
        filesystem::path left, right;
        readNodeFromFile(nodeFile, key, left, right, height);
        return right;
    }

    filesystem::path leftRotate(const filesystem::path& xFile, const filesystem::path& yFile) 
    {
        //get attrubutes
        int xKey, yKey, xHeight, yHeight;
        filesystem::path xLeft, xRight, yLeft, yRight;

        readNodeFromFile(xFile, xKey, xLeft, xRight, xHeight);
        readNodeFromFile(yFile, yKey, yLeft, yRight, yHeight);

        //set left and right
        xRight = yLeft;
        yLeft = xFile;

        writeNodeToFile(xFile, xKey, xLeft, xRight, 1 + max(getHeight(xLeft), getHeight(xRight)));
        writeNodeToFile(yFile, yKey, yLeft, yRight, 1 + max(getHeight(yLeft), getHeight(yRight)));

        return yFile;
    }

    filesystem::path rightRotate(const filesystem::path& yFile, const filesystem::path& xFile) 
    {
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

