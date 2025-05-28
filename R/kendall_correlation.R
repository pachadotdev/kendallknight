# Correlation ----

#' @title Kendall Correlation
#'
#' @description `kendall_cor()` calculates the Kendall correlation
#'  coefficient between two numeric vectors. It uses the algorithm described in
#'  Knight (1966), which is based on the number of concordant and discordant
#'  pairs. The computational complexity of the algorithm is
#'  \eqn{O(n \log(n))}{O(n log(n))}, which is faster than the base R
#'  implementation in `stats::cor(..., method = "kendall")`
#'  that has a computational complexity of \eqn{O(n^2)}{O(n^2)}. For small
#'  vectors (i.e., less than 100 observations), the time difference is
#'  negligible. However, for larger vectors, the difference can be substantial.
#'
#'  By construction, the implementation drops missing values on a pairwise
#'  basis. This is the same as using
#'  `stats::cor(..., use = "pairwise.complete.obs")`.
#' 
#' @param x a numeric vector or matrix.
#' @param y an optional numeric vector.
#' 
#' @return A numeric value between -1 and 1.
#' 
#' @references Knight, W. R. (1966). "A Computer Method for Calculating
#'  Kendall's Tau with Ungrouped Data". Journal of the American Statistical
#'  Association, 61(314), 436–439.
#'
#'  Abrevaya J. (1999). Computation of the Maximum Rank Correlation Estimator.
#'  Economic Letters 62, 279-285.
#'
#'  Christensen D. (2005). Fast algorithms for the calculation of Kendall's Tau.
#'  Journal of Computational Statistics 20, 51-62.
#'
#'  Emara (2024). Khufu: Object-Oriented Programming using C++
#'
#' @examples
#' # input vectors -> scalar output
#' x <- c(1, 0, 2)
#' y <- c(5, 3, 4)
#' kendall_cor(x, y)
#' 
#' # input matrix -> matrix output
#' x <- mtcars[, c("mpg", "cyl")]
#' kendall_cor(x)
#'
#' @export
kendall_cor <- function(x, y = NULL) {
  if (!is.null(y)) {
    if (is.matrix(x) || is.data.frame(x)) {
      if (ncol(x) != 1L) {
        stop("x must be one-dimensional when y is not NULL")
      }
    }
    x2 <- NA
    y2 <- NA
    n <- NA
    ok <- check_data_(x, y)
    if (isFALSE(ok)) {
      return(NA)
    }
    return(kendall_cor_(x2, y2))
  } else {
    if (!is.matrix(x) && !is.data.frame(x)) {
      stop("x must be a matrix or data.frame when y is NULL")
    }
    # if (ncol(x) < 2L) {
    #   stop("x must be a matrix with at least 2 columns when y is NULL")
    # }

    x2 <- NA
    y2 <- NA
    n <- NA
    m <- ncol(x)
    res <- matrix(NA, nrow = m, ncol = m)
    for (i in seq_len(m)) {
      for (j in seq_len(m)) {
        if (i == j) {
          res[i, j] <- 1
          next
        }
        if (i < j) {
          x2 <- x[, i]
          y2 <- x[, j]
          ok <- check_data_(x2, y2)
          if (isFALSE(ok)) {
            res[i, j] <- NA
            res[j, i] <- NA
          } else {
            cor_value <- kendall_cor_(x2, y2)
            res[i, j] <- cor_value
            res[j, i] <- cor_value
          }
        }
      }
    }
    return(res)
  }
}

# Inference ----

#' @title Kendall Correlation Test
#' 
#' @description `kendall_cor_test()` calculates p-value for the the
#'  Kendall correlation using the exact values when the number of observations
#'  is less than 50. For larger samples, it uses an approximation as in base R.
#'
#' @param x a numeric vector.
#' @param y a numeric vector.
#' @param alternative a character string specifying the alternative hypothesis.
#'  The possible values are `"two.sided"`, `"greater"`, and `"less"`.
#' @param conf.level confidence level for the returned confidence interval.
#'  Must be a single number between 0 and 1. Default is 0.95.
#' 
#' @return A list with the following components:
#' \item{statistic}{The Kendall correlation coefficient.}
#' \item{p_value}{The p-value of the test.}
#' \item{alternative}{A character string describing the alternative hypothesis.}
#'
#' @references Knight, W. R. (1966). "A Computer Method for Calculating
#'  Kendall's Tau with Ungrouped Data". Journal of the American Statistical
#'  Association, 61(314), 436–439.
#'
#'  Abrevaya J. (1999). Computation of the Maximum Rank Correlation Estimator.
#'  Economic Letters 62, 279-285.
#'
#'  Christensen D. (2005). Fast algorithms for the calculation of Kendall's Tau.
#'  Journal of Computational Statistics 20, 51-62.
#'
#'  Emara (2024). Khufu: Object-Oriented Programming using C++
#'
#' @examples
#' x <- c(1, 0, 2)
#' y <- c(5, 3, 4)
#' kendall_cor_test(x, y)
#' 
#' @export
kendall_cor_test <- function(x, y,
  alternative = c("two.sided", "greater", "less"),
  conf.level = 0.95) {
  alternative <- match.arg(alternative)

  if (!is.numeric(conf.level) || length(conf.level) != 1 ||
    conf.level <= 0 || conf.level >= 1) {
    stop("'conf.level' must be a single number between 0 and 1")
  }

  x2 <- NA
  y2 <- NA
  n <- NA

  ok <- check_data_(x, y)
  if (isFALSE(ok)) {
    return(NA)
  }

  r <- kendall_cor_(x2, y2)
  n <- length(x2) # Ensure n is correctly assigned

  xties <- table(x[duplicated(x)]) + 1
  yties <- table(y[duplicated(y)]) + 1
  T0 <- n * (n - 1) / 2
  T1 <- sum(xties * (xties - 1)) / 2
  T2 <- sum(yties * (yties - 1)) / 2
  v0 <- n * (n - 1) * (2 * n + 5)
  vt <- sum(xties * (xties - 1) * (2 * xties + 5))
  vu <- sum(yties * (yties - 1) * (2 * yties + 5))
  v1 <- sum(xties * (xties - 1)) * sum(yties * (yties - 1))
  v2 <- sum(xties * (xties - 1) * (xties - 2)) * sum(yties * (yties - 2))
  var_S <- (v0 - vt - vu) / 18 + v1 / (2 * n * (n - 1)) + v2 / (9 * n * (n - 1) * (n - 2))
  se <- sqrt(var_S) / sqrt((T0 - T1) * (T0 - T2))

  if (n < 50) {
    q <- round((r + 1) * n * (n - 1) / 4)
    pv <- switch(alternative,
      "two.sided" = {
        if (q > n * (n - 1) / 4) {
          p <- 1 - pkendall_(q - 1, n)
        } else {
          p <- pkendall_(q, n)
        }
        min(2 * p, 1)
      },
      "greater" = 1 - pkendall_(q - 1, n),
      "less" = pkendall_(q, n)
    )
  } else {
    S <- r * sqrt((T0 - T1) * (T0 - T2)) / sqrt(var_S)
    pv <- switch(alternative,
      "two.sided" = 2 * min(pnorm(S), pnorm(S, lower.tail = FALSE)),
      "greater" = pnorm(S, lower.tail = FALSE),
      "less" = pnorm(S)
    )
  }

  z <- qnorm((1 + conf.level) / 2)

  ci <- switch(alternative,
    "two.sided" = c(max(-1, r - z * se), min(1, r + z * se)),
    "greater" = c(max(-1, r - z * se), 1),
    "less" = c(-1, min(1, r + z * se))
  )

  attr(ci, "conf.level") <- conf.level

  alt <- switch(alternative,
    "two.sided" = "true tau is not equal to 0",
    "greater" = "true tau is greater than 0",
    "less" = "true tau is less than 0"
  )

  result <- list(
    statistic = c(tau = r),
    p.value = pv,
    conf.int = ci,
    alternative = alt,
    method = "Kendall's rank correlation tau",
    data.name = paste(deparse(substitute(x)), "and", deparse(substitute(y)))
  )
  class(result) <- "htest"
  return(result)
}

# Internals ----

warn_variance <- function(v) {
  res <- all.equal(var(v), 0, check.class = FALSE)
  if (isTRUE(res)) {
    # warning(paste("X has zero variance"))
    warning(paste(deparse(substitute(v)), "has zero variance"))
    return(FALSE)
  }
  TRUE
}

check_data_ <- function(x,y) {
  if (is.matrix(x)) {
    mx <- min(dim(x))
    # if (mx == 1L && (nrow(x) < ncol(x))) {
    #   x <- as.vector(x)
    # } else if (mx != 1L) {
    #   stop_unidimensional("x")
    # }
  }

  if (is.matrix(y)) {
    my <- min(dim(y))
    if (my == 1L && (nrow(y) < ncol(y))) {
      y <- as.vector(y)
    } else if (my != 1L) {
      stop("y must be a uni-dimensional vector or coercible to a vector")
    }
  }

  if (length(x) != length(y)) {
    stop("x and y must have the same length")
  }

  if (!is.numeric(x) || !is.numeric(y)) {
    stop("x and y must be numeric")
  }

  ok <- complete.cases(x, y)
  x <- rank(x[ok])
  y <- rank(y[ok])

  n <- length(x)

  if (n < 2) {
    stop("x and y must have at least 2 non-null observations")
  }

  ok <- c(warn_variance(x), warn_variance(y))
  if (!all(ok)) {
    return(FALSE)
  }

  assign("x2", x, envir = parent.frame())
  assign("y2", y, envir = parent.frame())
  assign("n", n, envir = parent.frame())

  TRUE
}
