#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>

#include "winning_check.h"
#include "ui.h"

card_t randCard( char used[52] )
{
    card_t res;
    int c=rand() % 52;
    while(used[c]!=0) c=rand() % 52;
    res.suit=c%4;
    res.value=(c/13)+2;
    used[c]=1;

    return res;
}

void printMessage( IuiHandler_t *ui, GameContext_t *ctx, const char *format, ... )
{
    va_list vl;
    va_start( vl, format );
    char strbuf[512];
    strbuf[511] = '\0';
    vsnprintf( strbuf, sizeof(strbuf)-1, format, vl );

    ui->messageUser(ui->data, ctx, strbuf);

    va_end( vl );
}

uint64_t bidding(uint64_t *min_bid, player_t *player, uint32_t *CntPlayers, IuiHandler_t *ui, GameContext_t *ctx){
    uint64_t bid;
    
    do
    {
        bid = ui->bid (ui -> data, ctx);

        if( bid == 0 ) // PASS 
        {
            (*CntPlayers)--;
            player->validCards = 0;

            return 0;
        }
        
        if( bid == UINT64_MAX )
        {
            // TODO: support ALL-IN
            if( *min_bid < player->balance ) *min_bid = player->balance;
            
            return player->bid = player->balance; // ????? 
        }

        if( bid < *min_bid )
            ui->messageUser( ui->data, ctx, "Your have bet is lower then the minimal amount" );

    } while(bid < *min_bid);

    return *min_bid = player->bid = bid;
}

void bidPlayer( IuiHandler_t *ui, GameContext_t *ctx, uint32_t i, uint64_t *min_bid, uint32_t *current_players )
{
    uint32_t playerCnt;
    player_t *players = ui->getPlayers( ui->data, &playerCnt );
    
    ctx->currentPlayer = playerCnt + 1; // do not show anyones cards!
    printMessage( ui, ctx, "Ready player %s! (%u)", players[i].name, i );

    ctx->currentPlayer = i;
    uint64_t lastBid = players[i].bid;
    uint64_t newBid = bidding( min_bid, &players[i], current_players, ui, ctx);

    if( newBid != 0 ) ctx->moneyOnTable += newBid - lastBid;
}

void bidFunction(IuiHandler_t *ui, GameContext_t *ctx, uint64_t *min_bid, uint32_t *playerCnt, uint32_t start, player_t *players){
    uint32_t current_players = *playerCnt;
    for( uint32_t i=start; i < *playerCnt; i++)
    {
        if(players[i].validCards != 0 && players[i].bid != players[i].balance)
            bidPlayer( ui, ctx, i, min_bid, &current_players );
    }
    uint32_t curPlayer = 0;
    while( current_players != 0 && 
        ( players[curPlayer].validCards == 0 || players[curPlayer].bid < *min_bid ) )
            {
                if( players[curPlayer].validCards != 0 && players[curPlayer].bid != players[curPlayer].balance )
                    bidPlayer( ui, ctx, curPlayer, min_bid, &current_players );
                    
                curPlayer = ( curPlayer + 1 ) % *playerCnt;
            }

}

void drop(IuiHandler_t *ui, GameContext_t *ctx, uint32_t *playerCnt, player_t *players){
    for(uint32_t i=0; i< *playerCnt; i++){
        if(players[i].validCards!=0){
            ctx.currentPlayer = *playerCnt+1;
            printMessage( ui, ctx, "Ready player %s! (%u)", players[i].name, i );
            ctx.currentPlayer= i;
            uint32_t drop=ui->drop (ui -> data, ctx);
            players[i].cards[drop]=players[i].cards[players[i].validCards-1];
            players[i].validCards--;
        }
    }
}

int main(int argc, char *argv[])
{
    const char *backendName = "ncurses";
    if( argc > 1 ) backendName = argv[1];

    IuiHandler_t *ui = uiHandlerFactory(backendName);
    if( !ui )
    {
        fprintf(stderr, "cannot crate ui system\n");
        return -1;
    }

    srand((unsigned int) time(NULL));
    char deck[52];
    
    while( 1 ) 
    {
        GameMenuOptions opt = ui->gameMenu( ui->data );
        if( opt == ExitGame ) break;

        if( opt == StartNewRound ) 
        {
            uint32_t playerCnt;
            player_t *players = ui->getPlayers( ui->data, &playerCnt );
            for( uint32_t i = 0; i < playerCnt; i++ )
            {
                players[i].bid = 0;
                players[i].validCards = 0;
            }

            GameContext_t ctx;
            ctx.currentPlayer = 0;
            ctx.moneyOnTable = 0;
            ctx.visibleTableCards = 0;

            if( playerCnt == 0 )
            {
                ui->messageUser( ui->data, &ctx, "No players to play!" );
                continue;
            }
            
            //dealing cards
            
            for(int i = 0; i < 52; i++)
                deck[i] = 0;

            for (uint32_t i = 0; i < playerCnt; i++)
                for (int j = 0; j < 5; j++)
                    players[i].cards[j] = randCard( deck );

            for (int i = 0; i < 5; i++)
                ctx.tableCards[i] = randCard( deck );
            
            
            // show beginning of the game and wait for input
            printMessage( ui, &ctx, "Start Game!" );

            for(uint32_t i = 0; i < playerCnt; i++ )
                players[i].validCards = 4;

            //preflop bids
            ctx.moneyOnTable = 0;

            uint64_t min_bid = players[0].bid = 10; //small blind
            ctx.moneyOnTable += 10;

            if( playerCnt > 1 )
            {
                min_bid = players[1].bid = 20; //big blind
                ctx.moneyOnTable += 20;
            }
            
            bidFunction(ui, &ctx, &min_bid, &playerCnt, 2, players);

            //flop
            ctx.visibleTableCards = 3;
            printMessage( ui, &ctx, "Flop!" );

            //2nd bids
            bidFunction(ui, &ctx, &min_bid, &playerCnt, 0, players);

            //drop the 4th card
            drop(ui, &ctx, &playerCnt, players);

            //turn
            ctx.visibleTableCards = 4;
            printMessage( ui, &ctx, "Turn!" );
            
            //3rd bids
            bidFunction(ui, &ctx, &min_bid, &playerCnt, 0, players);

            //drop the 3rd card
            drop(ui, &ctx, &playerCnt, players);

            //river
            ctx.visibleTableCards = 5;
            printMessage( ui, &ctx, "River!" );
            
            //4th bids
            bidFunction(ui, &ctx, &min_bid, &playerCnt, 0, players);

            //check the winner

            //add table money to the winners balance

            //switch the first player to the last position after the end of the round

            // ?????
        }
    }

    ui->destroy( ui->data );
    return 0;
}
