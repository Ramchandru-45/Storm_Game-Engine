#pragma once

#include "Storm/Core/Layer.h"
#include "Storm/Engine/Events/Event.h"
#include "Storm/Engine/Events/KeyEvent.h"
#include "Storm/Engine/Events/MouseEvent.h"
#include "Storm/Core/Application.h"


namespace Storm
{
	class Directx12;
	class STORM_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		void Initialize();
		bool OnKeyEvent(KeyEvent& e);
		bool OnMouseEvent(MouseButtonEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
	private:
		bool onImgui = false;
		Directx12* obj;
	};
}

