cmake_minimum_required( VERSION 3.20 )
project( ui-ncurses )

set( SOURCES 
    ncurses.c    
)

set( CURSES_NEED_WIDE TRUE )
find_package(Curses REQUIRED)

add_library( ui-ncurses SHARED ${SOURCES} )
target_include_directories( ui-ncurses PRIVATE ../../include ${CURSES_INCLUDE_DIR} )
target_link_libraries( ui-ncurses PRIVATE ${CURSES_LIBRARIES} )

install( TARGETS ui-ncurses 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)