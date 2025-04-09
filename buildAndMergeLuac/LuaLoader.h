#pragma once

#include <string>
#include <cstdio>

#include "lua.hpp"

class LuaLoader {
public:
    bool LoadPackage(lua_State* L, const std::string& pakFile);
}; 