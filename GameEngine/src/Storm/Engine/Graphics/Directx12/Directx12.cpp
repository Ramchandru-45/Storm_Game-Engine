#include "se_pch.h"
#include "Directx12.h"


Storm::Directx12* context = nullptr;
static bool g_SwapChainOccluded;


Storm::GraphicsContext* Storm::GraphicsContext::CreateGraphicsContext(const APIProp& props)
{
	if(context == nullptr)
		context = new Directx12(props);
	return context;
}

Storm::Directx12::Directx12(const APIProp& props)
{

	{
		prop.width = props.width;
		prop.height = props.height;
		prop.hwnd = props.hwnd;
		prop.gwnd = props.gwnd;
	}
	Init();
}

Storm::Directx12::~Directx12()
{
	Destroy();
}

void Storm::Directx12::Init()
{
	if (prop.hwnd)
	{
	#ifdef ENABLE_DEBUG_LAYER

		HR_EVAL(D3D12GetDebugInterface(IID_PPV_ARGS(&d3d12.pDebug)), "Failed to Get Debug Interface");

		if(d3d12.pDebug) d3d12.pDebug->EnableDebugLayer();

	#endif
		{
			m_viewport.TopLeftX = 0;
			m_viewport.TopLeftY = 0;
			m_viewport.Width = (float)prop.width;
			m_viewport.Height = (float)prop.height;
		}
		{
			m_rect.left = 0;
			m_rect.top = 0;
			m_rect.right = prop.width;
			m_rect.bottom = prop.height;

		}
		SE_ASSERT(CreateFactory(), "Factory Done");
		SE_ASSERT(CreateDevice(), "Device Done");
		SE_ASSERT(CommandQueue(), "Command Queue Done");
		SE_ASSERT(CommandAllocator(), "Allocator Done");
		SE_ASSERT(SwapChain(), "SwapChain Done");
		SE_ASSERT(DescriptorHeap(), "Descriptor heap done");
		SE_ASSERT(RootSignature(), "RootSignature done");
		SE_ASSERT(GraphicsPipelineObject(), "GPO done");
		SE_ASSERT(CommandList(), "CmdList Done");

		//WaitForPreviousFrame();
	}
	else
		Destroy();
}

void Storm::Directx12::Destroy()
{
	cleanD3D();
#ifdef ENABLE_DEBUG_LAYER
	if(d3d12.pDebug) d3d12.pDebug->Release();
	IDXGIDebug1* pDebug = nullptr;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
	{
		pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
		pDebug->Release();
		pDebug = nullptr;
	}
#endif
}

void Storm::Directx12::OnUpdate()
{
	// Handle window screen locked
	if ((g_SwapChainOccluded && d3d12.pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) || ::IsIconic(prop.hwnd))
	{
		::Sleep(10);
		return;
	}
	g_SwapChainOccluded = false;

	ExecuteCommandList();
}

void Storm::Directx12::setmultiGPUSupport(bool mGPU) const
{
	
}

void Storm::Directx12::ExecuteCommandList()
{
	PopulateCommandList();

	ID3D12CommandList* ppCommandLists[] = { d3d12.pCmdList };
	SE_INFO("Commandlist size {0}", _countof(ppCommandLists));
	d3d12.pQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	
	// Present the frame.
	auto hr = d3d12.pSwapChain->Present(0, 0);
	g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

	WaitForPreviousFrame();
}


//helper functions for creating and managing directx12

bool Storm::Directx12::CreateFactory()
{
	HR_EVAL(CreateDXGIFactory(IID_PPV_ARGS(&d3d12.pFactory)), "Failed to Create DXGI Factory");

	return true;
}

bool Storm::Directx12::CreateDevice()
{
	HR_EVAL(D3D12CreateDevice(NULL, feature_level, IID_PPV_ARGS(&d3d12.pDevice)), "Failed to create Directx device");

	return true;
}

bool Storm::Directx12::CommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = NULL;

	HR_EVAL(d3d12.pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12.pQueue)), "Failed to Create CommandQueue");

	return true;
}

bool Storm::Directx12::CommandAllocator()
{
	HR_EVAL(d3d12.pDevice->CreateCommandAllocator(d3d12.pQueue->GetDesc().Type, IID_PPV_ARGS(&d3d12.pAllocator)), "Failed to create Command Allocator");

	return true;
}

bool Storm::Directx12::SwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = APP_NUM_BACK_BUFFERS;
	swapChainDesc.Width = prop.width;
	swapChainDesc.Height = prop.height;
	swapChainDesc.Format = rtv_format;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Stereo = 0;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;


	DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainDesc1;
	ZeroMemory(&swapChainDesc1, sizeof(swapChainDesc1));
	swapChainDesc1.Scaling = DXGI_MODE_SCALING_CENTERED;
	swapChainDesc1.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc1.Windowed = TRUE;

	HR_EVAL(d3d12.pFactory->CreateSwapChainForHwnd(d3d12.pQueue, prop.hwnd, &swapChainDesc, &swapChainDesc1, NULL, reinterpret_cast<IDXGISwapChain1**>(&d3d12.pSwapChain)), "Failed to Create SwapChain");
	d3d12.pSwapChain->SetMaximumFrameLatency(APP_NUM_BACK_BUFFERS);
	d3d12.p_hSwapChainWaitableObject = d3d12.pSwapChain->GetFrameLatencyWaitableObject();

	m_frameIndex =  d3d12.pSwapChain->GetCurrentBackBufferIndex();
	return true;
}

bool Storm::Directx12::DescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 64;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HR_EVAL(d3d12.pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&d3d12.pDescHeap)), "Failed to Create Descriptor Heap");
	m_rtvDescriptorSize = d3d12.pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(d3d12.pDescHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; i++)
	{
		d3d12.p_cpuHandle[i] = rtvHandle;
		rtvHandle.Offset(1, m_rtvDescriptorSize);
	}

	SE_ASSERT(CreateRenderTarget(), "Created RenderTarget View");

	return true;
}

bool Storm::Directx12::RootSignature()
{
	D3D12_ROOT_SIGNATURE_DESC rootDesc = {};
	rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signature = NULL;
	ID3DBlob* error = NULL;

	HR_EVAL(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error), "Failed to Serialize Root Signature");
	HR_EVAL(d3d12.pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&d3d12.pSignature)), "Failed to Create Root Signature");

	return true;
}

void LogShaderError(ID3DBlob* errorBlob) {
	if (errorBlob) {
		std::string errorMessage(
			static_cast<const char*>(errorBlob->GetBufferPointer()),
			errorBlob->GetBufferSize()
		);
		SE_CORE_INFO("Shader Compilation Failed: {0}" , errorMessage);
	}
}

bool Storm::Directx12::GraphicsPipelineObject()
{

	ID3DBlob* vertexShader;
	ID3DBlob* pixelShader;

	ID3DBlob* vertexerror;
	ID3DBlob* pixelerror;

#if defined(SE_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif

		HR_EVAL(D3DCompileFromFile(L"D:\\C++ projects\\StormEngine\\GameEngine\\Shaders\\VShader.hlsl", nullptr, nullptr, "VS_Main", "vs_5_0", compileFlags, 0, &vertexShader, &vertexerror), "");
		HR_EVAL(D3DCompileFromFile(L"D:\\C++ projects\\StormEngine\\GameEngine\\Shaders\\PShader.hlsl", nullptr, nullptr, "PS_Main", "ps_5_0", compileFlags, 0, &pixelShader, &pixelerror), "");

		//LogShaderError(vertexerror);
		//LogShaderError(pixelerror);

	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsDesc = {};
	ZeroMemory(&graphicsDesc, sizeof(graphicsDesc));
	graphicsDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	graphicsDesc.pRootSignature = d3d12.pSignature;
	graphicsDesc.VS = { reinterpret_cast<UINT8*> (vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
	graphicsDesc.PS = { reinterpret_cast<UINT8*> (pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
	graphicsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	graphicsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	graphicsDesc.DepthStencilState.DepthEnable = FALSE;
	graphicsDesc.DepthStencilState.StencilEnable = FALSE;
	graphicsDesc.SampleMask = UINT_MAX;
	graphicsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsDesc.NumRenderTargets = 1;
	graphicsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	graphicsDesc.SampleDesc.Count = 1;

	HR_EVAL(d3d12.pDevice->CreateGraphicsPipelineState(&graphicsDesc, IID_PPV_ARGS(&d3d12.pGPSO)), "Unable to create Graphics Pipeline");
	return true;
}

bool Storm::Directx12::CommandList()
{
	HR_EVAL(d3d12.pDevice->CreateCommandList(0, d3d12.pQueue->GetDesc().Type, d3d12.pAllocator, d3d12.pGPSO, IID_PPV_ARGS(&d3d12.pCmdList)), "Failed to Create Command list");
	HR_EVAL(d3d12.pCmdList->Close(), "Unable to Close command list");

	{
		 Vertex triangleVertices[] =
		{
			{ { 0.0f, 0.25f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
		auto desc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		HR_EVAL(d3d12.pDevice->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&d3d12.p_vertexBuffer)), "Unable to Create Commited Resource");

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		HR_EVAL(d3d12.p_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))," ");
		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		d3d12.p_vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		d3d12.p_vertexBufferView.BufferLocation = d3d12.p_vertexBuffer->GetGPUVirtualAddress();
		d3d12.p_vertexBufferView.StrideInBytes = sizeof(Vertex);
		d3d12.p_vertexBufferView.SizeInBytes = vertexBufferSize;
	}
	HR_EVAL(d3d12.pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&d3d12.pFence)), "Failed to Create Fence");
	
	d3d12.g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	SE_ASSERT(d3d12.g_fenceEvent != nullptr, "Created Fence Event");

	return true;
}

bool Storm::Directx12::CreateRenderTarget()
{
	for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; i++)
	{
		d3d12.pSwapChain->GetBuffer(i, IID_PPV_ARGS(&d3d12.pSurface[i]));
		d3d12.pDevice->CreateRenderTargetView(d3d12.pSurface[i], nullptr, d3d12.p_cpuHandle[i]);
	}

	return true;
}

void Storm::Directx12::WaitForPreviousFrame()
{
	// Signal and increment the fence value.
	const UINT64 fence = m_fenceValue;
	HR_EVAL(d3d12.pQueue->Signal(d3d12.pFence, fence), "Failed to Signal Fence");
	m_fenceValue++;

	// Wait until the previous frame is finished.
	if (d3d12.pFence->GetCompletedValue() < fence)
	{
		HR_EVAL(d3d12.pFence->SetEventOnCompletion(fence, d3d12.g_fenceEvent), "Failed to Set Event");
		WaitForSingleObject(d3d12.g_fenceEvent, INFINITE);
	}

	m_frameIndex = d3d12.pSwapChain->GetCurrentBackBufferIndex();
}

void Storm::Directx12::PopulateCommandList()
{

	
	HR_EVAL(d3d12.pAllocator->Reset(), "Failed to Reset Allocator");

	HR_EVAL(d3d12.pCmdList->Reset(d3d12.pAllocator, d3d12.pGPSO), "Failed to reset Command List");

	d3d12.pCmdList->SetGraphicsRootSignature(d3d12.pSignature);
	d3d12.pCmdList->RSSetViewports(1, &m_viewport);
	d3d12.pCmdList->RSSetScissorRects(1, &m_rect);

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier = CD3DX12_RESOURCE_BARRIER::Transition(d3d12.pSurface[m_frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	d3d12.pCmdList->ResourceBarrier(1, &barrier);
	//CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(d3d12.pDescHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
	
	d3d12.pCmdList->ClearRenderTargetView(d3d12.p_cpuHandle[m_frameIndex], clearColor, 0, nullptr);
	d3d12.pCmdList->OMSetRenderTargets(1, &d3d12.p_cpuHandle[m_frameIndex], FALSE, nullptr);
	d3d12.pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//d3d12.pCmdList->IASetVertexBuffers(0, 1, &d3d12.p_vertexBufferView);
	//d3d12.pCmdList->DrawInstanced(3, 1, 0, 0);
	
	//between barrier

	barrier = CD3DX12_RESOURCE_BARRIER::Transition(d3d12.pSurface[m_frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	d3d12.pCmdList->ResourceBarrier(1, &barrier);

	
	HR_EVAL(d3d12.pCmdList->Close(), "Unable to Close Command List");
}

bool Storm::Directx12::cleanD3D()
{
	clearRTV();
	if (d3d12.pSwapChain) { d3d12.pSwapChain->SetFullscreenState(false, nullptr); d3d12.pSwapChain->Release(); d3d12.pSwapChain = nullptr; }
	if (d3d12.p_hSwapChainWaitableObject != nullptr) { CloseHandle(d3d12.p_hSwapChainWaitableObject); d3d12.p_hSwapChainWaitableObject = nullptr; }
	if (d3d12.p_vertexBuffer) { d3d12.p_vertexBuffer->Release(); d3d12.p_vertexBuffer = nullptr; }
	if (d3d12.pAllocator) { d3d12.pAllocator->Release(); d3d12.pAllocator = nullptr; }
	if (d3d12.pQueue) { d3d12.pQueue->Release(); d3d12.pQueue = nullptr; }
	if (d3d12.pCmdList) { d3d12.pCmdList->Release(); d3d12.pCmdList = nullptr; }
	if (d3d12.pDescHeap) { d3d12.pDescHeap->Release(); d3d12.pDescHeap = nullptr; }
	if (d3d12.pSignature) { d3d12.pSignature->Release(); d3d12.pSignature = nullptr; }
	if (d3d12.pFence) { d3d12.pFence->Release(); d3d12.pFence = nullptr; }
	if (d3d12.g_fenceEvent != nullptr) { CloseHandle(d3d12.g_fenceEvent); d3d12.g_fenceEvent = nullptr; }
	if (d3d12.pGPSO) { d3d12.pGPSO->Release(); d3d12.pGPSO = nullptr; }
	if (d3d12.pDevice) { d3d12.pDevice->Release(); d3d12.pDevice = nullptr; }
	if (d3d12.pFactory) { d3d12.pFactory->Release(); d3d12.pFactory = nullptr; }
	return true;
}

bool Storm::Directx12::clearRTV()
{
	WaitForPreviousFrame();

	for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; i++)
	{
		if (d3d12.pSurface[i]) { d3d12.pSurface[i]->Release(); d3d12.pSurface[i] = nullptr; }
	}
	

	return true;
}
