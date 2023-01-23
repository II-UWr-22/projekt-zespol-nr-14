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
            
            // need to switch the first player to the last position after the end of the round
            uint32_t current_players = playerCnt;
            for( uint32_t i=2; i < playerCnt; i++)
            {
                bidPlayer( ui, &ctx, i, &min_bid, &current_players );
            }

            uint32_t curPlayer = 0;
            while( current_players != 0 && 
                    ( players[curPlayer].validCards == 0 || players[curPlayer].bid < min_bid ) )
            {
                if( players[curPlayer].validCards != 0 && players[curPlayer].bid != players[curPlayer].balance )
                    bidPlayer( ui, &ctx, curPlayer, &min_bid, &current_players );
                
                curPlayer = ( curPlayer + 1 ) % playerCnt;
            }

            //flop
            ctx.visibleTableCards = 3;
            printMessage( ui, &ctx, "Flop!" );

            // ?????

            //decrease player balance after game
            for( uint32_t i=0; i < playerCnt; i++)
            {
                players[i].balance -= players[i].bid;
            }

            int winnersCnt = 0;
            int winnersTable[playerCnt];
            winner_check(playerCnt, players, ctx.tableCards, &winnersCnt, winnersTable);
            uint64_t winnerMoney = ctx.moneyOnTable / winnersCnt;
            
            //add money for winners to balance
            for( int i = 0; i < winnersCnt; i++)
            {
                players[winnersTable[i]].balance += winnerMoney;
            }
        }
    }

    ui->destroy( ui->data );
    return 0;
}
