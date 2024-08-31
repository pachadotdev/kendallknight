## R CMD check results

0 errors | 0 warnings | 1 note

* This is a new release.
* David Vaughan from Posit indicates that the warning I see with R-devel is
  because of a reported non-API call in cpp11, which will be fixed in the next
  release of cpp11 (~1 month)
* The warning is: Found non-API calls to R: 'SETLENGTH', 'SET_GROWABLE_BIT',
  'SET_TRUELENGTH'. Compiled code should not call non-API entry points in R.
* I cannot fix the warning on my end without a pull request to cpp11.
* I have also replaced  \code{\link[stats]{XYZ}} into `PKG::XYZ()` in the
  documentation.
