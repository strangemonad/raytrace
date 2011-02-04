
#include <cmath>

#include <iostream>
#include "scene_lua.hpp"
#include "stats.hpp"
#include "algebra.hpp"

int 
main(int argc, char** argv)
{
   std::string filename = "scene.lua";
   
   if (argc == 2) {
         filename = argv[1];
      std::cerr << "running " << filename << std::endl;
   } else if (argc != 1) {
      goto invalidUsage;
   }
   
   Stats::sharedStats().init();
   
   if (!runLua(filename)) {
      std::cerr << "Could not open " << filename << std::endl;
      return 1;
   }
   
   Stats::sharedStats().exit();
   
   return 0;
   
invalidUsage:
   std::cerr << "Usage:" << std::endl
             << "rt : \t\twill run non-interactively using scene.lua" << std::endl
             << "rt -i : \twill run interactively using scene.lua" << std::endl
             << "rt file.lua : \twill run non-interactively using file.lua" 
             << std::endl
             << "rt -i file.lua: will run interactively using file.lua" 
             << std::endl;
   return -1;
}

