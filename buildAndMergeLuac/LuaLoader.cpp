#include "LuaLoader.h"
#include <cstdint>

bool LuaLoader::LoadPackage(lua_State* L, const std::string& pakFile) {
    FILE* f = fopen(pakFile.c_str(), "rb");
    if(!f) return false;
    
    // 读取文件数量
    uint32_t fileCount;
    fread(&fileCount, sizeof(fileCount), 1, f);
    
    for(uint32_t i = 0; i < fileCount; i++) {
        // 读取文件名
        uint32_t nameLen;
        fread(&nameLen, sizeof(nameLen), 1, f);
        
        char* filename = new char[nameLen + 1];
        fread(filename, 1, nameLen, f);
        filename[nameLen] = 0;
        
        // 读取bytecode
        uint32_t size;
        fread(&size, sizeof(size), 1, f);
        
        char* buffer = new char[size];
        fread(buffer, 1, size, f);
        
        // 加载bytecode
        if(luaL_loadbuffer(L, buffer, size, filename) == 0) {
            lua_pcall(L, 0, 0, 0);
        }
        
        delete[] filename;
        delete[] buffer;
    }
    
    fclose(f);
    return true;
}