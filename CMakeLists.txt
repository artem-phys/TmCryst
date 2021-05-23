cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(TmCryst)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()


include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(TmCryst TmCryst.cpp ${sources} ${headers})
target_link_libraries(TmCryst ${Geant4_LIBRARIES})

set(EXAMPLE1_SCRIPTS
  )
  file(COPY macros DESTINATION ${PROJECT_BINARY_DIR})


foreach(_script ${EXAMPLE1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()



add_custom_target(ex1 DEPENDS TmCryst)
install(TARGETS TmCryst DESTINATION bin)
