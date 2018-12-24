find_path(CCONTROLLERS_INCLUDE_DIR
    NAMES 
        attitude-controller.h
        altitude-controller.h
    PATHS 
        $ENV{HOME}/PO-EAGLE/Groups/ANC/Cleanup-Pieter/Code-Generators/
        # TODO: Bas, add your EAGLE path
    PATH_SUFFIXES
        Controllers/include
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CControllers
    REQUIRED_VARS CCONTROLLERS_INCLUDE_DIR
)

get_filename_component(CCONTROLLERS_LIBRARIES 
    "${CCONTROLLERS_INCLUDE_DIR}/../x86_64/lib/libcontrollers.a" 
    ABSOLUTE
)
message("CControllers include ${CCONTROLLERS_INCLUDE_DIR}")
message("CControllers libraries ${CCONTROLLERS_LIBRARIES}")

if(NOT TARGET CControllers::CControllers)
    add_library(CControllers::CControllers INTERFACE IMPORTED)
    set_target_properties(CControllers::CControllers PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CCONTROLLERS_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${CCONTROLLERS_LIBRARIES}"
    )
endif()