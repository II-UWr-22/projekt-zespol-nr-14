cmake_minimum_required( VERSION 3.20 )
project( StrangePoker )

set( CMAKE_INSTALL_RPATH $ORIGIN/../lib )

if( WIN32 )
	add_compile_definitions( _CRT_SECURE_NO_WARNINGS )
endif()

option( USE_ALL_WARNINGS "Generate all warnings" ON)
if( USE_ALL_WARNINGS )
	if (MSVC)
		add_compile_options( /W4 /WX )
	else()
		add_compile_options( -Wall -Wextra -pedantic $<$<COMPILE_LANG_AND_ID:CXX,AppleClang,Clang>:-Wno-gnu-zero-variadic-macro-arguments> -Werror )
	endif()
endif()

set( SOURCES 
    src/strange_poker.c
    src/ui.c
    include/card.h
    include/player.h
    include/ui.h
)

add_executable( StrangePoker ${SOURCES} )
target_include_directories( StrangePoker PUBLIC include )

install( TARGETS StrangePoker )

option( USE_NCURSES_UI "Create ncurses ui backend" ON )
if( USE_NCURSES_UI )
    add_subdirectory( uibackends/ncurses )
    target_link_libraries( StrangePoker PRIVATE ui-ncurses )
endif()