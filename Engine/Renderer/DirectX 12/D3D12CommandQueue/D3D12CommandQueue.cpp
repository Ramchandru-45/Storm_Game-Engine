#include "pch.h"
#include "Renderer/DirectX 12/Directx12.h"

bool Storm::Directx12::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = NULL;

	HR_EVAL(d3d12.pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(d3d12.pQueue.GetAddressOf())), "Failed to Create CommandQueue");

	return true;
}