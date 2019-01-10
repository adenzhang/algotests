#include <ftl/catch_or_ignore.h>

#include <algorithm>
#include <bitset>
#include <cassert>
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
    int leastOpsExpressTarget(int x, int T)
    {

        return 0;
    }
};
TEST_FUNC(tmp_tests)
{
    Solution sln;
    {
        REQUIRE(50 == sln.leastOpsExpressTarget(2, 125046)); // 100 * 100 * 100 * 100
    }
    {
        REQUIRE(17 == sln.leastOpsExpressTarget(3, 365)); // 100 * 100 * 100 * 100
    }
}
