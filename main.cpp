#include "GlobalFunctions.h"
#include "AVLTree.h"
using namespace std;

int main() 
{
    cout << "\nWelcome to Team Exception Thrown's GitLite!" << endl;

    filesystem::path filePath;
    do
    {
        cout << "Enter the complete path of the .csv file: ";
        cin >> filePath;
        //check if file path exists
        if (!filesystem::exists(filePath))
            cout << "This file does not exist. Please try again." << endl;
    } 
    while (!filesystem::exists(filePath));

    ifstream file(filePath);
    if (!file.is_open()) 
        cout << "Could not open the file " << filePath << "." << endl;

    char line[1024]; //buffer to store each line
    int lineSize = 1024;
    int numFields = 0;

    if (file.getline(line, lineSize)) 
    {
        char* temporaryTokens = tokenize(line, ",");
        while (temporaryTokens)
        {
            numFields++;
            temporaryTokens = tokenize(nullptr, ",");
        }
    }
    else
        cout << "Could not read column names. Please try again." << endl;

    //make array for columns based on number of columns
    char** columns = new char* [numFields];
    for (int i = 0; i < numFields; i++)
        columns[i] = new char[500]; //assuming a single field is 500 characters max

    //reset the file pointer to the start to store field names
    file.clear();
    file.seekg(0);
    if (file.getline(line, lineSize)) 
    {
        int index = 0;
        char* token = tokenize(line, ",");
        while (token)
        {
            copyIn(columns[index], token); // copy the strings in
            index++;
            token = tokenize(nullptr, ",");
        }
    }

    cout << "------COLUMN NAMES------" << endl;
    for (int i = 0; i < numFields; i++) //output columns for user
        cout << "Column #" << i + 1 << ": " << columns[i] << endl;

    int columnNum;
    do
    {
        cout << "Enter the number of the column/field you want to make a tree for: ";
        cin >> columnNum;
        columnNum -= 1;
        if (columnNum < 0 || columnNum >= numFields)
            cout << "Invalid field number. Please try again." << endl;
    } 
    while (columnNum < 0 || columnNum >= numFields);

    int treeType, m = 0;
    do
    {
        cout << "1. AVL\n2. RED BLACK TREE\n3. B TREES\nEnter the tree you want to make: ";
        cin >> treeType;
        if (treeType < 0 || treeType > 3)
            cout << "Invalid tree type, please try again." << endl;
    } 
    while (treeType < 0 || treeType > 3);

    if (treeType == 3) 
    {
        cout << "Enter m: ";
        cin >> m;
    }

    int lineNumber = 1; 
    //process records
    while (file.getline(line, sizeof(line)))
    {
        lineNumber++;
        char* token = tokenize(line, ",");
        int currentFieldIndex = 0;
        char fieldValue[100] = "";

        // Extract the chosen field's value
        while (token && currentFieldIndex <= numFields) 
        {
            if (currentFieldIndex == numFields)
                copyIn(fieldValue, token);
            currentFieldIndex++;
            token = tokenize(nullptr, ",");
        }

        cout << "Field val: " << fieldValue << endl;

        if (fieldValue[0] == '\0') 
        {
            cout << "Error: Field value is empty for line " << lineNumber << endl;
            continue;
        }

        int hashKey = instructorHash(fieldValue);

        //create a file for the key
        char filename[20];
        snprintf(filename, sizeof(filename), "%s.txt", fieldValue); //create file name as key.txt
        ofstream keyFile(filename);
        if (keyFile.is_open()) 
        {
            cout << "file made";
            keyFile << "Line Number: " << lineNumber << "\n";
            keyFile.close();
        }
    }



    file.close();
    return 0;
}
