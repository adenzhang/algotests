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

// https://leetcode.com/problems/swim-in-rising-water/description/
class Solution {
public:
    // find the path with min max value
    int swimInWater(vector<vector<int>>& grid)
    {
        using Pos = std::pair<int, int>;
        const size_t N = grid.size();
        auto posLess = [&](const Pos& a, const Pos& b) { return grid[a.first][a.second] > grid[b.first][b.second]; };
        priority_queue<Pos, deque<Pos>, decltype(posLess)> adj(posLess);
        vector<bool> visited(N * N, false);
        adj.push(Pos{ 0, 0 });
        int maxV = grid[0][0];
        while (!adj.empty()) {
            auto res = adj.top(); // find the adjencency having min value
            int r = res.first, c = res.second;
            adj.pop();
            if (grid[r][c] > maxV)
                maxV = grid[r][c];
            auto foundIt = [&](int r1, int c1) {
                if (c1 >= 0 && c1 < N && r1 >= 0 && r1 < N) {
                    if (visited[r1 * N + c1])
                        return false;
                    visited[r1 * N + c1] = true;
                    if (r1 == N - 1 && c1 == N - 1) {
                        if (grid[N - 1][N - 1] > maxV)
                            maxV = grid[N - 1][N - 1];
                        return true;
                    }
                    adj.push(Pos{ r1, c1 });
                }
                return false;
            };
            if (foundIt(r, c + 1))
                break; // right
            if (foundIt(r - 1, c))
                break; // up
            if (foundIt(r, c - 1))
                break; // right
            if (foundIt(r + 1, c))
                break; // down
        }
        return maxV;
    }
};
TEST_FUNC(swimInWater_tests)
{
    Solution sln;
    {
        vector<vector<int>> A = { { 0, 2 }, { 1, 3 } };
        REQUIRE(3 == sln.swimInWater(A));
    }
    {
        vector<vector<int>> A = { { 0, 1, 2, 3, 4 }, { 24, 23, 22, 21, 5 }, { 12, 13, 14, 15, 16 }, { 11, 17, 18, 19, 20 }, { 10, 9, 8, 7, 6 } };
        REQUIRE(16 == sln.swimInWater(A));
    }
}
