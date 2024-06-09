/**
 * @file utils.h
 * @brief This file contains the utility functions.
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
using namespace std;

/**
 * @brief make_result is a function used to create a .csv file containing points of type (x, y)
 * 
 * @param list It is the list of points.
 * @param addr It is the address of .csv file we want to create.
 */
void make_result(vector<pair<float, float>> list, string addr){
  fstream file;
  ostringstream str1;
  file.open(addr, ios::out);
  for (int i = 0; i < list.size(); i++){
    pair<float, float> p = list[i];
    file << p.first << "," << p.second << "\n";
  }
  file.close();
}

/**
 * @brief make_list is a function that stores points of type (x, y) in list format.
 * 
 * @param addr It is the address of .csv file in which the points are stored.
 * @return vector<pair<float, float>> 
 */
vector<pair<float, float>> make_list(string addr){
  vector<pair<float, float>> list;
  fstream file;
  file.open(addr);
  string line;
  while (getline(file, line, '\n')){
    istringstream templine(line);
    string data;
    int i = 0;
    double a[2];
    while (getline(templine, data, ',')){
      a[i] = stof(data);
      i++;
    }
    pair<float, float> p(a[0], a[1]);
    list.push_back(p);
  }
  file.close();
  return list;
}

#endif
