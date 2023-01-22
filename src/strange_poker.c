#include <stdio.h>
#include <ui.h>
#include <time.h>
#include <stdlib.h>

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

void bidding(uint64_t *min_bid, uint32_t val_Cards, player_t player, uint32_t *CntPlayers, IuiHandler_t *ui, GameContext_t *ctx){
    player.validCards = val_Cards;
    ui->updateState(ui->data, &ctx);
    uint32_t bid = ui->bid (ui -> data, &ctx);
    player.bid=bid;
    while(bid < *min_bid){
        if(bid == 0) CntPlayers--;
        else{
            ui->messageUser( ui->data, &ctx, "Your bid is lower then the minimal amount" );
            uint32_t bid = ui->bid (ui -> data, &ctx);
            player.bid=bid;
        }
    }
    if(bid > *min_bid){
        *min_bid = bid;
    }
    player.validCards = 0;
    ui->updateState(ui->data, &ctx);
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
            ui->messageUser( ui->data, &ctx, "Start Game!" );

            //preflop bids
            uint64_t min_bid=20;
            players[0].bid = 10;//small blind
            players[1].bid = 20; //big blind
            // need to switch the first player to the last position after the end of the round
            int last_player=0;
            uint32_t current_players = playerCnt;
            for( uint32_t i=2; i < playerCnt; i++){
                ctx.currentPlayer=i;
                uint64_t pom = min_bid;
                bidding( &min_bid, 4, players[i], &current_players, ui, &ctx);
                if(min_bid > pom){
                    if (i == 0) last_player = playerCnt-1;
                    else last_player = i-1;
                }
            }
            while(players[last_player].bid < min_bid){
                for( uint32_t i=0; i < playerCnt; i++){
                    ctx.currentPlayer=i;
                    uint64_t pom = min_bid;
                    bidding( &min_bid, 4, players[i], &current_players, ui, &ctx);
                    if(min_bid > pom){
                        if (i == 0) last_player = playerCnt-1;
                        else last_player = i-1;
                    }
                }
            }
            ctx.moneyOnTable = min_bid * current_players;


            //flop
            ctx.visibleTableCards = 3;
            

            // ?????
        }
    }

    ui->destroy( ui->data );
    return 0;
}
