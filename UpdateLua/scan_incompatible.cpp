#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <regex>

namespace fs = std::filesystem;

struct IncompatibleCode {
    std::string filename;
    int line_number;
    std::string code;
    std::string reason;
};

// 检查单行代码是否存在不兼容
bool checkLineIncompatible(const std::string& line, std::string& reason) {
    // 1. 检查table.getn的使用 (在5.3中被移除)
    if (line.find("table.getn") != std::string::npos) {
        reason = "table.getn在Lua 5.3中被移除，请使用#操作符";
        return true;
    }

    // 2. 检查math.mod的使用 (在5.3中被移除)
    if (line.find("math.mod") != std::string::npos) {
        reason = "math.mod在Lua 5.3中被移除，请使用%操作符";
        return true;
    }

    // 3. 检查string.gfind的使用 (在5.3中被移除)
    if (line.find("string.gfind") != std::string::npos) {
        reason = "string.gfind在Lua 5.3中被移除，请使用string.gmatch";
        return true;
    }

    // 4. 检查unpack的使用 (在5.3中移至table.unpack)
    if (std::regex_search(line, std::regex("\\bunpack\\s*\\("))) {
        reason = "unpack在Lua 5.3中被移至table.unpack";
        return true;
    }

    // 5. 检查module/setfenv/getfenv的使用 (在5.3中被移除)
    if (line.find("module") != std::string::npos ||
        line.find("setfenv") != std::string::npos ||
        line.find("getfenv") != std::string::npos) {
        reason = "module/setfenv/getfenv在Lua 5.3中被移除";
        return true;
    }

    // 6. 检查数值运算可能的不兼容
    if (line.find("/") != std::string::npos) {
        reason = "除法运算在Lua 5.3中可能有不同结果，请检查是否需要使用//";
        return true;
    }

    return false;
}

std::vector<IncompatibleCode> scanFile(const std::string& filepath) {
    std::vector<IncompatibleCode> results;
    std::ifstream file(filepath);
    std::string line;
    int line_number = 0;
    
    while (std::getline(file, line)) {
        line_number++;
        std::string reason;
        if (checkLineIncompatible(line, reason)) {
            results.push_back({
                filepath,
                line_number,
                line,
                reason
            });
        }
    }
    
    return results;
}

void scanDirectory(const std::string& dir_path) {
    for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
        if (entry.path().extension() == ".lua") {
            auto results = scanFile(entry.path().string());
            for (const auto& result : results) {
                std::cout << "文件: " << result.filename << "\n";
                std::cout << "行号: " << result.line_number << "\n";
                std::cout << "代码: " << result.code << "\n";
                std::cout << "原因: " << result.reason << "\n";
                std::cout << "-------------------\n";
            }
        }
    }
}

int main() {
    try {
        scanDirectory("UIScript");
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    return 0;
} 