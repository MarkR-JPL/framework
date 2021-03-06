# Tell code that Lua was found, since it is required here, it is always found!
# Needs some more work to make Lua truly optional
add_definitions(-DHAVE_LUA)

# Initialize variables used in subdirectories
set(FP_SOURCE_FILES "")
set(FP_TEST_FILES "")
set(SWIG_INTERFACE_FILES "")

# Install all common include files
#
# The top level lib folder has some header and interface files we need to install
file(GLOB LIB_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.i)
install(FILES ${LIB_INCLUDE_FILES} DESTINATION include/refractor)

foreach(sub_dir FixedLevel RadiativeTransfer Implementation Interface Lua RegisterOutput Support Example)
    list(APPEND FP_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/${sub_dir})
    add_subdirectory(${sub_dir})

    # Install all include files except excluded patterns such as unit test files
    #
    # The trailing / on the sub_dir makes all files get coppied to the same destination path
    # instead of having the directory (ie Interface/) copied to the destination path
    install(DIRECTORY ${sub_dir}/ DESTINATION include/refractor
        FILES_MATCHING 
            PATTERN "*.h" 
            PATTERN "*.i" 
            PATTERN "*.hpp" 
            PATTERN "*_fixture.h" EXCLUDE
            PATTERN "unit_test*.h" EXCLUDE
            PATTERN "l_rad_fortran" EXCLUDE
            PATTERN "spectrum_doxygen.h" EXCLUDE)
endforeach(sub_dir)

include_directories(SYSTEM ${THIRDPARTY_INCLUDES})
include_directories(${FP_INCLUDES})

add_library(refractor SHARED ${FP_SOURCE_FILES})
target_link_libraries(refractor ${THIRDPARTY_LIBRARIES})

# Add SWIG files included in lib directory
list(APPEND SWIG_INTERFACE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/swig_std.i
    ${CMAKE_CURRENT_SOURCE_DIR}/swig_array.i
    ${CMAKE_CURRENT_SOURCE_DIR}/swig_rational.i)

# Expose to the parent scope
set(FP_INCLUDES ${FP_INCLUDES} PARENT_SCOPE)
set(FP_TEST_FILES ${FP_TEST_FILES} PARENT_SCOPE)
set(SWIG_INTERFACE_FILES ${SWIG_INTERFACE_FILES} PARENT_SCOPE)

# Create installation rules
install(TARGETS refractor
    EXPORT RefractorTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib)
