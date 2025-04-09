# lua-compat-5.3

为Lua 5.1和5.2提供Lua 5.3风格的API。

## 这是什么

这是一个小型模块，旨在让使用Lua 5.1和5.2编写的代码更容易兼容Lua 5.3的编程风格。这个模块**并不是**让Lua 5.1/5.2完全兼容Lua 5.3，而是让API更接近Lua 5.3的风格。

它包含：

* _Lua脚本部分_：提供`compat53` Lua模块，可以在Lua 5.1、5.2和5.3中使用。包括：
  - 从Lua 5.3源码移植的`utf8`模块
  - Lua 5.3风格的`table`模块
  - 直接从Lua 5.3源码移植的字符串打包函数

* _C语言部分_：提供C头文件和源文件，可以链接到你的C模块中，提供了Lua 5.3 C API中在Lua 5.1/5.2中不存在的函数，使得C代码更容易在三个版本间兼容。

## 如何使用

### Lua模块使用

```lua
require("compat53")
```

`compat53`会修改你的全局环境，不会返回有意义的返回值，所以通常将require的返回值存储在局部变量中是没有意义的。

在Lua 5.3+环境下，这个模块不做任何事情。

在Lua 5.2或5.1环境下，它会替换一些标准函数并添加新函数，使环境更接近Lua 5.3。它也会尝试自动加载移植的`utf8`、`table`和字符串打包模块。如果加载失败：
- 会使用纯Lua实现的新table函数作为后备
- 会尝试使用Roberto的struct库来处理字符串打包

#### Lua子模块

```lua
local _ENV = require("compat53.module")
if setfenv then setfenv(1, _ENV) end
```

`compat53.module`模块不修改全局环境，所以在模块中使用是安全的。它应该被设置为当前环境（如上所示），不支持从这个模块中单独选取个别函数！某些功能在使用这个模块时不可用（如可让出的pcall支持、字符串/文件方法等），所以建议尽可能使用普通的`require("compat53")`。

### C代码使用

有两种方式添加C API兼容性函数/宏：

1. 如果没有定义`COMPAT53_PREFIX`：
   - `compat-5.3.h`会包含`compat-5.3.c`
   - 所有函数都是static的
   - 不需要单独编译/链接`compat-5.3.c`
   - 适合单文件项目

2. 如果定义了`COMPAT53_PREFIX`：
   - 所有导出函数会使用这个前缀重命名以避免链接冲突
   - 调用兼容性函数的方式不变
   - 需要自行编译和链接`compat-5.3.c`
   - 可以通过`COMPAT53_API`宏修改函数导出方式
   - 虽然可以使用"lua"作为前缀，但不建议这样做，因为LuaJIT已开始实现自己的Lua 5.2+ C API函数

## 已实现的功能

### Lua部分
* 从Lua 5.3源码移植的`utf8`模块
* 字符串打包相关函数：`string.pack`、`string.packsize`和`string.unpack`
* 数学函数：`math.maxinteger`、`math.mininteger`、`math.tointeger`、`math.type`和`math.ult`
* `assert`支持非字符串错误消息
* `ipairs`尊重`__index`元方法
* `table.move`
* `table`库尊重元方法

对于Lua 5.1额外实现：
* `load`和`loadfile`支持`mode`和`env`参数
* `table.pack`和`table.unpack`
* 字符串模式可以包含嵌入的零
* `string.rep`支持`sep`参数
* `string.format`的`%s`会对参数调用`tostring`
* `math.log`支持base参数
* `xpcall`支持额外参数
* `pcall`和`xpcall`可以执行会让出的函数
* `pairs`尊重`__pairs`元方法
* `rawlen`
* `package.searchers`作为`package.loaders`的别名
* `package.searchpath`
* 处理主协程的`coroutine`函数
* `coroutine.create`接受C函数
* `os.execute`的返回码
* `io.write`和`file:write`返回文件句柄
* `io.lines`和`file:lines`接受格式参数
* 使用`io.popen`时`file:close`返回三个结果
* `debug.setmetatable`返回对象
* `debug.getuservalue`和`debug.setuservalue`

## 未实现的功能

* 位运算操作符
* 整除操作符
* utf8转义序列
* 64位整数
* `coroutine.isyieldable`
* Lua 5.1：`_ENV`、`goto`、标签、ephemeron表等
* 以下C API函数/宏：
  - `lua_isyieldable`
  - `lua_arith`（新操作符未实现）
  - `lua_push(v)fstring`（新格式未实现）
  - `lua_upvalueid`（5.1）
  - `lua_upvaluejoin`（5.1）
  - `lua_version`（5.1）
  - `lua_yieldk`（5.1）

## 相关项目

* Lua 5.1下的Lua-5.2风格API，参见[lua-compat-5.2](http://github.com/lunarmodules/lua-compat-5.2/)
* Lua 5.0下的Lua-5.1风格API，参见[Compat-5.1](http://lunarmodules.org/compat/)

## 贡献者

本项目包含以下开发者的代码：

* [Lua开发团队](http://www.lua.org)
* Philipp Janda ([@siffiejoe](http://github.com/siffiejoe))
* Tomás Guisasola Gorham ([@tomasguisasola](http://github.com/tomasguisasola))
* Hisham Muhammad ([@hishamhm](http://github.com/hishamhm))
* Renato Maia ([@renatomaia](http://github.com/renatomaia))
* [@ThePhD](http://github.com/ThePhD)
* [@Daurnimator](http://github.com/Daurnimator) 