#pragma once

namespace NK {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1, Vulkan = 2
		};

	public:

		inline static API GetAPI() { return s_API_; }

	private:
		static API s_API_;
	};

}