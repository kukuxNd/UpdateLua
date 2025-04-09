#include "LuaPackager.h"
#include <cstdint>
#include <fstream>

bool LuaPackager::CompileDirectory(const std::string& dir) {
    std::vector<std::string> files;
    ScanDirectory(dir, files);
    
    if(files.empty()) {
        return false;
    }
    
    // 编译每个文件
    for(const auto& file : files) {
        // 判定 .lua结尾
        // 检查文件是否以.lua结尾,而不仅仅是包含.lua字符串
        size_t len = file.length();
        if(len < 4 || file.substr(len-4) != ".lua") {
            continue;
        }
        if(!CompileToByteCode(file)) {
            printf("编译文件 %s 失败\n", file.c_str());
            return false;
        }
    }
    
    // 合并所有编译后的文件
    bool result = MergeByteCode(files, "output.pak");

    // 生成清单日志
    std::ofstream log("output.log");
    if(log.is_open()) {
        log << "打包文件清单:\n";
        for(const auto& file : files) {
            log << file << "\n";
            // 删除临时的luac文件
            std::string luacFile = file + "c";
            remove(luacFile.c_str());
        }
        log.close();
    }

    return result;
}

void LuaPackager::ScanDirectory(const std::string& dir, std::vector<std::string>& files) {
    WIN32_FIND_DATAA findData;  // 使用ANSI版本
    
    // 先扫描当前目录下的lua文件
    std::string searchPath = dir + "\\*.lua";
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);  // 使用ANSI版本
    
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string filename = dir + "\\" + findData.cFileName;
                files.push_back(filename);
            }
        } while(FindNextFileA(hFind, &findData));  // 使用ANSI版本
        FindClose(hFind);
    }
    
    // 扫描子目录
    searchPath = dir + "\\*";
    hFind = FindFirstFileA(searchPath.c_str(), &findData);
    
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
               strcmp(findData.cFileName, ".") != 0 &&
               strcmp(findData.cFileName, "..") != 0) {
                std::string subdir = dir + "\\" + findData.cFileName;
                ScanDirectory(subdir, files);  // 递归扫描子目录
            }
        } while(FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
}

bool LuaPackager::CompileToByteCode(const std::string& file) {
    lua_State* L = luaL_newstate();
    if(!L) return false;
    
    luaL_openlibs(L);
    bool success = false;
    
    if(luaL_loadfile(L, file.c_str()) == 0) {
        std::string outFile = file + "c";
        FILE* f = fopen(outFile.c_str(), "wb");
        if(f) {
            lua_dump(L, WriteBytecode, f);
            fclose(f);
            success = true;
        }
    }
    
    lua_close(L);
    return success;
}

bool LuaPackager::MergeByteCode(const std::vector<std::string>& files, const std::string& outFile) {
    FILE* out = fopen(outFile.c_str(), "wb");
    if(!out) return false;
    
    // 写入文件数量
    uint32_t fileCount = files.size();
    fwrite(&fileCount, sizeof(fileCount), 1, out);
    
    for(const auto& file : files) {
        std::string luacFile = file + "c";
        FILE* f = fopen(luacFile.c_str(), "rb");
        if(f) {
            // 写入文件名
            uint32_t nameLen = file.length();
            fwrite(&nameLen, sizeof(nameLen), 1, out);
            fwrite(file.c_str(), 1, nameLen, out);
            
            // 获取文件大小
            fseek(f, 0, SEEK_END);
            uint32_t size = ftell(f);
            fseek(f, 0, SEEK_SET);
            
            // 写入bytecode大小和内容
            fwrite(&size, sizeof(size), 1, out);
            char* buffer = new char[size];
            fread(buffer, 1, size, f);
            fwrite(buffer, 1, size, out);
            
            delete[] buffer;
            fclose(f);
        }
    }
    
    fclose(out);
    return true;
}

int LuaPackager::WriteBytecode(lua_State* L, const void* p, size_t size, void* u) {
    return (fwrite(p, size, 1, (FILE*)u) != 1) && (size != 0);
}