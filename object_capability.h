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
    void* ptr;
}ocap;

typedef struct ocapObj{
    // place holder, one store Lua value object, if it's table, or primitive, or function, a ptr would do
    // only table or function has reason to be wrapped, gonna be a deep copy
    // coz, what can a primitive do?
    // another holds the permission it has, we will work on revoking authorities latter on
    // one obj, one table

    int wrappedObj;
    int capTable;

}ocapObj;

static int newOcapObj(lua_State *L){
    // todo: deep copy the wrapped

    if (!lua_istable(L, 1)) {
        luaL_error(L, "Expected a table as the first argument");
        return 1;
    }

    ocapObj  *refPtr = (ocapObj *) lua_newuserdata(L, sizeof (newOcapObj));

    // store the table and get a reference
    lua_pushvalue(L, 1); // shallow copy of the table
    refPtr -> wrappedObj = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_newtable(L);
    refPtr -> capTable = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pushvalue(L, 2);
    return 1;
}


int static connector(lua_State *L){
    printf("size of stack is %d", lua_gettop(L));
    lua_pushvalue(L, lua_upvalueindex(1)); // -> gives the wrapped
    lua_pushvalue(L, 2);
    lua_gettable(L, -2);
//    todo: handle when nothing is found
    // yeah, I can call this lua function I've got using pcall
    // how can I get the parameters passed into here?
    // the only known way so far is to register the function


    return 1;
};


static int altNew(lua_State *L){
    //first var is the wrapped
    // create new table, wrapped & caps
    // __ index is set to wrapped
    if (!lua_istable(L, 1)) {
        luaL_error(L, "Expected a table as the first argument");
        return 1;
    }

    lua_newtable(L);
    lua_pushstring(L, "wrapped");
    lua_pushvalue(L, 1);
    lua_settable(L, -3);

    // consider upvaules cap
    lua_pushstring(L, "caps");
    lua_newtable(L);
    lua_settable(L, -3);


    // create metatable
    lua_newtable(L);
    lua_pushstring(L, "__index");
//    lua_pushvalue(L, 1);
    lua_pushvalue(L, 1);
    lua_pushvalue(L, 2);
    lua_pushcclosure(L, connector, 2);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);


    // todo: set up metatable that it 1. set up the environment 2. redirect call to the wrapped

    return 1;
}

static int altaltNew(lua_State *L){
//    return a function with 2 up values that are wrapped and cap?


}


static int toWrapped(lua_State *L){
    // this

    return 0;
}

static int run(){
    // do three things 1, set up the env, execute, reset the env
    // put the resource in




    return  1;
}

static int setUpEnv(lua_State *L){
    // don't mess up the global var
    // set up the execution env the execution need with ref it received
    // #1 ocapObj #2 env,it is, however passed on stack
    // if I do something to it,


//    lua_getglobal(L, "_ENV");
//    lua_newtable(L);
//    lua_setglobal(L, "_ENV");
//
//    lua_getglobal(L, "_ENV");
//    lua_pushvalue(L, 1);
//    lua_pushstring(L, "caps");
//    lua_gettable(L, -2);
//    lua_pushstring(L, "os");
//    lua_gettable(L, -2); // os table is on top
//    lua_pushstring(L, "os");
//    lua_pushvalue(L, -2);
//    lua_settable(L, 3);
//
//    lua_pushvalue(L, 3);

    // create new table, copy _ENV, caps init, set it as _ENV

//    lua_newtable(L);

// -----
    // #1 var: wrap
    lua_getglobal(L, "_G"); // #2
    lua_newtable(L); // #3, gonna be new _ENV
    lua_pushstring(L, "caps");
    lua_gettable(L, 1); // caps is #4
//
//    // copying _ENV
    lua_pushnil(L);
    while(lua_next(L, 2) != 0){
        lua_pushvalue(L, -2);
        lua_pushvalue(L, -2);
        lua_settable(L, 3);
        lua_pop(L, 1);
    }
//
    // copying _caps
    lua_pushnil(L);
    while(lua_next(L, 4) != 0){
        lua_pushvalue(L, -2);
        lua_pushvalue(L, -2);
        lua_settable(L, 3);
        lua_pop(L, 1);
    }

//    lua_pushstring(L, "_G");
//    lua_pushvalue(L, 3); // the new table
//    lua_setglobal(L, "_ENV");

//    lua_getglobal(L, "_G");
//    lua_newtable(L);
//    lua_pushstring(L, "key");
//    lua_pushstring(L, "val");
//    lua_settable(L, -3);
//    lua_setglobal(L, "_G");
//
//    lua_getglobal(L, "key");

    lua_pushvalue(L, 3);
    lua_setglobal(L, "_G");

    // ha, I see, maybe before the funtion is over,it doesn't finalize the changes

//    lua_pushstring(L, "os")
//    lua_setglobal(L,"_ENV");
//    lua_getglobal(L, "os");

//    lua_pushglobaltable(L);

//    lua_setlocal()

    return 1;
}


static int getCapabilities(lua_State *L){
    ocapObj *obj = (ocapObj *) lua_touserdata(L, 1);

//    lua_rawgeti(L, LUA_REGISTRYINDEX, obj->capTable);
    lua_pushinteger(L, obj->capTable);
    lua_gettable(L, LUA_REGISTRYINDEX);

    return 1;
}

static int getWrappedObj(lua_State *L){
    ocapObj  *refPtr = (ocapObj *) lua_touserdata(L, 1);

    lua_pushinteger(L, refPtr->wrappedObj);
    lua_gettable(L, LUA_REGISTRYINDEX);

    return 1;
}
static int tableSize(lua_State *L){
    // first argument need to be a table

    return 1;
}

// real cap transaction is much more complicated than this
// How the fk Lua's table is implemented, why is it used for Array and set?

static int receiveCap(lua_State * L){
    // first is the ocapObj, second is cap name, third is the table

    /*Although cap is a table, we treat it more like a set
    Coz, how are we supposed to know what capability this is receiving
    unless we have a API with exhaustive list of all possible caps, right?*/

    ocapObj *obj = (ocapObj *) lua_touserdata(L, 1);
    lua_pushinteger(L, obj->capTable);
    lua_gettable(L, LUA_REGISTRYINDEX);

    lua_pushvalue(L, 2);
    lua_pushvalue(L, 3);
    lua_settable(L, -3);
    return  1;
}

static int altReceiveCap(lua_State * L){
    lua_pushstring(L, "caps");
    lua_gettable(L, 1);

    lua_pushvalue(L, 2);
    lua_pushvalue(L, 3);
    lua_settable(L, -3);
    return  1;
}





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
        {"newOcapObj", newOcapObj},
        {"getWrappedObj", getWrappedObj},
        {"ocap", thisIsOcap },
        {"getCapabilities", getCapabilities},
        {"receiveCap", receiveCap},
        //-------------
        {"altNew", altNew},
        {"altReceiveCap", altReceiveCap},
        {"setUpEnv", setUpEnv},

        {NULL, NULL},
};

int luaopen_ocap(lua_State *L){
    luaL_newlib(L, OCaplib);
    return 1;
};

int ocaping(lua_State *L){
    // Handler, called in main
    //----
    luaopen_array(L);
    lua_setglobal(L, "array");

    luaopen_ocap(L);
    lua_setglobal(L, "ocap");

    //#include "bool_array.h"
//    lua_pushcfunction(L, l_sin);
//    lua_setglobal(L, "mysin");
//
//    lua_pushcfunction(L, helloWord);
//    lua_setglobal(L, "helloWorld");
//    luaopen_mylib(L);
//    lua_pushcfunction(L, luaopen_mylib);
//    lua_setglobal(L, "openmylib");


    lua_setglobal(L, "mylib");
}


static int l_sin (lua_State *L) {
    double d = lua_tonumber(L, 1); /* get argument */
    lua_pushnumber(L, sin(d)); /* push result */
    return 1; /* number of results */
}

static int helloWord(lua_State *L){
    char *str = "Hello world";
    lua_pushnumber(L, 9);
    lua_pushstring(L, str);
    return 2;
}

static const struct luaL_Reg ocapLib[] = {
        {"helloWorld", helloWord},
        {"mysin", l_sin},

        {NULL, NULL}
};









#endif //OCAP_LUA_OBJECT_CAPABILITY_H
