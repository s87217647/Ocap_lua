//
// Created by Swift Sheng on 10/1/24.
//

#ifndef OCAP_LUA_OBJECT_CAPABILITY_H
#define OCAP_LUA_OBJECT_CAPABILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"
#include "llimits.h"


// what's a write ocap look like?

typedef struct ocap{
    char* ID;
    void* ptr; // should be point to the obj eventually
}ocap;

typedef struct OcapObj{

};


static int resolvingRefernce(){
    // The Idea is that reference could lead to a reference

    return 1;
};

static int thisIsOcap(lua_State *L){
    lua_pushstring(L, "this is OCap");
//    lua_pushinteger(L, 1);
    return 1;
}

int static readFile(){
    return 1;
};


static int give(){

    return 1;
}

static int revoke(){

    return 1;
}

static int print(lua_State *L){
    lua_pushstring(L, "Print not allowed");
    return 1;
}

static const struct luaL_Reg OCaplib[] = {
        {"print", print},
        {"ocap", thisIsOcap },
        {NULL, NULL}
};

int luaopen_ocap(lua_State *L){
    luaL_newlib(L, OCaplib);
    return 1;
};


#endif //OCAP_LUA_OBJECT_CAPABILITY_H
