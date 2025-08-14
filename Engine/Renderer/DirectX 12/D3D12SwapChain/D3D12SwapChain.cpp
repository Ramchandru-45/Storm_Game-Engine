#include "pch.h"
#include "Renderer/DirectX 12/Directx12.h"

bool Storm::Directx12::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = Renderer::getWidth();
    swapChainDesc.Height = Renderer::getHeight();
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

    HR_EVAL(d3d12.pFactory->CreateSwapChainForHwnd(d3d12.pQueue.Get(), Renderer::GetHwnd(), &swapChainDesc, NULL, NULL, reinterpret_cast<IDXGISwapChain1**>(d3d12.pSwapChain.GetAddressOf())),"Failed to Create SwapChain");
    frameIndex = d3d12.pSwapChain->GetCurrentBackBufferIndex();
    LOG_INFO("frame Index "<< frameIndex)
	return true;
}

bool Storm::Directx12::CreateDescriptorHeap()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    HR_EVAL(d3d12.pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(d3d12.pDescHeap.GetAddressOf())), "Failed to Create Descriptor Heap");
    m_rtvDescriptorSize = d3d12.pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = d3d12.pDescHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < FrameCount; i++)
    {
        HR_EVAL(d3d12.pSwapChain->GetBuffer(i, IID_PPV_ARGS(d3d12.pResource[i].GetAddressOf())), "Failed to get buffer");
        d3d12.pDevice->CreateRenderTargetView(d3d12.pResource[i].Get(), nullptr, rtvHandle);
        rtvHandle.ptr += m_rtvDescriptorSize;
    }

	return true;
}

