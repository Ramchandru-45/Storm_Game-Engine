#include "pch.h"
#include "Renderer/DirectX 12/Directx12.h"

bool Storm::Directx12::CreateCommandList()
{
	HR_EVAL(d3d12.pDevice->CreateCommandList(0, d3d12.pQueue->GetDesc().Type, d3d12.pAllocator.Get(), 0, IID_PPV_ARGS(d3d12.pCmdList.GetAddressOf())), "Failed to Create Command list");
	HR_EVAL(d3d12.pCmdList->Close(),"Unable to Close command list")
	return true;
}

void Storm::Directx12::PopulateCommandList()
{
	HR_EVAL(d3d12.pAllocator.Get()->Reset(), "Failed to Reset Allocator");

	HR_EVAL(d3d12.pCmdList.Get()->Reset(d3d12.pAllocator.Get(), 0), "Failed to reset Command List");

	d3d12.pCmdList->SetGraphicsRootSignature(d3d12.pSignature.Get());

	D3D12_RESOURCE_BARRIER barrier {}; 
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = d3d12.pResource[frameIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	d3d12.pCmdList->ResourceBarrier(1, &barrier);
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = d3d12.pDescHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += (m_rtvDescriptorSize * frameIndex);
	const float red = 1.0f;
	const float green = 0.0f;
	const float blue = 1.0f;
	const float clearColor[] = { red, green, blue, 1.0f };
	d3d12.pCmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	d3d12.pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = d3d12.pResource[frameIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	d3d12.pCmdList->ResourceBarrier(1, &barrier);

	HR_EVAL(d3d12.pCmdList->Close(),"Unable to Close Command List");
}
void Storm::Directx12::WaitForPreviousFrame()
{
	const UINT64 fence = m_fence;
	HR_EVAL(d3d12.pQueue->Signal(d3d12.pFence.Get(), m_fence), "Failed to Signal");
	m_fence++;

	//Wait until the previous frame is finished
	if (d3d12.pFence->GetCompletedValue() < fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
		if (!eventHandle)
			throw std::runtime_error("shit");
		auto hr = d3d12.pFence->SetEventOnCompletion(fence, eventHandle);
		// Error handling for hr
		HR_EVAL(hr, "Failed to set Event");

		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	frameIndex = d3d12.pSwapChain->GetCurrentBackBufferIndex();
	//LOG_INFO("Current Back buffer " << frameIndex)
}