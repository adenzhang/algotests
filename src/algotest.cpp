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
    /**
    https://leetcode.com/contest/weekly-contest-117/problems/numbers-with-same-consecutive-differences/

967. Numbers With Same Consecutive Differences
User Accepted: 1313
User Tried: 1535
Total Accepted: 1329
Total Submissions: 4743
Difficulty: Medium
Return all non-negative integers of length N such that the absolute difference between every two consecutive digits is K.

Note that every number in the answer must not have leading zeros except for the number 0 itself. For example, 01 has one leading zero and is invalid, but 0 is valid.

You may return the answer in any order.
    ***/

    // x: current value,
    // m: 10**y where y==N, number of digits to be appended.
    //   if N == 0, push x into x and return
    void numsSameDiff(vector<int>& s, int N, int K, int x, int m)
    {
        if (N == 0) {
            if (x)
                s.push_back(x);
            return;
        }
        auto D0 = (x % (m * 10)) / 10;

        auto D = D0 + K * (m / 10);
        if (D > 9 * (m / 10))
            D -= m;

        numsSameDiff(s, N - 1, K, x + D, m / 10);

        D = D0 - K * (m / 10);
        if (D < 0)
            D += m;

        numsSameDiff(s, N - 1, K, x + D, m / 10);
    }
    vector<int> numsSameConsecDiff(int N, int K)
    {
        return {};
    }

    // output dist: dist of nearest child installed camera. 0, if root is installed.
    int postOrder(TreeNode* root, int& minDist, int& maxDist)
    {
        if (!root)
            return 0;
        const auto INF = numeric_limits<int>::max();
        minDist = INF;
        maxDist = numeric_limits<int>::min();
        int childDist[4] = { INF, INF, INF, INF };
        int ncams = 0;
        ncams += postOrder(root->left, childDist[0], childDist[1]);
        ncams += postOrder(root->right, childDist[2], childDist[3]);
        for (int i = 0; i < 2; ++i) {
            int d0 = childDist[i * 2], d1 = childDist[i * 2 + 1];
            if (d1 == 2) { // d0 == 1, 2
                maxDist = minDist = 0;
                return ncams + 1;
            }
            if (d0 == INF) { // no child on this side
                maxDist = max(maxDist, 2);
                minDist = min(minDist, 2);
            } else { // (d0, d1) = (1,1), (0,0)
                maxDist = max(maxDist, d1 + 1);
                minDist = min(minDist, d0 + 1);
            }
        }
        return ncams;
    }
    int minCameraCover(TreeNode* root)
    {
        unordered_map<TreeNode*, bool> cam;
        int mindist = INT_MAX, maxdist = INT_MAX;
        auto res = postOrder(root, mindist, maxdist);
        if (mindist == 2)
            res += 1; // install at root
        return res;
    }

    // find max n number among sums of A[i]+B[j], n == A.size() == B.size()
    vector<int> max_n_sums(vector<int>& A, vector<int>& B)
    {
        std::sort(A.begin(), A.end(), std::greater<int>());
        std::sort(B.begin(), B.end(), std::greater<int>());
        vector<int> C;
        for (auto itA = A.begin(); itA != A.end(); ++itA) {
            auto nextA = next(itA);
            auto x = nextA == A.end() ? 0 : (*nextA + B.front());
            for (auto itB = B.begin(); itB != B.end(); ++itB) {
                auto y = *itA + *itB;
                if (nextA != A.end() && y < x)
                    break;
                C.push_back(y);
                if (C.size() == B.size())
                    return C;
            }
        }
    }

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

TEST_FUNC(tmp_tests)
{
    const auto null = INT_MAX;
    Solution sln;
    {
        vector<int> A = { 12, 34, 67, 90 };
        REQUIRE(113 == sln.allocate_books(A, 2));
    }

    {
        vector<int> A = { 31, 14, 19, 75 };
        REQUIRE(-1 == sln.allocate_books(A, 12));
    }

    //    {
    //        vector<int> A = { 36, 27, -35, 43, -15, 36, 42, -1, -29, 12, -23, 40, 9, 13, -24, -10, -24, 22, -14, -39, 18, 17, -21, 32, -20, 12, -27, 17, -15, -21, -48, -28, 8, 19, 17, 43, 6, -39, -8, -21, 23, -29, -31, 34, -13, 48, -26, -35, 20, -37, -24, 41, 30, 6, 23, 12, 20, 46, 31, -45, -25, 34, -23, -14, -45, -4, -21, -37, 7, -26, 45, 32, -5, -36, 17, -16, 14, -7, 0, 37, -42, 26, 28 };
    //        vector<int> B = { 38, 34, -47, 1, 4, 49, -18, 10, 26, 18, -11, -38, -24, 36, 44, -11, 45, 20, -16, 28, 17, -49, 47, -48, -33, 42, 2, 6, -49, 30, 36, -9, 15, 39, -6, -31, -10, -21, -19, -33, 47, 21, 31, 25, -41, -23, 17, 6, 47, 3, 36, 15, -44, 33, -31, -26, -22, 21, -18, -21, -47, -31, 20, 18, -42, -35, -10, -1, 46, -27, -32, -5, -4, 1, -29, 5, 29, 38, 14, -22, -9, 0, 43 };
    //        vector<int> C;
    //        std::stringstream ss;
    //        ss << "97 95 95 95 95 94 94 93 93 93 93 92 92 92 92 92 92 92 91 91 91 91 90 90 90 90 90 90 90 90 90 90 89 89 89 89 89 89 89 89 88 88 88 88 88 88 88 88 87 87 87 87 87 87 87 87 87 86 86 86 86 86 86 86 86 85 85 85 85 85 85 85 85 84 84 84 84 84 84 84 84 84 84";
    //        readiterator(ss, back_inserter(C), ' ', nullptr);
    //        REQUIRE(C == sln.max_n_sums(A, B));
    //    }

    //    {
    //        IntVec v = { 1, 2, null, 3, 4 };
    //        auto pRoot = ftl::create_tree(v.begin(), v.end(), null);

    //        REQUIRE(1 == sln.minCameraCover(pRoot));
    //        delete pRoot;
    //    }
    //    {
    //        IntVec v = { 0, 0, null, 0, null, 0, null, null, 0 };
    //        auto pRoot = ftl::create_tree(v.begin(), v.end(), null);

    //        REQUIRE(2 == sln.minCameraCover(pRoot));
    //        delete pRoot;
    //    }

    //    {
    //        IntVec v = { 1, 2, null, null, 3, 4, null, null, 5, 6 };
    //        auto pRoot = ftl::create_tree(v.begin(), v.end(), null);

    //        REQUIRE(2 == sln.minCameraCover(pRoot));
    //        delete pRoot;
    //    }
}
