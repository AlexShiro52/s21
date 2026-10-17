#!/bin/sh
# Match Clang's coverage format instead of using a different GCC gcov version.
if command -v llvm-cov >/dev/null 2>&1; then
  exec llvm-cov gcov "$@"
fi
exec xcrun llvm-cov gcov "$@"
