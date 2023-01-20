#include <stdio.h>
#include <ui.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

char cards[52];

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
            
            for(int i=0; i<52;i++){
                cards[i]='0';
            }
            for (uint32_t i = 0; i < playerCnt; i++){
                for (int j=0; j<5;j++){
                    int c=rand() % 52;
                       while(cards[c]!='0') c=rand() % 52;
                        players[i].cards[j].suit=c/4;
                        players[i].cards[j].value=(c%13)+2;
                        cards[c]='1';
                }
            }
            for (int i=0; i<5;i++){
                int c=rand() % 52;
                while(cards[c]!='0') c=rand() % 52;
                ctx.tableCards[i].suit=c/4;
                ctx.tableCards[i].value=(c%13)+2;
                cards[c]='1';
            }

            // show beginning of the game and wait for input
            ui->messageUser( ui->data, &ctx, "Start Game!" );
            

            // ?????
        }
    }

    ui->destroy( ui->data );
    return 0;
}
