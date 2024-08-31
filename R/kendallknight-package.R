#' @importFrom stats complete.cases pnorm var
#' @useDynLib kendallknight, .registration = TRUE
"_PACKAGE"

#' Life expectancy and cigarettes per day
#' 
#' A dataset containing life expectancy and cigarettes per day.
#' 
#' @format A data frame with 15 rows and 2 variables:
#' \describe{
#'  \item{life_expectancy}{Life expectancy in years.}
#' \item{cigarettes_per_day}{Cigarettes smoked per day.}
#' }
#' 
#' @source
#' Real Statistics Using Excel (\url{https://real-statistics.com/correlation/kendalls-tau-correlation/kendalls-correlation-testing-with-ties/}).
#' 
#' @examples
#' cigarettes
"cigarettes"
