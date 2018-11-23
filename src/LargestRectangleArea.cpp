
/*

  Given n non-negative integers representing the histogram's bar height where the width of each bar is 1, find the area of largest rectangle in the histogram.

For example,
Given height = [2,1,5,6,2,3],
return 10.

*/
#include <ftl/catch_or_ignore.h>

#include <algorithm>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <fstream>
#include <ftl/container_serialization.h>
#include <ftl/indexed_tree.h>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
using namespace ftl::serialization;

typedef vector<int> IntVector;
typedef vector<vector<int>> IntVecVec;
typedef vector<string> StrVec;
typedef unordered_map<int, int> IntIntHashMap;
typedef map<int, int> IntIntTreeMap;

class Solution {
public:
    typedef vector<int> IntVector;

    void popToUpperBound(IntVector& v, int bound)
    {
        while (v.size() > 1 && v.back() >= bound && v[v.size() - 1] >= bound) {
            v.pop_back();
        }
    }

    int largestRectangleArea0(IntVector& height)
    {
        IntVector iTroughs;
        iTroughs.reserve(height.size());
        iTroughs.push_back(height[0]);
        int area = 0;
        int asc = 0; // 1 for ascending; 0 for descending;
        for (int i = 1; i < height.size(); ++i) {
            int h = height[i];
            if (h > height[i - 1]) {
                iTroughs.push_back(i);
                asc = 1;
            } else if (h < height[i - 1] || (h == height[i - 1] && !asc)) {
                popToUpperBound(iTroughs, h);
                int k = iTroughs.back();
                int a = (i - k) * height[k];
                // todo
                asc = 0;
            } else {
                // todo
            }
        }
    }
    int largestRectangleArea1(vector<int>& heights)
    {
        if (heights.size() == 0)
            return 0;
        int imax = heights[0];
        stack<size_t> q; // index stack: increasing que
        q.push(0);
        heights.push_back(0); // sentinel
        for (size_t i = 1; i < heights.size(); ++i) {
            int x = heights[i];
            if (x > heights[i - 1]) { // increasing
                q.push(i);
            } else if (x < heights[i - 1]) { // descreasing
                //- heights[q.top()] is the rectangle top line
                //  left: left side of q.top() bar
                //  right: left side of current bar
                while (!q.empty() && heights[q.top()] > x) {
                    int a = (i - q.top()) * heights[q.top()];
                    if (a > imax)
                        imax = a;
                    q.pop();
                }
                //- current is the rectangle top line
                // left: right sidef of q.top() bar
                // right: right sidef of current bar
                int leftIdx = q.empty() ? (-1) : q.top();
                int a = (i - leftIdx) * x;
                if (a > imax)
                    imax = a;
                q.push(i);
            }
        }
        return imax;
    }
    int largestRectangleArea(vector<int>& height)
    {
        int ret = 0;
        height.push_back(0);
        vector<int> index;

        for (int i = 0; i < height.size(); i++) {
            while (index.size() > 0 && height[index.back()] >= height[i]) {
                int h = height[index.back()];
                index.pop_back();

                int sidx = index.size() > 0 ? index.back() : -1;
                if (h * (i - sidx - 1) > ret)
                    ret = h * (i - sidx - 1);
            }
            index.push_back(i);
        }

        return ret;
    }

    /// https://www.hackerrank.com/challenges/crush/problem
    // Complete the arrayManipulation function below.
    long arrayManipulation(int n, vector<vector<int>> queries)
    {
        struct endpoint {
            size_t idx;
            int bStart; // 1: start; -1, end
        };
        const auto& getPos = [&](const endpoint& x) {
            return x.bStart == 1 ? queries[x.idx][0] : (queries[x.idx][1] + 1);
        };
        const auto& getVal = [&](const endpoint& x) {
            return queries[x.idx][2] * x.bStart;
        };
        const auto& cmp = [&](const endpoint& x, const endpoint& y) {
            return getPos(x) < getPos(y);
        };
        const size_t N = queries.size();
        vector<endpoint> q; // sort by interval endpoint positions
        for (size_t i = 0; i < N; ++i) {
            q.push_back(endpoint{ i, 1 });
            q.push_back(endpoint{ i, -1 });
        }
        sort(q.begin(), q.end(), cmp);

        long v = 0; // current value
        long imax = 0;
        for (size_t i = 0; i < q.size();) {
            auto pos = getPos(q[i]);
            for (; pos == getPos(q[i]) && i < q.size(); ++i) {
                v += getVal(q[i]);
            }
            imax = max(imax, v);
        }
        return imax;
    }

    /// https://www.hackerrank.com/challenges/morgan-and-a-string/problem
    string morganAndString(string a, string b)
    {
        const auto& nextDiff = [](const char* p) { auto n=p; while( *n && *p == *n) ++n;return n; };
        string s;
        s.reserve(a.size() + b.size() + 1);
        for (auto p = a.c_str(), q = b.c_str(); *q || *p;) {
            if (*p == *q) {
                // look ahead next char
                auto np = nextDiff(p), nq = nextDiff(q);
                if (*np <= *nq) {
                    for (; p != np; ++p)
                        s += *p;
                } else {
                    for (; q != nq; ++q)
                        s += *q;
                }
            } else if (*p && *p < *q || !(*q)) {
                s += *p;
                ++p;
            } else {
                s += *q;
                ++q;
            }
        }
        return s;
    }
};

TEST_FUNC(morganAndString_tests)
{
    Solution sln;
    {
        REQUIRE(sln.morganAndString("ACA", "BCF") == "ABCACF");
    }
    {
        REQUIRE(sln.morganAndString("JACK", "DANIEL") == "DAJACKNIEL");
    }
    {
        REQUIRE(sln.morganAndString("ABACABA", "ABACABA") == "AABABACABACABA");
    }
}

TEST_FUNC(largestRectangleArea_tests)
{
    Solution sln;
    {
        IntVector A{ 2, 1, 5, 6, 2, 3 };
        REQUIRE(sln.largestRectangleArea(A) == 10);
    }
    {
        IntVector A{ 1, 1 };
        REQUIRE(sln.largestRectangleArea(A) == 2);
    }
    {
        IntVector A{ 2, 1, 2 };
        REQUIRE(sln.largestRectangleArea(A) == 3);
    }
}

TEST_FUNC(arrayManipulation_tests)
{
    Solution sln;
    {
        IntVecVec A{ { 1, 5, 3 },
            { 4, 8, 7 },
            { 6, 9, 1 } };
        REQUIRE(sln.arrayManipulation(0, A) == 10);
    }
    {
        IntVecVec A{
            { 1, 2, 100 },
            { 2, 5, 100 },
            { 3, 4, 100 }
        };
        REQUIRE(sln.arrayManipulation(0, A) == 200);
    }
    {
        IntVecVec A{ { 2, 3, 603 },
            { 1, 1, 286 },
            { 4, 4, 882 } };
        REQUIRE(sln.arrayManipulation(0, A) == 882);
    }
}
