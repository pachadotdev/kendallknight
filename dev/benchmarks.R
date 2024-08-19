library(bench)
library(dplyr)
library(purrr)
library(kendallknight)

set.seed(200100)

n <- seq(1,10,1) * 10^(4)

map(
  n,
  function(n) {
    x <- rnorm(n)
    y <- rpois(n, 2)

    res <- bench::mark(
      kendall_cor(x, y),
      cor(x, y, method = "kendall"),
      iterations = 3
    )

    res %>%
      select(expression, median, mem_alloc) %>%
      mutate(nobs = length(x)) %>%
      saveRDS(paste0("dev/benchmarks_", n, ".rds"))
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
