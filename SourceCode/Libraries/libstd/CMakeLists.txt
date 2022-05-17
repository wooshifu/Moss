project(ObjLibGccStd LANGUAGES CXX ASM DESCRIPTION "gcc stdlib")

#[[
set(_LIBCPP_HAS_NO_THREADS 1)
set(_LIBCPP_HAS_NO_VENDOR_AVAILABILITY_ANNOTATIONS 1)
set(_LIBCPP_HAS_NO_STDIN 1)
set(_LIBCPP_HAS_NO_STDOUT 1)
set(_LIBCPP_HAS_MUSL_LIBC 0)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/__config_site.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/__config_site"
        @ONLY)
]]
