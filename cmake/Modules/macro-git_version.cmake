# A function 
function( GIT_VERSION VAR_PREFIX)

set( GIT_COMMIT_DESC_DEFAULT    "None" )

# we need git of course...
find_package( Git )
if( GIT_FOUND )
  # make sure that this is actually a git repo
  execute_process( COMMAND ${GIT_EXECUTABLE} status
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   RESULT_VARIABLE IsGitRepo
                   OUTPUT_VARIABLE OutputTrash
                   ERROR_VARIABLE ErrorTrash)
else()
  message( FATAL_ERROR "Could not find `git` command, cannot determine version number. Please set the `${VAR_PREFIX}_VERSION` cache variable." )
endif()

if( ${IsGitRepo} EQUAL 0 )
    if( "${GIT_COMMIT_DESC}" STREQUAL "" )
      execute_process( COMMAND ${GIT_EXECUTABLE} describe --tags HEAD
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       OUTPUT_VARIABLE GIT_COMMIT_DESC
                       ERROR_VARIABLE Trash)
    endif()
else()
  message( FATAL_ERROR "Directory is not a Git repository, cannot determine version number. Please set the `${VAR_PREFIX}_VERSION` cache variable." )
endif()

# set defaults for any items that were not found
if( "${GIT_COMMIT_DESC}" STREQUAL "" )
  set( GIT_COMMIT_DESC ${GIT_COMMIT_DESC_DEFAULT} )
endif()

# set full version string
set(VERSION_FULL "${GIT_COMMIT_DESC}" )

# look for major, minor, and patch numbers in the version string
# this is required if the developer uses CPack for example.
# need to consider two possible formats:
#    Major.Minor.Patch
# or 
#    Major.Minor-PatchStr
# the first format is the standard format used for version numbers,
# but would have to be set by the developer as a tag name.
# however, if the developer just sets a major and minor version with
# a tag, then git will add the number of commits since the tag. for example
#     0.1-10-devel
# indicates that this is the 10th commit on the devel branch after 0.1 patch.

set( TMP ${VERSION_FULL} )

# first number in version string will be the major version
# major number may be followed by a . or -
if( ${TMP} MATCHES "^v*([0-9]+)[\\.\\-]" )
  set( VERSION_MAJOR ${CMAKE_MATCH_1}  )
else()
  set( VERSION_MAJOR "X" )
endif()
# strip off the major number
string( REGEX REPLACE "^v*${VERSION_MAJOR}" "" TMP "${TMP}" )

# minor number will only be preceeded by a .
# but may be followed by a . or -
if( ${TMP} MATCHES "^[\\.]([0-9]+)[\\.\\-]" )
  set( VERSION_MINOR ${CMAKE_MATCH_1}  )
else()
  set( VERSION_MINOR "X" )
endif()
# strip off the minor number
string( REGEX REPLACE "^[\\.]${VERSION_MINOR}" "" TMP ${TMP} )

# patch may be preceeded by a . or -, but must be a number
if( ${TMP} MATCHES "^[\\.\\-]([0-9]+)" )
  set( VERSION_PATCH ${CMAKE_MATCH_1}  )
else()
  set( VERSION_PATCH "X" )
endif()

# build cmake-compatible version string
set(VERSION "${VERSION_MAJOR}")
if( NOT "${VERSION_MINOR}" STREQUAL "X" )
  set(VERSION "${VERSION}.${VERSION_MINOR}")
  if( NOT "${VERSION_PATCH}" STREQUAL "X" )
    set(VERSION "${VERSION}.${VERSION_PATCH}")
  endif()
else()
  if( NOT "${VERSION_PATCH}" STREQUAL "X" )
    set(VERSION "${VERSION}-${VERSION_PATCH}")
  endif()
endif()

# now set version information in the parent scope
set( ${VAR_PREFIX}_VERSION_FULL   ${VERSION_FULL}  PARENT_SCOPE )
set( ${VAR_PREFIX}_VERSION        ${VERSION}       PARENT_SCOPE )
set( ${VAR_PREFIX}_VERSION_MAJOR  ${VERSION_MAJOR} PARENT_SCOPE )
set( ${VAR_PREFIX}_VERSION_MINOR  ${VERSION_MINOR} PARENT_SCOPE )
set( ${VAR_PREFIX}_VERSION_PATCH  ${VERSION_PATCH} PARENT_SCOPE )

endfunction(GIT_VERSION)

