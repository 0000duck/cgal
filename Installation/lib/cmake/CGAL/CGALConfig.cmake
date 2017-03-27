# 
# This file is the CGALConfig.cmake for a pure header-only CGAL installion
#

#set(CGAL_Boost_USE_STATIC_LIBS ON)
set(CGAL_AUTO_LINK_ENABLED)

if(CGALConfig_included)
  return()
endif()

set(CGALConfig_included TRUE)

get_filename_component(CGAL_CONFIG_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

set(CGAL_HEADER_ONLY TRUE)

# Save the current source directory. That variable can be changed by
# a `CMakeLists.txt`, for `CMakeLists.txt` files that are created in
# the binary directory.
set(CGAL_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function(_detect_branch_build VAR_NAME)
  if(IS_DIRECTORY ${CGAL_CONFIG_DIR}/../../../../Installation)
    set(${VAR_NAME} TRUE PARENT_SCOPE)
  else()
    set(${VAR_NAME} FALSE PARENT_SCOPE)
  endif()
endfunction()

_detect_branch_build(BRANCH_BUILD)
if(BRANCH_BUILD)
  set(CGAL_ROOT ${CGAL_CONFIG_DIR}/../../../..)
  set(CGAL_INSTALLATION_PACKAGE_DIR ${CGAL_ROOT}/Installation)
  set(CGAL_GRAPHICSVIEW_PACKAGE_DIR ${CGAL_ROOT}/GraphicsView)
  file(GLOB packages_dirs ${CGAL_ROOT}/*)
#  message("packages_dirs: ${packages_dirs}")
  foreach(package_dir ${packages_dirs})
    set(inc_dir ${package_dir}/include)
    if(IS_DIRECTORY ${inc_dir}
	AND IS_DIRECTORY ${package_dir}/package_info)
      list(APPEND CGAL_INCLUDE_DIRS ${inc_dir})
      if(EXISTS ${inc_dir}/CGAL/config.h)
	set(CGAL_FOUND TRUE)
      endif()
    endif()
  endforeach()
else()
  set(CGAL_ROOT ${CGAL_CONFIG_DIR}/../../..)
  set(CGAL_INSTALLATION_PACKAGE_DIR ${CGAL_ROOT})
  set(CGAL_GRAPHICSVIEW_PACKAGE_DIR ${CGAL_ROOT})
  set(CGAL_INCLUDE_DIRS ${CGAL_ROOT}/include)
  if(EXISTS ${CGAL_ROOT}/include/CGAL/config.h)
    set(CGAL_FOUND TRUE)
  endif()
endif()

if(NOT CGAL_FOUND)
  return()
endif()

set(CGAL_MODULES_DIR ${CGAL_CONFIG_DIR}/../../../cmake/modules)
list(APPEND CMAKE_MODULE_PATH ${CGAL_MODULES_DIR})

include( ${CGAL_MODULES_DIR}/CGAL_SCM.cmake )
CGAL_detect_git(${CGAL_CONFIG_DIR}/../../../..)

include( ${CGAL_MODULES_DIR}/CGAL_SetupCGALDependencies.cmake )

#
# Define the CGAL targets and theirs CGAL:: aliases
#
foreach(cgal_lib CGAL CGAL_Core CGAL_ImageIO CGAL_Qt5)
  if(${cgal_lib}_FOUND)
    add_library(${cgal_lib} INTERFACE)
    add_library(CGAL::${cgal_lib} ALIAS ${cgal_lib})
    include(CGAL_Setup${cgal_lib}Dependencies)
  endif()
endforeach()

# this include has to be after the loop that includes the
# `CGAL_Setup${cgal_lib}Dependencies` files
include(CGAL_setup_target_dependencies)

foreach(cgal_lib CGAL CGAL_Core CGAL_ImageIO CGAL_Qt5)
  if(${cgal_lib}_FOUND)
    CGAL_setup_target_dependencies(${cgal_lib} INTERFACE)
  endif()
endforeach()

target_compile_definitions(CGAL INTERFACE CGAL_HEADER_ONLY=1)
target_compile_definitions(CGAL INTERFACE CGAL_HEADER_ONLY_STEP_2=1)

#
#
#

include(${CGAL_MODULES_DIR}/CGAL_CreateSingleSourceCGALProgram.cmake)
include(${CGAL_MODULES_DIR}/CGAL_Macros.cmake)
