
<!-- README.md is generated from README.Rmd. Please edit that file -->

# kendallknight <img src="man/figures/logo.svg" align="right" height="139" alt="" />

<!-- badges: start -->

[![Lifecycle:
stable](https://img.shields.io/badge/lifecycle-stable-brightgreen.svg)](https://lifecycle.r-lib.org/articles/stages.html#stable)
[![R-CMD-check](https://github.com/pachadotdev/kendallknight/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/pachadotdev/kendallknight/actions/workflows/R-CMD-check.yaml)
[![CRAN
status](https://www.r-pkg.org/badges/version/kendallknight)](https://CRAN.R-project.org/package=kendallknight)
[![Test
coverage](https://raw.githubusercontent.com/pachadotdev/kendallknight/main/badges/coverage.svg)](https://github.com/pachadotdev/kendallknight/actions/workflows/test-coverage.yaml)
[![BuyMeACoffee](https://raw.githubusercontent.com/pachadotdev/buymeacoffee-badges/main/bmc-yellow.svg)](https://buymeacoffee.com/pacha)
<!-- badges: end -->

## About

Please read my article for the full details of this project (Open
Access):

**Vargas Sepulveda, Mauricio. 2025. ‘Kendallknight: An R package for
efficient implementation of Kendall’s correlation coefficient
computation’. PLOS ONE 20 (6): e0326090.
<https://doi.org/10.1371/journal.pone.0326090>.**

This package implements a different algorithm from the one implemented
in base R, and it reduces the complexity of the Kendall’s correlation
coefficient from O(n^2) to O(n log n) resulting in a runtime of nano
seconds or minutes instead of minutes or hours. This package is written
in C++ and uses cpp11 to export the functions to R. See the vignette for
the mathematical details.

If this software is useful to you, please consider donating on [Buy Me A
Coffee](https://buymeacoffee.com/pacha). All donations will be used to
continue improving `kendallknight`.

## Installation

You can install the released version of kendallknight from CRAN with:

``` r
install.packages("kendallknight")
```

You can install the development version of kendallknight like so:

``` r
remotes::install_github("pachadotdev/kendallknight")
```

## Examples

See the documentation and vignette: <https://pacha.dev/kendallknight/>.

## Code of Conduct

Please note that the kendallknight project is released with a
[Contributor Code of
Conduct](https://contributor-covenant.org/version/2/1/CODE_OF_CONDUCT.html).
By contributing to this project, you agree to abide by its terms.
