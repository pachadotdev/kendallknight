# kendallknight 0.4.0

* Provides an implementation of chebyshev_eval, gammafn, lgammacor and stirlerr
  functions in C++. This solves CRAN warnings about using non-API non-API calls
  to R.

# kendallknight 0.3.0

* Provides the option to pass a matrix to obtain a correlation matrix.

# kendallknight 0.2.0

* Uses parallelization to speed up the computation of the Kendall's correlation.
* It still needs additional checks with Inf values.

# kendallknight 0.1.0

* The Kendall's correlation function from capybara was moved here to have a
  dedicated package for it.
