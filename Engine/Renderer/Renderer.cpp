#include "pch.h"
#include "Renderer.h"

void Storm::Renderer::ListAdapters()
{
	UINT adapter_count = 0;
#if DIRECTX12 1

	IDXGIFactory6* factory;
	CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	IDXGIAdapter1* adapter;
	DXGI_ADAPTER_DESC1 desc = {};
	while (factory->EnumAdapters1(adapter_count, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapter->GetDesc1(&desc);

		std::cout << std::endl;
		std::cout << "Adapter " << (adapter_count + 1) << std::endl;
		std::wcout << "Adapter Name: " << desc.Description << std::endl;
		std::cout << "VRAM: " << desc.DedicatedVideoMemory << std::endl;
		std::cout << "SysMemory: " << desc.DedicatedSystemMemory << std::endl;
		std::cout << "Shared Memory: " << desc.SharedSystemMemory << std::endl;

		adapter_count++;
	}

	LOG_INFO("number of adapter found: " << adapter_count);

#endif
}