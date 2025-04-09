#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <windows.h>

#include "lua.hpp"

class LuaPackager {
public:
    bool CompileDirectory(const std::string& dir);

private:
    void ScanDirectory(const std::string& dir, std::vector<std::string>& files);
    bool CompileToByteCode(const std::string& file);
    bool MergeByteCode(const std::vector<std::string>& files, const std::string& outFile);
    static int WriteBytecode(lua_State* L, const void* p, size_t size, void* u);
}; 