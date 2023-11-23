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
  camelot-py==0.10.1
  cftime==1.6.0
  chardet==5.0.0
  cryptography==35.0.0
  cudnn-python-wrappers==1.0.0
  Cython==0.29.28
  dask-image==0.4.0
  dask[complete]==2.30.0
  detect_delimiter==0.1.1
  distinctipy==1.1.5
  dlisio==0.3.5
  h5py==3.6.0
  humanize==3.13.1
  Jinja2==2.11.1
  joblib==1.1.
  lasio==0.25.1
  loguru==0.6.0
  markdown2==2.4.2
  markupsafe==2.0.1
  matplotlib==3.5.1
  mkl==2022.2.1
  monailabel==0.6.0
  natsort==6.2.0
  netCDF4==1.5.4
  numba==0.56.2
  numexpr==2.7.2
  numpy==1.23.1
  opencv-python-headless==4.5.5.64
  scikit-fmm==2022.3.26 # Workaround for failed wheels bulding on porespy package install in linux
  openpnm==3.1.1
  pandas==1.4.2
  paramiko==2.10.3
  pathvalidate==2.5.0
  pint==0.19.2
  # porespy @handle-unconnected-networks
  git+https://github.com/Arenhart/porespy.git@f79c8d7e6e49159e3826d5e4ce32981ec9b05100
  psutil==5.9.0
  # pyedt @develop
  git+https://github.com/ltracegeo/pyedt.git@develop
  pygments==2.11.2
  pyqtgraph==0.12.4
  PySide2==5.15.2
  pytesseract==0.3.7
  pywin32==228 ; sys_platform == "win32"
  pyzmq==22.3.0
  recordtype==1.3.0
  scikit-gstat==1.0.1
  scikit-image==0.19.2
  scikit-learn==1.0.2
  scipy==1.8.1
  sympy==1.10.1
  tensorflow==2.8.2
  toolz==0.11.1
  torch==1.12.1
  trimesh==3.9.35
  xarray==2022.3.0
  zarr==2.5.0
]===])

  ExternalProject_Add(${proj}
    ${${proj}_EP_ARGS}
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${PYTHON_EXECUTABLE} -m pip install -r ${requirements_file}
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
