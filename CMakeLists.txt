cmake_minimum_required(VERSION 3.0.0)
project(LIB_XML_EXAMPLE VERSION 0.1.0)

include(CTest)
enable_testing()
find_package(LibXml2 REQUIRED)

add_executable(LIB_XML_EXAMPLE main.cpp lib_xml2_example.cpp lib_xml2_example.hpp)
target_link_libraries(LIB_XML_EXAMPLE PUBLIC LibXml2::LibXml2)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
