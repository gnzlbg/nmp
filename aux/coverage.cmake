################################################################################
# Coverage target: make cov
################################################################################

# LCov (coverage): make cov
add_custom_command(OUTPUT coverage COMMAND
  ${PROJECT_SOURCE_DIR}/build_system/coverage.sh "${PROJECT_SOURCE_DIR}/include")
add_custom_target(cov DEPENDS coverage)
