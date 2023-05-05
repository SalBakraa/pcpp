# CHANGELOG

## [0.3.1] - 2023-05-05

### Changed

- Backslash newlines in code are maintained as is now.

## [0.3.0] - 2023-05-04

### Changed

- **Breaking:** `--only-process` and `--process-all` now only control macros that are in conditionals.

### Added

- Add `--only-define` flag to define a list of macros to process in define derictives.
- Add `--define-all` flag to process all define directives.
- Add `--only-undef` flag to define a list of macros to process in undef derictives.
- Add `--undef-all` flag to process all undef directives.
- Add `--only` & `--all` flag as replacement to `--only-process` & `--process-all` respectively.
- Add `--ignore-undetermined` flag as replacement to avoid processing undetermined conditionals.

## [0.2.3] - 2023-05-02

### Added

- Add `-h` and `--help` flag to print the program's usage.

## [0.2.2] - 2023-05-02

### Added

- Add `--implicitly-undef` flag to treate undetermined macros as undefined.
- Add `--line-around-include` flag to surround included code with line directives.
- Add `-o` and `--output` flags to redired `pcpp`'s output to a file.

## [0.2.1] - 2023-05-02

### Fixed

- Fix segfault when parsing files with more than 13 macros.

## [0.2.0] - 2023-05-02

#### Changed

- **Breaking:** input file is not processed by default.

### Fixed

- Correctly parse multi-line comments.
- Correctly parse arguments in function-like macro definitions.
- Verify `#define` directive grammar even if it was not parsed.

### Added

- Add `--process-only` flag to define a list of macros to process.
- Add `--process-all` flag to process all macros.
- Add `--include-only` flag to define a list of files to be expanded and included.
- Add `--include-all` flag to include all files.
- Add `-D` and `--define` flags to allow the user to define macros on the command line.
- Add `-U` and `--undef` flags to allow the user to undefine macros on the command line.
- Add `--conflict` flag to allow the user to define a strategies for resolving definition conflicts.

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
- Process `#include` directives.
- Print the processed file to standard output.
