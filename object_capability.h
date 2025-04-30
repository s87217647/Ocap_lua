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


static int printTable(lua_State *L, int index){
    lua_pushnil(L);
    while (lua_next(L, index) != 0) {
        // Key
        if (lua_type(L, -2) == LUA_TSTRING)
            printf("%s, ", lua_tostring(L, -2));
        else if (lua_type(L, -2) == LUA_TNUMBER)
            printf("%d, ", lua_tonumber(L, -2));

        // Print value
        if (lua_type(L, -1) == LUA_TSTRING)
            printf("%s\n", lua_tostring(L, -1));
        else if (lua_type(L, -1) == LUA_TNUMBER)
            printf("%d\n", lua_tonumber(L, -1));
        else if (lua_type(L, -1) == LUA_TFUNCTION)
            printf("cFunc-%d\n", lua_tocfunction(L, -1));
        else if (lua_type(L, -1) == LUA_TNIL)
            printf("%s\n", "nil");
        else if (lua_type(L, -1) == LUA_TBOOLEAN)
            printf("Bool-%d\n", lua_toboolean(L, -1));

        else
            printf("non-premitive \n");

        lua_pop(L, 1);
    }
    return 0;
}

static const luaL_Reg danger[] = {
        {"io", iolib},
        {"os", syslib},
        {"debug", dblib},
        {"load", luaB_load},
        {"loadfile", luaB_loadfile},
//        {"require", luaL_requiref},
        {NULL, NULL}
};


int static loadEnv(lua_State *L){
    int capsIndex = 1;
    // Table for keeping replaced elements, for future restoring
    lua_newtable(L); int replacedIndex = lua_gettop(L);

    // remove dangerous resources, put in restoring table
    for(const luaL_Reg  *lib = danger; lib->name != NULL; lib ++){
        lua_getglobal(L, lib->name);
        lua_setfield(L, replacedIndex, lib->name);
        lua_pushnil(L);
        lua_setglobal(L, lib->name);
    }

    //put given resources in global env
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

//    lua_getglobal(L, "io");
//    printTable(L, lua_gettop(L));

    lua_pushvalue(L, replacedIndex);
    return 1;
}

int static cleanEnv(lua_State *L){
    int replacedIdx = 1;
    int capsIndex = 2;


    lua_pushnil(L);
    while(lua_next(L, replacedIdx)){
        int keyIdx = lua_gettop(L) - 1;
        int valIdx = lua_gettop(L);

//        printf(L, lua_tostring(L, keyIdx));
        lua_pushvalue(L, valIdx);
        lua_setglobal(L, lua_tostring(L, keyIdx));

        lua_pop(L, 1);
    }

    lua_pushboolean(L, 1);
    return 1;
};

int static customizedSandbox(lua_State *L) {
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

    return lua_gettop(L) - stackSizeBefore;
}



int static receiveCap(lua_State *L){
    int capsIndex = lua_upvalueindex(1);
    int resourceIdx = 1;
    int pathIdx = 2;

    char *path = lua_tostring(L, pathIdx);
    char *token = strtok(path, ".");
    char *next = strtok(NULL, ".");

    while(next != NULL){
        lua_getfield(L, capsIndex, token);

        if(lua_isnil(L, -1)){
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, capsIndex, token);
            capsIndex = lua_gettop(L);
        }else{
            capsIndex = lua_gettop(L);
        }
        token = next;
        next = strtok(NULL, ".");
    }
    lua_pushvalue(L, resourceIdx);
    lua_setfield(L, capsIndex, token);
    lua_pushboolean(L, 1);
    return 1;

}

// The way I am treating this handler and functions
// it invokes is almost like object oriented programming
// Given each function parameters it needs (sorta like vars associated with class)then pass it
// user passes other parameters into the wrapping function
// then, it load env, then execute


int static handler(lua_State *L){
    int indsideObjIdx = lua_upvalueindex(1);
    int capsIdx = lua_upvalueindex(2);
    int wrapIdx = 1;
    int keyIdx = 2;

    const char *key = lua_tostring(L, keyIdx);

    if(strcmp(key, "caps") == 0){
        lua_pushvalue(L, capsIdx);
        return 1;
    }

    if(strcmp(key, "receiveCap") == 0){
        lua_pushvalue(L, capsIdx);
        lua_pushcclosure(L, receiveCap, 1);
        return 1;
    }

    if(strcmp(key, "removeCap") == 0){
        lua_pushvalue(L, capsIdx);
        lua_pushcclosure(L, receiveCap, 1);
        return 1;
    }

    lua_getfield(L, indsideObjIdx, key);

    //results comes back a table or primitive
    if(!lua_isfunction(L, -1)){
        // todo: if is table, return it as an ocaped obj
        return  1;
    }

    // results comes back a function
    int funcIndex = lua_gettop(L);

    lua_pushvalue(L, funcIndex);
    lua_pushvalue(L, capsIdx);
    lua_pushcclosure(L, customizedSandbox, 2);

    return 1;
};

int static getCaps(lua_State *L){
    lua_pushvalue(L, lua_upvalueindex(1));
    return 1;
}


static int ocapify(lua_State *L){
    //first var is the wrapped
    // create new table, wrapped & caps
    // __ index is set to wrapped
    if (!lua_istable(L, 1)) {
        luaL_error(L, "Expecting Table");
        return 1;
    }

    int objIdx = 1;

    lua_newtable(L); int ocapedObjIdx = lua_gettop(L);
    lua_newtable(L); int capsIdx = lua_gettop(L);
    lua_newtable(L); int mtIndex = lua_gettop(L);

    lua_pushvalue(L, objIdx);
    lua_pushvalue(L, capsIdx);
    lua_pushcclosure(L, handler, 2);

    lua_setfield(L, mtIndex, "__index"); // mt { "__index" = handler + 2upvals}
    lua_setmetatable(L, ocapedObjIdx); // setmt(wrap, mt)

    lua_pushvalue(L, ocapedObjIdx);
    return 1;
}


static const struct luaL_Reg OCaplib[] = {
        {"ocapify",    ocapify},
        {NULL, NULL},
};


int luaopen_ocap(lua_State *L){
    luaL_newlib(L, OCaplib);
    lua_setglobal(L, "ocap");
    return 1;
};

#endif //OCAP_LUA_OBJECT_CAPABILITY_H
