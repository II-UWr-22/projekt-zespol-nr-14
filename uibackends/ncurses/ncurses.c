#include "ui.h"
#include <memory.h>
#include <stdlib.h>
#include <inttypes.h>

#include <ncurses.h>

typedef struct dat_t {
    struct MyUIHandler_t *obj;
    
    uint32_t numpl;
    player_t *players;
} dat_t;

typedef struct MyUIHandler_t {
    IuiHandler_t handler;
    dat_t data;
} MyUIHandler_t;

#define THIS (*(dat_t*)data)
static int initialize( void *data )
{
    (void)data;
    if( !initscr() ) return -1;

    THIS.numpl = 0;
    THIS.players = NULL;

    refresh();

    return 0;
}

static player_t *getPlayers( void *data, uint32_t *cnt )
{
    *cnt = THIS.numpl;
    return THIS.players;
}

static GameMenuOptions gameMenu( void *data )
{
    while( true )
    {
        clear();
        move(0, COLS/2 - 6);
        printw("Strange Poker");
        
        move(2, 0);
        printw("choose option:\n");
        const char *options[] = {
            "Add player",
            "Remove player",
            "Begin game!",
            "Exit"
        };

        for( uint32_t i = 0; i < sizeof(options)/sizeof(options[0]); i++ )
            printw("%7d. %s\n", i, options[i] );
        
        move(2, COLS/2);
        printw("players (%d):\n", THIS.numpl);
        for( uint32_t i = 0; i < THIS.numpl; i++ )
        {
            move(3+i,COLS/2 + 4);
            printw("%4" PRIu32 " %10s: %8" PRIu64 "$", i, THIS.players[i].name, THIS.players[i].balance );
        }

        move(LINES - 3, 0);
        refresh();

        uint32_t option;
        if( scanw("%" SCNu32, &option) == 1 )
        {
            char buff[256];
            uint64_t budget;
            int32_t index;
            void *tmpptr;

            move( LINES - 3, 0 );
            switch( option )
            {
            case 0: 
                printw( "what is the name and budget of the new player?\n" );
                refresh();
                scanw("%256s %" SCNu64, buff, &budget);
                tmpptr = realloc( THIS.players, sizeof(player_t) * (THIS.numpl+1) );
                if( !tmpptr )
                {
                    fprintf(stderr, "couldnot reallocate player table\n" );
                    return ExitGame;
                }
                THIS.players = (player_t*)tmpptr;
                THIS.numpl += 1;
                {
                    player_t *pl = &THIS.players[THIS.numpl - 1];
                    char *name = calloc( strlen(buff) + 1, 1 );

                    if( !name )
                    {
                        fprintf(stderr, "could not allocate name\n" );
                        return ExitGame;
                    }

                    strcpy(name, buff);
                    pl->name = name;
                    pl->balance = budget;
                }
                break;
            case 1:
                printw( "what player to remove (index) (-1 to cancel)?\n" );

                do
                {
                    move( LINES - 2, 0 );
                    clrtoeol();
                    refresh();

                    scanw("%" SCNd32, &index);

                    if( index < (int32_t)THIS.numpl ) break;

                    move( LINES - 1, 0 );
                    printw("invalid player index!");
                }
                while( true );

                if( index <= -1 ) break;

                free( (void*)THIS.players[index].name );
                if( THIS.numpl - index - 1 > 0 )
                    memmove( THIS.players + index, THIS.players + index + 1, sizeof(player_t) * (THIS.numpl - index - 1) );
                
                tmpptr = realloc( THIS.players, sizeof(player_t) * THIS.numpl-1 );
                if( !tmpptr )
                {
                    fprintf(stderr, "couldnot reallocate player table\n" );
                    return ExitGame;
                }
                THIS.players = (player_t*)tmpptr;
                THIS.numpl -= 1;
                break;
            case 2: return StartNewRound;
            case 3: return ExitGame;
            }
        }

        // clear last 3 lines

        for( int i = LINES - 3; i < LINES; i++ )
        {
            move( i, 0 );
            clrtoeol();
        }

        move( LINES - 3, 0 );
    }
}

static void updateState( void *data, uint32_t pl )
{
    (void)data;
    clear();
    printw("Strange Poker, player %d", pl);
    refresh();
}

static uint32_t drop( void *data, uint32_t pl )
{
    updateState( data, pl );
    printw("What card to drop (index)");

    uint32_t dr;
    scanw("%" SCNu32, &dr);

    return dr;
}

static uint64_t bid( void *data, uint32_t pl )
{
    updateState(data, pl);

    while( true )
    {
        printw("What amount of money bid (number, 'ALL-IN' or 'PASS')");

        char buff[256];
        getnstr(buff, sizeof(buff) );
        
        if( strcmp(buff, "ALL-IN") == 0) return UINT64_MAX;
        if( strcmp(buff, "PASS") == 0) return 0;
        
        uint64_t v = strtoull(buff, NULL, 10);
        if( v != 0 ) return v;
    }
}

static void messageUser( void *data, uint32_t pl, const char *msg )
{
    updateState( data, pl );
    printw("%s", msg );
}

static void destroy( void *data )
{
    endwin();

    for( uint32_t i = 0; i < THIS.numpl; i++ ) 
        free( (void*)THIS.players[i].name );
    
    free( THIS.players );
    free( THIS.obj );
}

IuiHandler_t *uiFactory()
{
    MyUIHandler_t *dat = (MyUIHandler_t*)calloc( 1, sizeof(MyUIHandler_t) );
    if( !dat ) return NULL;
    dat->handler.data = &dat->data;
    dat->data.obj = dat;

#define ADDFN( fname ) dat->handler. fname = fname;
    ADDFN( getPlayers )
    ADDFN( gameMenu )
    ADDFN( drop )
    ADDFN( bid )
    ADDFN( messageUser )
    ADDFN( updateState )
    ADDFN( destroy )

#undef ADDFN

    if( initialize( &dat->data ) ) 
    {
        free( dat );
        return NULL;
    }

    return &dat->handler;
}