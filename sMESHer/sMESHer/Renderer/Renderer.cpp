#include "Renderer.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler")



std::vector<Renderer::AvailableAdapterDesc> Renderer::m_availableDevices = std::vector<Renderer::AvailableAdapterDesc>();

ID3D12Device* Renderer::m_pDevice = nullptr;
ID3D12CommandQueue* Renderer::m_pCmdQueue = nullptr;
ID3D12CommandAllocator* Renderer::m_pCmdAllocator;
IDXGISwapChain4* Renderer::m_pSwapChain = nullptr;
D3D12_VIEWPORT Renderer::m_viewport{};
D3D12_RECT Renderer::m_surfaceSize{};

ID3D12Resource* Renderer::m_ppRenderTargets[NUM_OF_BACK_BUFFERS]{};
ID3D12DescriptorHeap* Renderer::m_pRtvDescriptorHeap = nullptr;
unsigned int Renderer::m_rtvDescriptorIncrementSize = 0;
D3D12_CPU_DESCRIPTOR_HANDLE Renderer::m_phRtvCpuDescriptorHandles[NUM_OF_BACK_BUFFERS]{};

ID3D12DescriptorHeap* Renderer::m_pCbvDescriptorHeap = nullptr;

ID3D12RootSignature* Renderer::m_pRootSignature = nullptr;
ID3D12PipelineState* Renderer::m_pPipelineState = nullptr;
ID3D12GraphicsCommandList* Renderer::m_pCmdList = nullptr;

ID3D12Resource* Renderer::m_pVertexBuffer = nullptr;
D3D12_VERTEX_BUFFER_VIEW Renderer::m_vertexBufferView{};
ID3D12Resource* Renderer::m_pConstantBuffer = nullptr;
Renderer::SceneConstantBuffer Renderer::m_constantBufferData{};
UINT8* Renderer::m_pCbvDataBegin = 0;


unsigned int Renderer::m_currentFrameIndex = 0;
HANDLE Renderer::m_hFenceEvent = nullptr;
ID3D12Fence* Renderer:: m_pFence = nullptr;
UINT64 Renderer::m_fenceValue = 0;

void Renderer::Initialize() noexcept{
	/*
	https://stackoverflow.com/questions/42354369/idxgifactory-versions
	1. The IDXGIFactory5 interface inherits from IDXGIFactory4
	2. The IDXGIFactory4 interface inherits from IDXGIFactory3
	3. The IDXGIFactory3 interface inherits from IDXGIFactory2
	4. The IDXGIFactory2 interface inherits from IDXGIFactory1

	You have to include the appropriate header for the interface type

	#include <DXGI1_5.h> -- also includes ..Factory4, ..Factory3, ..Factory2 ...

	And have declared an instance of the proper pointer type

	IDXGIFactory5* outPtr = nullptr; \\this really should be a CComPtr

	And call CreateDXGIFactory1(__uuidof(IDXGIFactory5),&outPtr);.

	If it succeeds you've got an IDXGIFactory5 that has all the methods
	that an IDXGIFactory4, IDXGIFactory3, IDXGIFactory2, and
	IDXGIFactory1 has, plus it's own method of CheckFeatureSupport.
	The CreateDXGIFactory1 and CreateDXGIFactory2 functions are
	just overloaded versions of CreateDXGIFactory for
	DXGI 1.1 - 1.5, however CreateDXGIFactory is for creating
	IDXGIFactory DXGI 1.0 objects. To create a DXGIFactory5
	object you have to be using DXGI 1.5 (Windows 10 Direct3D 12).
	To get a better understanding of the purpose of the
	CheckFeatureSupport function and why you'd want it see this
	explanation of Direct3D 12 levels.
*/
	Microsoft::WRL::ComPtr<IDXGIFactory4> cpFactory;
	CreateDXGIFactory2(0, __uuidof(IDXGIFactory4), &cpFactory);
	
	if (m_availableDevices.empty()) {
		IDXGIAdapter1* pAdapter = GetMostPowerfulGPUAdapter(cpFactory.Get());

		D3D12CreateDevice(m_pDevice, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)(&m_pDevice));
	}

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	{
		cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	}
	m_pDevice->CreateCommandQueue(&cmdQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&m_pCmdQueue);



	m_surfaceSize.left = 0;
	m_surfaceSize.top = 0;
	m_surfaceSize.right = APP_WINDOW_WIDTH;
	m_surfaceSize.bottom = APP_WINDOW_HEIGHT;

	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width = APP_WINDOW_WIDTH;
	m_viewport.Height = APP_WINDOW_HEIGHT;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;


	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	{
		swapChainDesc.Width = 0;
		swapChainDesc.Height = 0;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Stereo = 0;
		DXGI_SAMPLE_DESC sampleDesc = {};
		{
			sampleDesc.Count = 1u;
			sampleDesc.Quality = 0u;
		}
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = NUM_OF_BACK_BUFFERS;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	}
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDesc = {};
	{
		swapChainFullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainFullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainFullscreenDesc.Windowed = FALSE;
	}
	
	IDXGISwapChain1* tmpSwapChain;
	cpFactory->CreateSwapChainForHwnd(m_pCmdQueue, AppWindow::s_hWnd, &swapChainDesc, nullptr, nullptr, &tmpSwapChain);
	
	m_pSwapChain = (IDXGISwapChain4*)tmpSwapChain;

	m_currentFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	//create render target view
	///create descriptor heap
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
		{
			rtvDescriptorHeapDesc.NumDescriptors = NUM_OF_BACK_BUFFERS;
			rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		}
		m_pDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, _uuidof(ID3D12DescriptorHeap), (void**)&m_pRtvDescriptorHeap);
		
		m_rtvDescriptorIncrementSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	

		// Describe and create a constant buffer view (CBV) descriptor heap.
		// Flags indicate that this descriptor heap can be bound to the pipeline 
		// and that descriptors contained in it can be referenced by a root table.
		D3D12_DESCRIPTOR_HEAP_DESC cbvDescriptorHeapDesc = {};
		cbvDescriptorHeapDesc.NumDescriptors = 1;
		cbvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		m_pDevice->CreateDescriptorHeap(&cbvDescriptorHeapDesc, _uuidof(ID3D12DescriptorHeap), (void**)&m_pCbvDescriptorHeap);
	}


	

	
	
	
	// Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE hRtvCpuHandle(m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		//create rtv for each frame
		for (int i = 0; i < NUM_OF_BACK_BUFFERS; i++) {
			m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ppRenderTargets[i]));
			m_pDevice->CreateRenderTargetView(m_ppRenderTargets[i], nullptr, hRtvCpuHandle);
			hRtvCpuHandle.Offset(1, m_rtvDescriptorIncrementSize);
		}
	}
	



	m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, _uuidof(ID3D12CommandAllocator), (void**)&m_pCmdAllocator);

	LoadAssets();
}
void Renderer::Terminate() noexcept {	
	WaitForPreviousFrame();

	CloseHandle(m_hFenceEvent);


	if (m_pDevice != nullptr) {
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

void Renderer::Render() noexcept {
	// Record all the commands we need to render the scene into the command list.
	PopulateCommandList();

	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { m_pCmdList };
	m_pCmdQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame.
	m_pSwapChain->Present(1, 0);

	WaitForPreviousFrame();
}
void Renderer::Update() noexcept {
	const float translationSpeed = 0.005f;
   const float offsetBounds = 1.25f;

   m_constantBufferData.offset.x += translationSpeed;
   if (m_constantBufferData.offset.x > offsetBounds)
   {
	   m_constantBufferData.offset.x = -offsetBounds;
   }
   memcpy(m_pCbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
}

IDXGIAdapter1* Renderer::GetMostPowerfulGPUAdapter(IDXGIFactory4* pFactory) noexcept {
	int bestScore = 0;
	IDXGIAdapter1* retpAdapter = nullptr;
	IDXGIAdapter1* pAdapter = nullptr;
	for (int i = 0; pFactory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		int score = 0;
		AvailableAdapterDesc desc;
		DXGI_ADAPTER_DESC1 adapterDesc;

		pAdapter->GetDesc1(&adapterDesc);
		for (int j = 0; j < 16; j++) {
			desc.name[j] = adapterDesc.Description[j];
		}
		desc.adapter = pAdapter;

		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)) {
			score += 2;
		}

		std::wstring description(adapterDesc.Description);

		if (description.find(L"RTX") != std::wstring::npos || description.find(L"GTX") != std::wstring::npos || description.find(L"RX") != std::wstring::npos) {
			score += 5;
		}
		score += (int)(adapterDesc.DedicatedVideoMemory / 1000000000);

		if (score > bestScore) {
			retpAdapter = pAdapter;
			bestScore = score;
		}

		OutputDebugString(adapterDesc.Description);
		OutputDebugString(L" - ");
		OutputDebugString(std::to_wstring(score).c_str());
		OutputDebugString(L"\n");



		//Sadly there is no elegant way to test for this so we will 
		// have to temporarily create a device using D3D12CreateDevice. 
		// If this function fails we know the adapter doesn’t suite 
		// our needs.

		Microsoft::WRL::ComPtr<ID3D12Device> pTempDevice;
		if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), &pTempDevice))) {
			m_availableDevices.push_back(desc);
		}
	}

	return retpAdapter;
}
void Renderer::LoadAssets() noexcept {
	// Create a root signature consisting of a descriptor table with a single CBV.
	{
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

		// This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

		if (FAILED(m_pDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}

		CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
		CD3DX12_ROOT_PARAMETER1 rootParameters[1];

		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);

		// Allow input layout and deny uneccessary access to certain pipeline stages.
		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);

		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error);
		m_pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));
	}


	// Create the pipeline state, which includes compiling and loading shaders.
	{
		Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

		UINT compileFlags = 0;


		//"C:/Users/EidanowI/Documents/GitHub/sMESHer/sMESHer/sMESHer/shaders.hlsl"
		D3DCompileFromFile(L"C:/Users/EidanowI/Documents/GitHub/sMESHer/sMESHer/sMESHer/shaders.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
		D3DCompileFromFile(L"C:/Users/EidanowI/Documents/GitHub/sMESHer/sMESHer/sMESHer/shaders.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr);

		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_pRootSignature;
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;

		HRESULT hr = m_pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPipelineState));
	}

	// Create the command list.
	m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCmdAllocator, m_pPipelineState, IID_PPV_ARGS(&m_pCmdList));

	// Command lists are created in the recording state, but there is nothing
	// to record yet. The main loop expects it to be closed, so close it now.
	m_pCmdList->Close();



	// Create the vertex buffer.
	{
		// Define the geometry for a triangle.
		Vertex triangleVertices[] =
		{
			{ { 0.0f, 0.25f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		auto a1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto a2 = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		m_pDevice->CreateCommittedResource(
			&a1,
			D3D12_HEAP_FLAG_NONE,
			&a2,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_pVertexBuffer));

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		m_pVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		m_pVertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;
	}

	// Create the constant buffer.
	{
		const UINT constantBufferSize = sizeof(SceneConstantBuffer);    // CB size is required to be 256-byte aligned.

		auto a3 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto a4 = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);
		m_pDevice->CreateCommittedResource(
			&a3,
			D3D12_HEAP_FLAG_NONE,
			&a4,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_pConstantBuffer));

		// Describe and create a constant buffer view.
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_pConstantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = constantBufferSize;
		m_pDevice->CreateConstantBufferView(&cbvDesc, m_pCbvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		// Map and initialize the constant buffer. We don't unmap this until the
		// app closes. Keeping things mapped for the lifetime of the resource is okay.
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		m_pConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin));
		memcpy(m_pCbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
	}

	// Create synchronization objects and wait until assets have been uploaded to the GPU.
	{
		m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
		m_fenceValue = 1;

		// Create an event handle to use for frame synchronization.
		m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_hFenceEvent == nullptr)
		{
			HRESULT_FROM_WIN32(GetLastError());
		}

		// Wait for the command list to execute; we are reusing the same command 
		// list in our main loop but for now, we just want to wait for setup to 
		// complete before continuing.
		WaitForPreviousFrame();
	}
}

void Renderer::WaitForPreviousFrame() noexcept {
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
	// sample illustrates how to use fences for efficient resource usage and to
	// maximize GPU utilization.

	// Signal and increment the fence value.
	const UINT64 fence = m_fenceValue;
	m_pCmdQueue->Signal(m_pFence, fence);
	m_fenceValue++;

	// Wait until the previous frame is finished.
	if (m_pFence->GetCompletedValue() < fence)
	{
		m_pFence->SetEventOnCompletion(fence, m_hFenceEvent);
		WaitForSingleObject(m_hFenceEvent, INFINITE);
	}

	m_currentFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
}

void Renderer::PopulateCommandList() noexcept {
	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	m_pCmdAllocator->Reset();

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.
	m_pCmdList->Reset(m_pCmdAllocator, m_pPipelineState);

	// Set necessary state.
	m_pCmdList->SetGraphicsRootSignature(m_pRootSignature);

	ID3D12DescriptorHeap* ppHeaps[] = { m_pCbvDescriptorHeap };
	m_pCmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	m_pCmdList->SetGraphicsRootDescriptorTable(0, m_pCbvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	m_pCmdList->RSSetViewports(1, &m_viewport);
	m_pCmdList->RSSetScissorRects(1, &m_surfaceSize);

	// Indicate that the back buffer will be used as a render target.
	auto a5 = CD3DX12_RESOURCE_BARRIER::Transition(m_ppRenderTargets[m_currentFrameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_pCmdList->ResourceBarrier(1, &a5);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_currentFrameIndex, m_rtvDescriptorIncrementSize);
	m_pCmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_pCmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	m_pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pCmdList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	m_pCmdList->DrawInstanced(3, 1, 0, 0);

	// Indicate that the back buffer will now be used to present.
	auto a6 = CD3DX12_RESOURCE_BARRIER::Transition(m_ppRenderTargets[m_currentFrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_pCmdList->ResourceBarrier(1, &a6);

	m_pCmdList->Close();
}