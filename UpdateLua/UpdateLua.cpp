#include <lua.hpp>
#include <thread>
#include <vector>

int main() {
    // 创建7个Lua虚拟机
    lua_State* L1 = luaL_newstate();
    lua_State* L2 = luaL_newstate();
    lua_State* L3 = luaL_newstate();
    lua_State* L4 = luaL_newstate();
    lua_State* L5 = luaL_newstate();
    lua_State* L6 = luaL_newstate();
    lua_State* L7 = luaL_newstate();

    // 创建7个线程执行虚拟机
    std::vector<std::thread> threads;

    threads.push_back(std::thread([&]() {
        luaL_openlibs(L1);
        // L1的工作...
    }));

    threads.push_back(std::thread([&]() {
        luaL_openlibs(L2);
        // L2的工作...
    }));

    threads.push_back(std::thread([&]() {
        luaL_openlibs(L3);
        // L3的工作...
    }));

    threads.push_back(std::thread([&]() {
        luaL_openlibs(L4);
        // L4的工作...
    }));

    threads.push_back(std::thread([&]() {
        luaL_openlibs(L5);
        // L5的工作...
    }));

    threads.push_back(std::thread([&]() {
        luaL_openlibs(L6);
        // L6的工作...
    }));

    threads.push_back(std::thread([&]() {
        luaL_openlibs(L7);
        // L7的工作...
    }));

    // 等待所有线程完成
    for(auto& thread : threads) {
        thread.join();
    }

    // 关闭所有虚拟机
    lua_close(L1);
    lua_close(L2);
    lua_close(L3);
    lua_close(L4);
    lua_close(L5);
    lua_close(L6);
    lua_close(L7);

    return 0;
}


