#include <iostream>
#include <windows.h>
#include <lua.hpp>

int main() {
    // 设置控制台输入输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001");

    // 初始化 Lua
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // 加载并执行 luaScript/test.lua 脚本
    if (luaL_dofile(L, "luaScript/test_new_printf.lua") != 0) {
        std::cout << "脚本加载错误: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return 1;
    }

    // 清理
    lua_close(L);
    return 0;
}

