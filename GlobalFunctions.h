#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
using namespace std;

//calculate the length of a char array
int stringLength(char* str)
{
    int length = 0;
    while (str[length] != '\0') 
        length++;
    return length;
}
//overloaded
int stringLength(const char* str)
{
    int length = 0;
    while (str[length] != '\0')
        length++;
    return length;
}

//copy two char arrays
void copyIn(char* copy, const char* paste) 
{
    int i = 0;
    while (paste[i] != '\0')
    {
        copy[i] = paste[i];
        i++;
    }
    copy[i] = '\0'; //null terminate it
}

//comapre two char arrays
int compareStrings(const char* arr1, const char* arr2) 
{
    int i = 0;
    while (arr1[i] != '\0' && arr2[i] != '\0') 
    {
        if (arr1[i] != arr2[i]) 
            return arr1[i] - arr2[i];
        i++;
    }
    return arr1[i] - arr2[i];
}

//concatenate two arrays
char* concatenate(const char* first, const char* second)
{
    int length1 = stringLength(first);
    int length2 = stringLength(second);
    char* result = new char[length1 + length2 + 1]; // +1 for null terminator

    //copy the first in
    int i = 0;
    while (i < length1)
    {
        result[i] = first[i];
        i++;
    }

    //copy second in
    int j = 0;
    while (j < length2)
    {
        result[i] = second[j];
        i++;
        j++;
    }
    //null-terminate the result
    result[i] = '\0';
    return result;
}
//tokenize an array
char* tokenize(char* arr, const char* delimiter)
{
    static char* nextToken = nullptr;
    if (arr)
        nextToken = arr;
    if (!nextToken || *nextToken == '\0')
        return nullptr;

    char* tokenStart = nextToken;
    bool inQuotes = false;

    while (*nextToken != '\0')
    {
        if (*nextToken == '"')
        {
            inQuotes = !inQuotes; //toggle quotes
        }
        else if (!inQuotes)
        {
            for (int i = 0; delimiter[i] != '\0'; i++)
            {
                if (*nextToken == delimiter[i])
                {
                    *nextToken = '\0'; //replace the delimiter with null character
                    nextToken++;
                    //skip spaces after the delimiter
                    while (*nextToken == ' ')
                        nextToken++;
                    return tokenStart;
                }
            }
        }
        nextToken++;
    }

    nextToken = nullptr;
    return tokenStart;
}

//convert string to integer
int convertStringToInt(const char* str) 
{
    int num = 0, i = 0;
    bool negativeNum = false;

    if (str[0] == '-') //negative num
    { 
        negativeNum = true;
        i++;
    }

    while (str[i] != '\0')
    {
        num = num * 10 + (str[i] - '0'); //convert char to int
        i++;
    }

    if (negativeNum)
        return -num;
    else
        return num;
}

//convert an integer to a character array
void convertIntToString(int n, char* result) 
{
    int i = 0;
    bool negativeNum = false;

    if (n < 0) 
    {
        negativeNum = true;
        n = -n;
    }

    do 
    {
        result[i++] = (n % 10) + '0';
        n /= 10;
    } 
    while (n > 0);

    if (negativeNum) 
        result[i++] = '-';

    result[i] = '\0';

    //reverse result
    for (int j = 0, k = i - 1; j < k; j++, k--)
    {
        char temp = result[j];
        result[j] = result[k];
        result[k] = temp;
    }
}


//extract substring from character array
void extractSubstring(char* substring, const char* source, int start, int length) 
{
    int i = 0;
    while (i < length && source[start + i] != '\0') 
    {
        substring[i] = source[start + i];
        i++;
    }
    substring[i] = '\0';
}

//find substring
int findSubstring(const char* str, const char* substring)
{
    int length = stringLength(str);
    int subLength = stringLength(substring);

    for (int i = 0; i <= length - subLength; i++) 
    {
        bool match = true;
        for (int j = 0; j < subLength; j++) 
        {
            if (str[i + j] != substring[j]) 
            {
                match = false;
                break;
            }
        }
        if (match)
            return i; //return index of the first occurrence
    }
    return -1; //not found
}

//instructor hash any char array
int instructorHash(const char* str) 
{
    int hash = str[0];
    int i = 1;
    while (str[i] != '\0') 
    {
        hash = hash * str[i];
        i++;
    }
    hash %= 29;
    if (hash < 0)
        hash *= -1;
    return hash;
}

//convert a const char array into a filesystem object
filesystem::path convertToFilesystemPath(char* filename)
{
    char* newFileName = concatenate(filename, ".txt"); //append .txt
    //convert
    std::filesystem::path newPath(newFileName);
    //free the allocated memory
    delete[] newFileName;
    return newPath;
}