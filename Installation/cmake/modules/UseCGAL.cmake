#
# UseCGAL.cmake can be included in a project to set the needed compiler and linker
# settings to use CGAL in a program.
#
# The variables used here are defined in the CGALConfig.cmake generated when CGAL was installed.
#
#

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CGAL_CMAKE_MODULE_PATH})

if(NOT USE_CGAL_FILE_INCLUDED) 
  set(USE_CGAL_FILE_INCLUDED 1)

  include(CGALcommon)
  
  if ( IS_TOP_LEVEL )
    include(FindCGALDependencies)
    include(CGAL_SetupFlags)
    include(GeneratorSpecificSettings)
  endif()
  
  include_directories (${CGAL_BINARY_DIR}/include) # Plaform-specific include folder where compiler_config.h is located
  
  set( CGAL_LIBRARIES ${CGAL_LIBRARY} )
  
  foreach ( CGAL_COMPONENT ${CGAL_FIND_COMPONENTS} )
    
    set( CGAL_LIBRARIES                ${CGAL_LIBRARIES}                ${CGAL_${CGAL_COMPONENT}_LIBRARY}                  )
    set( CGAL_3RD_PARTY_INCLUDE_DIRS   ${CGAL_3RD_PARTY_INCLUDE_DIRS}   ${CGAL_${CGAL_COMPONENT}_3RD_PARTY_INCLUDE_DIRS}   )
    set( CGAL_3RD_PARTY_DEFINITIONS    ${CGAL_3RD_PARTY_DEFINITIONS}    ${CGAL_${CGAL_COMPONENT}_3RD_PARTY_DEFINITIONS}    )
    set( CGAL_3RD_PARTY_LIBRARIES_DIRS ${CGAL_3RD_PARTY_LIBRARIES_DIRS} ${CGAL_${CGAL_COMPONENT}_3RD_PARTY_LIBRARIES_DIRS} )
    set( CGAL_3RD_PARTY_LIBRARIES      ${CGAL_3RD_PARTY_LIBRARIES}      ${CGAL_${CGAL_COMPONENT}_3RD_PARTY_LIBRARIES}      )
    
  endforeach()
    
  include_directories (${CGAL_INCLUDE_DIRS} ${CGAL_3RD_PARTY_INCLUDE_DIRS} )     
          
  add_definitions(${CGAL_DEFINITIONS} ${CGAL_3RD_PARTY_DEFINITIONS} )
  
  link_directories( ${CGAL_LIBRARIES_DIR} ${CGAL_3RD_PARTY_LIBRARIES_DIRS} )
  
endif()
