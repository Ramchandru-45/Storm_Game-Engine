#pragma once
#include <wrl.h>

#ifdef WIN_PLATFORM

#include <d3d12.h>
#include <dxgi1_6.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#endif


namespace Storm
{
	constexpr UINT FrameCount = 2;
	typedef struct directx12
	{
		Microsoft::WRL::ComPtr<IDXGIFactory6> pFactory;
		Microsoft::WRL::ComPtr<ID3D12Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter;
		Microsoft::WRL::ComPtr<ID3D12Debug> pDebug;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> pQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> pAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> pSwapChain;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> pSignature;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pDescHeap;
		Microsoft::WRL::ComPtr<ID3D12Fence1> pFence;
		Microsoft::WRL::ComPtr<ID3D12Resource> pResource[FrameCount];
	}directx12;

	class SE_API Renderer
	{
	public:
		Renderer() = default;
		~Renderer() = default;

		virtual void Create(HWND hwnd) {};
		virtual void Update() {};
		virtual void Render() {};
		virtual void Destroy() {};
		
		void ListAdapters();

		inline int getWidth() { return m_width; }
		inline int getHeight() { return m_height; }
		inline HWND GetHwnd() { return m_hwnd; }

		inline void setWidth(int width) { m_width = width; }
		inline void setHeight(int height) { m_height = height; }
		inline void SetHwnd(HWND hwnd) { m_hwnd = hwnd; }
		
	private:
		int m_width = 0;
		int m_height = 0;
		HWND m_hwnd = nullptr;
	};
}
