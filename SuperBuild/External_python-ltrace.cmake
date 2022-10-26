set(proj python-ltrace)

# Set dependency list
set(${proj}_DEPENDENCIES
  python
  python-pip
  python-setuptools
  )

if(NOT DEFINED Slicer_USE_SYSTEM_${proj})
  set(Slicer_USE_SYSTEM_${proj} ${Slicer_USE_SYSTEM_python})
endif()

# Include dependent projects if any
ExternalProject_Include_Dependencies(${proj} PROJECT_VAR proj DEPENDS_VAR ${proj}_DEPENDENCIES)

if(Slicer_USE_SYSTEM_${proj})
  foreach(module_name IN ITEMS PySide2)
    ExternalProject_FindPythonPackage(
      MODULE_NAME "${module_name}"
      REQUIRED
      )
  endforeach()
endif()

if(NOT Slicer_USE_SYSTEM_${proj})
  set(requirements_file ${CMAKE_BINARY_DIR}/${proj}-requirements.txt)
  file(WRITE ${requirements_file} [===[
  # [PySide2]
  # Hashes correspond to the following packages:
  #  - PySide2-5.15.2.1-5.15.2-cp35.cp36.cp37.cp38.cp39.cp310-none-win_amd64.whl
  #  - PySide2-5.15.2.1-5.15.2-cp35.cp36.cp37.cp38.cp39.cp310-abi3-manylinux1_x86_64.whl
  #  - PySide2-5.15.2.1-5.15.2-cp35.cp36.cp37.cp38.cp39.cp310-abi3-macosx_10_13_intel.whl
  PySide2==5.15.2.1 --hash=sha256:af6b263fe63ba6dea7eaebae80aa7b291491fe66f4f0057c0aafe780cc83da9d \
                    --hash=sha256:23886c6391ebd916e835fa1b5ae66938048504fd3a2934ae3189a96cd5ac0b46 \
                    --hash=sha256:a9e2e6bbcb5d2ebb421e46e72244a0f4fe0943b2288115f80a863aacc1de1f06
  # [/PySide2]
  # [shiboken2]
  # Hashes correspond to the following packages:
  #  - shiboken2-5.15.2.1-5.15.2-cp35.cp36.cp37.cp38.cp39.cp310-none-win_amd64.whl
  #  - shiboken2-5.15.2.1-5.15.2-cp35.cp36.cp37.cp38.cp39.cp310-abi3-manylinux1_x86_64.whl
  #  - shiboken2-5.15.2.1-5.15.2-cp35.cp36.cp37.cp38.cp39.cp310-abi3-macosx_10_13_intel.whl
  shiboken2==5.15.2.1 --hash=sha256:a0d0fdeb12b72c8af349b9642ccc67afd783dca449309f45e78cda50272fd6b7 \
                      --hash=sha256:63debfcc531b6a2b4985aa9b71433d2ad3bac542acffc729cc0ecaa3854390c0 \
                      --hash=sha256:ffd3d0ec3d508e592d7ee3885d27fee1f279a49989f734eb130f46d9501273a9
  # [/shiboken2]
  ]===])

  ExternalProject_Add(${proj}
    ${${proj}_EP_ARGS}
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${PYTHON_EXECUTABLE} -m pip install --require-hashes -r ${requirements_file}
    LOG_INSTALL 1
    DEPENDS
      ${${proj}_DEPENDENCIES}
    )

  ExternalProject_GenerateProjectDescription_Step(${proj}
    VERSION ${_version}
    )

else()
  ExternalProject_Add_Empty(${proj} DEPENDS ${${proj}_DEPENDENCIES})
endif()
