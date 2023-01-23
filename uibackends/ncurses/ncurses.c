#include "ui.h"
#include <memory.h>
#include <stdlib.h>
#include <inttypes.h>
#include <locale.h>

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
    setlocale(LC_ALL,"");
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
            printw("%4" PRIu32 " %15s: %8" PRIu64 "$", i, THIS.players[i].name, THIS.players[i].balance );
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
                if( scanw("%256s %" SCNu64, buff, &budget) != 2 ) break;
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
                    pl->name = NULL;
                    
                    char *name = (char *)calloc( strlen(buff) + 1, 1 );

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

                while( true )
                {
                    move( LINES - 2, 0 );
                    clrtoeol();
                    refresh();

                    if( scanw("%" SCNd32, &index) != 1 )
                        index = -1;

                    if( index < (int32_t)THIS.numpl ) break;

                    move( LINES - 1, 0 );
                    printw("invalid player index!");
                }

                if( index <= -1 ) break;

                free( (void*)THIS.players[index].name );
                THIS.players[index].name = NULL;

                if( THIS.numpl - index - 1 > 0 )
                    memmove( THIS.players + index, THIS.players + index + 1, sizeof(player_t) * (THIS.numpl - index - 1) );
                
                THIS.numpl -= 1;

                tmpptr = realloc( THIS.players, sizeof(player_t) * THIS.numpl-1 );
                if( !tmpptr )
                {
                    fprintf(stderr, "couldnot reallocate player table\n" );
                    return ExitGame;
                }
                THIS.players = (player_t*)tmpptr;
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

static int hiddenCard = 0x1F0A0;
static int cardToUnicode( const card_t *card )
{
    int suitOff = card->suit;
    int valOff = card->value == 14 ? 1 : card->value;

    return hiddenCard + suitOff * 0x10 + valOff;
}

static void updateState( void *data, const GameContext_t *ctx )
{
    clear();
    move(0, COLS/2 - 6);
    printw("Strange Poker");

    move( 1, 0 );
    printw("players:\n");
    printw("%5s %15s %8s %8s %s\n", "", "name", "balance", "bid", "cards");
    for( uint32_t i = 0; i < THIS.numpl; i++ )
    {
        const player_t *pl = &THIS.players[i];
        printw("%s %15s ", i == ctx->currentPlayer ? " --->" : "     ", pl->name );
        printw("%8" PRIu64 " ", pl->balance );
        if( pl->bid == UINT64_MAX ) printw("%8s ", "ALL-IN");
        else if( pl->bid == 0 ) printw("%8s ", "");
        else printw("%8" PRIu64 " ", pl->bid );

        for( uint32_t j = 0; j < pl->validCards; j++ )
            printw("%lc ", (ctx->currentPlayer == i || ctx->currentPlayer == UINT32_MAX) ? cardToUnicode(&pl->cards[j]) : hiddenCard );

        printw("\n"); 
    }

    mvprintw( 1, COLS/2, "table:");
    move( 2, COLS/2 );
    for( uint32_t j = 0; j < ctx->visibleTableCards; j++ )
        printw("%lc ", cardToUnicode(&ctx->tableCards[j]) );
    mvprintw( 3, COLS/2, "%" PRIu64 "$ on the table", ctx->moneyOnTable );
    
    refresh();
}

static uint32_t drop( void *data, const GameContext_t *ctx )
{
    updateState( data, ctx );

    move(LINES - 3, 0);
    printw("What card to drop (index)\n");
    
    uint32_t dr;
    while( true )
    {
        move( LINES - 2, 0 );
        clrtoeol();
        refresh();

        scanw("%" SCNu32, &dr);

        if( dr < THIS.players[ctx->currentPlayer].validCards ) break;

        move( LINES - 1, 0 );
        printw("invalid card index!");
    }

    return dr;
}

static uint64_t bid( void *data, const GameContext_t *ctx  )
{
    updateState(data, ctx);
    
    move( LINES - 3, 0 );    
    printw("What amount of money bid (number > 0, 'ALL-IN' or 'PASS')\n");
        
    while( true )
    {
        move( LINES - 2, 0 );
        clrtoeol();
        refresh();

        char buff[256];
        getnstr(buff, sizeof(buff) );
        
        if( strcmp(buff, "ALL-IN") == 0) return UINT64_MAX;
        if( strcmp(buff, "PASS") == 0) return 0;
        
        uint64_t v = strtoull(buff, NULL, 10);

        if( v != 0 && v <= THIS.players[ctx->currentPlayer].balance ) return v;
        
        move( LINES - 1, 0 );
        printw("invalid bid value! Must be >0 and <=budget or 'ALL-IN' or 'PASS'");
    }
}

static void messageUser( void *data, const GameContext_t *ctx, const char *msg )
{
    updateState( data, ctx );
    move( LINES - 2, 0 );
    printw("%s\npress enter to continue...", msg );
    refresh();
    wgetch(stdscr);
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