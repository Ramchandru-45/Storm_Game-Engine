#pragma once
#include "Renderer/Renderer.h"

#ifdef _DEBUG
#define ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#endif

namespace Storm
{
	class SE_API Directx12 : public Storm::Renderer
	{
	public:
		Directx12() = default;
		~Directx12() = default;

		void Create(HWND hwnd) override;
		void Update() override;
		void Render() override;
		void Destroy() override;

		bool CreateFactory();
		bool CreateDevice();
		bool CreateSwapChain();
		bool CreateCommandQueue();
		bool CreateCommandAllocator();
		bool CreateCommandList();
		bool CreateDescriptorHeap();
		bool CreateRootSignature();

		void PopulateCommandList();
		void WaitForPreviousFrame();


	private:
		directx12 d3d12;
		D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_1_0_CORE;
		UINT m_rtvDescriptorSize = 0;
		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_rect;
		UINT64 m_fence;
		UINT frameIndex;
	};
}

