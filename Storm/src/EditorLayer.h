#pragma once

#include "Storm/Core/Layer.h"
#include "Storm/Engine/Events/Event.h"
#include "Storm/Engine/Events/KeyEvent.h"
#include "Storm/Engine/Events/MouseEvent.h"

namespace Storm
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	};
}

