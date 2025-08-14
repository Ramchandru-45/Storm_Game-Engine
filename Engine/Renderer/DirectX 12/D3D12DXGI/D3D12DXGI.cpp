#include "pch.h"
#include "Renderer/DirectX 12/Directx12.h"

bool Storm::Directx12::CreateFactory()
{

	HR_EVAL(CreateDXGIFactory1(IID_PPV_ARGS(d3d12.pFactory.GetAddressOf())), "Failed to Create DXGI Factory");
	
	//ListAdapters();
	

	return true;
}