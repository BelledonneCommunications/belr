############################################################################
# CMakeLists.txt
# Copyright (C) 2015  Belledonne Communications, Grenoble France
#
############################################################################
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
############################################################################


cmake_minimum_required(VERSION 3.0)
project(BELR VERSION 0.1.3 LANGUAGES C CXX)

set(BELR_SO_VERSION "1")

option(ENABLE_SHARED "Build shared library." ON)
option(ENABLE_STATIC "Build static library." ON)
option(ENABLE_STRICT "Build with strict compile options." YES)
option(ENABLE_TOOLS "Turn on or off compilation of tools." YES)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(NOT CPACK_GENERATOR AND NOT CMAKE_INSTALL_RPATH AND CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})
	message(STATUS "Setting install rpath to ${CMAKE_INSTALL_RPATH}")
endif()

include(GNUInstallDirs)
include(CheckSymbolExists)
include(CMakePushCheckState)

# find_package should be invoked here to check for libraries - however do NOT
# call include_directories here (see below)

if(LINPHONE_BUILDER_GROUP_EXTERNAL_SOURCE_PATH_BUILDERS)
	set(BcToolbox_FIND_COMPONENTS tester)
	include("${EP_bctoolbox_CONFIG_DIR}/BcToolboxConfig.cmake")
else()
	set(BELR_LIBRARIES_FOR_TESTER belr-static)
endif()

set(GRAMMAR_FILES
#LES GRAMMAIRES A INSERER ICI
#	grammars/.....
)

set(HEADER_FILES_CXX belr-tester.hpp)
set(SOURCE_FILES_CXX
#LES TESTEURS UNITAIRES ICI
	belr-tester.cpp
	belr-charrecognizer-tester.cpp
	belr-selector-tester.cpp
	belr-exclusiveselector-tester.cpp
	belr-sequence-tester.cpp
	belr-loop-tester.cpp
	belr-charrange-tester.cpp
	belr-literal-tester.cpp
	belr-recognizerpointer-tester.cpp
	belr-basicgrammar-tester.cpp
	belr-vcardgrammar-tester.cpp
)

set(STRICT_OPTIONS_CPP )
set(STRICT_OPTIONS_C )
set(STRICT_OPTIONS_CXX )
set(STRICT_OPTIONS_OBJC )
if(MSVC)
	if(ENABLE_STRICT)
		list(APPEND STRICT_OPTIONS_CPP "/WX")
	endif()
else()
	list(APPEND STRICT_OPTIONS_CPP "-Wall" "-Wuninitialized" "-Wno-error=deprecated-declarations")
	list(APPEND STRICT_OPTIONS_C "-Wdeclaration-after-statement" "-Wstrict-prototypes" "-Wno-error=strict-prototypes")
	list(APPEND STRICT_OPTIONS_CXX "-std=c++11")
	if(CMAKE_C_COMPILER_ID MATCHES "Clang")
		list(APPEND STRICT_OPTIONS_CPP "-Qunused-arguments" "-Wno-array-bounds")
	endif()
	if(APPLE)
		list(APPEND STRICT_OPTIONS_CPP "-Wno-error=unknown-warning-option" "-Qunused-arguments" "-Wno-tautological-compare" "-Wno-unused-function" "-Wno-array-bounds")
		list(APPEND STRICT_OPTIONS_CXX "-stdlib=libc++")
	endif()
	if(ENABLE_STRICT)
		list(APPEND STRICT_OPTIONS_CPP "-Werror" "-Wextra" "-Wno-unused-parameter" "-fno-strict-aliasing")
	endif()
endif()
if(STRICT_OPTIONS_CPP)
	list(REMOVE_DUPLICATES STRICT_OPTIONS_CPP)
endif()
if(STRICT_OPTIONS_C)
	list(REMOVE_DUPLICATES STRICT_OPTIONS_C)
endif()

add_executable(belr_tester ${SOURCE_FILES_CXX} ${HEADER_FILES_CXX})
set_target_properties(belr_tester PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(belr_tester PUBLIC ${BCTOOLBOX_TESTER_INCLUDE_DIRS})
target_link_libraries(belr_tester ${BELR_LIBRARIES_FOR_TESTER} ${BCTOOLBOX_TESTER_LIBRARIES})

if(APPLE)
	set_target_properties(belr_tester PROPERTIES LINK_FLAGS "-stdlib=libc++")
endif()
if(NOT IOS)
	install(TARGETS belr_tester
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	)
endif()

include(CMakePackageConfigHelpers)
export(EXPORT ${EXPORT_TARGETS_NAME}Targets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/BelrTargets.cmake"
)
configure_file(cmake/BelrConfig.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/BelrConfig.cmake"
	@ONLY
)
set(ConfigPackageLocation share/Belr/cmake)
install(EXPORT ${EXPORT_TARGETS_NAME}Targets
	FILE BelrTargets.cmake
	DESTINATION ${ConfigPackageLocation}
)
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/BelrConfig.cmake"
	DESTINATION ${ConfigPackageLocation}
)

add_subdirectory(build)
