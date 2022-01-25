//
//  LearnCpp.cpp
//  PersonalProjects
//
//  Created by Yiyun Wu on 5/28/21.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std; //std::cout

int main(){
    //PRINT
    //cout << "Hello World" << endl; //endl is carriage return
    
    //DATA TYPES
    const double PI = 3.1415926535;
    bool isHappy = true; // true = 1, false = 0
    //int, float, char, short int, long int, long long int, unsigned int, long double
    
    //cout << "Favorite Number " << PI << endl;
    //cout << "Size of bool " << sizeof(isHappy) <<endl;
    
    //ARITHMETIC
    int five = 5;
    //decrement or increment on right, action occurs after taking current value
    //cout << "5++ = " << five++ << endl;
    //cout << "4 / 5 = " << (float) 4/5 << endl;
    
    //AUTO
    auto WhatAmI = true; //automatically assigned to boolean data type
    
    //CASTING
    // stoi string object with the representation of an integral number FIX ME
    
    //USER INPUT
    int num1;
    cout << "Enter a number: ";
    cin >> num1;
    cout << "Your number is " << num1 << endl;
    
    //TERNARY OPERATOR
    //variable = (condition) ? true : false
    int largestNum = (num1>2) ? 5 : 2;
    cout << "Your largestnum is " << largestNum << endl;
    
    //RNG
    //int randNum = (rand() % 100) + 1;
    
    // VECTORS
    vector<int> vNums(2); //used when we don't know how many values we have
    vNums[0] = 1;
    vNums[1] = 2;
    vNums.push_back(3);
    //cout << "Vector Size : " << vNums.size() << endl;
    
    // newthinktank.com/2019/10/c-tutorial-2019/
    
    return 0;
}
