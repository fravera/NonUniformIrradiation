cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(NonUniformIrradiatioin)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(NonUniformIrradiation NonUniformIrradiation.cc ${sources} ${headers})
target_link_libraries(NonUniformIrradiation ${Geant4_LIBRARIES})
target_link_libraries(NonUniformIrradiation cadmesh)

set(EXAMPLEB1_SCRIPTS
  init_vis.mac
  vis.mac
  data/SpectrumLegnaroW.dat
  data/*stl
  run1.mac
)

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

install(TARGETS NonUniformIrradiation DESTINATION bin)