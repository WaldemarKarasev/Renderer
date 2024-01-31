#include "nkpch.h"
#include "RendererAPI.hpp"

namespace NK {

	// TODO: Implementa function Create fot choosing RendererAPI at runtime.
	// Right now it is implemented as fixed API
	RendererAPI::API RendererAPI::s_API_ = RendererAPI::API::Vulkan;

}