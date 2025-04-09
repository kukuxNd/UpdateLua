-- 测试脚本
print("Hello kuku xndLua!")

-- 加载 xndLua 库
local xndLua = require("xndLua")

-- 测试加法函数
local result_add = xndLua.add(5, 3)
print("5 + 3 =", result_add)

-- 测试减法函数
local result_sub = xndLua.sub(5, 3)
print("5 - 3 =", result_sub)

-- 基本数据类型测试
local number = 42
local text = "测试字符串"
local bool = true
local t = {1, 2, 3, name = "table"}

print("数字:", number)
print("字符串:", text) 
print("布尔值:", bool)
print("表:", t.name)

-- 函数测试
local function add(a, b)
    return a + b
end

print("2 + 3 =", add(2, 3))

-- 循环测试
print("For循环:")
for i = 1, 3 do
    print(i)
end

-- 条件语句测试
local x = 10
if x > 5 then
    print("x大于5")
else
    print("x小于或等于5")
end

-- 错误处理测试
local status, err = pcall(function()
    error("这是一个测试错误")
end)
print("错误处理:", status, err)


local co = coroutine.create(function()
    for i = 1, 3 do
        print("协程", i)
        coroutine.yield()
    end
end)

coroutine.resume(co)  -- 协程 1
coroutine.resume(co)  -- 协程 2
coroutine.resume(co)  -- 协程 3

local mt = {
    __add = function(a, b)
        return a.value + b.value
    end
}

local a = {value = 10}
local b = {value = 20}
setmetatable(a, mt)
setmetatable(b, mt)

local c = a + b  -- 使用元表中的__add方法
print(c)  --