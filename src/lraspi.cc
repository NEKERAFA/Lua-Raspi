/**
 * lua-raspi.cc - NEKERAFA - 25th july 2019
 * 
 * See Copyright Notice in lraspi.h 
 */

#include <cstdlib>
#include <cstring>
#include <iostream>

#include <lua.hpp>

#include "lraspi.h"
#include "lraspi/lauxlib.h"

namespace lraspi
{

/**
 * @brief Loads the file and runs
 * 
 * @param L A lua_State object
 * @param path The path of the file
 */
void dofile(lua_State* L, const char* path)
{
    if (luaL_loadfile(L, path) == LUA_OK)
        lua::call(L, 0, LUA_MULTRET);
    else
        std::cerr << "error: " << lua_tostring(L, -1) << std::endl; 
}

const char* checkargs(int argc, const char* argv[])
{
    const char* path = nullptr;

    // Read all the arguments
    for (int arg = 1; arg < argc; arg++)
    {
        // Check arguments
        if ((std::strncmp(argv[arg], "--", 2) == 0))
        {
            // Print Lua Raspi version
            if ((std::strcmp(argv[arg]+2, "version") == 0))
            {
                std::cout << "Lua Raspi " << LRASPI_VERSION << std::endl;
                std::exit(EXIT_SUCCESS);
            }
            else
            {
                std::cerr << "option " << argv[arg] << " not recognize" << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
        else if (path == nullptr)
        {
            path = argv[arg];
        }
    }

    // If the user cannot enter a file path, loads "script.lua"
   return path != nullptr ? path : "script.lua";
}

} // namespace lraspi

int main(int argc, const char* argv[])
{
    // Check the arguments
    const char* path = lraspi::checkargs(argc, argv);

    // Create new virtual machine
    lua_State *L = luaL_newstate();

    // Load all libraries
    luaL_openlibs(L);
    lraspi::openlibs(L);

    // Open the file
    lraspi::dofile(L, path);

    // Close all libraries
    lraspi::closelibs(L);

    return EXIT_SUCCESS;
}