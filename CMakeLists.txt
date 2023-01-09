cmake_minimum_required( VERSION 3.20 )
project( StrangePoker )

if( WIN32 )
	add_compile_definitions( _CRT_SECURE_NO_WARNINGS )
endif()

option( USE_ALL_WARNINGS "Generate all warnings" ON)
if( USE_ALL_WARNINGS )
	if (MSVC)
		add_compile_options( /W4 )
	else()
		add_compile_options( -Wall -Wextra -pedantic $<$<COMPILE_LANG_AND_ID:CXX,AppleClang,Clang>:-Wno-gnu-zero-variadic-macro-arguments> )
	endif()
endif()

set( SOURCES 
    src/strange_poker.c
    include/card.h
    include/player.h
)

add_executable( StrangePoker ${SOURCES} )
target_include_directories( StrangePoker PUBLIC include )