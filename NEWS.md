# kendallknight 1.0.0

* New vignette and link to published article.

# kendallknight 0.7.0

* Replaces the example dataset with one based on real data.
* Adds confidence intervals to correlation tests and allows different confidence
  levels.

# kendallknight 0.6.0

* Uses "htest" S3 class for the p-value print method. This is a minor change
  that makes the output more consistent with other R functions.
  
# kendallknight 0.5.0

* Corrects a severe error with the p-value computation. Thanks to @ouroboro for
  pointing this out (#3).

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
