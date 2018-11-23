#include <ftl/catch_or_ignore.h>

#include <algorithm>
#include <bitset>
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
    // https://leetcode.com/contest/weekly-contest-110/problems/distinct-subsequences-ii/
    int distinctSubseqII(string S)
    {
        using CharPosMap = unordered_map<char, const char*>;
        int64_t res = 0; // empty
        const int64_t MOD = 1000000000 + 7;
        CharPosMap pos; // { 'a': pos, 'b':' pos2}
        const char* pS = S.c_str();
        // init with length = 1
        for (char c = 'a'; c <= 'z'; ++c) {
            if (auto p = strchr(pS, c)) {
                pos[c] = p;
            }
        }
        res += pos.size();
        // loop for length i
        while (!pos.empty()) {
            CharPosMap tp;
            for (auto& chpos : pos) {
                auto prevPos = chpos.second + 1;
                for (char c = 'a'; c <= 'z' && *prevPos; ++c) {
                    if (auto p = strchr(prevPos, c)) {
                        ++res;
                        res %= MOD;
                        tp[c] = tp.count(c) ? min(tp[c], p) : p;
                    }
                }
            }
            pos.swap(tp);
        }
        return res;
    }
};

TEST_FUNC(distinctSubsequncesII_tests)
{
    Solution sln;
    {
        REQUIRE(sln.distinctSubseqII("abc") == 7);
    }
    {
        REQUIRE(sln.distinctSubseqII("aba") == 6);
    }
    {
        REQUIRE(sln.distinctSubseqII("a") == 1);
        REQUIRE(sln.distinctSubseqII("aaa") == 3);
    }
}
