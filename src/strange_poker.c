#include <stdio.h>
#include <ui.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{  
    printf("Hello World! 🂡 🂢 🂣 🂤 🂥 🂦 🂧 🂨 🂩 🂪 🂫 🂬 🂭 🂮\n");

    IuiHandler_t *ui = uiHandlerFactory("ncurses");
    if( !ui )
    {
        fprintf(stderr, "cannot crate ui system\n");
        return -1;
    }

    while( 1 ) 
    {
        GameMenuOptions opt = ui->gameMenu( ui->data );
        if( opt == ExitGame ) break;

        if( opt == StartNewRound ) {}; // ?????
    }
    
    ui->destroy( ui->data );
}