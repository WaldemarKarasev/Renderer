#pragma once 

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream> //???
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Nutckracker/Log.h"

#ifdef NK_PLATFORM_LINUX

#elif NK_PLATFORM_WINDOW
    #include <Windows.h>
#endif
