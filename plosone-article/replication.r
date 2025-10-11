# 1: packages ----

if (!require("kendallknight")) install.packages("kendallknight")
if (!require("tradepolicy")) install.packages("tradepolicy")
if (!require("lfe")) install.packages("lfe")
if (!require("bench")) install.packages("bench")

# it cannot be installed in the Niagara cluster (MKL link error)
# if (!require("pcaPP")) install.packages("pcaPP")

if (!require("Kendall")) install.packages("Kendall")

library(kendallknight)
library(tradepolicy)
library(lfe)
library(bench)
# library(pcaPP)
library(Kendall)

# 2: examples ----

## simple examples ----

cigarettes

kendall_cor(cigarettes$life_expectancy, cigarettes$cigarettes_per_day)

kendall_cor_test(
  cigarettes$life_expectancy,
  cigarettes$cigarettes_per_day,
  alternative = "less"
)

## pseudo R2 ----

data8694 <- subset(agtpa_applications, year %in% seq(1986, 1994, 4))

ftime <- "data/rsq2.rds"

if (!file.exists(ftime)) {
  t1 <- Sys.time()
  fit <- fepois(
    trade ~ dist + cntg + lang + clny + rta |
      as.factor(paste0(exporter, year)) +
        as.factor(paste0(importer, year)),
    data = data8694
  )
  t2 <- Sys.time()
  psr <- (cor(data8694$trade, fit$fitted.values, method = "kendall"))^2
  t3 <- Sys.time()
  psr2 <- (kendall_cor(data8694$trade, fit$fitted.values))^2
  t4 <- Sys.time()

  tmodel <- as.numeric(t2 - t1)
  tpsr <- as.numeric(t3 - t2)
  tpsr2 <- as.numeric(t4 - t3)

  res <- list(tmodel = tmodel, tpsr = tpsr, tpsr2 = tpsr2)
  saveRDS(res, ftime)
} else {
  res <- readRDS(ftime)
  tmodel <- res$tmodel
  tpsr <- res$tpsr
  tpsr2 <- res$tpsr2
}

100 * tpsr / tmodel
100 * tpsr2 / tmodel

# 3: benchmark ----

len <- 1:10 * 10000

res <- list()

for (l in len) {
  set.seed(123)
  x <- rnorm(l) + rpois(l, 1)

  set.seed(321)
  y <- rnorm(l) + rpois(l, 2)
  
  b <- bench::mark(
    round(kendallknight::kendall_cor(x, y), 3),
    round(as.numeric(Kendall::Kendall(x, y)$tau), 3),
    # round(pcaPP::cor.fk(x, y), 3),
    round(cor(x, y, method = "kendall"), 3)
  )
  
  b$nobs = l

  pos <- which(len == l)

  res[[pos]] <- b
}

saveRDS(res, "data/benchmarks_all.rds")
