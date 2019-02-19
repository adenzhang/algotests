#include <ftl/ftl.h>

using namespace std;

using namespace ftl::serialization;

typedef vector<int> IntVec;
typedef vector<vector<int>> IntVecVec;
typedef vector<string> StrVec;
typedef unordered_map<int, int> IntIntHashMap;
typedef map<int, int> IntIntTreeMap;

using TreeNode = ftl::TreeNode<int>;

class Solution {
public:
    int gcd(int A, int B)
    {
        int x = std::min(A, B), y = std::max(A, B);
        while (x != 0) {
            auto t = x;
            x = y % x;
            y = t;
        }
        return y;
    }
    size_t lcm(size_t A, size_t B)
    {
        size_t c = gcd(A, B);
        return c * (A / c) * (B / c);
    }
    int nthMagicalNumber(int N, int A, int B)
    {
        const size_t MOD = 1000000000 + 7;
        if (A == B) {
            return size_t(A) * N % MOD;
        }
        size_t C = lcm(A, B);
        size_t M = C / A + C / B - 1;
        size_t res = C * (N / M);
        N %= M;
        for (size_t prevB = res, prevA = res; N > 0; --N) { // previous value that A/B was added
            if (prevB + B < prevA + A) {
                res = prevB = (prevB + B); //% MOD;
            } else {
                res = prevA = (prevA + A); // % MOD;
            }
        }
        return res % MOD;
    }
};
TEST_FUNC(nthMagicalNumber_tests)
{
    Solution sln;
    REQUIRE(2 == sln.nthMagicalNumber(1, 2, 3));
    REQUIRE(6 == sln.nthMagicalNumber(4, 2, 3));
    REQUIRE(999720007 == sln.nthMagicalNumber(1000000000, 40000, 40000));
    REQUIRE(8 == sln.nthMagicalNumber(3, 6, 4));
    REQUIRE(8 == sln.nthMagicalNumber(3, 8, 3));
    REQUIRE(28 == sln.nthMagicalNumber(10, 9, 4));

    REQUIRE(999860007 == sln.nthMagicalNumber(1000000000, 39999, 40000));
}
