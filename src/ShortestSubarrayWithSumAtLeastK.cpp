#ifdef UNITTEST_MODE
#define TEST_MODE
#include <ftl/catch.hpp>
#endif

#ifdef TEST_MODE
#include <ftl/container_serialization.h>
#endif

#include <algorithm>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <fstream>
#include <ftl/circular_queue.h>
#include <ftl/out_printer.h>
#include <iostream>
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
using namespace ftl;
using namespace ftl::serialization;

typedef vector<int> IntVector;
typedef unordered_map<int, int> IntIntHashMap;
typedef map<int, int> IntIntTreeMap;

/**
  https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/description/
Return the length of the shortest, non-empty, contiguous subarray of A with sum at least K.

If there is no non-empty subarray with sum at least K, return -1.

 */
namespace ShortestSubarrayWithSumAtLeastK {

class Solution : NullPrinter /*ftl::OSPrinter*/ {
public:
    // slow
    int shortestSubarray1(vector<int>& A, int K)
    {
        vector<int> s(A.size(), 0);
        int y = 0;
        int w = -1;
        for (size_t i = 0; i < A.size(); ++i) {
            s[i] = y + A[i];
            for (int j = i - 1; j >= 0; --j) {
                if (s[i] - s[j] >= K) {
                    if (w < 0 || i - j < w) {
                        w = i - j;
                        println(" foundMin <", j + 1, " -> ", i, "> ", w, " sum:", s[i] - s[j]);
                    }
                }
            }
            y = s[i];
        }
        println("=== foundMin <", w);
        return w;
    }

    // let w is the min length of subarray, of which sum >= K.
    // for Moving window (j, i) ending at ai,
    // deque q which is an increasing serial si = sum(aj, ai)
    //      if ai<=0, then ignore current
    //     else, then
    //             pop_back until sumBeforeCurrent is the largest.
    //             q.pop_front() while sum q[1..end] >= K.
    //             then check if queued subarray or the whole window is min w.
    //    time complexity O(n)
    int shortestSubarray(vector<int>& A, int K)
    {
        static int* thefirst = nullptr;
        thefirst = &A.front();
        struct Sum {
            int s;
            int* i;
            int sumBeforeCurrent() const // Sum.sumBeforeCurrent is increasing
            {
                return s - *i;
            }
            const char* c_str() const
            {
                static char buf[32];
                sprintf(buf, "(%d : %d %d)", int(i - thefirst), *i, s);
                return buf;
            }
        };
        if (A.size() == 0)
            return -1;
        if (K <= 0)
            return 0;
        //        std::deque<Sum> q;
        circular_queue<Sum> q(A.size());
        int w = -1; // min width
        int s = 0; // sum including current element
        int* b = &A.front(); // begin of window
        for (int *e = &A.front(), *END = &(*A.end()); e != END; ++e) {
            s += *e;
            if (s <= 0) {
                q.clear();
                s = 0; // clear window,  ensure si>0
                b = e + 1;
                println(" reset ", e - &A.front(), " -: ", *e, " ", s);
            } else if (*e > 0) {
                while (!q.empty() && q.back().sumBeforeCurrent() >= s - *e) {
                    print(" popback <", q.back().i - &A.front(), " : ", q.back().s, "> ");
                    q.pop_back();
                }
                q.push_back(Sum{ s, e });
                print(" pushback ", e - &A.front(), " : ", *e, " ", s);
                // pop front until shink to min window
                while (q.size() > 1 && q.back().s - q[1].sumBeforeCurrent() >= K) { // still >= K after removing the first
                    print(" popfront<", q.front().i - &A.front(), " : ", q.front().s, ">", q.back().s - q.front().s);
                    q.pop_front();
                }
                auto sum = q.back().s - q.front().s + (*q.front().i);
                if (sum >= K) { // if start from begin of stack
                    println("\n  q: ", q);
                    auto width = q.back().i - q.front().i + 1;
                    if (width < w || w < 0) {
                        w = width;
                        print(" foundMin Q <", q.front().i - &A.front(), " -> ", q.back().i - &A.front(), "> ", w, " sum:", sum);
                    } else {
                        print(" larger foundMin Q <", q.front().i - &A.front(), " -> ", q.back().i - &A.front(), "> ", w, " sum:", sum);
                    }
                } else if (q.back().s >= K) { // or if from begin of the window
                    auto width = q.back().i - b + 1;
                    if (width < w || w < 0) {
                        w = width;
                        print(" foundMin W<", b - &A.front(), " -> ", q.back().i - &A.front(), "> ", w, " sum:", q.back().s);
                    }
                }
            }
            println(" # ", e - &A.front(), " : ", *e, " ", s);
        }

        return w;
    }
};
}
#ifdef UNITTEST_MODE
TEST_CASE("ShortestSubarrayWithSumAtLeastK_tests", "")
#else
int main()
#endif

{
    ShortestSubarrayWithSumAtLeastK::Solution sln;
    {
        IntVector a{ 1 };
        REQUIRE(sln.shortestSubarray(a, 1) == 1);
    }
    {
        IntVector a{ 1, 2 };
        REQUIRE(sln.shortestSubarray(a, 4) == -1);
    }
    {
        IntVector a{ 2, -1, 2 };
        REQUIRE(sln.shortestSubarray(a, 3) == 3);
    }
    {
        IntVector a{ 31, 63, -38, 43, 65, 74, 90, -23, 45, 22 };
        REQUIRE(sln.shortestSubarray(a, 341) == 9);
    }
    {
        IntVector a{ 77, 19, 35, 10, -14 };
        REQUIRE(sln.shortestSubarray(a, 19) == 1);
    }
    {
        IntVector a{ 84, -37, 32, 40, 95 };
        REQUIRE(sln.shortestSubarray(a, 167) == 3);
    }
    {
        IntVector a{ 75, -32, 50, 32, 97 };
        REQUIRE(sln.shortestSubarray(a, 129) == 2);
    }
    {
        IntVector a{ -34, 37, 51, 3, -12, -50, 51, 100, -47, 99, 34, 14, -13, 89, 31, -14, -44, 23, -38, 6 };
        REQUIRE(sln.shortestSubarray1(a, 151) == 2);
        REQUIRE(sln.shortestSubarray(a, 151) == 2);
    }
    {
        IntVector a{ 17985, -36292, -23941, 80618, 20594, -6181, 9181, 65796, -25716, 66593, -6873, 34062, 29878, 852, -4767, -36415, 11783, 8085, -41063, -39940, 74284, 66272, 82385, 51634, -48550, 9028, -30777, 86509, 44623, 9413, -38369, -1822, 46408, 35217, 72635, -16560, 85373, 52105, 39477, 3852, 45974, -21593, 15481, 47280, 73889, -42981, 54978, 95169, -19615, 93133 };
        REQUIRE(sln.shortestSubarray(a, 387303) == 11);
    }
    {
        IntVector a{ 23861, 90783, -44784, 2768, 57262, 35666, -30625, 55819, 46011, 941, -33506, 80774, -49801, 14371, 14932, 54474, 96691, 30838, 47431, 11319, 88003, 4392, 56840, 34802, 57174, 9922, 85769, 86039, 58482, 63191, -27779, -25100, -47727, 75639, -48574, 75854, 27354, 44817, 28507, 49650, 5509, -26841, 15647, 59003, 86696, 87024, 18426, -26125, 49241, 19245, -47776, 82866, 19654, 17477, -11861, 47380, -47457, -45039, -25637, 68816, 74809, 27252, -24463, 33480, 71651, 67925, 23876, 25375, -23705, -44638, 94300, -48735, -26580, -13016, 62891, 15575, 95919, 61186, 15487, 87453, -5205, 172, -7309, 37776, 46780, -41977, -45365, 3040, 4791, 81624, 4756, 70977, 4191, 90842, 33201, 49623, -5190, 15213, -27740, 86843, 83438, -43390, -28834, 33180, -33511, 10468, 17132, 53135, 8836, 16150, -10538, 75096, 30446, 34090, 66048, -39819, -23566, -10387, 2232, 50142, -17236, 53525, 56480, -37085, 1720, -15220, 54864, -24227, 21155, 39231, 33193, 50699, 52200, 6654, 10090, 1669, 90301, 27929, -37519, 66391, -14580, 12464, 10779, 57558, -24004, -29733, -20325, 37478, 66908, -1528, 43931, 98896, -38562, 21391, 79178, -29310, -48250, -49575, -15305, 43392, 16120, 87186, 1247, 74940, 86452, -44797, 79144, -34276, -34688, -44763, 75263, -35036, 49272, 59948, -22333, 18931, 54062, -38150, -39007, -24287, 79183, 53379, 46778, -45069, 85732, 75736, -5512, 12167, 28689, 86621, 41175, 92073, 99212, -28898, 99863, 6428, 66574, -8317, 71722, 34681 };
        REQUIRE(sln.shortestSubarray1(a, 2795680) == 114);
        REQUIRE(sln.shortestSubarray(a, 2795680) == 114);
    }
}
