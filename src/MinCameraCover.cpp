#include <ftl/ftl.h>

using namespace std;
using namespace ftl::serialization;

using TreeNode = ftl::TreeNode<int>;

class Solution
{
public:
    /**
    https://leetcode.com/contest/weekly-contest-117/problems/numbers-with-same-consecutive-differences/

967. Numbers With Same Consecutive Differences
Return all non-negative integers of length N such that the absolute difference between every two consecutive digits is K.

Note that every number in the answer must not have leading zeros except for the number 0 itself. For example, 01 has one leading zero and is invalid,
but 0 is valid.

You may return the answer in any order.
    ***/

    // x: current value,
    // m: 10**y where y==N, number of digits to be appended.
    //   if N == 0, push x into x and return
    void numsSameDiff( vector<int> &s, int N, int K, int x, int m )
    {
        if ( N == 0 )
        {
            if ( x )
                s.push_back( x );
            return;
        }
        auto D0 = ( x % ( m * 10 ) ) / 10;

        auto D = D0 + K * ( m / 10 );
        if ( D > 9 * ( m / 10 ) )
            D -= m;

        numsSameDiff( s, N - 1, K, x + D, m / 10 );

        D = D0 - K * ( m / 10 );
        if ( D < 0 )
            D += m;

        numsSameDiff( s, N - 1, K, x + D, m / 10 );
    }
    vector<int> numsSameConsecDiff( int N, int K )
    {
        return {};
    }

    // output dist: dist of nearest child installed camera. 0, if root is installed.
    int postOrder( TreeNode *root, int &minDist, int &maxDist )
    {
        if ( !root )
            return 0;
        const auto INF = numeric_limits<int>::max();
        minDist = INF;
        maxDist = numeric_limits<int>::min();
        int childDist[4] = {INF, INF, INF, INF};
        int ncams = 0;
        ncams += postOrder( root->left, childDist[0], childDist[1] );
        ncams += postOrder( root->right, childDist[2], childDist[3] );
        for ( int i = 0; i < 2; ++i )
        {
            int d0 = childDist[i * 2], d1 = childDist[i * 2 + 1];
            if ( d1 == 2 )
            { // d0 == 1, 2
                maxDist = minDist = 0;
                return ncams + 1;
            }
            if ( d0 == INF )
            { // no child on this side
                maxDist = max( maxDist, 2 );
                minDist = min( minDist, 2 );
            }
            else
            { // (d0, d1) = (1,1), (0,0)
                maxDist = max( maxDist, d1 + 1 );
                minDist = min( minDist, d0 + 1 );
            }
        }
        return ncams;
    }
    int minCameraCover( TreeNode *root )
    {
        unordered_map<TreeNode *, bool> cam;
        int mindist = INT_MAX, maxdist = INT_MAX;
        auto res = postOrder( root, mindist, maxdist );
        if ( mindist == 2 )
            res += 1; // install at root
        return res;
    }
};

TEST_FUNC( minCameraCover_tests )
{
    const auto null = INT_MAX;
    Solution sln;

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
