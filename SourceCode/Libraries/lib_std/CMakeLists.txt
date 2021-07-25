project(ObjLibGccStd LANGUAGES CXX ASM DESCRIPTION "gcc stdlib")

set(_LIBCPP_HAS_NO_THREADS 1)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/__config_site.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/__config_site"
        @ONLY)
