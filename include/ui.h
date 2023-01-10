#ifndef UI_H_
#define UI_H_

#include "card.h"
#include "player.h"
#include <stdint.h>

typedef enum GameMenuOptions
{
    ExitGame = 0,
    StartNewRound = 1,
} GameMenuOptions;

/*
Interface to uiHandler implementation.
It will not check validity of input so other checks are required.
*/
typedef struct IuiHandler_t
{
    /*
    get array of players

    returns:
        pointer to array of players and size of it in *count
        array is owned by uiHandler so DO NOT FREE IT
    
    !! count must be a valid pointer to uint32_t !!
    */
    player_t *(*getPlayers)( void *data, uint32_t *count );

    /*
    show menu and handle it.

    returns:
        action that user requests
    
    !! players and player count may change after this call !!
    */
    GameMenuOptions (*gameMenu)( void *data );

    /*
    prompt user to drop a card from hand

    returns:
        index of dropped card
    */
    uint32_t (*drop)( void *data, uint32_t currentPlayer );

    /*
    prompt user to choose bid amount

    returns:
        amount of money bid or UINT64_MAX if all-in
    */
    uint64_t (*bid)( void *data, uint32_t currentPlayer );

    /*
    print a message to a user

    call it if user inputted invalid data
    */
    void (*messageUser)( void *data, uint32_t currentPlayer, const char *msg );

    /*
    update state.
    call it after external change of player_t members
    */
    void (*updateState)( void *data, uint32_t currentPlayer );

    /*
    destroy uiHandler
    */
    void (*destroy)( void *data );
    void *data;
} IuiHandler_t;

/*
create uiHandler from requested backend

returns:
    pointer to new uiHandler if `backend` is valid backend name and created successfully
*/
IuiHandler_t *uiHandlerFactory( const char *backend );

#endif