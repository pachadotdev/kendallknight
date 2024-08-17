test_that("same result as base R, no NAs", {
  x <- 1:2
  expect_equal(kendall_cor(x, x), cor(x, x, method = "kendall"))

  x <- 1:3
  expect_equal(kendall_cor(x, x), 1)

  x <- rep(1, 3)
  expect_warning(kendall_cor(x, x), "zero variance")

  x <- c(1, 0, 2)
  y <- c(5, 3, 4)
  expect_equal(kendall_cor(x, y), cor(x, y, method = "kendall"))

  k1 <- kendall_cor_test(x, y, alternative = "two.sided")
  k2 <- cor.test(x, y, method = "kendall", alternative = "two.sided")
  expect_equal(k1$statistic, unname(k2$estimate))
  expect_equal(k1$p_value, k2$p.value)

  x <- 1:3
  y <- 3:1
  expect_equal(kendall_cor(x, y), cor(x, y, method = "kendall"))

  x <- c(1, NA, 2)
  y <- 3:1
  expect_equal(kendall_cor(x, y), cor(x, y, method = "kendall", use = "pairwise.complete.obs"))

  set.seed(123)
  x <- rnorm(100)
  y <- rpois(100, 2)
  expect_equal(kendall_cor(x, y), cor(x, y, method = "kendall"))

  k1 <- kendall_cor_test(x, y, alternative = "two.sided")
  k2 <- cor.test(x, y, method = "kendall", alternative = "two.sided")
  expect_equal(k1$statistic, unname(k2$estimate))
  expect_equal(k1$p_value, k2$p.value)

  k1 <- kendall_cor_test(x, y, alternative = "greater")
  k2 <- cor.test(x, y, method = "kendall", alternative = "greater")
  expect_equal(k1$statistic, unname(k2$estimate))
  expect_equal(k1$p_value, k2$p.value)

  k1 <- kendall_cor_test(x, y, alternative = "less")
  k2 <- cor.test(x, y, method = "kendall", alternative = "less")
  expect_equal(k1$statistic, unname(k2$estimate))
  expect_equal(k1$p_value, k2$p.value)
  
  x <- rnorm(1e3)
  y <- rpois(1e3, 2)
  expect_equal(kendall_cor(x, y), cor(x, y, method = "kendall"))
})

test_that("computation time is less than base R, no NAs", {
  set.seed(123)
  x <- rnorm(1000)
  y <- rpois(1000, 2)
  
  t_kendall <- c()
  for (i in 1:100) {
    t1 <- Sys.time()
    kendall_cor(x, y)
    t2 <- Sys.time()
    t_kendall[i] <- t2 - t1
  }
  t_kendall <- median(t_kendall)

  t_cor <- c()
  for (i in 1:100) {
    t1 <- Sys.time()
    cor(x, y, method = "kendall")
    t2 <- Sys.time()
    t_cor[i] <- t2 - t1
  }
  t_cor <- median(t_cor)

  expect_lt(t_kendall, t_cor)
})

test_that("incompatible dimensions gives an error", {
  x <- 1:3
  y <- NA
  expect_error(kendall_cor(x, y), "same length")

  y <- matrix(1:3, ncol = 1)
  expect_equal(kendall_cor(x, y), 1)

  y <- matrix(1:3, nrow = 1)
  expect_equal(kendall_cor(x, y), 1)

  y <- matrix(1:4, nrow = 1)
  expect_error(kendall_cor(x, y), "same length")

  y <- matrix(1:6, nrow = 2)
  expect_error(kendall_cor(x, y), "y must be a uni-dimensional vector")

  x <- matrix(1:6, nrow = 2)
  y <- 1:3
  expect_error(kendall_cor(x, y), "x must be a uni-dimensional vector")

  x <- matrix(1:3, nrow = 1)
  y <- 1:3
  expect_equal(kendall_cor(x, y), 1)
})

test_that("passing strings gives an error", {
  x <- letters[1:3]
  expect_error(kendall_cor(x, x), "must be numeric")
})

test_that("less than 2 usable observations gives an error", {
  x <- 1:3
  y <- c(1, NA, NA)
  expect_error(kendall_cor(x, y), "non-null")

  x <- 1:3
  y <- c(1, NaN, NA)
  expect_error(kendall_cor(x, y), "non-null")

  x <- 1:3
  y <- c(1, -Inf, NA)
  expect_error(kendall_cor(x, y), "non-null")

  x <- 1:3
  y <- c(1, NA, Inf)
  expect_error(kendall_cor(x, y), "non-null")
})

test_that("constant vectors give a warning", {
  x <- rep(1, 3)
  y <- 1:3
  expect_warning(kendall_cor(x, y), "zero variance")
  expect_warning(kendall_cor(y, x), "zero variance")
  expect_warning(kendall_cor_test(x, y), "zero variance")
  expect_warning(kendall_cor_test(y, x), "zero variance")
})

test_that("adding random NAs gives the same result as base R", {
  set.seed(123)
  x <- rnorm(100)
  y <- rpois(100, 2)
  x[sample(1:100, 5)] <- NA
  y[sample(1:100, 5)] <- NA
  expect_equal(kendall_cor(x, y), cor(x, y, method = "kendall", use = "pairwise.complete.obs"))
})


test_that("check particular case for q when testing hypothesis", {
  # change x and y until q < n * (n - 1) / 4
  # x <- as.numeric(c(1, 2, 3, 4, 5))
  # y <- as.numeric(c(5, 2, 3, 2, 1))
  # arr <- cbind(x, y)
  # r <- kendall_cor_(arr)
  # n <- nrow(arr)
  # q <- round((r + 1) * n * (n - 1) / 4)
  # q
  # n * (n - 1) / 4

  x <- c(1, 2, 3, 4, 5)
  y <- c(5, 2, 3, 2, 1)
  expect_type(kendall_cor_test(x, y, alternative = "two.sided"), "list")
})