
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

tldr; If you have a 2-4GB dataset and you need to estimate a
(generalized) linear model with a large number of fixed effects, this
package is for you. It works with larger datasets as well and facilites
computing clustered standard errors.

‘kendallknight’ is a fast and small footprint software that provides
efficient functions for demeaning variables before conducting a GLM
estimation. This technique is particularly useful when estimating linear
models with multiple group fixed effects. It is a fork of the excellent
Alpaca package created and maintained by [Dr. Amrei
Stammann](https://github.com/amrei-stammann). The software can estimate
Exponential Family models (e.g., Poisson) and Negative Binomial models.

Traditional QR estimation can be unfeasible due to additional memory
requirements. The method, which is based on Halperin (1962) vector
projections offers important time and memory savings without
compromising numerical stability in the estimation process.

The software heavily borrows from Gaure (2013) and Stammann (2018) works
on OLS and GLM estimation with large fixed effects implemented in the
‘lfe’ and ‘alpaca’ packages. The differences are that ‘kendallknight’
does not use C nor Rcpp code, instead it uses cpp11 and
[cpp11armadillo](https://github.com/pachadotdev/cpp11armadillo).

The summary tables borrow from Stata outputs. I have also provided
integrations with ‘broom’ to facilitate the inclusion of statistical
tables in Quarto/Jupyter notebooks.

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
"kendall"` for vectors of different lengths. The results are shown in
the following table:

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

## Code of Conduct

Please note that the kendallknight project is released with a
[Contributor Code of
Conduct](https://contributor-covenant.org/version/2/1/CODE_OF_CONDUCT.html).
By contributing to this project, you agree to abide by its terms.
