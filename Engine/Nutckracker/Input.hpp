#pragma once

#include "Nutckracker/Core.h"

#include "KeyCodes.hpp"

namespace NK {

	class NK_API Input
	{
	public:
		inline static bool IsKeyPressed(KeyCodes keycode)  { return s_Instance->IsKeyPressedImpl(static_cast<int>(keycode)); }
		inline static bool IsKeyReleased(KeyCodes keycode) {	return s_Instance->IsKeyReleasedImpl(static_cast<int>(keycode)); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsKeyReleasedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};

}