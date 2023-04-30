# CHANGELOG

## [0.1.2] - 2023-04-30

### Fixed

- Fix various issues with processing conditional directives
- Fix `path_dirname` not correctly null-terminating returned string
- Avoid printing a superfluous newline at the end of the generated output

## [0.1.1] - 2023-04-28

### Fixed

- Fix `endif` derictives being displayed if the conditional was resolved.

## [0.1.0] - 2023-04-28

_The first release of PCPP_

### Added

- Process `#define` directives.
- Process `#undef` directives.
- Process `#ifdef`, `#elifdef` directives.
- Process `#ifndef`, `#elifndef` directives.
- Process `#else`, `#endif` directives.
- Print the processed file to standard output.
