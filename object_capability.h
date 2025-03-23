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


// it is the mini prison, loads up environment, execute, restore environment

static const luaL_Reg safe_funcs[] = {
    {LUA_GNAME, luaopen_base},
    {LUA_LOADLIBNAME, luaopen_package},
    {LUA_COLIBNAME, luaopen_coroutine},
    {LUA_DBLIBNAME, luaopen_debug},
    {LUA_IOLIBNAME, luaopen_io},
    {LUA_MATHLIBNAME, luaopen_math},
    {LUA_OSLIBNAME, luaopen_os},
    {LUA_STRLIBNAME, luaopen_string},
    {LUA_TABLIBNAME, luaopen_table},
    {LUA_UTF8LIBNAME, luaopen_utf8},
    {NULL, NULL}
};


#define MAX_RETURN 32

// The name is because only when it gets activated by ()
// It starts executes functions and return
// otherwise, it sits there, like a box with jack in it
int static jackInTheBox(lua_State *L) {
    lua_pushstring(L, "box activated");
    l_print(L);

    int functionIndex = lua_upvalueindex(1);
    int oCapEnvIdx = lua_upvalueindex(2);
    int inputSize = lua_gettop(L);

    // Later restoring _G
    lua_rawgeti(L,LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
    int outsideEnvIdx = lua_gettop(L);

//    lua_pushglobaltable(L);
//    l_print(L);
//
//    lua_getglobal(L, "_G");
//    l_print(L);

    // Set env to this objects's
    lua_pushvalue(L, oCapEnvIdx);
    lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
    luaopen_base(L);


    lua_pushnil(L);
    lua_setglobal(L, "io");



    int stackSize = lua_gettop(L);
    lua_pushvalue(L, functionIndex);
    //push all the input back on top
    for(int i = 1; i <= inputSize; i++){
        lua_pushvalue(L, i);
    }
    lua_pcall(L, inputSize, MAX_RETURN, 0);

    // remove Nil Paddings
    while(lua_isnil(L, -1)){
        lua_pop(L, 1);
    }

    lua_pushvalue(L, outsideEnvIdx);
    lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
    return lua_gettop(L) - stackSize;
}


// handler - act as a proxy, redirect all traffic through this function
// other than special key worlds, it's either getting value from the inner obj, or function execution
// function execution is redirected to  the jack in the box
// load the environment, before return a value or execute a function
// need to return the values as an ocap obj, which is a new wrap? but they can share the same wrap, right?
// that's for the later
int static handler(lua_State *L){
    // *------
    int innerObjIdx = lua_upvalueindex(1);
    int envIdx = lua_upvalueindex(2);

    int wrapIdx = 1;
    int keyIdx = 2;
    // *---------

    const char *key = lua_tostring(L, keyIdx);

    // special keywords
    if(strcmp(key, "env") == 0){
        lua_pushvalue(L, envIdx);
        return 1;
    }
    if(strcmp(key, "inside") == 0){
        lua_pushvalue(L, innerObjIdx);
        return 1;
    }

    if(strcmp(key, "addCap") == 0){

    }

    if(strcmp(key, "removeCap") == 0){

    }

    lua_getfield(L, innerObjIdx, key);

    // val from inner object is  not a function
    // todo: return objects as ocaped obj
    if(!lua_isfunction(L, -1)){
        return  1;
    }

    // It's a function, return the box with Jack in it
    int funcIndex = lua_gettop(L);
    lua_pushvalue(L, funcIndex);
    lua_pushvalue(L, envIdx);
    lua_pushcclosure(L, jackInTheBox, 2);


    return 1;
};

int static getCaps(lua_State *L){
    lua_pushvalue(L, lua_upvalueindex(1));
    return 1;
}

// This function create an ocapified object
// An table surround the original object, carrying it's own environment
// deprived of outside environment accessibility

static int ocapify(lua_State *L){
    int innerObjIdx = lua_gettop(L);
    if (!lua_istable(L, innerObjIdx)) {
        luaL_error(L, "Expecting object");
        return 1;
    }

    lua_newtable(L); int envIdx = lua_gettop(L);
    luaL_setfuncs(L,safe_funcs, 0);

    lua_newtable(L); int outsideObjIdx = lua_gettop(L);
    lua_newtable(L); int mtIndex = lua_gettop(L);

    lua_pushvalue(L, innerObjIdx);
    lua_pushvalue(L, envIdx);
    lua_pushcclosure(L, handler, 2);

    lua_setfield(L, mtIndex, "__index"); // mt { "__index" = handler + 2 upvals}
    lua_setmetatable(L, outsideObjIdx); // setmt(wrap, mt)

    lua_pushvalue(L, outsideObjIdx);
    return 1;
}



static int tableSize(lua_State *L){
    // first argument need to be a table

    return 1;
}


static const struct luaL_Reg oCapLib[] = {
        {"ocapify", ocapify},

        {NULL, NULL},
};




int register_ocap_lib(lua_State *L){
    luaL_newlib(L, oCapLib);
    lua_setglobal(L, "ocap");
}

#endif //OCAP_LUA_OBJECT_CAPABILITY_H
