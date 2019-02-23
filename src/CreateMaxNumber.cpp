#include <ftl/ftl.h>

using namespace std;
using namespace ftl::serialization;

/**
 * @brief The Solution class
Given two arrays of length m and n with digits 0-9 representing two numbers. Create the maximum number of length k <= m + n from digits of the two. The relative order of the digits from the same array must be preserved. Return an array of the k digits.

Note: You should try to optimize your time and space complexity.
https://leetcode.com/problems/create-maximum-number/
 */
class Solution {
public:
    vector<int> maxNumber(vector<int>& A, vector<int>& B, int K)
    {
        const int N = A.size(), M = B.size();
        vector<int> C;
        C.reserve(K);
        for (int i = 0, j = 0; K > 0; --K) {
            int i1 = N - (K - M + j) + 1; // end index of window
            int j1 = M - (K - N + i) + 1;
            i1 = std::min(N, i1);
            j1 = std::min(M, j1);
            auto pMaxI = std::max_element(A.begin() + i, A.begin() + i1);
            auto pMaxJ = std::max_element(B.begin() + j, B.begin() + j1);
            bool bUseI = false;
            if (pMaxI == A.begin() + i1) {
                bUseI = false;
            } else if (pMaxJ == B.begin() + j1) {
                bUseI = true;
            } else {
                bUseI = *pMaxI > *pMaxJ; // todo : if equals
            }

            if (bUseI) {
                C.push_back(*pMaxI);
                i = std::distance(A.begin(), pMaxI) + 1;
            } else {
                C.push_back(*pMaxJ);
                j = std::distance(B.begin(), pMaxJ) + 1;
            }
        }
        return C;
    }
};

TEST_FUNC(maxNumber_tests)
{
    Solution sln;
    //    {
    //        vector<int> A = { 3, 4, 6, 5 };
    //        vector<int> B = { 9, 1, 2, 5, 8, 3 };
    //        vector<int> C = { 9, 8, 6, 5, 3 };
    //        REQUIRE(C == sln.maxNumber(A, B, 5));
    //    }
    {
        vector<int> A = { 6, 7 };
        vector<int> B = { 6, 0, 4 };
        vector<int> C = { 6, 7, 6, 0, 4 };
        //        REQUIRE(C == sln.maxNumber(A, B, 5));
    }
}
