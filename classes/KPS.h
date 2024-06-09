/**
 * @file KPS.h
 * @brief This is a header file containing the implementation of Kirkpatrick Seidel algorithm.
 * @date 2024-03-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef KPS_H
#define KPS_H

#include "../utils.h"
#include <bits/stdc++.h>
using namespace std;

class KPS
{
  /**
   * @brief KPS is a class which contains all the functions related to Kirkpatrick Seidel Algorithm.
   * 
   */
  vector<pair<float, float>> points;

  /**
   * @brief findMedian function returns the median of given x coordinates.
   * 
   * @param arr It is an array containing x coordinates.
   * @param offset It is the point from where we need to start finding the median. 
   * @param n It is the number of elements in the array.
   * @return double 
   */
  double findMedian(vector<double> arr, int offset, int n)
  {
    sort(arr.begin() + offset, arr.end());
    return arr[offset + n / 2];
  }

  /**
   * @brief swap function swaps the given two points.
   * 
   * @param a It is a value to be swapped.
   * @param b It is a value to be swapped.
   */
  void swap(double *a, double *b)
  {
    double temp = *a;
    *a = *b;
    *b = temp;
  }

  /**
   * @brief partition function returns the index for the partition.
   * 
   * @param arr It is an array containing x coordinates.
   * @param l It is the left index.
   * @param r It is the right index.
   * @param x It is the x coordinate for which we want to find the y coordinate.
   * @return int 
   */
  int partition(vector<double> arr, int l, int r, double x)
  {
    int i;
    for (i = l; i < r; i++)
      if (arr[i] == x)
        break;
    swap(&arr[i], &arr[r]);

    i = l;
    for (int j = l; j <= r - 1; j++)
    {
      if (arr[j] <= x)
      {
        swap(&arr[i], &arr[j]);
        i++;
      }
    }
    swap(&arr[i], &arr[r]);
    return i;
  }

  /**
   * @brief kthSmallest function returns K-th smallest element in the array.
   * 
   * @param arr It is an array containing x coordinates.
   * @param l It is the left index.
   * @param r It is the right index.
   * @param k 
   * @return double 
   */
  double kthSmallest(vector<double> arr, int l, int r, int k)
  {

    if (k > 0 && k <= r - l + 1)
    {
      int n = r - l + 1;
      int i;
      vector<double> median((n + 4) / 5);
      for (i = 0; i < n / 5; i++)
        median[i] = findMedian(arr, l + i * 5, 5);
      if (i * 5 < n)
      {
        median[i] = findMedian(arr, l + i * 5, n % 5);
        i++;
      }

      double medOfMed = (i == 1) ? median[i - 1] : kthSmallest(median, 0, i - 1, i / 2);
      int pos = partition(arr, l, r, medOfMed);

      if (pos - l == k - 1)
        return arr[pos];
      if (pos - l > k - 1)
        return kthSmallest(arr, l, pos - 1, k);

      return kthSmallest(arr, pos + 1, r, k - pos + l - 1);
    }

    return INT_MAX;
  }

  /**
   * @brief abs function returns the absolute value of a number.
   * 
   * @param a It is a number whose absolute value needs to be calculated.
   * @return double 
   */
  double abs(double a)
  {
    if (a < 0)
      return 0 - a;
    else
      return a;
  }

  /**
   * @brief get_T function returns a list of all points p such that x(pmin) < x(p[i]) < x(pmax) is satisfied.
   * 
   * @param p1 It is pmin.
   * @param p2 It is pmax.
   * @param points It is the list of all points of which we need to filter out some points.
   * @param flag It is to check whether we want to look for upper hull or lower hull. If it's true then we are looking for lower hull, else we are looking for upper hull.
   * @return vector<pair<float, float>> 
   */
  vector<pair<float, float>> get_T(pair<float, float> p1, pair<float, float> p2, vector<pair<float, float>> &points, bool flag)
  {

    vector<pair<float, float>> upper_T;
    double slope = (p1.second - p2.second) / (p1.first - p2.first);
    for (int i = 0; i < points.size(); i++)
    {
      pair<float, float> curr_point;
      curr_point.first = points[i].first;
      curr_point.second = points[i].second;

      if (curr_point.first > p1.first && curr_point.first < p2.first)
      {
        double curr_slope = (p1.second - curr_point.second) / (p1.first - curr_point.first);
        if (flag == false)
        {
          if (curr_slope > slope)
            upper_T.push_back(curr_point);
        }
        else
        {
          if (curr_slope < slope)
          {
            upper_T.push_back(curr_point);
          }
        }
      }
    }
    upper_T.push_back(p1);
    upper_T.push_back(p2);

    return upper_T;
  }

  /**
   * @brief get_upper_bridge function returns us the upper bridge.
   * 
   * @param points It is the list of points from which we have to find the upper bridge.
   * @param median
   * @return pair<pair<float, float>, pair<float, float>> 
   */
  pair<pair<float, float>, pair<float, float>> get_upper_bridge(vector<pair<float, float>> &points, double median)
  {
    sort(points.begin(), points.end(), [](pair<float, float> a, pair<float, float> b)
         { return a.first < b.first; });
    vector<pair<float, float>> candidates;
    vector<pair<pair<float, float>, pair<float, float>>> pairs;
    if (points.size() % 2 == 0)
    {
      for (int i = 0; i < points.size(); i += 2)
      {
        pair<float, float> first_pt = points[i];
        pair<float, float> second_pt = points[i + 1];
        pair<pair<float, float>, pair<float, float>> curr_pair;
        curr_pair.first = first_pt;
        curr_pair.second = second_pt;
        pairs.push_back(curr_pair);
      }
    }
    else
    {
      candidates.push_back(points[0]);
      for (int i = 1; i < points.size(); i += 2)
      {
        pair<float, float> first_pt = points[i];
        pair<float, float> second_pt = points[i + 1];
        pair<pair<float, float>, pair<float, float>> curr_pair;
        curr_pair.first = first_pt;
        curr_pair.second = second_pt;
        pairs.push_back(curr_pair);
      }
    }
    int slopes_len = pairs.size();
    vector<double> slopes(slopes_len);
    for (int i = 0; i < pairs.size(); i++)
    {
      pair<float, float> p1 = pairs[i].first;
      pair<float, float> p2 = pairs[i].second;
      double x1 = p1.first;
      double x2 = p2.first;
      double y1 = p1.second;
      double y2 = p2.second;

      if (x1 == x2)
      {
        if (y1 > y2)
        {
          candidates.push_back(p1);
        }
        else
        {
          candidates.push_back(p2);
        }
        slopes[i] = INT_MAX;
      }
      else
      {
        double slope = (y2 - y1) / (double)(x2 - x1);
        slopes[i] = slope;
      }
    }

    vector<double> arr(slopes_len);
    int len = 0;
    for (int i = 0; i < slopes_len; i++)
    {
      if (slopes[i] != INT_MAX)
      {
        arr[len++] = slopes[i];
      }
    }

    double median_slope;
    if (len == 1)
      median_slope = arr[0];
    else
      median_slope = kthSmallest(arr, 0, len - 1, (len + 1) / 2);

    vector<pair<pair<float, float>, pair<float, float>>> SMALL;
    vector<pair<pair<float, float>, pair<float, float>>> EQUAL;
    vector<pair<pair<float, float>, pair<float, float>>> LARGE;

    for (int i = 0; i < pairs.size(); i++)
    {
      pair<float, float> p1 = pairs[i].first;
      pair<float, float> p2 = pairs[i].second;
      double x1 = p1.first;
      double x2 = p2.first;
      double y1 = p1.second;
      double y2 = p2.second;

      if (x1 != x2)
      {
        double slope = (y2 - y1) / (x2 - x1);
        if (abs(slope - median_slope) < 0.001)
        {
          pair<pair<float, float>, pair<float, float>> curr_pair(p1, p2);
          EQUAL.push_back(curr_pair);
        }
        else if (slope < median_slope)
        {
          pair<pair<float, float>, pair<float, float>> curr_pair(p1, p2);
          SMALL.push_back(curr_pair);
        }
        else if (slope > median_slope)
        {
          pair<pair<float, float>, pair<float, float>> curr_pair(p1, p2);
          LARGE.push_back(curr_pair);
        }
      }
    }

    double max_c = INT_MIN;
    for (int i = 0; i < points.size(); i++)
    {
      double x = points[i].first;
      double y = points[i].second;
      double curr_c = (y - (double)median_slope * x);

      if (curr_c > max_c)
      {
        max_c = curr_c;
      }
    }

    pair<float, float> pmin(INT_MAX, INT_MAX);
    pair<float, float> pmax(INT_MIN, INT_MIN);

    for (int i = 0; i < points.size(); i++)
    {
      double x = points[i].first;
      double y = points[i].second;
      double curr_c = y - (double)median_slope * x;

      if (abs((double)curr_c - max_c) < 0.001)
      {
        if (x < pmin.first)
        {
          pmin.first = x;
          pmin.second = y;
        }
        if (x > pmax.first)
        {
          pmax.first = x;
          pmax.second = y;
        }
      }
    }

    if (pmin.first <= median && pmax.first > median)
    {
      pair<pair<float, float>, pair<float, float>> upper_bridge(pmin, pmax);
      return upper_bridge;
    }
    else if (pmax.first <= median)
    {
      for (int i = 0; i < EQUAL.size(); i++)
      {
        pair<float, float> pt = EQUAL[i].second;
        candidates.push_back(pt);
      }
      for (int i = 0; i < LARGE.size(); i++)
      {
        pair<float, float> pt = LARGE[i].second;
        candidates.push_back(pt);
      }
      for (int i = 0; i < SMALL.size(); i++)
      {
        pair<float, float> pt1 = SMALL[i].first;
        pair<float, float> pt2 = SMALL[i].second;
        candidates.push_back(pt1);
        candidates.push_back(pt2);
      }
      return get_upper_bridge(candidates, median);
    }
    else if (pmin.first > median)
    {
      for (int i = 0; i < EQUAL.size(); i++)
      {
        pair<float, float> pt = EQUAL[i].first;
        candidates.push_back(pt);
      }
      for (int i = 0; i < LARGE.size(); i++)
      {
        pair<float, float> pt1 = LARGE[i].first;
        pair<float, float> pt2 = LARGE[i].second;
        candidates.push_back(pt1);
        candidates.push_back(pt2);
      }
      for (int i = 0; i < SMALL.size(); i++)
      {
        pair<float, float> pt = SMALL[i].first;
        candidates.push_back(pt);
      }
      return get_upper_bridge(candidates, median);
    }
    return {};
  }

  /**
   * @brief get_lower_bridge function returns us the lower bridge.
   * 
   * @param points It is the list of points from which we have to find the lower bridge.
   * @param median
   * @return pair<pair<float, float>, pair<float, float>> 
   */
  pair<pair<float, float>, pair<float, float>> get_lower_bridge(vector<pair<float, float>> &points, double median)
  {
    sort(points.begin(), points.end(), [](pair<float, float> a, pair<float, float> b)
         { return a.first < b.first; });
    vector<pair<float, float>> candidates;
    vector<pair<pair<float, float>, pair<float, float>>> pairs;
    if (points.size() % 2 == 0)
    {
      for (int i = 0; i < points.size(); i += 2)
      {
        pair<float, float> first_pt = points[i];
        pair<float, float> second_pt = points[i + 1];

        pair<pair<float, float>, pair<float, float>> curr_pair(first_pt, second_pt);
        pairs.push_back(curr_pair);
      }
    }
    else
    {
      candidates.push_back(points[0]);
      for (int i = 1; i < points.size(); i += 2)
      {
        pair<float, float> first_pt = points[i];
        pair<float, float> second_pt = points[i + 1];

        pair<pair<float, float>, pair<float, float>> curr_pair(first_pt, second_pt);
        pairs.push_back(curr_pair);
      }
    }

    int slopes_len = pairs.size();
    vector<double> slopes(slopes_len);
    for (int i = 0; i < pairs.size(); i++)
    {
      pair<float, float> p1 = pairs[i].first;
      pair<float, float> p2 = pairs[i].second;
      double x1 = p1.first;
      double x2 = p2.first;
      double y1 = p1.second;
      double y2 = p2.second;

      if (x1 == x2)
      {
        if (y1 > y2)
        {
          candidates.push_back(p2);
        }
        else
        {
          candidates.push_back(p1);
        }
        slopes[i] = INT_MAX;
      }
      else
      {
        double slope = (y2 - y1) / (x2 - x1);
        slopes[i] = slope;
      }
    }

    vector<double> arr(slopes_len);
    int len = 0;
    for (int i = 0; i < slopes_len; i++)
    {
      if (slopes[i] != INT_MAX)
      {
        arr[len++] = slopes[i];
      }
    }

    double median_slope;
    if (len == 1)
      median_slope = arr[0];
    else
      median_slope = kthSmallest(arr, 0, len - 1, (len + 1) / 2);

    vector<pair<pair<float, float>, pair<float, float>>> SMALL;
    vector<pair<pair<float, float>, pair<float, float>>> EQUAL;
    vector<pair<pair<float, float>, pair<float, float>>> LARGE;

    for (int i = 0; i < pairs.size(); i++)
    {
      pair<float, float> p1 = pairs[i].first;
      pair<float, float> p2 = pairs[i].second;
      double x1 = p1.first;
      double x2 = p2.first;
      double y1 = p1.second;
      double y2 = p2.second;

      if (x1 != x2)
      {
        double slope = (y2 - y1) / (x2 - x1);
        if (abs(slope - median_slope) < 0.001)
        {
          pair<pair<float, float>, pair<float, float>> curr_pair(p1, p2);
          EQUAL.push_back(curr_pair);
        }
        else if (slope < median_slope)
        {
          pair<pair<float, float>, pair<float, float>> curr_pair(p1, p2);
          SMALL.push_back(curr_pair);
        }
        else if (slope > median_slope)
        {
          pair<pair<float, float>, pair<float, float>> curr_pair(p1, p2);
          LARGE.push_back(curr_pair);
        }
      }
    }

    double min_c = INT_MAX;

    for (int i = 0; i < points.size(); i++)
    {
      double x = points[i].first;
      double y = points[i].second;
      double curr_c = (y - median_slope * x);

      if (curr_c < min_c)
      {
        min_c = curr_c;
      }
    }

    pair<float, float> pmin(INT_MAX, INT_MAX);
    pair<float, float> pmax(INT_MIN, INT_MIN);

    for (int i = 0; i < points.size(); i++)
    {
      double x = points[i].first;
      double y = points[i].second;
      double curr_c = (y - median_slope * x);

      if (abs(curr_c - min_c) < 0.001)
      {
        if (x < pmin.first)
        {
          pmin.first = x;
          pmin.second = y;
        }
        if (x > pmax.first)
        {
          pmax.first = x;
          pmax.second = y;
        }
      }
    }

    if (pmin.first <= median && pmax.first > median)
    {
      pair<pair<float, float>, pair<float, float>> lower_bridge(pmin, pmax);
      return lower_bridge;
    }
    else if (pmax.first <= median)
    {
      for (int i = 0; i < EQUAL.size(); i++)
      {
        pair<float, float> pt = EQUAL[i].second;
        candidates.push_back(pt);
      }
      for (int i = 0; i < LARGE.size(); i++)
      {
        pair<float, float> pt1 = LARGE[i].first;
        pair<float, float> pt2 = LARGE[i].second;
        candidates.push_back(pt1);
        candidates.push_back(pt2);
      }
      for (int i = 0; i < SMALL.size(); i++)
      {
        pair<float, float> pt = SMALL[i].second;
        candidates.push_back(pt);
      }
      return get_lower_bridge(candidates, median);
    }
    else if (pmin.first > median)
    {
      for (int i = 0; i < EQUAL.size(); i++)
      {
        pair<float, float> pt = EQUAL[i].first;
        candidates.push_back(pt);
      }
      for (int i = 0; i < LARGE.size(); i++)
      {
        pair<float, float> pt = LARGE[i].first;
        candidates.push_back(pt);
      }
      for (int i = 0; i < SMALL.size(); i++)
      {
        pair<float, float> pt1 = SMALL[i].first;
        pair<float, float> pt2 = SMALL[i].second;
        candidates.push_back(pt1);
        candidates.push_back(pt2);
      }
      return get_lower_bridge(candidates, median);
    }
    return {};
  }

  /**
   * @brief get_upper_hull function returns the list of points that form upper hull.
   * 
   * @param pmin 
   * @param pmax 
   * @param points It is the list of points from which upper hull will be formed.
   * @return vector<pair<float, float>> 
   */
  vector<pair<float, float>> get_upper_hull(pair<float, float> pmin, pair<float, float> pmax, vector<pair<float, float>> &points)
  {
    vector<pair<float, float>> upper_hull;
    int n = points.size();
    vector<double> arr(n);
    for (int i = 0; i < n; i++)
    {
      arr[i] = points[i].first;
    }

    double median;
    if (n == 1)
      median = arr[0];
    else
      median = kthSmallest(arr, 0, n - 1, (n + 1) / 2);
    pair<pair<float, float>, pair<float, float>> upper_bridge = get_upper_bridge(points, median);

    pair<float, float> pl = upper_bridge.first;
    pair<float, float> pr = upper_bridge.second;

    if (pl.first > pr.first)
    {
      pair<float, float> temp(pl);
      pl.first = pr.first;
      pl.second = pr.second;

      pr.first = temp.first;
      pr.second = temp.second;
    }

    upper_hull.push_back(pl);
    upper_hull.push_back(pr);

    if (pmin.first != pl.first || pmin.second != pl.second)
    {
      vector<pair<float, float>> upper_T_left = get_T(pmin, pl, points, false);
      vector<pair<float, float>> left = get_upper_hull(pmin, pl, upper_T_left);

      for (int i = 0; i < left.size(); i++)
      {
        pair<float, float> temp = left[i];
        upper_hull.push_back(temp);
      }
    }

    if (pmax.first != pr.first || pmax.second != pr.second)
    {
      vector<pair<float, float>> upper_T_right = get_T(pr, pmax, points, false);
      vector<pair<float, float>> right = get_upper_hull(pr, pmax, upper_T_right);

      for (int i = 0; i < right.size(); i++)
      {
        pair<float, float> temp = right[i];
        upper_hull.push_back(temp);
      }
    }

    return upper_hull;
  }

  /**
   * @brief get_lower_hull function returns the list of points that form lower hull.
   * 
   * @param pmin 
   * @param pmax 
   * @param points It is the list of points from which lower hull will be formed.
   * @return vector<pair<float, float>> 
   */
  vector<pair<float, float>> get_lower_hull(pair<float, float> pmin, pair<float, float> pmax, vector<pair<float, float>> &points)
  {
    vector<pair<float, float>> lower_hull;
    int n = points.size();
    vector<double> arr(n);
    for (int i = 0; i < n; i++)
    {
      arr[i] = points[i].first;
    }
    double median;
    if (n == 1)
      median = arr[0];
    else
      median = kthSmallest(arr, 0, n - 1, (n + 1) / 2);
    pair<pair<float, float>, pair<float, float>> lower_bridge = get_lower_bridge(points, median);
    pair<float, float> pl = lower_bridge.first;
    pair<float, float> pr = lower_bridge.second;

    if (pl.first > pr.first)
    {
      pair<float, float> temp(pl);
      pl.first = pr.first;
      pl.second = pr.second;
      pr.first = temp.first;
      pr.second = temp.second;
    }

    lower_hull.push_back(pl);
    lower_hull.push_back(pr);

    if (pmin.first != pl.first || pmin.second != pmin.second)
    {
      vector<pair<float, float>> lower_T_left = get_T(pmin, pl, points, true);
      vector<pair<float, float>> left = get_lower_hull(pmin, pl, lower_T_left);
      for (int i = 0; i < left.size(); i++)
      {
        pair<float, float> temp = left[i];
        lower_hull.push_back(temp);
      }
    }
    if (pmax.first != pr.first || pmax.second != pr.second)
    {
      vector<pair<float, float>> lower_T_right = get_T(pr, pmax, points, true);
      vector<pair<float, float>> right = get_lower_hull(pr, pmax, lower_T_right);

      for (int i = 0; i < right.size(); i++)
      {
        pair<float, float> temp = right[i];
        lower_hull.push_back(temp);
      }
    }

    return lower_hull;
  }

public:
  /**
   * @brief Construct a new KPS object
   * 
   */
  KPS()
  {
    points = vector<pair<float, float>>();
  }

  /**
   * @brief fit_set function accepts a list of  points to make the dataset for finding the convex hull.
   * 
   * @param list 
   */
  void fit_set(vector<pair<float, float>> list)
  {
    points = list;
  }

  /**
   * @brief add_point function is used to add point in the dataset.
   * 
   * @param pt 
   */
  void add_point(pair<float, float> pt)
  {
    points.push_back(pt);
  }
  
  /**
   * @brief compute_hull returns the list of points in the order they form the complete convex hull
   * 
   * @return vector<pair<float, float>> 
   */
  vector<pair<float, float>> compute_hull()
  {
    if (points.size() < 3)
    {
      cout << "Hull doesn't exist!!" << endl;
      exit(0);
    }

    pair<float, float> pmin_u, pmin_l, pmax_u, pmax_l;

    pmin_l = pmin_u = pmax_u = pmax_l = points[0];
    for (int i = 1; i < points.size(); i++)
    {
      pair<float, float> curr_point = points[i];
      if (curr_point.first < pmin_l.first)
      {
        pmin_l = curr_point;
        pmin_u = curr_point;
      }
      else if (curr_point.first > pmax_l.first)
      {
        pmax_l = curr_point;
        pmax_u = curr_point;
      }
      else if (curr_point.first == pmin_l.first)
      {
        if (curr_point.second > pmin_u.second)
        {
          pmin_u = curr_point;
        }
        else if (curr_point.second < pmin_l.second)
        {
          pmin_l = curr_point;
        }
      }
      else if (curr_point.first == pmax_l.first)
      {
        if (curr_point.second > pmax_u.second)
        {
          pmax_u = curr_point;
        }
        else if (curr_point.second < pmax_l.second)
        {
          pmax_l = curr_point;
        }
      }
    }

    vector<pair<float, float>> upper_T = get_T(pmin_u, pmax_u, points, false);
    vector<pair<float, float>> upper_hull = get_upper_hull(pmin_u, pmax_u, upper_T);

    vector<pair<float, float>> lower_T = get_T(pmin_l, pmax_l, points, true);
    vector<pair<float, float>> lower_hull = get_lower_hull(pmin_l, pmax_l, lower_T);

    vector<pair<float, float>> hull_edges;
    for (int i = 0; i < upper_hull.size(); i++)
    {
      pair<float, float> temp = upper_hull[i];
      hull_edges.push_back(temp);
    }
    for (int i = 0; i < lower_hull.size(); i++)
    {
      pair<float, float> temp = lower_hull[i];
      hull_edges.push_back(temp);
    }

    if (pmin_u.first != pmin_l.first || pmin_u.second != pmin_l.second)
    {
      hull_edges.push_back(pmin_l);
      hull_edges.push_back(pmin_u);
    }
    if (pmax_l.first != pmax_u.first || pmax_l.second != pmax_u.second)
    {
      hull_edges.push_back(pmax_l);
      hull_edges.push_back(pmax_u);
    }

    make_result(hull_edges, "results/results_1.csv");

    sort(hull_edges.begin(), hull_edges.end(), [](pair<float, float> a, pair<float, float> b)
         {if(a.first < b.first) return true;else if(a.first > b.first)
      return false;else return a.second < b.second; });

    vector<pair<float, float>> hull;
    hull.push_back(hull_edges[0]);
    int i = 1;
    while (i < hull_edges.size())
    {
      while (i < hull_edges.size() && (hull_edges[i].first == hull_edges[i - 1].first && hull_edges[i].second == hull_edges[i - 1].second))
        i++;

      if (i < hull_edges.size())
        hull.push_back(hull_edges[i]);

      i++;
    }

    return hull;
  }
};

#endif