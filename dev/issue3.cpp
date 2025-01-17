#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


uint64_t insertionSort(double*, size_t);

//#define kendallTest
#ifdef kendallTest

#include <stdio.h>
#include <assert.h>
#include <time.h>



/* Kludge:  In testing mode, just forward R_rsort to insertionSort to make this
 * module testable without having to include (and compile) a bunch of other
 * stuff.
 */
void R_rsort(double* arr, int len) {
    insertionSort(arr, len);
}
#else

#include <R_ext/Utils.h>  /* For R_rsort. */

#endif

/* Sorts in place, returns the bubble sort distance between the input array
 * and the sorted array.
 */
uint64_t insertionSort(double* arr, size_t len) {
    size_t maxJ, i;
    uint64_t swapCount = 0;

    if(len < 2) {
        return 0;
    }

    maxJ = len - 1;
    for(i = len - 2; i < len; --i) {
        size_t j = i;
        double val = arr[i];

        for(; j < maxJ && arr[j + 1] < val; ++j) {
            arr[j] = arr[j + 1];
        }

        arr[j] = val;
        swapCount += (j - i);
    }

    return swapCount;
}

static uint64_t merge(double* from, double* to, size_t middle, size_t len) {
    size_t bufIndex, leftLen, rightLen;
    uint64_t swaps;
    double* left;
    double* right;

    bufIndex = 0;
    swaps = 0;

    left = from;
    right = from + middle;
    rightLen = len - middle;
    leftLen = middle;

    while(leftLen && rightLen) {
        if(right[0] < left[0]) {
            to[bufIndex] = right[0];
            swaps += leftLen;
            rightLen--;
            right++;
        } else {
            to[bufIndex] = left[0];
            leftLen--;
            left++;
        }
        bufIndex++;
    }

    if(leftLen) {
        memcpy(to + bufIndex, left, leftLen * sizeof(double));
    } else if(rightLen) {
        memcpy(to + bufIndex, right, rightLen * sizeof(double));
    }

    return swaps;
}

/* Sorts in place, returns the bubble sort distance between the input array
 * and the sorted array.
 */
uint64_t mergeSort(double* x, double* buf, size_t len) {
    uint64_t swaps;
    size_t half;

    if(len < 10) {
        return insertionSort(x, len);
    }

    swaps = 0;

    if(len < 2) {
        return 0;
    }

    half = len / 2;
    swaps += mergeSort(x, buf, half);
    swaps += mergeSort(x + half, buf + half, len - half);
    swaps += merge(x, buf, half, len);

    memcpy(x, buf, len * sizeof(double));
    return swaps;
}

static uint64_t getMs(double* data, size_t len) {  /* Assumes data is sorted.*/
    uint64_t Ms = 0, tieCount = 0;
    size_t i;

    for(i = 1; i < len; i++) {
        if(data[i] == data[i-1]) {
            tieCount++;
        } else if(tieCount) {
            Ms += (tieCount * (tieCount + 1)) / 2;
            tieCount++;
            tieCount = 0;
        }
    }
    if(tieCount) {
        Ms += (tieCount * (tieCount + 1)) / 2;
        tieCount++;
    }
    return Ms;
}

/* This function calculates the Kendall covariance (if cor == 0) or
 * correlation (if cor != 0), but assumes arr1 has already been sorted and
 * arr2 has already been reordered in lockstep.  This can be done within R
 * before calling this function by doing something like:
 *
 * perm <- order(arr1)
 * arr1 <- arr1[perm]
 * arr2 <- arr2[perm]
 */
double kendallNlogN(double* arr1, double* arr2, size_t len, int cor) {
    uint64_t m1 = 0, m2 = 0, tieCount, swapCount, nPair;
    int64_t s;
    size_t i;

    nPair = (uint64_t) len * ((uint64_t) len - 1) / 2;
    s = nPair;

    tieCount = 0;
    for(i = 1; i < len; i++) {
        if(arr1[i - 1] == arr1[i]) {
            tieCount++;
        } else if(tieCount > 0) {
            R_rsort(arr2 + i - tieCount - 1, tieCount + 1);
            m1 += tieCount * (tieCount + 1) / 2;
            s += getMs(arr2 + i - tieCount - 1, tieCount + 1);
            tieCount++;
            tieCount = 0;
        }
    }
    if(tieCount > 0) {
        R_rsort(arr2 + i - tieCount - 1, tieCount + 1);
        m1 += tieCount * (tieCount + 1) / 2;
        s += getMs(arr2 + i - tieCount - 1, tieCount + 1);
        tieCount++;
    }

    swapCount = mergeSort(arr2, arr1, len);

    m2 = getMs(arr2, len);
    s -= (m1 + m2) + 2 * swapCount;

    if(cor) {
        double denominator1 = nPair - m1;
        double denominator2 = nPair - m2;
        double cor = s / sqrt(denominator1) / sqrt(denominator2);
        return cor;
    } else {
        /* Return covariance. */
        return 2 * s;
    }
}

/* This function uses a simple O(N^2) implementation.  It probably has a smaller
 * constant and therefore is useful in the small N case, and is also useful
 * for testing the relatively complex O(N log N) implementation.
 */
double kendallSmallN(double* arr1, double* arr2, size_t len, int cor) {
    /* Not using 64-bit ints here because this function is meant only for
       small N and for testing.
    */
    int m1 = 0, m2 = 0, s = 0, nPair;
    size_t i, j;
    double denominator1, denominator2;

    for(i = 0; i < len; i++) {
        for(j = i + 1; j < len; j++) {
            if(arr2[i] > arr2[j]) {
                if (arr1[i] > arr1[j]) {
                    s++;
                } else if(arr1[i] < arr1[j]) {
                    s--;
                } else {
                    m1++;
                }
            } else if(arr2[i] < arr2[j]) {
                if (arr1[i] > arr1[j]) {
                    s--;
                } else if(arr1[i] < arr1[j]) {
                    s++;
                } else {
                    m1++;
                }
            } else {
                m2++;

                if(arr1[i] == arr1[j]) {
                    m1++;
                }
            }
        }
    }

    nPair = len * (len - 1) / 2;
    if(cor) {
        denominator1 = nPair - m1;
        denominator2 = nPair - m2;
        return s / sqrt(denominator1) / sqrt(denominator2);
    } else {
        /* Return covariance. */
        return 2 * s;
    }
}

#ifdef kendallTest

int main() {
    double a[100], b[100];
    double smallNCor, smallNCov, largeNCor, largeNCov;
    int i;

    /* Test the small N version against a few values obtained from the old
     * version in R.  Only exercising the small N version because the large
     * N version requires the first array to be sorted and the second to be
     * reordered in lockstep before it's called.*/
    {
        double a1[] = {1,2,3,5,4};
        double a2[] = {1,2,3,3,5};
        assert(kendallSmallN(a1, a2, 5, 1) - 0.7378648 < 0.00001);
        assert(kendallSmallN(a1, a2, 5, 0) == 14);

        double b1[] = {8,6,7,5,3,0,9};
        double b2[] = {3,1,4,1,5,9,2};
        assert(kendallSmallN(b1, b2, 7, 1) + 0.39036 < 0.00001);
        assert(kendallSmallN(b1, b2, 7, 0) == -16);

        double c1[] = {1,1,1,2,3,3,4,4};
        double c2[] = {1,2,1,3,3,5,5,5};
        assert(kendallSmallN(c1, c2, 8, 1) - 0.8695652 < 0.00001);
        assert(kendallSmallN(c1, c2, 8, 0) == 40);
    }

    /* Now that we're confident that the simple, small N version works,
     * extensively test it against the much more complex and bug-prone
     * O(N log N) version.
     */
    for(i = 0; i < 10000; i++) {
        int j, len;
        for(j = 0; j < 100; j++) {
            a[j] = rand() % 30;
            b[j] = rand() % 30;
        }

        len = rand() % 50 + 50;

        /* The large N version assumes that the first array is sorted.  This
         * will usually be made true in R before passing the arrays to the
         * C functions.
         */
        insertionSort(a, len);

        if(i & 1) {
            /* Test correlation on odd iterations, covariance on even ones.
             * Can't test both on every iteration because the large N
             * impl destroys the contents of the arrays passed in.*/
            smallNCor = kendallSmallN(a, b, len, 1);
            largeNCor = kendallNlogN(a, b, len, 1);
            assert(largeNCor == smallNCor);
        } else {
            smallNCov = kendallSmallN(a, b, len, 0);
            largeNCov = kendallNlogN(a, b, len, 0);
            assert(largeNCov == smallNCov);
        }
    }

    printf("Passed all tests.\n");

    /* Speed test.  Compare the O(N^2) version, which is very similar to
     * R's current impl, to my O(N log N) version.
     */
    {
        const int N = 30000;
        double *foo, *bar, *buf;
        size_t i;
        double startTime, stopTime;

        foo = (double*) malloc(N * sizeof(double));
        bar = (double*) malloc(N * sizeof(double));
        for(i = 0; i < N; i++) {
            foo[i] = rand();
            bar[i] = rand();
        }

        startTime = clock();
        kendallSmallN(foo, bar, N, 1);
        stopTime = clock();
        printf("O(N^2) version:  %f milliseconds\n", stopTime - startTime);

        startTime = clock();

        /* Only sorting first array.  Normally the second one would be
         * reordered in lockstep.
         */
        buf = (double*) malloc(N * sizeof(double));
        mergeSort(foo, buf, N);
        kendallNlogN(foo, bar, N, 1);
        stopTime = clock();
        printf("O(N log N) version:  %f milliseconds\n", stopTime - startTime);
    }

    return 0;
}

#endif
