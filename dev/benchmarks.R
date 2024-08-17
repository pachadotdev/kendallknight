library(bench)
library(dplyr)
library(purrr)
library(kendallknight)

set.seed(200100)

n <- 10^(2:5)

out <- map_df(
  n,
  function(n) {
    x <- rnorm(n)
    y <- rpois(n, 2)

    res <- bench::mark(
      kendall_cor(x, y),
      cor(x, y, method = "kendall"),
      iterations = 5
    )

    res %>%
      select(expression, median, mem_alloc) %>%
      mutate(nobs = length(x))
  }
)

saveRDS(out, "dev/benchmarks.rds")

out <- readRDS("dev/benchmarks.rds")

out
