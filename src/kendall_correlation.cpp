// this part heavily borrows from
// Knight 1966 (A Computer Method for Calculating Kendall's Tau with Ungrouped
// Data)
// Filzmoser, Fritz, and Kalcher 2023 (pcaPP package)

// note: the len < 2 conditions are commented out because the R function checks
// for this condition before calling the C++ functions

#include <cpp11.hpp>
#include <numeric>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef M_LN_SQRT_2PI
#define M_LN_SQRT_2PI 0.9189385332046727
#endif

using namespace cpp11;

uint64_t insertion_sort_(double *arr, size_t len) {
  // if (len < 2) {
  //   return 0;
  // }

  size_t maxJ = len - 1, i;
  uint64_t swapCount = 0;

  for (i = len - 2; i < len; --i) {
    size_t j = i;
    double val = arr[i];

    while (j < maxJ && arr[j + 1] < val) {
      arr[j] = arr[j + 1];
      ++j;
    }

    arr[j] = val;
    swapCount += (j - i);
  }

  return swapCount;
}

static uint64_t merge_(double *from, double *to, size_t middle, size_t len) {
  size_t bufIndex = 0, leftLen, rightLen;
  uint64_t swaps = 0;
  double *left;
  double *right;

  left = from;
  right = from + middle;
  rightLen = len - middle;
  leftLen = middle;

  while (leftLen && rightLen) {
    if (right[0] < left[0]) {
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

  if (leftLen) {
    memcpy(to + bufIndex, left, leftLen * sizeof(double));
  } else if (rightLen) {
    memcpy(to + bufIndex, right, rightLen * sizeof(double));
  }

  return swaps;
}

uint64_t merge_sort_(double *x, double *buf, size_t len) {
  // if (len < 2) {
  //   return 0;
  // }

  if (len < 10) {
    return insertion_sort_(x, len);
  }

  uint64_t swaps = 0;
  size_t half = len / 2;

  swaps += merge_sort_(x, buf, half);
  swaps += merge_sort_(x + half, buf + half, len - half);
  swaps += merge_(x, buf, half, len);

  memcpy(x, buf, len * sizeof(double));
  return swaps;
}

[[cpp11::register]] double kendall_cor_(const doubles &x, const doubles &y) {
  size_t len = x.size();
  std::vector<double> arr1(len), arr2(len);
  std::vector<double> buf(len);
  uint64_t m1 = 0, m2 = 0, tieCount, swapCount, nPair;
  int64_t s;

  // Sort the 1st vector and rearrange the 2nd vector accordingly
  std::vector<size_t> perm(len);
  std::iota(perm.begin(), perm.end(), 0);
  std::sort(perm.begin(), perm.end(),
            [&](size_t i, size_t j) { return x[i] < x[j]; });
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (size_t i = 0; i < len; i++) {
    arr1[i] = x[perm[i]];
    arr2[i] = y[perm[i]];
  }

  // Compute nPair and initialize s
  nPair = static_cast<uint64_t>(len) * (static_cast<uint64_t>(len) - 1) / 2;
  s = nPair;

  // Compute m1
  tieCount = 0;
  for (size_t i = 1; i < len; i++) {
    if (arr1[i] == arr1[i - 1]) {
      tieCount++;
    } else if (tieCount > 0) {
      m1 += tieCount * (tieCount + 1) / 2;
      tieCount = 0;
    }
  }
  if (tieCount > 0) {
    m1 += tieCount * (tieCount + 1) / 2;
  }

  swapCount = merge_sort_(arr2.data(), buf.data(), len);

  // Compute m2
  m2 = 0;
  tieCount = 0;
  for (size_t i = 1; i < len; i++) {
    if (arr2[i] == arr2[i - 1]) {
      tieCount++;
    } else if (tieCount) {
      m2 += (tieCount * (tieCount + 1)) / 2;
      tieCount = 0;
    }
  }
  if (tieCount) {
    m2 += (tieCount * (tieCount + 1)) / 2;
  }

  // Adjust for ties
  s -= (m1 + m2) + 2 * swapCount;

  return s / (std::sqrt(nPair - m1) * std::sqrt(nPair - m2));
}

double ckendall_(int k, int n, std::vector<std::vector<double>> &w) {
  int u = n * (n - 1) / 2;
  if (k < 0 || k > u) {
    return 0;
  }
  if (w[n][k] < 0) {
    if (n == 1) {
      w[n][k] = (k == 0) ? 1 : 0;
    } else {
      double s = 0;
#ifdef _OPENMP
#pragma omp parallel for reduction(+ : s)
#endif
      for (int i = 0; i <= u; i++) {
        s += ckendall_(k - i, n - 1, w);
      }
      w[n][k] = s;
    }
  }
  return w[n][k];
}

double chebyshev_eval_(double x, const double *a, const int n) {
  if (n < 1 || n > 1000) {
    throw std::invalid_argument("n must be between 1 and 1000");
  }

  if (x < -1.1 || x > 1.1) {
    throw std::invalid_argument("x must be between -1.1 and 1.1");
  }

  double b0 = 0.0, b1 = 0.0, b2 = 0.0, twox = 2.0 * x;

  for (int i = 1; i <= n; ++i) {
    b2 = b1;
    b1 = b0;
    b0 = twox * b1 - b2 + a[n - i];
  }

  return (b0 - b2) * 0.5;
}

double stirlerr_(double n) {
  const double S0 = 0.083333333333333333333;         // 1/12
  const double S1 = 0.00277777777777777777778;       // 1/360
  const double S2 = 0.00079365079365079365079365;    // 1/1260
  const double S3 = 0.000595238095238095238095238;   // 1/1680
  const double S4 = 0.0008417508417508417508417508;  // 1/1188

  // Error for 0, 0.5, 1.0, 1.5, ..., 14.5, 15.0.
  const std::vector<double> sferr_halves = {
      0.0,                            // n=0 - wrong, place holder only
      0.1534264097200273452913848,    // 0.5
      0.0810614667953272582196702,    // 1.0
      0.0548141210519176538961390,    // 1.5
      0.0413406959554092940938221,    // 2.0
      0.03316287351993628748511048,   // 2.5
      0.02767792568499833914878929,   // 3.0
      0.02374616365629749597132920,   // 3.5
      0.02079067210376509311152277,   // 4.0
      0.01848845053267318523077934,   // 4.5
      0.01664469118982119216319487,   // 5.0
      0.01513497322191737887351255,   // 5.5
      0.01387612882307074799874573,   // 6.0
      0.01281046524292022692424986,   // 6.5
      0.01189670994589177009505572,   // 7.0
      0.01110455975820691732662991,   // 7.5
      0.010411265261972096497478567,  // 8.0
      0.009799416126158803298389475,  // 8.5
      0.009255462182712732917728637,  // 9.0
      0.008768700134139385462952823,  // 9.5
      0.008330563433362871256469318,  // 10.0
      0.007934114564314020547248100,  // 10.5
      0.007573675487951840794972024,  // 11.0
      0.007244554301320383179543912,  // 11.5
      0.006942840107209529865664152,  // 12.0
      0.006665247032707682442354394,  // 12.5
      0.006408994188004207068439631,  // 13.0
      0.006171712263039457647532867,  // 13.5
      0.005951370112758847735624416,  // 14.0
      0.005746216513010115682023589,  // 14.5
      0.005554733551962801371038690   // 15.0
  };

  double nn;

  if (n <= 15.0) {
    nn = n + n;
    if (nn == static_cast<int>(nn)) return sferr_halves[static_cast<int>(nn)];
    return std::lgamma(n + 1.0) - (n + 0.5) * std::log(n) + n - M_LN_SQRT_2PI;
  }

  nn = n * n;
  if (n > 500) return (S0 - S1 / nn) / n;
  if (n > 80) return (S0 - (S1 - S2 / nn) / nn) / n;
  if (n > 35) return (S0 - (S1 - (S2 - S3 / nn) / nn) / nn) / n;
  // 15 < n <= 35
  return (S0 - (S1 - (S2 - (S3 - S4 / nn) / nn) / nn) / nn) / n;
}

double lgammacor_(double x) {
  const static double algmcs[15] = {+.1666389480451863247205729650822e+0,
                                    -.1384948176067563840732986059135e-4,
                                    +.9810825646924729426157171547487e-8,
                                    -.1809129475572494194263306266719e-10,
                                    +.6221098041892605227126015543416e-13,
                                    -.3399615005417721944303330599666e-15,
                                    +.2683181998482698748957538846666e-17,
                                    -.2868042435334643284144622399999e-19,
                                    +.3962837061046434803679306666666e-21,
                                    -.6831888753985766870111999999999e-23,
                                    +.1429227355942498147573333333333e-24,
                                    -.3547598158101070547199999999999e-26,
                                    +.1025680058010470912000000000000e-27,
                                    -.3401102254316748799999999999999e-29,
                                    +.1276642195630062933333333333333e-30};

  double tmp;

  // Constants for IEEE double precision
  const int nalgm = 5;
  const double xbig = 94906265.62425156;
  const double xmax = 3.745194030963158e306;

  if (x < 10) {
    throw std::invalid_argument("x must be >= 10");
  } else if (x >= xmax) {
    throw std::underflow_error("lgammacor underflow");
  } else if (x < xbig) {
    tmp = 10 / x;
    return chebyshev_eval_(tmp * tmp * 2 - 1, algmcs, nalgm) / x;
  }
  return 1 / (x * 12);
}

double sinpi_(double x) { return std::sin(M_PI * x); }

double gammafn_(double x) {
  const static double gamcs[42] = {+.8571195590989331421920062399942e-2,
                                   +.4415381324841006757191315771652e-2,
                                   +.5685043681599363378632664588789e-1,
                                   -.4219835396418560501012500186624e-2,
                                   +.1326808181212460220584006796352e-2,
                                   -.1893024529798880432523947023886e-3,
                                   +.3606925327441245256578082217225e-4,
                                   -.6056761904460864218485548290365e-5,
                                   +.1055829546302283344731823509093e-5,
                                   -.1811967365542384048291855891166e-6,
                                   +.3117724964715322277790254593169e-7,
                                   -.5354219639019687140874081024347e-8,
                                   +.9193275519859588946887786825940e-9,
                                   -.1577941280288339761767423273953e-9,
                                   +.2707980622934954543266540433089e-10,
                                   -.4646818653825730144081661058933e-11,
                                   +.7973350192007419656460767175359e-12,
                                   -.1368078209830916025799499172309e-12,
                                   +.2347319486563800657233471771688e-13,
                                   -.4027432614949066932766570534699e-14,
                                   +.6910051747372100912138336975257e-15,
                                   -.1185584500221992907052387126192e-15,
                                   +.2034148542496373955201026051932e-16,
                                   -.3490054341717405849274012949108e-17,
                                   +.5987993856485305567135051066026e-18,
                                   -.1027378057872228074490069778431e-18,
                                   +.1762702816060529824942759660748e-19,
                                   -.3024320653735306260958772112042e-20,
                                   +.5188914660218397839717833550506e-21,
                                   -.8902770842456576692449251601066e-22,
                                   +.1527474068493342602274596891306e-22,
                                   -.2620731256187362900257328332799e-23,
                                   +.4496464047830538670331046570666e-24,
                                   -.7714712731336877911703901525333e-25,
                                   +.1323635453126044036486572714666e-25,
                                   -.2270999412942928816702313813333e-26,
                                   +.3896418998003991449320816639999e-27,
                                   -.6685198115125953327792127999999e-28,
                                   +.1146998663140024384347613866666e-28,
                                   -.1967938586345134677295103999999e-29,
                                   +.3376448816585338090334890666666e-30,
                                   -.5793070335782135784625493333333e-31};

  static const int ngam = 22;
  static const double xmin = -170.5674972726612;
  static const double xmax = 171.61447887182298;
  static const double xsml = 2.2474362225598545e-308;
  static const double dxrel = 1.490116119384765696e-8;

  if (std::isnan(x)) return x;

  if (x == 0 || (x < 0 && x == std::round(x))) {
    warning("gammafn_: ME_DOMAIN");
    return R_NaN;
  }

  int i;
  double y = std::fabs(x), value;

  if (y <= 10) {
    int n = static_cast<int>(x);
    if (x < 0) --n;
    y = x - n;
    --n;
    value = chebyshev_eval_(y * 2 - 1, gamcs, ngam) + .9375;
    if (n == 0) return value;

    if (n < 0) {
      if (x < -0.5 && std::fabs(x - static_cast<int>(x - 0.5) / x) < dxrel) {
        warning("gammafn_: ME_PRECISION");
      }

      if (y < xsml) {
        warning("gammafn_: ME_RANGE");
        return (x > 0) ? R_PosInf : R_NegInf;
      }

      n = -n;
      for (i = 0; i < n; i++) {
        value /= (x + i);
      }
      return value;
    } else {
      for (i = 1; i <= n; i++) {
        value *= (y + i);
      }
      return value;
    }
  } else {
    if (x > xmax) {
      return R_PosInf;
    }

    if (x < xmin) {
      return 0.;
    }

    if (y <= 50 && y == static_cast<int>(y)) {
      value = 1.;
      for (i = 2; i < y; i++) value *= i;
    } else {
      value = std::exp(
          (y - 0.5) * std::log(y) - y + M_LN_SQRT_2PI +
          ((2 * y == static_cast<int>(2 * y)) ? stirlerr_(y) : lgammacor_(y)));
    }

    if (x > 0) return value;

    if (std::fabs((x - static_cast<int>(x - 0.5)) / x) < dxrel) {
      warning("gammafn_: ME_PRECISION");
    }

    double sinpiy = sinpi_(y);
    if (sinpiy == 0) {
      warning("gammafn_: ME_RANGE");
      return R_PosInf;
    }

    return -M_PI / (y * sinpiy * value);
  }
}

[[cpp11::register]] doubles pkendall_(doubles Q, int n) {
  int len = Q.size();
  writable::doubles P(len);
  std::vector<std::vector<double>> w(
      n + 1, std::vector<double>((n * (n - 1) / 2) + 1, -1));

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (int i = 0; i < len; i++) {
    double q = std::floor(Q[i] + 1e-7);
    if (q < 0) {
      P[i] = 0;
    } else if (q > n * (n - 1) / 2) {
      P[i] = 1;
    } else {
      double p = 0;
      for (int j = 0; j <= q; j++) {
        p += ckendall_(j, n, w);
      }
      P[i] = p / gammafn_(n + 1);
    }
  }
  return P;
}
