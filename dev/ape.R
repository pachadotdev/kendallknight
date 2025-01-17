library(bench)
library(dplyr)
library(purrr)
load_all()

n <- c(10, 100, 1000, 10000)

# absolute percentage error
ape <- function(x, y) {
  abs((x - y) / x) * 100
}

errors <- map_dbl(
  n,
  function(n) {
    set.seed(200100)
    
    x <- rnorm(n)
    y <- rnorm(n)
    z <- rbinom(n / 1, 1, 0.5)
    y[z == 1] <- x[z == 1]

    c1 <- cor(x, y, method = "kendall")
    c2 <- kendall_cor(x, y)

    ape(c1, c2)
  }
)

finp <- list.files("dev", pattern = "benchmarks_[0-9]", full.names = TRUE)

dout <- map_df(
  finp,
  ~ readRDS(.x) %>%
    mutate(
      median = as.numeric(as_bench_time(median)),
      mem_alloc = as.numeric(mem_alloc) / 1024^2
    ) %>%
    select(expression, nobs, median, mem_alloc) %>%
    group_by(expression, nobs) %>%
    summarise(
      median = min(median),
      mem_alloc = min(mem_alloc)
    ) %>%
    ungroup() %>%
    arrange(expression, nobs)
)

saveRDS(dout, "dev/benchmarks_all.rds")
