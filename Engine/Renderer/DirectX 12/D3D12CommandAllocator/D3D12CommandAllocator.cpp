#include "pch.h"
#include "Renderer/DirectX 12/Directx12.h"

bool Storm::Directx12::CreateCommandAllocator()
{
	
	HR_EVAL(d3d12.pDevice->CreateCommandAllocator(d3d12.pQueue->GetDesc().Type, IID_PPV_ARGS(d3d12.pAllocator.GetAddressOf())), "Failed to create Command Allocator");

	return true;
}