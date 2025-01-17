load_all()

a <- c(4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2)
b <- c(3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6)

kendall_cor(a, b) # 0.0971

kendall_cor_test(a, b)

# $statistic
# [1] 0.2670642

# $p_value
# [1] -54.29205

# $alternative
# [1] "alternative hypothesis: true tau is not equal to 0"

cor(a, b, method = "kendall") # 0.2671

pcaPP::cor.fk(a, b) # 0.2671
