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


//    lua_createtable(L);

    // setting userdata's metadata to the wrap
    // maybe wanna do something like this userdata -> middle -> inner
    // that way, changes can be chained to middle without going to the inner obj
//    luaL_newmetatable(L, "mt");

    // wanna support his eventually- obj.extraFunc = function() print("something extra") end

//    luaL_newmetatable(L, "middle");
//    luaL_newmetatable(L, "inner");

    lua_newtable(L);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, 1);
    lua_settable(L, -3);

    lua_pushvalue(L, 2);
    lua_pushvalue(L, -2);
    lua_setmetatable(L, -2);


    lua_pushvalue(L, 2);
    return 1;
}

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


    //todo: clean nil

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

// The way I am treating this connector and functions
// it invokes is almost like object oriented programming
// Given each function parameters it needs (sorta like vars associated with class)then pass it
// user passes other parameters into the wrapping function
// then, it load env, then execute


int static connector(lua_State *L){
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
    if (!lua_istable(L, 1)) {
        luaL_error(L, "Expected a table as the first argument");
        return 1;
    }

    int wrappedIndex = 1;

    //set wrapped
    lua_newtable(L);
    int wrapIndex = lua_gettop(L);
//    lua_pushstring(L, "wrapped");
//    lua_pushvalue(L, wrapIndex);
//    lua_settable(L, -3);

    // wrap.caps = new_table

    lua_newtable(L); int caps_index = lua_gettop(L);
//    lua_setfield(L, wrapIndex, "caps");


    // create metatable
    lua_newtable(L); int mtIndex = lua_gettop(L);

    lua_pushvalue(L, wrappedIndex);
    lua_pushvalue(L, caps_index);
    lua_pushcclosure(L, connector, 2);

    lua_setfield(L, mtIndex, "__index"); // mt { "__index" = connector + 2upvals}
    lua_setmetatable(L, wrapIndex); // setmt(wrap, mt)

    lua_pushvalue(L, wrapIndex);
    return 1;
}

static int altaltNew(lua_State *L){
//    return a function with 2 up values that are wrapped and cap?


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
        {"ocap", thisIsOcap },
        {"getCapabilities", getCapabilities},
        {"receiveCap", receiveCap},
        {"run", run},
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
