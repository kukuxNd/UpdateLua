# 编译器设置
CC = gcc
CFLAGS = -Wall -O2 -DLUA_USE_LINUX
AR = ar rcu
RANLIB = ranlib
RM = rm -f

# 目标文件夹
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj

# Lua源文件目录
SRCDIR = lua-5.3.6/src

# 头文件目录
INCLUDES = -I$(SRCDIR)

# 源文件
CORE_SRCS = lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c \
	lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c \
	ltm.c lundump.c lvm.c lzio.c
LIB_SRCS = lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c \
	lmathlib.c loslib.c lstrlib.c ltablib.c lutf8lib.c loadlib.c linit.c
LUA_SRCS = lua.c
LUAC_SRCS = luac.c

# 目标文件
CORE_OBJS = $(CORE_SRCS:%.c=$(OBJDIR)/%.o)
LIB_OBJS = $(LIB_SRCS:%.c=$(OBJDIR)/%.o)
LUA_OBJS = $(LUA_SRCS:%.c=$(OBJDIR)/%.o)
LUAC_OBJS = $(LUAC_SRCS:%.c=$(OBJDIR)/%.o)

# 最终目标
LIBLUA = $(BUILDDIR)/liblua.a
LUA = $(BUILDDIR)/lua
LUAC = $(BUILDDIR)/luac

# 添加新的目标
SCAN = $(BUILDDIR)/scan_incompatible

# 添加源文件
SCAN_SRCS = scan_incompatible.cpp

# 添加编译规则
$(SCAN): $(SCAN_SRCS)
	$(CXX) $(CXXFLAGS) -std=c++17 $^ -o $@

# 默认目标
all: prepare $(LIBLUA) $(LUA) $(LUAC) $(SCAN)

# 创建必要的目录
prepare:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(OBJDIR)

# 编译规则
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 生成静态库
$(LIBLUA): $(CORE_OBJS) $(LIB_OBJS)
	$(AR) $@ $^
	$(RANLIB) $@

# 生成lua解释器
$(LUA): $(OBJDIR)/lua.o $(LIBLUA)
	$(CC) -o $@ $^ -lm -ldl

# 生成lua编译器
$(LUAC): $(OBJDIR)/luac.o $(LIBLUA)
	$(CC) -o $@ $^ -lm

# 清理目标
clean:
	$(RM) -r $(BUILDDIR)

# 安装目标
install: all
	install -d /usr/local/bin
	install -d /usr/local/lib
	install -d /usr/local/include
	install $(LUA) /usr/local/bin
	install $(LUAC) /usr/local/bin
	install $(LIBLUA) /usr/local/lib
	install $(SRCDIR)/lua.h /usr/local/include
	install $(SRCDIR)/luaconf.h /usr/local/include
	install $(SRCDIR)/lualib.h /usr/local/include
	install $(SRCDIR)/lauxlib.h /usr/local/include

.PHONY: all prepare clean install 