cd buildAndMergeLuac
g++ -g -o  bluac.exe    main.cpp    LuaPackager.cpp LuaLoader.cpp     -I"dll/inc"     -L"dll/lib"     -llua51   -static-libgcc  -static-libstdc++ 
