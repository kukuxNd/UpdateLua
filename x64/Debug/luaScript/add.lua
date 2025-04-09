-- 加法函数
function add(a, b)
    return a + b
end
-- 测试加法函数(每个测试运行1万次)
for i = 1, 10000 do
    add(1, 2)
end
print("测试1: 1 + 2 =", add(1, 2))

for i = 1, 10000 do
    add(5.5, 4.5)
end
print("测试2: 5.5 + 4.5 =", add(5.5, 4.5))

for i = 1, 10000 do
    add(-3, 7)
end
print("测试3: -3 + 7 =", add(-3, 7))

for i = 1, 10000 do
    add(100, 200)
end
print("测试4: 100 + 200 =", add(100, 200))

for i = 1, 10000 do
    add(0, 0)
end
print("测试5: 0 + 0 =", add(0, 0))
