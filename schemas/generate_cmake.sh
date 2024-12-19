#!/bin/bash

CMAKE_CONTENT=$'# THIS FILE IS AUTOGENERATED. DON\'T EDIT\n\n'
CMAKE_CONTENT+=$'add_library(${PROJECT_NAME}_gen OBJECT\n'

FILES="$(find ./src/gen -name "*.hpp" -o -name "*.cpp" -o -name "*.ipp")"
CMAKE_CONTENT+="$FILES"

CMAKE_CONTENT+=$')\n'
CMAKE_CONTENT+=$'target_link_libraries(${PROJECT_NAME}_gen PUBLIC userver::postgresql userver::chaotic)\n'
CMAKE_CONTENT+=$'target_include_directories(${PROJECT_NAME}_gen PUBLIC src/gen)'

echo "$CMAKE_CONTENT" > src/gen/CMakeLists.txt