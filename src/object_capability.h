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
// instead of load and clean environment, why not use local upvalue?


int static loadEnv(lua_State *L){
    int capsIndex = 1;
    // use this table to keep the replaced var from elements
    // later restore it
    lua_newtable(L); int replacedIndex = lua_gettop(L);

    lua_pushnil(L);
    while(lua_next(L, capsIndex) != 0){
        int valIndex = lua_gettop(L);
        int keyIndex = valIndex -1;

        // if key exists in _G already, put it in the replaced

        char *key = lua_tostring(L, keyIndex);
        lua_getglobal(L, key);
        if(lua_isnil(L, -1)){
            lua_pop(L, 1);
        }else{
            // note that two level reference need two steps
            lua_setfield(L, replacedIndex, key);
        }
        lua_pushvalue(L, valIndex);
        lua_setglobal(L, key);

        lua_pop(L, 1);
    }

    lua_pushvalue(L, replacedIndex);
    return 1;

}

int static cleanEnv(lua_State *L){
    int capsIndex = lua_gettop(L);
    int theReplaced = capsIndex - 1;
    // set all caps.val = nil

    lua_pushnil(L);
    while(lua_next(L, capsIndex) != 0){
        int keyIdx = lua_gettop(L) - 1;
        int valIdx = lua_gettop(L);

        char *key = lua_tostring(L, keyIdx);
        lua_pushnil(L);
        lua_setglobal(L, key);

        lua_pop(L, 1);
    }

    // put the overwritten ones

    lua_pushnil(L);
    while(lua_next(L, theReplaced)){
        int keyIdx = lua_gettop(L) - 1;
        int valIdx = lua_gettop(L);

        lua_pushvalue(L, valIdx);
        lua_setglobal(L, lua_tostring(L, keyIdx));

        lua_pop(L, 1);
    }

    return 0;
};


int static jackInTheBox(lua_State *L) {
    int functionIndex = lua_upvalueindex(1);
    int capsIndex = lua_upvalueindex(2);
    int varIndex = 1;
    int numVars = lua_gettop(L);

    lua_pushcfunction(L, loadEnv);
    lua_pushvalue(L, capsIndex);
    lua_pcall(L, 1, 1, 0);
    int replacedIdx = lua_gettop(L);


    int stackSizeBefore = lua_gettop(L);
    lua_pushvalue(L, functionIndex);
    lua_pushvalue(L, varIndex);
    lua_pcall(L, numVars, 10, 0);
    while(lua_isnil(L, -1)){
        lua_pop(L, 1);
    }

    lua_pushcfunction(L, cleanEnv);
    lua_pushvalue(L,replacedIdx);
    lua_pushvalue(L, capsIndex);
    lua_pcall(L, 2, 0, 0);

    int stackSizeAfter = lua_gettop(L);

    //return is not yet fully designed
    return stackSizeAfter - stackSizeBefore;
}
int static setCap(lua_State *L){
    //it's receiving cap, avoiding renaming
    // the function will be passed to client, then trigger
    int capsIndex = lua_upvalueindex(1);
    int keyIndex = 1;
    int capIndex = 2;

    lua_setfield(L, capsIndex, lua_tostring(L, keyIndex));
    return 0;
}

// The way I am treating this handler and functions
// it invokes is almost like object oriented programming
// Given each function parameters it needs (sorta like vars associated with class)then pass it
// user passes other parameters into the wrapping function
// then, it load env, then execute


int static handler(lua_State *L){
    // *------
    int wrappedIdx = lua_upvalueindex(1);
    int capsIdx = lua_upvalueindex(2);
    int wrapIdx = 1;
    int keyIdx = 2;
    // *---------

//    lua_pushvalue(L, keyIdx);
    const char *key = lua_tostring(L, keyIdx);
//    printf("key is %s ", key);


    if(strcmp(key, "caps") == 0){
        lua_pushvalue(L, capsIdx);
        return 1;
    }


    if(strcmp(key, "receiveCap") == 0){
        lua_pushvalue(L, capsIdx);
        lua_pushcclosure(L, setCap, 1);
        return 1;
    }


    lua_getfield(L, wrappedIdx, key);
    if(!lua_isfunction(L, -1)){
        return  1;
    }
    int funcIndex = lua_gettop(L);

    lua_pushvalue(L, funcIndex);
    lua_pushvalue(L, capsIdx);
    lua_pushcclosure(L, jackInTheBox, 2);

    return 1;
};

int static getCaps(lua_State *L){
    lua_pushvalue(L, lua_upvalueindex(1));
    return 1;
}


static int altNew(lua_State *L){
    //first var is the wrapped
    // create new table, wrapped & caps
    // __ index is set to wrapped
    int fallBackTableIndex = lua_gettop(L);
    if (!lua_istable(L, fallBackTableIndex)) {
        luaL_error(L, "Expected a table as the first argument");
        return 1;
    }



    lua_newtable(L); int oCapObjIndex = lua_gettop(L);
    lua_newtable(L); int caps_index = lua_gettop(L);
    lua_newtable(L); int mtIndex = lua_gettop(L);
//    lua_newtable(L);

    lua_pushvalue(L, fallBackTableIndex);
    lua_pushvalue(L, caps_index);
    lua_pushcclosure(L, handler, 2);

    lua_setfield(L, mtIndex, "__index"); // mt { "__index" = handler + 2upvals}

    lua_setmetatable(L, oCapObjIndex); // setmt(wrap, mt)

    lua_pushvalue(L, oCapObjIndex);
    return 1;
}


static int toWrapped(lua_State *L){
    // this

    return 0;
}

static int run(lua_State *L){
    // do three things 1, set up the env, execute, reset the env
    // put the resource in
    lua_pushinteger(L, lua_gettop(L));

    return  1;
}

static int tableSize(lua_State *L){
    // first argument need to be a table

    return 1;
}

static int introduction(lua_State *L){
    // take in two caps object A, B


    return 0;
}

static const struct luaL_Reg oCapLib[] = {
        {"run", run},
        //-------------
        {"altNew", altNew},

        {NULL, NULL},
};




int register_ocap_lib(lua_State *L){
    luaL_newlib(L, oCapLib);
    lua_setglobal(L, "ocap");
}

#endif //OCAP_LUA_OBJECT_CAPABILITY_H
