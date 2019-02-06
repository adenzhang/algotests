#include <ftl/ftl.h>

using namespace std;
using namespace ftl::serialization;

/*
 * KthSmallestSumFromSortedArrays.cpp

You have two sorted lists A and B, find the kth smallest sum.
Where sum is defined as a+b such that a belongs to list A and b belongs to list B.
For e.g, if A=[1,2,3] B=[4,5,6], the list of possible sums are [5,6,6,7,7,7,8,8,9],
and hence the 5th smallest sum is 7.

 */

namespace KthSmallestSumFromSortedArrays {
// take first k elements from array A and array B to generate k*k sums.
// push sums into a max heap.
// the one on top is the k-th smallest sume.
// Time complexity O(k*k*log(k)

typedef vector<int> IntVec;
int solve(const IntVec& A, const IntVec& B, int k)
{
    IntVec q(k);
    int n = 0;
    int NA = A.size() >= k ? k : A.size();
    int NB = B.size() >= k ? k : B.size();

    for (int i = 0; i < NA; ++i) {
        for (int j = 0; j < NB; ++j) {
            int s = A[i] + B[i];
            if (n == k) {
                if (s < q[0]) {
                    pop_heap(q.begin(), q.end());
                    q[n - 1] = A[i] + B[j];
                    push_heap(q.begin(), q.begin() + n);
                }
            } else {
                q[n++] = A[i] + B[j];
                push_heap(q.begin(), q.begin() + n);
            }
            //			cout << n << "," << q << endl;
        }
    }
    //	cout << q << endl;
    return q[0];
}

void test()
{
    IntVec A{ 1, 2, 3 }, B{ 4, 5, 6 };
    cout << solve(A, B, 5) << endl; // 7
}
}

int main_KthSmallestSumFromSortedArrays()
{
    KthSmallestSumFromSortedArrays::test();
}
// todo: max_n_sums
// hint: use priority_queue

TEST_FUNC(KthSmallestSumFromSortedArrays_tests)
{
    //    {
    //        vector<int> A = { 36, 27, -35, 43, -15, 36, 42, -1, -29, 12, -23, 40, 9, 13, -24, -10, -24, 22, -14, -39, 18, 17, -21, 32, -20, 12, -27, 17, -15, -21, -48, -28, 8, 19, 17, 43, 6, -39, -8, -21, 23, -29, -31, 34, -13, 48, -26, -35, 20, -37, -24, 41, 30, 6, 23, 12, 20, 46, 31, -45, -25, 34, -23, -14, -45, -4, -21, -37, 7, -26, 45, 32, -5, -36, 17, -16, 14, -7, 0, 37, -42, 26, 28 };
    //        vector<int> B = { 38, 34, -47, 1, 4, 49, -18, 10, 26, 18, -11, -38, -24, 36, 44, -11, 45, 20, -16, 28, 17, -49, 47, -48, -33, 42, 2, 6, -49, 30, 36, -9, 15, 39, -6, -31, -10, -21, -19, -33, 47, 21, 31, 25, -41, -23, 17, 6, 47, 3, 36, 15, -44, 33, -31, -26, -22, 21, -18, -21, -47, -31, 20, 18, -42, -35, -10, -1, 46, -27, -32, -5, -4, 1, -29, 5, 29, 38, 14, -22, -9, 0, 43 };
    //        vector<int> C;
    //        std::stringstream ss;
    //        ss << "97 95 95 95 95 94 94 93 93 93 93 92 92 92 92 92 92 92 91 91 91 91 90 90 90 90 90 90 90 90 90 90 89 89 89 89 89 89 89 89 88 88 88 88 88 88 88 88 87 87 87 87 87 87 87 87 87 86 86 86 86 86 86 86 86 85 85 85 85 85 85 85 85 84 84 84 84 84 84 84 84 84 84";
    //        readiterator(ss, back_inserter(C), ' ', nullptr);
    //        REQUIRE(C == sln.max_n_sums(A, B));
    //    }
}
