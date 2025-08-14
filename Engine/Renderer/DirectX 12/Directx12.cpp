#include "pch.h"
#include "Directx12.h"


void Storm::Directx12::Create(HWND hwnd)
{
#ifdef ENABLE_DEBUG_LAYER

	HR_EVAL(D3D12GetDebugInterface(IID_PPV_ARGS(d3d12.pDebug.GetAddressOf())), "Failed to Get Debug Interface");

	d3d12.pDebug->EnableDebugLayer();

#endif

	Renderer::SetHwnd(hwnd);
	SE_ASSERT(CreateFactory(), "Factory Done");
	SE_ASSERT(CreateDevice(), "Device Done");
	SE_ASSERT(CreateCommandQueue(), "Command Queue Done");
	SE_ASSERT(CreateCommandAllocator(), "Allocator Done");
	SE_ASSERT(CreateSwapChain(), "SwapChain Done");
	SE_ASSERT(CreateDescriptorHeap(), "Descriptor heap done");
	SE_ASSERT(CreateCommandList(), "CmdList Done");
	SE_ASSERT(CreateRootSignature(), "RootSignature done");

	WaitForPreviousFrame();
}

void Storm::Directx12::Update()
{
}

void Storm::Directx12::Render()
{
	// Record all the commands we need to render the scene into the command list.
	PopulateCommandList();

	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { d3d12.pCmdList.Get() };
	d3d12.pQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame.
	(d3d12.pSwapChain->Present(1, 0));

	WaitForPreviousFrame();
}

void Storm::Directx12::Destroy()
{
	if(d3d12.pResource )d3d12.pResource->Reset();
	if(d3d12.pAllocator) d3d12.pAllocator->Reset();
	if(d3d12.pCmdList) d3d12.pCmdList->Reset(d3d12.pAllocator.Get(),0);

}
