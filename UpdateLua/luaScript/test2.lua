
-- 加载 xndLua 库
local xndLua = require("xndLua")

-- 测试加法函数
local result_add = xndLua.add(5, 3)
print("5 + 3 =", result_add)

-- 测试减法函数
local result_sub = xndLua.sub(5, 3)
print("5 - 3 =", result_sub)

-- 测试版本函数
local version = xndLua.version()
print("版本号:", version)
