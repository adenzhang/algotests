#include <ftl/catch_or_ignore.h>

#include <algorithm>
#include <bitset>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace ftl::serialization;

typedef vector<int> IntVec;
typedef vector<vector<int>> IntVecVec;
typedef vector<string> StrVec;
typedef unordered_map<int, int> IntIntHashMap;
typedef map<int, int> IntIntTreeMap;

class Solution {
public:
    // https://leetcode.com/problems/smallest-range/
    vector<int> smallestRange(vector<vector<int>>& nums)
    {
        struct Elem {
            size_t iVec;
            size_t idx;
        };
        const auto greaterVal = [&](const Elem& i, const Elem& j) {
            return nums[i.iVec][i.idx] > nums[j.iVec][j.idx];
        };
        priority_queue<Elem, deque<Elem>, decltype(greaterVal)> q(greaterVal); // min queue
        const auto INF = numeric_limits<size_t>::max();
        size_t minWidth = INF;
        Elem startp, endp; // record
        const auto width = [&](const Elem& i, const Elem& j) -> size_t {
            return nums[j.iVec][j.idx] - nums[i.iVec][i.idx]; // i.value < j.value
        };

        deque<Elem> window;
        vector<size_t> countForVec(nums.size(), 0); // elements of each vetor in window
        size_t nVecInWindow = 0; // count of vectors having elements in q
        for (size_t i = 0; i < nums.size(); ++i) { // init q
            q.push(Elem{ i, 0 });
        }
        while (!q.empty()) {
            auto e = q.top();
            q.pop();
            if (nums[e.iVec].size() > e.idx + 1) {
                q.push(Elem{ e.iVec, e.idx + 1 }); // advance
            }
            window.push_back(e);

            if (++countForVec[e.iVec] == 1) {
                ++nVecInWindow;
            }
            while (countForVec[window.front().iVec] > 1) {
                --countForVec[window.front().iVec];
                window.pop_front();
            }
            if (nVecInWindow == nums.size()) {
                if (auto w = width(window.front(), window.back()); w < minWidth) {
                    startp = window.front();
                    endp = window.back();
                    minWidth = w;
                }
            }
        }
        return minWidth == INF ? vector<int>{} : vector<int>{ nums[startp.iVec][startp.idx], nums[endp.iVec][endp.idx] };
    }
};
TEST_FUNC(SmallestRange_tests)
{
    Solution sln;
    {
        vector<vector<int>> A = { { 4, 10, 15, 24, 26 }, { 0, 9, 12, 20 }, { 5, 18, 22, 30 } };
        vector<int> res{ 20, 24 };
        REQUIRE(res == sln.smallestRange(A));
    }
}
