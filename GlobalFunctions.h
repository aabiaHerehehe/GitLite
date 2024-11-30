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

//tokenize a char array
char* tokenize(char* arr, const char* delimiter) 
{
    static char* nextToken = nullptr;
    if (arr)
        nextToken = arr;
    if (!nextToken || *nextToken == '\0')
        return nullptr;

    char* tokenStart = nextToken;
    while (*nextToken != '\0') 
    {
        for (int i = 0; delimiter[i] != '\0'; i++) 
        {
            if (*nextToken == delimiter[i]) 
            {
                *nextToken = '\0'; //replace the delimiter with null character
                nextToken++;
                return tokenStart;
            }
        }
        nextToken++;
    }
    nextToken = nullptr; //tokens finish
    return tokenStart;
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
    return hash;
}

