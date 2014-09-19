#!/bin/bash

make tests
lcov --capture --base-directory $1 --gcov-tool gcov-4.8 --directory . --output-file coverage.info --no-external
genhtml coverage.info --output-directory coverage_results --title "NMP Test Coverage" --prefix $1 --legend
