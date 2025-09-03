#include "se_pch.h"
#include "EditorLayer.h"

namespace Storm
{
	EditorLayer::EditorLayer() : Layer("EditorLayer")
	{
	}
	void EditorLayer::OnAttach()
	{
	}
	void EditorLayer::OnDetach()
	{
	}
	void EditorLayer::OnUpdate(Timestep ts)
	{
	}
	void EditorLayer::OnImGuiRender()
	{
	}
	void EditorLayer::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(SE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(SE_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (!e.Handled)
			SE_TRACE("MousePressed Editor: {0}", e.GetMouseButton());
		return false;
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if(!e.Handled)
			SE_TRACE("KeyPressed Editor: {0}", e.GetKeyCode());
		return false;
	}
}