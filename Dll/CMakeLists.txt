add_library(NoRestrictionsConstruction SHARED
    dllmain.cpp
    pch.cpp
)

target_precompile_headers(NoRestrictionsConstruction PRIVATE pch.h)

target_compile_definitions(NoRestrictionsConstruction PRIVATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
)

target_link_libraries(NoRestrictionsConstruction PRIVATE kernel32)

set_target_properties(NoRestrictionsConstruction PROPERTIES
    OUTPUT_NAME "main"
    SUFFIX ".dll"
)
