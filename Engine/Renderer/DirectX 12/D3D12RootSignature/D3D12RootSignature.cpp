#include "pch.h"
#include "Renderer/DirectX 12/Directx12.h"

bool Storm::Directx12::CreateRootSignature()
{
    D3D12_ROOT_SIGNATURE_DESC rootDesc = {};
    rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    Microsoft::WRL::ComPtr<ID3DBlob> signature;
    Microsoft::WRL::ComPtr<ID3DBlob> error;
    HR_EVAL(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, signature.GetAddressOf(), error.GetAddressOf()), "Failed to Serialize Root Signature");
    HR_EVAL(d3d12.pDevice->CreateRootSignature(0, signature.Get()->GetBufferPointer(), signature.Get()->GetBufferSize(), IID_PPV_ARGS(d3d12.pSignature.GetAddressOf())), "Failed to Create Root Signature");
    

    HR_EVAL(d3d12.pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(d3d12.pFence.GetAddressOf())), "Failed to Create Fence");
    m_fence = 1;

    return true;
}
