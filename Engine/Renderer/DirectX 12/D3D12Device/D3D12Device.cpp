#include "pch.h"
#include "Renderer/DirectX 12/Directx12.h"

bool Storm::Directx12::CreateDevice()
{
	
	HR_EVAL(D3D12CreateDevice((IUnknown*)d3d12.pAdapter.Get(), feature_level, IID_PPV_ARGS(d3d12.pDevice.GetAddressOf())), "Failed to Create Device");
	
	return true;
}