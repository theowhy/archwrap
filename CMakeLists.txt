cmake_minimum_required(VERSION 3.0)

project(archwrap)

find_package(LibArchive)

set(ARCHWRAP_INSTALL_HEADER ${ARCHWRAP_INSTALL_HEADER} CACHE BOOL "Install header for archwrap")

add_library(archwrap SHARED
	src/archwrap.c

	include/archwrap.h
)
target_include_directories(archwrap PUBLIC include)
target_include_directories(archwrap PRIVATE ${LibArchive_INCLUDE_DIRS})

if(ARCHWRAP_INSTALL_HEADER)
	set_target_properties(archwrap PROPERTIES PUBLIC_HEADER "include/archwrap.h")
endif()

target_link_libraries(archwrap ${LibArchive_LIBRARIES})

install(TARGETS archwrap
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
	PUBLIC_HEADER DESTINATION include
)

add_executable(archive_read
	test/read.c
)

target_link_libraries(archive_read archwrap)
