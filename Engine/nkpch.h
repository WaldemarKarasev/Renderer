#pragma once 

#include <chrono>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include<experimental/filesystem>
//#include <filesystem>
#include <functional>


#include <array>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Nutckracker/Log.h"
#include "Nutckracker/Core.h"

#ifdef NK_PLATFORM_LINUX

#elif NK_PLATFORM_WINDOW
    #include <Windows.h>
#endif
