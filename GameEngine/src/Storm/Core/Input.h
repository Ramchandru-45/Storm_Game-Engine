#pragma once

#include "Storm/Core/KeyCodes.h"
#include "Storm/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Storm {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
