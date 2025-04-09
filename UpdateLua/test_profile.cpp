// Active code page: 65001
// 性能测试结果 (10,000 次迭代):
// C++ 原生加法耗时: 13 微秒    
// Lua 脚本加法耗时: 215 微秒   
// 性能差异比例: 16倍
// 性能测试结果 (1,000,000 次迭代):
// C++ 原生加法耗时: 1,352 微秒    
// Lua 脚本加法耗时: 22,430 微秒   
// 性能差异比例: 16倍
// 性能测试结果 (100,000,000 次迭代):
// C++ 原生加法耗时: 164,113 微秒    
// Lua 脚本加法耗时: 2,132,386 微秒  
// 性能差异比例: 12倍
// 性能测试结果 (1,000,000,000 次迭代):
// C++ 原生加法耗时: 1,558,495 微秒
// Lua 脚本加法耗时: 18,383,604 微秒
// 性能差异比例: 11倍

// UpdateLua.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <chrono>
//测试5.1
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
// xndLua类定义

int main()
{
    // 设置控制台输入输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // 启用控制台 UTF-8 代码页
    system("chcp 65001");
    
    // 设置 cout 的本地化
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale(""));
    std::cout.imbue(std::locale(""));

    // 创建Lua状态并加载库
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // 定义不同的迭代次数
    const int iterations_list[] = {10000, 1000000, 100000000, 1000000000};
    
    for (int iterations : iterations_list) {
        double a = 5.0, b = 3.0;

        // 测试C++原生加法
        auto start_cpp = std::chrono::high_resolution_clock::now();
        double cpp_result = 0;
        for(int i = 0; i < iterations; i++) {
            cpp_result = a + b;
        }
        auto end_cpp = std::chrono::high_resolution_clock::now();
        auto duration_cpp = std::chrono::duration_cast<std::chrono::microseconds>(end_cpp - start_cpp).count();

        // 测试Lua脚本加法
        luaL_dostring(L, R"(
            function test_add(a, b, iterations)
                local result = 0
                for i = 1, iterations do
                    result = a + b
                end
                return result
            end
        )");

        lua_getglobal(L, "test_add");
        lua_pushnumber(L, a);
        lua_pushnumber(L, b);
        lua_pushinteger(L, iterations);

        auto start_lua = std::chrono::high_resolution_clock::now();
        lua_pcall(L, 3, 1, 0);
        auto end_lua = std::chrono::high_resolution_clock::now();
        auto duration_lua = std::chrono::duration_cast<std::chrono::microseconds>(end_lua - start_lua).count();

        double lua_result = lua_tonumber(L, -1);
        lua_pop(L, 1);

        std::cout << "性能测试结果 (" << iterations << " 次迭代):" << std::endl;
        std::cout << "C++ 原生加法耗时: " << duration_cpp << " 微秒" << std::endl;
        std::cout << "Lua 脚本加法耗时: " << duration_lua << " 微秒" << std::endl;
        if (duration_cpp < 0.000001) {  // 如果 C++ 执行时间过短
            std::cout << "C++ 执行时间过短，无法计算准确的性能比例" << std::endl;
        } else {
            std::cout << "性能差异比例: " << duration_lua / duration_cpp << "倍" << std::endl;
        }
    }

    // 清理
    lua_close(L);
    return EXIT_SUCCESS;
}

