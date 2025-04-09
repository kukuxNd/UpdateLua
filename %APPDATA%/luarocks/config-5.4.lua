rocks_trees = {
    { name = "user", root = home .. [[/luarocks]] }
}

variables = {
    LUA_INCDIR = [[F:\job\UpdateLua\lua-5.4.7\lua-5.4.7\src]],
    LUA_LIBDIR = [[F:\job\UpdateLua\lua-5.4.7\lua-5.4.7\src]],
    LUALIB = [[lua54.dll]],
    CC = [[gcc]],
    LD = [[gcc]],
    LIBFLAG = [[-shared -o]],
    CFLAGS = [[
        -O2 -Wall -DLUA_BUILD_AS_DLL -DWIN32 
        -IF:\job\UpdateLua\lua-5.4.7\lua-5.4.7\src
    ]],
    LIBFLAG = [[-L/F:\job\UpdateLua\lua-5.4.7\lua-5.4.7\src -llua54 -shared -o]]
}

external_deps_dirs = {
    [[F:\job\UpdateLua\lua-5.4.7\lua-5.4.7\src]]
}

verbose = true 