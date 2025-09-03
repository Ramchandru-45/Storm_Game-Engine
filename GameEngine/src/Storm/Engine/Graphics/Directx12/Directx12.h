#pragma once

#include "Storm/Core/core.h"
#include "Storm/Engine/Graphics/GraphicsContext.h"

#include "d3dx12.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef SE_DEBUG
#define ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

namespace Storm
{
	static const int APP_NUM_FRAMES_IN_FLIGHT = 2;
	static const int APP_NUM_BACK_BUFFERS = 2;
	static const int APP_SRV_HEAP_SIZE = 64;
	static float clearColor[4];

	struct Vertex
	{
		float position[3];
		float color[4];
	};

	typedef struct directx12
	{
		IDXGIFactory6*				pFactory = nullptr;
		ID3D12Device*				pDevice = nullptr;
		IDXGIAdapter1*				pAdapter = nullptr;
		ID3D12Debug*				pDebug = nullptr;
		ID3D12CommandQueue*			pQueue = nullptr;
		ID3D12CommandAllocator*		pAllocator = nullptr;
		ID3D12GraphicsCommandList*	pCmdList = nullptr;
		IDXGISwapChain3*			pSwapChain = nullptr;
		HANDLE                      p_hSwapChainWaitableObject = NULL;
		ID3D12RootSignature*		pSignature = nullptr;
		ID3D12PipelineState*		pGPSO = nullptr;
		ID3D12DescriptorHeap*		pDescHeap = nullptr;
		ID3D12Fence1*				pFence = nullptr;
		ID3D12Resource*				pSurface[APP_NUM_BACK_BUFFERS] = {};
		D3D12_CPU_DESCRIPTOR_HANDLE p_cpuHandle[APP_NUM_BACK_BUFFERS] = {};
		HANDLE                      g_fenceEvent = NULL;
		ID3D12Resource*				p_vertexBuffer = nullptr;
		D3D12_VERTEX_BUFFER_VIEW	p_vertexBufferView;

	}directx12;

	class STORM_API Directx12 : public GraphicsContext
	{
	public:
		Directx12(const APIProp& props);
		~Directx12();

		inline virtual void SetWindowSize(int w, int h) override { prop.width = w, prop.height = h; }

		virtual void OnUpdate() override;

		virtual bool multiGPUEnabled() const override { return m_mGPU; }
		virtual void setmultiGPUSupport(bool mGPU) const override;

		virtual void ExecuteCommandList() override;

		inline HWND GetHWND() { return prop.hwnd; }
		inline int GetWidth() { return prop.width; }
		inline int GetHeight() { return prop.height; }

	private:
		void Init();
		void Destroy();

		//helper Methods for directx12
		bool CreateFactory();
		bool CreateDevice();
		bool CommandQueue();
		bool CommandAllocator();
		bool SwapChain();
		bool DescriptorHeap();
		bool RootSignature();
		bool GraphicsPipelineObject();
		bool CommandList();
		void WaitForPreviousFrame();
		void PopulateCommandList();
		bool cleanD3D();
		bool clearRTV();
		bool CreateRenderTarget();

	private:
		bool m_mGPU = false;

		struct APIProps
		{
			uint32_t width = 0, height = 0;
			HWND hwnd = nullptr;
			GLFWwindow* gwnd = nullptr;

			APIProps() {}
			APIProps(int w, int h, HWND wnd, GLFWwindow* gwnd) : width(w), height(h), hwnd(wnd), gwnd(gwnd) {}
		};
		APIProps prop;

		directx12 d3d12;
		D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
		DXGI_FORMAT rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM;
		UINT m_rtvDescriptorSize = 0;
		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_rect;
		UINT64 m_fenceValue = 0;
		UINT m_frameIndex;

		float m_aspectRatio = 1.5f;
		friend class ImGuiLayer;
	};
}

