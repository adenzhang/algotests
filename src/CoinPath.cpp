#include <ftl/ftl.h>

using namespace std;
using namespace ftl::serialization;

/*********
Given an array A (index starts at 1) consisting of N integers: A1, A2, ..., AN and an integer B. The integer Bdenotes that from any place (suppose
the index is i) in the array A, you can jump to any one of the place in the array A indexed i+1, i+2, …, i+B if this place can be jumped to. Also, if
you step on the index i, you have to pay Ai coins. If Ai is -1, it means you can’t jump to the place indexed i in the array.

Now, you start from the place indexed 1 in the array A, and your aim is to reach the place indexed N using the minimum coins. You need to return the
path of indexes (starting from 1 to N) in the array you should take to get to the place indexed N using minimum coins.

If there are multiple paths with the same cost, return the lexicographically smallest such path.

If it's not possible to reach the place indexed N then you need to return an empty array.

Example 1:

Input: [1,2,4,-1,2], 2
Output: [1,3,5]


Example 2:

Input: [1,2,4,-1,2], 1
Output: []


Note:

Path Pa1, Pa2, ..., Pan is lexicographically smaller than Pb1, Pb2, ..., Pbm, if and only if at the first i where Pai and Pbi differ, Pai < Pbi; when
no such i exists, then n < m. A1 >= 0. A2, ..., AN (if exist) will in the range of [-1, 100]. Length of A is in the range of [1, 1000]. B is in the
range of [1, 100].

  **********/
// 656. Coin Path
// https://leetfree.com/problems/coin-path.html
// jump from to N-1 with min cost, each jump distance <= B
// O(n)
struct Solution
{
    vector<int> coinPath( vector<int> &A, unsigned B )
    {
        const unsigned N = A.size();
        vector<unsigned> D( N, INT_MAX ), prePos( N, INT_MAX ); // min cost jump to i, and previous position
        D[0] = 0; // D[i] = min{ D[i-k], k=1..B }
        deque<unsigned> minPosQ; // increasing q to keep indice of min values in D
        minPosQ.push_back( 0 );
        for ( unsigned i = 1; i < N; ++i )
        {
            if ( A[i] != -1 )
            {
                if ( minPosQ.empty() )
                    return {}; // unable to jump forward
                unsigned y = D[minPosQ.front()] + A[i]; // front is min. current D
                D[i] = y;
                prePos[i] = minPosQ.front();

                while ( !minPosQ.empty() && D[minPosQ.back()] > y )
                    minPosQ.pop_back();
                minPosQ.push_back( i );
            }
            if ( !minPosQ.empty() && i - minPosQ.front() + 1 > B ) // ensure window size max B
                minPosQ.pop_front();
        }
        if ( D.back() == INT_MAX )
            return {};
        else
        {
            vector<int> res;
            for ( unsigned i = N - 1;; i = prePos[i] )
            {
                res.push_back( i + 1 );
                if ( i == 0 )
                    break;
            }
            std::reverse( res.begin(), res.end() );
            return res;
        }
    }
};
TEST_FUNC( coinPath_tests )
{
    Solution sln;
    {
        vector<int> A = {1, 2, 4, -1, 2}, C = {1, 3, 5};
        REQUIRE( C == sln.coinPath( A, 2 ) );
    }
    {
        vector<int> A = {1, 2, 4, -1, 2}, C = {};
        REQUIRE( C == sln.coinPath( A, 1 ) );
    }
}
