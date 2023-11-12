#pragma once

#ifdef NK_PLATFORM_LINUX

extern NK::Application* NK::CreateApplication();


int main(int argc, char** argv)
{
	NK::Log::Init();
	NK_CORE_WARN("Initialized Log!");
	int a = 5;
	NK_CORE_INFO("Hello Nutckracker! Var{0}", a);

	auto app = NK::CreateApplication();
	app->Run();
	delete app;
}

#endif