// Lua标准库头文件

// 自定义类的头文件
#include "lua.hpp"
#include "LuaPackager.h"
#include "LuaLoader.h"

#include <string>

// 解析命令行参数获取目录
std::string getDirectory(int argc, char* argv[]) {
    std::string dir;
    for(int i = 1; i < argc; i++) {
        if(std::string(argv[i]) == "-f" && i + 1 < argc) {
            dir = argv[i + 1];
            break;
        }
    }
    return dir;
}

// 修改main函数声明,添加命令行参数
int main(int argc, char* argv[]) {
    try {
        // 获取目录参数
        std::string dir = getDirectory(argc, argv);
        if(dir.empty()) {
            printf("请使用 -f 参数指定目录\n");
            return 1;
        }

        // 打包阶段
        LuaPackager packager;
        if(!packager.CompileDirectory(dir)) {
            printf("编译目录 %s 失败\n", dir.c_str());
            return 1;
        }
        
        // 加载阶段
        lua_State* L = luaL_newstate();
        if (!L) {
            printf("Failed to create Lua state\n");
            return 1;
        }
        
        luaL_openlibs(L);
        
        LuaLoader loader;
        if (!loader.LoadPackage(L, "output.pak")) {
            printf("Failed to load package\n");
            lua_close(L);
            return 1;
        } else {
            FILE* fp = fopen("output.lua", "wb");
            if (!fp) {
                printf("Failed to create output.lua\n");
                lua_close(L);
                return 1;
            }
            lua_getglobal(L, "string");
            lua_getfield(L, -1, "dump");
            lua_pushvalue(L, -3);
            lua_call(L, 1, 1);
            size_t size;
            const char* buffer = lua_tolstring(L, -1, &size);
            fwrite(buffer, 1, size, fp);
            fclose(fp);
        }
        lua_close(L);
        return 0;
    } catch (const std::exception& e) {
        printf("Exception caught: %s\n", e.what());
        return 1;
    } catch (...) {
        printf("Unknown exception caught\n");
        return 1;
    }
}