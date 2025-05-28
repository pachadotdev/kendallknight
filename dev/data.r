library(dplyr)
library(spuriouscorrelations)

arcade <- spurious_correlations %>%
  filter(var2_short == "Arcade revenue") %>%
  select(year, var1, var2, var1_value, var2_value)

unique(arcade$var1)
unique(arcade$var2)

arcade <- spurious_correlations %>%
  filter(var2_short == "Arcade revenue") %>%
  select(year, doctorates = var1_value, revenue = var2_value)

use_data(arcade, overwrite = TRUE)
