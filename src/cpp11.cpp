// Generated by cpp11: do not edit by hand
// clang-format off


#include "cpp11/declarations.hpp"
#include <R_ext/Visibility.h>

// kendall_correlation.cpp
double kendall_cor_(const doubles_matrix<> & m);
extern "C" SEXP _kendallknight_kendall_cor_(SEXP m) {
  BEGIN_CPP11
    return cpp11::as_sexp(kendall_cor_(cpp11::as_cpp<cpp11::decay_t<const doubles_matrix<> &>>(m)));
  END_CPP11
}
// kendall_correlation.cpp
doubles pkendall_(doubles Q, int n);
extern "C" SEXP _kendallknight_pkendall_(SEXP Q, SEXP n) {
  BEGIN_CPP11
    return cpp11::as_sexp(pkendall_(cpp11::as_cpp<cpp11::decay_t<doubles>>(Q), cpp11::as_cpp<cpp11::decay_t<int>>(n)));
  END_CPP11
}

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    {"_kendallknight_kendall_cor_", (DL_FUNC) &_kendallknight_kendall_cor_, 1},
    {"_kendallknight_pkendall_",    (DL_FUNC) &_kendallknight_pkendall_,    2},
    {NULL, NULL, 0}
};
}

extern "C" attribute_visible void R_init_kendallknight(DllInfo* dll){
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
  R_forceSymbols(dll, TRUE);
}
