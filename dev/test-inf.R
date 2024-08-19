set.seed(321)
x <- rnorm(100)
y <- rpois(100, 2)
x[sample(1:100, 5)] <- -1 * x[sample(1:100, 5)]
y[sample(1:100, 5)] <- -1 * y[sample(1:100, 5)]
x[sample(1:100, 5)] <- Inf
x[sample(1:100, 5)] <- -Inf
y[sample(1:100, 5)] <- Inf
y[sample(1:100, 5)] <- -Inf

method <- "kendall"
use <- "pairwise.complete.obs"

na.method <- pmatch(use, c(
  "all.obs", "complete.obs", "pairwise.complete.obs",
  "everything", "na.or.complete"
))

if (is.data.frame(y)) {
  y <- as.matrix(y)
}
if (is.data.frame(x)) {
  x <- as.matrix(x)
}

if (!is.matrix(x) && is.null(y)) {
  stop("supply both 'x' and 'y' or a matrix-like 'x'")
}

if (!(is.numeric(x) || is.logical(x))) {
  stop("'x' must be numeric")
}

stopifnot(is.atomic(x))

if (!is.null(y)) {
  if (!(is.numeric(y) || is.logical(y))) {
    stop("'y' must be numeric")
  }
  stopifnot(is.atomic(y))
}

if (length(x) == 0L || length(y) == 0L) {
  stop("both 'x' and 'y' must be non-empty")
}

matrix_result <- is.matrix(x) || is.matrix(y)
if (!is.matrix(x)) {
  x <- matrix(x, ncol = 1L)
}

if (!is.matrix(y)) {
  y <- matrix(y, ncol = 1L)
}

ncx <- ncol(x)
ncy <- ncol(y)

r <- matrix(0, nrow = ncx, ncol = ncy)

for (i in seq_len(ncx)) {
  for (j in seq_len(ncy)) {
    x2 <- x[, i]
    y2 <- y[, j]
    ok <- complete.cases(x2, y2)
    x2 <- rank(x2[ok])
    y2 <- rank(y2[ok])
    r[i, j] <- if (any(ok)) {
      .Call(stats:::C_cor, x2, y2, 1L, method == "kendall")
    } else {
      NA
    }
  }
}

r

length(x)
length(x2)

kendall_cor(x, y)
kendall_cor(x2, y2)

cor(x2, y2, method = "kendall")
