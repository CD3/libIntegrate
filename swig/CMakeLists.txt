message( STATUS "Configuring SWIG interface build for ${PROJECT_NAME}" )

# Allow the caller to disable certain wrappers.
if(    NOT DEFINED PHP_WRAPPERS )
  set( PHP_WRAPPERS ON )
endif( NOT DEFINED PHP_WRAPPERS )
if(    NOT DEFINED PYTHON_WRAPPERS )
  set( PYTHON_WRAPPERS ON )
endif( NOT DEFINED PYTHON_WRAPPERS )

if( GENERATE_SWIG_WRAPPERS )
  if( PHP_WRAPPERS )
    message(STATUS "\tGenerage PHP wrapper source is enabled" )
  endif( PHP_WRAPPERS )
  if( PYTHON_WRAPPERS )
    message(STATUS "\tGenerage Python wrapper source is enabled" )
  endif( PYTHON_WRAPPERS )
endif( GENERATE_SWIG_WRAPPERS )

if( BUILD_SWIG_WRAPPERS )
  if( PHP_WRAPPERS )
    message(STATUS "\tBuild PHP wrappers is enabled" )
  endif( PHP_WRAPPERS )
  if( PYTHON_WRAPPERS )
    message(STATUS "\tBuild Python wrappers is enabled" )
  endif( PYTHON_WRAPPERS )
endif( BUILD_SWIG_WRAPPERS )


set( SWIG_PHP_MODULE_NAME        "php_${PROJECT_NAME}" )
set( SWIG_PHP_OUTPUT_DIR         "${PROJECT_SOURCE_DIR}/swig/" )
set( SWIG_PHP_WRAPPER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/swig" )
set( SWIG_PHP_MODULE_FILE        "${CMAKE_CURRENT_BINARY_DIR}/${SWIG_PHP_MODULE_NAME}.php" )
set( SWIG_PHP_WRAPPER_SOURCE     "${SWIG_PHP_WRAPPER_SOURCE_DIR}/${SWIG_PHP_MODULE_NAME}_wrap.cpp" )
set( SWIG_PHP_WRAPPER_HEADER     "${SWIG_PHP_WRAPPER_SOURCE_DIR}/php_${SWIG_PHP_MODULE_NAME}.h" )

set( SWIG_PYTHON_MODULE_NAME        "py_${PROJECT_NAME}" )
set( SWIG_PYTHON_OUTPUT_DIR         "${PROJECT_SOURCE_DIR}/swig/" )
set( SWIG_PYTHON_WRAPPER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/swig" )
set( SWIG_PYTHON_MODULE_FILE        "${CMAKE_CURRENT_BINARY_DIR}/${SWIG_PYTHON_MODULE_NAME}.py" )
set( SWIG_PYTHON_WRAPPER_SOURCE     "${SWIG_PYTHON_WRAPPER_SOURCE_DIR}/${SWIG_PYTHON_MODULE_NAME}_wrap.cpp" )
set( SWIG_PYTHON_WRAPPER_HEADER     "${SWIG_PYTHON_WRAPPER_SOURCE_DIR}/py_${SWIG_PYTHON_MODULE_NAME}.h" )


if( BUILD_SWIG_WRAPPERS OR GENERATE_SWIG_WRAPPERS )
  if( PHP_WRAPPERS )
    find_package( PHP5 REQUIRED )
    include_directories( ${PHP5_INCLUDE_DIR}      )
    include_directories( ${PHP5_MAIN_INCLUDE_DIR} )
    include_directories( ${PHP5_ZEND_INCLUDE_DIR} )
    include_directories( ${PHP5_TSRM_INCLUDE_DIR} )
  endif( PHP_WRAPPERS )

  if( PYTHON_WRAPPERS )
    find_package( PythonLibs 2.7 REQUIRED )
    include_directories(${PYTHON_INCLUDE_PATH})
  endif( PYTHON_WRAPPERS )

endif( BUILD_SWIG_WRAPPERS OR GENERATE_SWIG_WRAPPERS )

#  ____                           _        __        __                                   
# / ___| ___ _ __   ___ _ __ __ _| |_ ___  \ \      / / __ __ _ _ __  _ __   ___ _ __ ___ 
#| |  _ / _ \ '_ \ / _ \ '__/ _` | __/ _ \  \ \ /\ / / '__/ _` | '_ \| '_ \ / _ \ '__/ __|
#| |_| |  __/ | | |  __/ | | (_| | ||  __/   \ V  V /| | | (_| | |_) | |_) |  __/ |  \__ \
# \____|\___|_| |_|\___|_|  \__,_|\__\___|    \_/\_/ |_|  \__,_| .__/| .__/ \___|_|  |___/
#                                                              |_|   |_|                  

if( GENERATE_SWIG_WRAPPERS )
  find_package(SWIG REQUIRED)

    # we need the .i file to be in the binary directory so swig will generate files in the places we want them
    add_custom_command( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.i
                        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.i
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.i"
                        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.i"
                        COMMENT "copy swig interface file to binary dir" )



  # ____  _   _ ____  
  #|  _ \| | | |  _ \ 
  #| |_) | |_| | |_) |
  #|  __/|  _  |  __/ 
  #|_|   |_| |_|_|    


  if( PHP_WRAPPERS AND PHP5_FOUND )

    get_property( dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach( dir ${dirs})
      list( APPEND ${PROJECT_NAME}_SWIG_PHP_FLAGS "-I${dir}" )
    endforeach( dir ${dirs})

    list( APPEND ${PROJECT_NAME}_SWIG_PHP_FLAGS -c++ -php                           )
    list( APPEND ${PROJECT_NAME}_SWIG_PHP_FLAGS -module ${SWIG_PHP_MODULE_NAME}     )
    list( APPEND ${PROJECT_NAME}_SWIG_PHP_FLAGS -outdir ${SWIG_PHP_OUTPUT_DIR}      )
    list( APPEND ${PROJECT_NAME}_SWIG_PHP_FLAGS -o      ${SWIG_PHP_WRAPPER_SOURCE}  )
    list( APPEND ${PROJECT_NAME}_SWIG_PHP_FLAGS -oh     ${SWIG_PHP_WRAPPER_HEADER}  )


    # for some reason, swig ignores the -oh flag. instead, it just create # php_<module_name>.h
    # so, if we set the module name to be php_${PROJECT_NAME}, then swig creates
    # php_php_${PROJECT_NAME}.h. this is annoying.
    # it also puts this header in the same directory as the .php file that it
    # creates, instead of where the .cpp file is. this is also annoying.
    # 
    # so, we have to move the header file after swig generates it

    add_custom_command( OUTPUT
                              #${SWIG_PHP_MODULE_FILE}
                               ${SWIG_PHP_WRAPPER_SOURCE}   # workaround.  specifiying multiple output files causes this target to build every time for some reason
                              #${SWIG_PHP_WRAPPER_HEADER}
                        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.i"
                        # execute swig (in current binary directory)
                        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.i" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.i"
                        # move the header file that swig creates by # copy/remove
                        COMMAND ${SWIG_EXECUTABLE} ${${PROJECT_NAME}_SWIG_PHP_FLAGS} "${PROJECT_NAME}.i"
                        COMMAND ${CMAKE_COMMAND} -E copy   "${SWIG_PHP_OUTPUT_DIR}/php_${SWIG_PHP_MODULE_NAME}.h" "${SWIG_PHP_WRAPPER_HEADER}"
                        COMMAND ${CMAKE_COMMAND} -E remove "${SWIG_PHP_OUTPUT_DIR}/php_${SWIG_PHP_MODULE_NAME}.h"
                        COMMENT "generate files for swig php wrapper" )

    if( NOT BUILD_SWIG_WRAPPERS )
      # if the build flag is not enabled, then we need to add a target that will
      # require the swig output files
      add_custom_target(php_${PROJECT_NAME}_swig ALL
                        DEPENDS php_${PROJECT_NAME}.php
                        )
    endif( NOT BUILD_SWIG_WRAPPERS )

  endif( PHP_WRAPPERS AND PHP5_FOUND )


# ____        _   _                 
#|  _ \ _   _| |_| |__   ___  _ __  
#| |_) | | | | __| '_ \ / _ \| '_ \ 
#|  __/| |_| | |_| | | | (_) | | | |
#|_|    \__, |\__|_| |_|\___/|_| |_|
#       |___/                       

  if( PYTHON_WRAPPERS AND PYTHONLIBS_FOUND )

    get_property( dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach( dir ${dirs})
      list( APPEND ${PROJECT_NAME}_SWIG_PYTHON_FLAGS "-I${dir}" )
    endforeach( dir ${dirs})

    list( APPEND ${PROJECT_NAME}_SWIG_PYTHON_FLAGS -c++ -python                           )
    list( APPEND ${PROJECT_NAME}_SWIG_PYTHON_FLAGS -module ${SWIG_PYTHON_MODULE_NAME}     )
    list( APPEND ${PROJECT_NAME}_SWIG_PYTHON_FLAGS -outdir ${SWIG_PYTHON_OUTPUT_DIR}      )
    list( APPEND ${PROJECT_NAME}_SWIG_PYTHON_FLAGS -o      ${SWIG_PYTHON_WRAPPER_SOURCE}  )


    # it seems that swig does not generate a heade file for python modules,
    # however, it also does not name the .cxx file what we want it to be, it
    # generateds ${PROJECT_NAME}_wrap.cxx, but we would like py_${PROJECT_NAME}_wrap.cxx
    add_custom_command( OUTPUT
                              # WARNING: specifiying multiple output files seems to cause this target to build every time for some reason
                              ${SWIG_PYTHON_MODULE_FILE}
                              #${SWIG_PYTHON_WRAPPER_SOURCE}
                              #${SWIG_PYTHON_WRAPPER_HEADER}
                        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.i"
                        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.i" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.i"
                        # execute swig (in current binary directory)
                        COMMAND ${SWIG_EXECUTABLE} ${${PROJECT_NAME}_SWIG_PYTHON_FLAGS} "${PROJECT_NAME}.i"
                        COMMENT "generate files for swig python wrapper" )

    if( NOT BUILD_SWIG_WRAPPERS )
      # if the build flag is not enabled, then we need to add a target that will
      # require the swig output files
      add_custom_target(py_${PROJECT_NAME}_swig ALL
                        DEPENDS py_${PROJECT_NAME}.py
                        )
    endif( NOT BUILD_SWIG_WRAPPERS )

  endif( PYTHON_WRAPPERS AND PYTHONLIBS_FOUND )


endif( GENERATE_SWIG_WRAPPERS )


# ____        _ _     _  __        __                                   
#| __ ) _   _(_) | __| | \ \      / / __ __ _ _ __  _ __   ___ _ __ ___ 
#|  _ \| | | | | |/ _` |  \ \ /\ / / '__/ _` | '_ \| '_ \ / _ \ '__/ __|
#| |_) | |_| | | | (_| |   \ V  V /| | | (_| | |_) | |_) |  __/ |  \__ \
#|____/ \__,_|_|_|\__,_|    \_/\_/ |_|  \__,_| .__/| .__/ \___|_|  |___/
#                                            |_|   |_|                  

if( BUILD_SWIG_WRAPPERS )

  # swig needs/wants a shared library to link against
  add_library( MPE_shared SHARED ${${PROJECT_NAME}_SOURCES})

  # ____  _   _ ____  
  #|  _ \| | | |  _ \ 
  #| |_) | |_| | |_) |
  #|  __/|  _  |  __/ 
  #|_|   |_| |_|_|    

  if( PHP_WRAPPERS AND PHP5_FOUND )

    add_library( php_${PROJECT_NAME} SHARED ${SWIG_PHP_WRAPPER_SOURCE} ${SWIG_PHP_WRAPPER_HEADER} )
    set_target_properties( php_${PROJECT_NAME} PROPERTIES OUTPUT_NAME "php_${PROJECT_NAME}" )
    set_target_properties( php_${PROJECT_NAME} PROPERTIES PREFIX      ""           )
    target_link_libraries( php_${PROJECT_NAME} MPE_shared )

  endif( PHP_WRAPPERS AND PHP5_FOUND )


  # ____        _   _                 
  #|  _ \ _   _| |_| |__   ___  _ __  
  #| |_) | | | | __| '_ \ / _ \| '_ \ 
  #|  __/| |_| | |_| | | | (_) | | | |
  #|_|    \__, |\__|_| |_|\___/|_| |_|
  #       |___/                       


  if(PYTHON_WRAPPERS AND PYTHONLIBS_FOUND)

    add_library( py_${PROJECT_NAME} SHARED ${SWIG_PYTHON_WRAPPER_SOURCE}  )
    set_target_properties( py_${PROJECT_NAME} PROPERTIES OUTPUT_NAME "_py_${PROJECT_NAME}" )
    set_target_properties( py_${PROJECT_NAME} PROPERTIES PREFIX      ""          )
    if(WIN32)
	    set_target_properties( py_${PROJECT_NAME} PROPERTIES SUFFIX ".pyd" ) # on windows, the python module needs to have a .pyd extension
    endif(WIN32)
    target_link_libraries( py_${PROJECT_NAME} MPE_shared ${PYTHON_LIBRARY} ${PYTHON_LIBRARIES} )


  endif(PYTHON_WRAPPERS AND PYTHONLIBS_FOUND)

endif( BUILD_SWIG_WRAPPERS )
