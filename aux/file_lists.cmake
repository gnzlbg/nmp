################################################################################
# Exports lists of files required by some targets
################################################################################

# List of include files:
file(GLOB_RECURSE include_files "include/*.hpp")
set(include_file_list ${include_files})
string(REPLACE ";" " " include_file_list "${include_file_list}")

# List of files to be formated:
file(GLOB_RECURSE test_header_files "test/*.hpp")
file(GLOB_RECURSE test_src_files "test/*.cpp")
set(fmt_file_list include_file_lis)
list(APPEND fmt_file_list ${test_header_files} ${test_src_files})
string(REPLACE ";" " " fmt_file_list "${fmt_file_list}")
