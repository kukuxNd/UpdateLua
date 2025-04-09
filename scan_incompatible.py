#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import re
from typing import List, NamedTuple
from collections import defaultdict

class IncompatibleCode(NamedTuple):
    filename: str
    line_number: int
    code: str
    reason: str

def check_line_incompatible(line: str) -> tuple[bool, str]:
    """检查单行代码是否存在不兼容"""
    
    # 移除注释
    line = re.sub(r'--.*$', '', line)
    if not line.strip():
        return False, ""

    # 1. 检查table.getn的使用 (在5.3中被移除)
    if 'table.getn' in line:
        return True, "table.getn在Lua 5.3中被移除，请使用#操作符"

    # 2. 检查math.mod的使用 (在5.3中被移除)
    if 'math.mod' in line:
        return True, "math.mod在Lua 5.3中被移除，请使用%操作符"

    # 3. 检查string.gfind的使用 (在5.3中被移除)
    if 'string.gfind' in line:
        return True, "string.gfind在Lua 5.3中被移除，请使用string.gmatch"

    # 4. 检查unpack的使用 (在5.3中移至table.unpack)
    if re.search(r'\bunpack\s*\(', line):
        return True, "unpack在Lua 5.3中被移至table.unpack"

    # 5. 检查module/setfenv/getfenv的使用 (在5.3中被移除)
    deprecated_funcs = ['module', 'setfenv', 'getfenv']
    for func in deprecated_funcs:
        if re.search(rf'\b{func}\s*\(', line):
            return True, f"{func}在Lua 5.3中被移除"

    # 6. 检查数值运算可能的不兼容
    if '/' in line and not '//' in line:
        # 排除字符串中的斜杠
        if not ('"' in line or "'" in line):
            return True, "除法运算在Lua 5.3中可能有不同结果，请检查是否需要使用//"

    # 7. 检查位运算操作 (Lua 5.3新增)
    bit_ops = ['bit32.', '&', '|', '~', '<<', '>>']
    for op in bit_ops:
        if op in line:
            return True, f"位运算在Lua 5.3中有变化，请检查{op}的使用"

    return False, ""

def scan_file(filepath: str) -> List[IncompatibleCode]:
    """扫描单个文件"""
    results = []
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            for line_number, line in enumerate(f, 1):
                is_incompatible, reason = check_line_incompatible(line)
                if is_incompatible:
                    results.append(IncompatibleCode(
                        filename=filepath,
                        line_number=line_number,
                        code=line.strip(),
                        reason=reason
                    ))
    except UnicodeDecodeError:
        try:
            # 尝试使用GBK编码
            with open(filepath, 'r', encoding='gbk') as f:
                for line_number, line in enumerate(f, 1):
                    is_incompatible, reason = check_line_incompatible(line)
                    if is_incompatible:
                        results.append(IncompatibleCode(
                            filename=filepath,
                            line_number=line_number,
                            code=line.strip(),
                            reason=reason
                        ))
        except UnicodeDecodeError:
            print(f"警告: 无法读取文件 {filepath} (编码问题)")
    
    return results

def scan_directory(dir_path: str, log_file: str) -> None:
    """递归扫描目录"""
    all_results = []
    stats = defaultdict(int)
    file_stats = defaultdict(int)
    
    for root, _, files in os.walk(dir_path):
        for file in files:
            if file.endswith('.lua'):
                filepath = os.path.join(root, file)
                results = scan_file(filepath)
                if results:
                    all_results.extend(results)
                    file_stats[filepath] = len(results)
                    for result in results:
                        stats[result.reason] += 1

    with open(log_file, 'w', encoding='utf-8') as f:
        # 写入统计摘要
        f.write("扫描结果统计\n")
        f.write("=" * 60 + "\n\n")
        
        f.write("问题类型统计:\n")
        f.write("-" * 60 + "\n")
        for reason, count in sorted(stats.items(), key=lambda x: x[1], reverse=True):
            f.write(f"{reason}: {count}处\n")
        
        f.write("\n文件问题统计:\n")
        f.write("-" * 60 + "\n")
        for filepath, count in sorted(file_stats.items(), key=lambda x: x[1], reverse=True):
            f.write(f"{filepath}: {count}处\n")
            
        f.write("\n\n详细问题列表\n")
        f.write("=" * 60 + "\n")
        
        # 写入详细扫描结果
        current_file = None
        for result in all_results:
            if current_file != result.filename:
                current_file = result.filename
                f.write(f"\n检查文件: {result.filename}\n")
                f.write("-" * 60 + "\n")
            f.write(f"行号: {result.line_number}\n")
            f.write(f"代码: {result.code}\n")
            f.write(f"原因: {result.reason}\n")
            f.write("-" * 60 + "\n")

def main():
    """主函数"""
    try:
        script_dir = os.path.dirname(os.path.abspath(__file__))
        ui_script_dir = os.path.join(script_dir, "UIScript")
        log_file = os.path.join(script_dir, "scan_lua51_2_53.log")
        
        if not os.path.exists(ui_script_dir):
            print(f"错误: 目录不存在: {ui_script_dir}")
            return

        print(f"开始扫描目录: {ui_script_dir}")
        print("=" * 60)
        
        scan_directory(ui_script_dir, log_file)
        
        print(f"\n扫描完成! 结果已保存到 {log_file}")
        
    except Exception as e:
        print(f"错误: {str(e)}")

if __name__ == '__main__':
    main() 