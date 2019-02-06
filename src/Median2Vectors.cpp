/*
here are N children standing in a line. Each child is assigned a rating value.

You are giving candies to these children subjected to the following requirements:

    Each child must have at least one candy.
    Children with a higher rating get more candies than their neighbors.

What is the minimum candies you must give?
*/

#include <ftl/ftl.h>

using namespace std;

struct Comparator {
    int asc;
    explicit Comparator(int b, int a)
        : asc(b - a)
    {
        //~ int d = second-first;
        //~ asc = d>0? 1: (d==0?0:(-1));
    }
    Comparator()
        : asc(1)
    {
    }
    int operator()(int b, int a)
    {
        int d = b - a;
        //~ d = d>0? 1: (d==0?0:(-1));
        if (asc < 0)
            d = -d;
        return d;
    }
};

class Solution {

public:
    template <class T>
    T nth_value(unsigned k, const vector<T>& A, const vector<T>& B)
    {
        // find k-th element among sorted A and B.  assume 1 <= k <= A.size()+B.size()
        assert(k <= A.size() + B.size());
        const vector<T>& a = A.size() <= B.size() ? A : B; // with min size
        const vector<T>& b = A.size() <= B.size() ? B : A;
        if (a.empty())
            return b[k - 1];

        if (A.back() <= B.front())
            return A.size() >= k ? A[k - 1] : B[k - A.size() - 1];
        if (B.back() <= A.front())
            return B.size() >= k ? B[k - 1] : A[k - B.size() - 1];

        const unsigned N = int(a.size()), M = int(b.size());

        auto cond = [&](unsigned i) -> int {
            // i: numbers count in a
            // return 0 found; -1, too large, move left; 1, too small, move right;
            assert(i <= N);
            if (i > k)
                return -1;
            unsigned j = k - i; // numbers in b
            if (j > M)
                return 1;
            if (i == N && j == M)
                return 0;

            if (j == M)
                return b[j - 1] <= a[i] ? 0 : 1;
            if (i == N)
                return a[i - 1] <= b[j] ? 0 : -1;

            if (i == 0)
                return b[j - 1] <= a[0] ? 0 : 1;
            if (j == 0)
                return a[i - 1] <= b[0] ? 0 : -1;

            if (a[i - 1] > b[j])
                return -1;
            if (b[j - 1] > a[i])
                return 1;
            if (b[j - 1] <= a[i] && a[i - 1] <= b[j])
                return 0;
            assert(false);
        };
        unsigned minI = 0, maxI = N;
        unsigned I = INT_MAX, J;
        while (maxI >= minI) {
            auto i = (minI + maxI) / 2;
            auto c = cond(i);
            if (c == 0) {
                I = i;
                break;
            }
            if (c < 0)
                maxI = i - 1;
            else
                minI = i + 1;
        }
        J = k - I;
        int res;

        if (I == 0)
            res = b[J - 1];
        else if (J == 0)
            res = a[I - 1];
        else
            res = std::max(a[I - 1], b[J - 1]);
        return res;
    }
    double findMedianSortedArrays0(vector<int>& A, vector<int>& B)
    {
        auto N = A.size() + B.size();
        if (N % 2)
            return nth_value(N / 2 + 1, A, B);
        else
            return (nth_value(N / 2, A, B) + nth_value(N / 2 + 1, A, B)) / 2.0;
    }

    double findMedianSortedArrays(int A[], int m, int B[], int n)
    {
        Comparator compA(A[m - 1], A[0]);
        Comparator compB(B[n - 1], B[0]);
        int ascB = compA.asc * compB.asc < 0 ? (-1) : 1;
        int iM = (m + n) / 2 + 1; // count
        int iA = 0, iB = ascB == 1 ? 0 : (n - 1);
        int k = 0, preV = 0, lastV = 0;
        do {
            while (k < iM && iA < m && (iB < 0 || iB >= n || compA(A[iA], B[iB]) <= 0)) {
                preV = lastV;
                lastV = A[iA];
                ++iA;
                ++k;
            }
            while (k < iM && iB < n && iB >= 0 && (iA >= m || compA(B[iB], A[iA]) <= 0)) {
                preV = lastV;
                lastV = B[iB];
                iB += ascB;
                ++k;
            }
        } while (k < iM);
        if ((m + n) % 2 == 0)
            return (preV + lastV) / 2.0;
        else
            return lastV;
    }
};

static void test()
{
    Solution sln;
    //    int A[] = {1,2,3};
    //    int B[] = {1,2};

    int A[] = { 2, 3, 5, 6, 7 };
    int B[] = { 1, 4 };

    cout << sln.findMedianSortedArrays(A, sizeof(A) / sizeof(int), B, sizeof(B) / sizeof(int)) << endl;
}
//=========== test cpp
template <typename Ptr, typename T, typename... Args>
Ptr newObject(Args... args)
{
    return Ptr(new T(args...));
};
template <typename Ptr, typename T>
Ptr newObjects(size_t n)
{
    return Ptr(new T[n]);
};
struct Obj {
    int id;
    Obj()
    {
        printf("default constuctor\n");
    }
    Obj(int x, int b)
    {
        printf("param constuctor\n");
    }
    Obj(const Obj& a)
    {
        printf("copy constructor\n");
    }
    Obj& operator=(const Obj& a)
    {
        printf("assignment\n");
        return *this;
    }
};
//~ typedef shared_ptr<Obj> ObjPtr;
typedef Obj* ObjPtr;
void testcpp()
{
    ObjPtr a(new Obj(1, 2));
    ObjPtr b(new Obj());

    a->id = 1;
    b->id = 2;
}
int main_median()
{
    testcpp();
    //~ test();

    cout << "The end." << endl;
    return 0;
}
