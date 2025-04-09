// UpdateLua.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <chrono>

// 指定 Lua 5.4 头文件的路径
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

// Lua 协程测试脚本
const char* coroutine_test = R"(
    function producer()
        local i = 0
        while i < 5 do
            i = i + 1
            print("生产者生产数据: " .. i)
            coroutine.yield(i)  -- 暂停并返回数据
        end
    end

    function consumer(prod)
        while true do
            local status, value = coroutine.resume(prod)
            if not status or value == nil then
                break
            end
            print("消费者消费数据: " .. value)
        end
    end
)";

int main()
{
    // 设置控制台输入输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001");

    // 初始化 Lua
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // 加载并执行协程测试脚本
    if (luaL_dostring(L, coroutine_test) != 0) {
        std::cout << "脚本加载错误: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return 1;
    }

    // 创建生产者协程
    lua_getglobal(L, "coroutine");
    lua_getfield(L, -1, "create");
    lua_getglobal(L, "producer");
    if (lua_pcall(L, 1, 1, 0) != 0) {
        std::cout << "创建协程错误: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return 1;
    }

    // 获取消费者函数
    lua_getglobal(L, "consumer");
    // 复制协程引用作为参数
    lua_pushvalue(L, -2);
    
    // 执行消费者函数
    if (lua_pcall(L, 1, 0, 0) != 0) {
        std::cout << "执行消费者错误: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return 1;
    }

    // 清理
    lua_close(L);
    return 0;
}

