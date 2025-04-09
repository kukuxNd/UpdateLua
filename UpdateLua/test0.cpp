// UpdateLua.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
//测试5.1
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
// xndLua类定义
class xndLua {
public:
    static int add(lua_State* L) {
        // 检查并获取参数
        double a = luaL_checknumber(L, 1);
        double b = luaL_checknumber(L, 2);
        
        // 计算结果并压入栈
        lua_pushnumber(L, a + b);
        return 1; // 返回值个数
    }

    static int sub(lua_State* L) {
        // 检查并获取参数
        double a = luaL_checknumber(L, 1);
        double b = luaL_checknumber(L, 2);
        
        // 计算结果并压入栈
        lua_pushnumber(L, a - b);
        return 1; // 返回值个数
    }

    static int version(lua_State* L) {
        // 返回版本号
        lua_pushstring(L, "xndLua v1.0.0");
        return 1; // 返回值个数
    }
};

// 注册函数列表
static const luaL_Reg xndLuaLib[] = {
    {"add", xndLua::add},
    {"sub", xndLua::sub}, // 添加减法函数
    {"version", xndLua::version}, // 添加版本函数
    {NULL, NULL}
};

// 注册类的函数
extern "C" int luaopen_xndLua(lua_State* L) {
    luaL_newlib(L, xndLuaLib);
    return 1;
}




int main()
{
    // 设置控制台输出编码为UTF-8
    SetConsoleOutputCP(CP_UTF8);

    // 创建一个新的Lua状态
    lua_State* L = luaL_newstate();
    if (L == nullptr) {
        std::cerr << "Failed to create Lua state." << std::endl;
        return EXIT_FAILURE;
    }

    // 打开Lua标准库
    luaL_openlibs(L);

    // 运行一个简单的Lua脚本
    const char* script = "print('Hello from kuku xnd Lua!')";
    if (luaL_dostring(L, script) != LUA_OK) {
        std::cerr << "Failed to run Lua script." << std::endl;
        return EXIT_FAILURE;
    }

    // 关闭Lua状态
    lua_close(L);

    //测试luaScript/test.lua脚本
    lua_State* L2 = luaL_newstate();
    luaL_openlibs(L2);
    luaL_requiref(L, "xndLua", luaopen_xndLua, 1);
    luaL_dofile(L2, "luaScript/test2.lua");
    lua_close(L2);
    
    return EXIT_SUCCESS;
}

