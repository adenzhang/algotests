#include <ftl/ftl.h>

using namespace std;
using namespace ftl::serialization;

struct Solution {
    // https://www.interviewbit.com/problems/allocate-books/
    // divide A into B subarrays in a way the max sum of all subarrays has min value
    int allocate_books(vector<int>& A, int B)
    {
        if (A.size() < B)
            return -1;
        int sum = accumulate(A.begin(), A.end(), 0, plus<int>());
        auto cond = [&](int maxSum) -> bool {
            int n = 0;
            int s = 0;
            for (const auto& e : A) {
                if (e > maxSum)
                    return false;
                s += e;
                if (s > maxSum) {
                    if (++n == B)
                        return false;
                    s = e;
                }
            }
            return true;
        };
        int s0 = 0, s1 = sum + 1;
        while (s0 + 1 < s1) {
            int s = (s0 + s1 + 1) / 2;
            if (cond(s))
                s1 = s;
            else
                s0 = s;
        }
        return s1;
    }
};

TEST_FUNC(allocate_books_tests)
{
    Solution sln;
    {
        vector<int> A = { 12, 34, 67, 90 };
        REQUIRE(113 == sln.allocate_books(A, 2));
    }

    {
        vector<int> A = { 31, 14, 19, 75 };
        REQUIRE(-1 == sln.allocate_books(A, 12));
    }
}
