#include <ftl/catch_or_ignore.h>

#include <algorithm>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
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
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
using namespace ftl::serialization;

typedef vector<int> IntVector;
typedef vector<string> StrVec;
typedef unordered_map<int, int> IntIntHashMap;
typedef map<int, int> IntIntTreeMap;

/// https://leetcode.com/contest/weekly-contest-111/
class Solution
{
public:
    bool validMountainArray( vector<int> &A )
    {
        const size_t N = A.size();
        if ( N < 3 )
            return false;
        size_t i = 0; // i is the peak
        for ( ; i < N - 1 && A[i] < A[i + 1]; ++i )
            ;
        if ( i == 0 || i == N - 1 )
            return false;
        for ( auto j = i + 1; j < N; ++j )
            if ( A[j - 1] <= A[j] )
                return false;
        return true;
    }

    template<typename ArrT, typename E = typename ArrT::value_type>
    int longestNoneDescreasing(
            ArrT &A, const function<int64_t( const E & )> &getH = []( const E &x ) {
                if constexpr ( is_convertible_v<E, int64_t> )
                    return x;
                else
                    return 0;
            } )
    {
        using HightT = int64_t;
        auto N = A.size();
        if ( N < 2 )
            return N;
        int64_t INF = numeric_limits<int64_t>::max();
        vector<HightT> I( N + 1, INF ); // I[i]: last value/height of subseqence with length i
        auto b = I.begin();
        advance( b, 1 );
        int i = 0;
        for ( auto &e : A )
        {
            ++i;
            HightT x = getH( e );
            auto p = b;
            advance( p, i );
            p = upper_bound( b, p, x ); // first elem > x
            *p = x;
        }
        // return the last non-INF index
        if ( I.back() != INF )
        {
            return N;
        }
        auto p = lower_bound( b, I.end(), INF );
        return distance( b, p );
    }
    template<typename ArrT, typename E = typename ArrT::value_type>
    int longestIncreasing(
            ArrT &A, const function<int64_t( const E & )> &getH = []( const E &x ) {
                if constexpr ( is_convertible_v<E, int64_t> )
                    return x;
                else
                    return 0;
            } )
    {
        using HightT = int64_t;
        auto N = A.size();
        if ( N < 2 )
            return N;
        int64_t INF = numeric_limits<int64_t>::max();
        vector<HightT> I( N + 1, INF ); // I[i]: last value/height of subseqence with length i
        auto b = I.begin();
        advance( b, 1 );
        int i = 0;
        for ( auto &e : A )
        {
            ++i;
            HightT x = getH( e );
            auto p = b;
            advance( p, i );
            p = lower_bound( b, p, x ); // first elem >= x
            *p = x;
        }
        // return the last non-INF index
        if ( I.back() != INF )
        {
            return N;
        }
        auto p = lower_bound( b, I.end(), INF );
        return distance( b, p );
    }

    // Delete Columns to Make non-increasing
    int minDeletionSize( vector<string> &A )
    {
        int n = 0;
        for ( size_t i = 0; i < A[0].size(); ++i )
        {
            for ( size_t j = 1; j < A.size(); ++j )
            {
                if ( A[j][i] < A[j - 1][i] )
                {
                    ++n;
                    break;
                }
            }
        }
        return n;
    }

    vector<int> diStringMatch( string S )
    {
        const size_t N = S.size();
        int imin = 0, imax = 0;
        vector<int> v( N + 1, 0 );
        size_t i = 1;
        for ( auto c : S )
        {
            v[i++] = c == 'I' ? ( ++imax ) : ( --imin );
        }
        if ( imin < 0 )
            for ( auto &x : v )
                x -= imin;
        return v;
    }
    // given "DIDDIII" string of length N, return array A containing all distinct elements from 0 to N.
    vector<int> smallestDIStringMatch( string S )
    {
        // todo
        return {};
    }
};

TEST_FUNC( minDeletionSize_tests )
{
    Solution sln;
    {
        IntVector A{1, 3, 3, 3, 2};
        REQUIRE( sln.longestNoneDescreasing( A ) == 4 );
        REQUIRE( sln.longestIncreasing( A ) == 2 );
    }
    {
        IntVector A{2, 3, 1, 1, 3, 2, 2};
        REQUIRE( sln.longestNoneDescreasing( A ) == 4 );
        REQUIRE( sln.longestIncreasing( A ) == 2 );
    }
    {
        IntVector A{1, 3, 3, 2, 2, 2, 4};
        REQUIRE( sln.longestNoneDescreasing( A ) == 5 );
        REQUIRE( sln.longestIncreasing( A ) == 3 );
    }
    /// minDeletionSize
    {
        StrVec A = {"cba", "daf", "ghi"};
        REQUIRE( sln.minDeletionSize( A ) == 1 );
    }
    {
        StrVec A = {"zyx", "wvu", "tsr"};
        REQUIRE( sln.minDeletionSize( A ) == 3 );
    }
    /// diStringMatch
    {
        std::cout << sln.diStringMatch( "IDID" ) << endl;
    }
    {
        std::cout << sln.diStringMatch( "III" ) << endl;
    }
    {
        std::cout << sln.diStringMatch( "DDI" ) << endl;
    }
}

TEST_FUNC( validMountainArray_tests )
{
    Solution sln;
    {
        IntVector A{1, 2, 3};
        REQUIRE( sln.validMountainArray( A ) == false );
    }
    {
        IntVector A{3, 2, 1};
        REQUIRE( sln.validMountainArray( A ) == false );
    }
    {
        IntVector A{1, 3, 2};
        REQUIRE( sln.validMountainArray( A ) == true );
    }
}
