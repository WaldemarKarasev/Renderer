#pragma once

#define NK_PLATFORM_LINUX
#define NK_DEBUG

#ifdef NK_DEBUG
	#define NK_ENABLE_ASSERT
#endif 



#if defined(NK_PLATFORM_WINDOWS)
	#ifdef NK_BUILD_DLL
		#define NK_API __declspec(dllexport)
	#else
		#define NK_API __declspec(dllimport)
	#endif

#elif defined(NK_PLATFORM_LINUX)
    #ifdef NK_BUILD_DLL
		#define NK_API __attribute__((visibility("default")))
	#else
		#define NK_API //empty //__attribute__((visibility("default")))
	#endif
#else
    #ifdef NK_BUILD_DLL
		#define NK_API 
	#else
		#define NK_API 
	#endif

	//#error NK only supports Windows!

#endif  


#ifdef NK_ENABLE_ASSERT
	#define NK_ASSERT(x, ...)	   {if(!x) { NK_ERROR("Assertion Failed: {0}", __VA_ARGS__); /*debugbreak();???*/}}
	#define NK_CORE_ASSERT(x, ...) {if(!x) { NK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); /*debugbreak();???*/}}
#else
	#define NK_ASSERT(x, ...)	   
	#define NK_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)