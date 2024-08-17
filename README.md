
<!-- README.md is generated from README.Rmd. Please edit that file -->

# kendallknight <img src="man/figures/logo.svg" align="right" height="139" alt="" />

<!-- badges: start -->

[![R-CMD-check](https://github.com/pachadotdev/kendallknight/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/pachadotdev/kendallknight/actions/workflows/R-CMD-check.yaml)
[![codecov](https://codecov.io/gh/pachadotdev/kendallknight/graph/badge.svg?token=kDP0pWmfRk)](https://codecov.io/gh/pachadotdev/kendallknight)
[![BuyMeACoffee](https://raw.githubusercontent.com/pachadotdev/buymeacoffee-badges/main/bmc-donate-yellow.svg)](https://www.buymeacoffee.com/pacha)
[![Lifecycle:
stable](https://img.shields.io/badge/lifecycle-stable-brightgreen.svg)](https://lifecycle.r-lib.org/articles/stages.html#stable)
<!-- badges: end -->

## About

tldr; If you have a large dataset, this package implements a different
algorithm from the one implemented in base R, and it reduces the
complexity of the Kendall’s correlation coefficient from O(n^2) to O(n
log n). This package is written in C++ and uses cpp11 to export the
functions to R. See the vignette for the mathematical details.

If this software is useful to you, please consider donating on [Buy Me A
Coffee](https://buymeacoffee.com/pacha). All donations will be used to
continue improving `kendallknight`.

## Installation

You can install the development version of kendallknight like so:

``` r
remotes::install_github("pachadotdev/kendallknight")
```

## Examples

See the documentation: <https://pacha.dev/kendallknight>.

# Benchmarks

We tested the `kendallknight` package against the base R implementation
of the Kendall correlation using the `cor` function with `method =
"kendall"` for randomly generated vectors of different lengths. The
results are shown in the following table:

| implementation | number of observations | median time | memory allocation |
| -------------- | ---------------------- | ----------- | ----------------- |
| kendallknight  | 100                    | 37 us       | 52 KB             |
| base R         | 100                    | 265 ms      | 129 KB            |
| kendallknight  | 1,000                  | 173 us      | 67 KB             |
| base R         | 1,000                  | 22 ms       | 75 KB             |
| kendallknight  | 10,000                 | 2 ms        | 665 KB            |
| base R         | 10,000                 | 2 s         | 743 KB            |
| kendallknight  | 100,000                | 22 ms       | 6 MB              |
| base R         | 100,000                | 4 m         | 7 MB              |

In order to avoid distorted results, we used the `bench` package to run
the benchmarking tests in a clean R session and in the Niagara
supercomputer cluster that, unlike personal computers, will not distort
the test results due to other processes running in the background (e.g.,
such as automatic updates).

# Testing

The package uses `testthat` for testing \[@wickham2011\]. The included
tests are exhaustive and covered the complete code to check for
correctness comparing with the base R implementation, checking corner
cases, and forcing errors by passing unusable input data to the
user-visible functions.

## Code of Conduct

Please note that the kendallknight project is released with a
[Contributor Code of
Conduct](https://contributor-covenant.org/version/2/1/CODE_OF_CONDUCT.html).
By contributing to this project, you agree to abide by its terms.
