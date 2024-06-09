/**
 * @file main1.cpp
 * @brief This file contains the code to run Jarvis March algorithm.
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include<iostream>
#include<ctime>

#include "classes/MyConvexHull.h"
#include "classes/KPS.h"
#include "utils.h"

using namespace std;

int main(){

    MyConvexHull jm;
    // time_t time1, time2;
    vector<pair<float,float>> points = make_list("points.csv");
    jm.fit_set(points);
    // time1 = time(NULL);
    vector<pair<float,float>> hull = jm.compute_hull();
    // time2 = time(NULL);
    // cout << "time : " << time2 - time1 << endl;
    int n=hull.size();
    for(int i=0;i<n;i++){
    	cout<<"("<<hull[i].first<<","<<hull[i].second<<")"<<" ";
    }
    cout<<endl;
    cout<<endl;

}
