# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/PathFinding_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/PathFinding_autogen.dir/ParseCache.txt"
  "PathFinding_autogen"
  )
endif()
