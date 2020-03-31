#include <ftl/ftl.h>
#include <ftl/enum_create.h>
#include <list>

using namespace std;

class Solution
{
public:
    /// @brief Beautiful numbers are those pylindomes containing only 4 or 5 and length can't be odd.
    /// E.g. 44 is the first one, 4554 is the fourth.
    /// @return n-th beautiful number
    /// @note For a number 45..|..45, we only look at the left half.
    ///     Given width of left half w, count of all combinations is 2^w.
    ///     So count of all numbers with legnth <= w, is m = 2^1+...2^w = 2^(w+1) - 2.
    ///     In order to find n-th number x, we can easily find it's width (half) w, where 2^(w+1)-2 >= n and 2^w-2 < n .
    ///         And within the group of numbers with length w, it's k-th number where k = n - (2^w-2) , k>=1 .
    ///         Replacing 4 with 0 and 4 with 1, we obtain the k-th number in binary mode.
    /// Time complexity O(w), where w is the width of the number.
    size_t nth_beautiful_number( size_t n )
    {
        size_t w = 1;
        while ( ( 1 << ( w + 1 ) ) - 2 < n )
            ++w;
        size_t k = n + 2 - ( 1 << w ) - 1, resL = 0, resR = 0, tens = 1;
        for ( size_t i = 0, N = w; i < N; ++i )
        {
            tens *= 10;
            resL = resL * 10 + ( k & ( 1 << ( N - i - 1 ) ) ? 5 : 4 );
            resR = resR * 10 + ( k & ( 1 << i ) ? 5 : 4 );
        }
        return resL * tens + resR;
    }
};

ADD_TEST_CASE( nth_beautiful_number_tests )
{
    Solution sln;
    REQUIRE( sln.nth_beautiful_number( 1 ) == 44 );
    REQUIRE( sln.nth_beautiful_number( 2 ) == 55 );
    REQUIRE( sln.nth_beautiful_number( 3 ) == 4444 );
    REQUIRE( sln.nth_beautiful_number( 4 ) == 4554 );
    REQUIRE( sln.nth_beautiful_number( 5 ) == 5445 );
    REQUIRE( sln.nth_beautiful_number( 6 ) == 5555 );
}
