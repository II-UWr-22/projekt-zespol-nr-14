#include "ui.h"
#include <dlfcn.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct dyn_dat_t{
    IuiHandler_t parent; // inherited
    
    void *library;
    IuiHandler_t *libHandler;
} dyn_dat_t;

typedef IuiHandler_t *(*uiFactory)();

#define DYN_HANDLER(dat) (*((dyn_dat_t*)(dat))->libHandler)
#define DYN_DATA(dat) (DYN_HANDLER(dat).data)

typedef const GameContext_t *ctx_t; 

static player_t *dyn_getPlayers( void *data, uint32_t *cnt )
{
    return DYN_HANDLER(data).getPlayers( DYN_DATA(data), cnt );
}

static GameMenuOptions dyn_gameMenu( void *data )
{
    return DYN_HANDLER(data).gameMenu( DYN_DATA(data) );
}

static uint32_t dyn_drop( void *data, ctx_t ctx )
{
    return DYN_HANDLER(data).drop( DYN_DATA(data), ctx );
}

static uint64_t dyn_bid( void *data, ctx_t ctx )
{
    return DYN_HANDLER(data).bid( DYN_DATA(data), ctx );
}

static void dyn_messageUser( void *data, ctx_t ctx, const char *msg )
{
    DYN_HANDLER(data).messageUser( DYN_DATA(data), ctx, msg );
}

static void dyn_updateState( void *data, ctx_t ctx )
{
    DYN_HANDLER(data).updateState( DYN_DATA(data), ctx );
}

static void dyn_destroy( void *data )
{
    DYN_HANDLER(data).destroy(DYN_DATA(data));
    if( dlclose( ((dyn_dat_t*)data)->library ) ) fprintf(stderr, "Could not close shared library: %s\n", dlerror());

    free( data );
}

static IuiHandler_t *dynamicUIHandlerFactory( const char *file, const char *factoryName )
{
    dyn_dat_t *dyn_data = NULL;
    uiFactory factory = NULL;

    dyn_data = malloc( sizeof( dyn_dat_t ) );
    if( !dyn_data ) goto error;

    dyn_data->library = dlopen( file, RTLD_NOW );
    if( !dyn_data->library ) 
    {
        fprintf(stderr, "%s\n", dlerror());
        goto error;
    }

    dyn_data->parent.data = (void *)dyn_data;

#define GET_MEMBERFN( fname ) dyn_data->parent. fname = dyn_ ## fname;

    GET_MEMBERFN( getPlayers );
    GET_MEMBERFN( gameMenu );
    GET_MEMBERFN( drop );
    GET_MEMBERFN( bid );
    GET_MEMBERFN( messageUser );
    GET_MEMBERFN( updateState );
    GET_MEMBERFN( destroy );

#undef GET_MEMBERFN

    // remdy ISO C forbids conversion of object pointer to function pointer type
    // factory = (uiFactory)dlsym(dyn_data->library, factoryName);
    *(void**)(&factory) = dlsym(dyn_data->library, factoryName);
    if( !factory ) goto error;

    dyn_data->libHandler = factory();
    if( !dyn_data->libHandler ) goto error;

    return &dyn_data->parent;

error:
    if( dyn_data && dyn_data->library ) dlclose( dyn_data->library );
    free(dyn_data);

    return NULL;
}

IuiHandler_t *uiHandlerFactory( const char *backend )
{
    char backendFileBuff[512];
    snprintf( backendFileBuff, sizeof(backendFileBuff), "libui-%s.so", backend );

    return dynamicUIHandlerFactory( backendFileBuff, "uiFactory" );
}