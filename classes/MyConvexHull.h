/**
 * @file MyConvexHull.h
 * @brief This is a header file containing the implementation of Jarvis March algorithm.
 * @date 2024-03-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef JARVIS_MARCH_H
#define JARVIS_MARCH_H

#include "../utils.h"
#include <bits/stdc++.h>
using namespace std;

class MyConvexHull
{
    /**
     * @brief MyCOnvexHull is a class which contains all the functions related to Jarvis March Algorithm.
     *
     */
    vector<pair<float, float>> pointSet;

    /**
     * @brief findMinimumYIndex function returns us the index of the point with minimum y value in the given set of points.
     * @param pointSet It is the set of points of the type (x, y) for which we want to find the minimum y coordinate.
     * @return int
     */
    int findMinimumYIndex(vector<pair<float, float>> pointSet)
    {
        int i;
        int currentY, currentX, index = 0;
        float minY = pointSet[0].second;

        for (i = 1; i < pointSet.size(); i++)
        {
            currentY = pointSet[i].second;
            currentX = pointSet[i].first;
            if ((currentY < minY) || (currentY == minY && currentX < pointSet[index].first))
            {
                minY = currentY;
                index = i;
            }
        }
        return index;
    }

public:
    /**
     * @brief Construct a new My Convex Hull object.
     *
     */
    MyConvexHull()
    {
        pointSet = vector<pair<float, float>>();
    }

    /**
     * @brief fit_set function copies the list of points from the user-defined set into the private member variable "pointSet".
     *
     * @param list list of points in format (x, y).
     */
    void fit_set(vector<pair<float, float>> list)
    {
        pointSet = list;
    }

    /**
     * @brief addPoint function appends the point "pt" to "pointSet".
     *
     * @param pt
     */
    void addPoint(pair<float, float> pt)
    {
        pointSet.push_back(pt);
    }

    /**
     * @brief compute_hull function performs the actual  convex hull computation on "pointSet".
     * It first sorts all points according to  their polar angle with respect to the leftmost point in "pointSet".
     * It uses "getLowestIndex()" to find the leftmost point and starts from there. Then it adds each possible next point
     * The result is stored in "convexHull", which is a set of points in counter-clockwise order.
     *
     * @return vector<pair<float, float>>
     */
    vector<pair<float, float>> compute_hull()
    {
        if (pointSet.size() < 3)
        {
            cout << "Convex hull doesn't exist!!" << endl;
            return {};
        }

        int ptIndex = findMinimumYIndex(pointSet);
        vector<pair<float, float>> hull;
        int l = ptIndex;
        int p = l;
        int q;
        do
        {
            hull.push_back(pointSet[p]);
            int n = pointSet.size();
            q = (p + 1) % n;
            for (int i = 0; i < n; i++)
            {
                int val = (pointSet[i].second - pointSet[p].second) * (pointSet[q].first - pointSet[i].first) - (pointSet[i].first - pointSet[p].first) * (pointSet[q].second - pointSet[i].second);
                if (val < 0)
                    q = i;
            }
            p = q;
        } while (p != l);

        make_result(hull, "results/results_1.csv");
        return hull;
    }
};

#endif