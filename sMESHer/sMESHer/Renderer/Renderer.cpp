#include "Renderer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")



IDXGIFactory* Renderer::s_pFactory = nullptr;

std::vector<Renderer::AvailableAdapterDesc> Renderer::m_availableDevices = std::vector<Renderer::AvailableAdapterDesc>();

ID3D11Device* Renderer::s_pDevice = nullptr;
ID3D11DeviceContext* Renderer::s_pDeviceContext = nullptr;
IDXGISwapChain* Renderer::s_pSwapChain = nullptr;
ID3D11RenderTargetView* Renderer::s_pRenderTargetView = nullptr;
ID3D11DepthStencilView* Renderer::s_pDepthStencilView = nullptr;

Camera* Renderer::s_pCamera = nullptr;


void Renderer::Initialize(int indexOfAdapter) noexcept{
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
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&s_pFactory);

	DXGI_SWAP_CHAIN_DESC swDesc{};
	{
		swDesc.BufferDesc.Width = 0;//0 - means get it from window
		swDesc.BufferDesc.Height = 0;
		swDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swDesc.BufferDesc.RefreshRate.Numerator = 0;
		swDesc.BufferDesc.RefreshRate.Denominator = 0;
		swDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swDesc.SampleDesc.Count = 1u;
		swDesc.SampleDesc.Quality = 0u;

		swDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swDesc.BufferCount = 1;//So we want one back buffer (double buffering one front and one back)
		swDesc.OutputWindow = AppWindow::s_hWnd;
		swDesc.Windowed = TRUE;
		swDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//vanila (gives you the best perfomance at many cases
		swDesc.Flags = 0;
	}

	IDXGIAdapter* pAdapter = nullptr;
	if (m_availableDevices.empty()) {
		pAdapter = GetMostPowerfulGPUAdapter();
	}
	else {
		pAdapter = m_availableDevices[indexOfAdapter].adapter;
	}

	D3D11CreateDeviceAndSwapChain(
		pAdapter,//video adapter device 0 - use system setings
		D3D_DRIVER_TYPE_UNKNOWN,// Driver.
		nullptr,//handle to dll with software driver
		D3D11_CREATE_DEVICE_DEBUG,//layers
		nullptr,//------features
		0,//features count
		D3D11_SDK_VERSION,
		&swDesc,//swap chain description (input)
		&s_pSwapChain,// (output)
		&s_pDevice,// (output)
		nullptr,// feature level (output)
		&s_pDeviceContext// (output)//
	);

	//get access to texture subresource in swap chain (back buffer)
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	s_pSwapChain->GetBuffer(0/*0 - is a back buffer*/, __uuidof(ID3D11Resource), &pBackBuffer);
	s_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &s_pRenderTargetView);//Set this parameter to NULL to create a view that accesses all of the subresources in mipmap level 0.

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	D3D11_DEPTH_STENCIL_DESC dptStnDesc{};
	{
		dptStnDesc.DepthEnable = TRUE;
		dptStnDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dptStnDesc.DepthFunc = D3D11_COMPARISON_LESS;
	}

	s_pDevice->CreateDepthStencilState(&dptStnDesc, &pDepthStencilState);
	s_pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture2D;
	D3D11_TEXTURE2D_DESC dpthStnTextureDesc{};
	{
		dpthStnTextureDesc.Width = APP_WINDOW_WIDTH;
		dpthStnTextureDesc.Height = APP_WINDOW_HEIGHT;
		dpthStnTextureDesc.MipLevels = 1u;
		dpthStnTextureDesc.ArraySize = 1u;
		dpthStnTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dpthStnTextureDesc.SampleDesc.Count = 1u;
		dpthStnTextureDesc.SampleDesc.Quality = 0u;
		dpthStnTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		dpthStnTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}
	s_pDevice->CreateTexture2D(&dpthStnTextureDesc, nullptr, &pDepthStencilTexture2D);


	D3D11_DEPTH_STENCIL_VIEW_DESC dpthStncViewDesc{};
	{
		dpthStncViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dpthStncViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dpthStncViewDesc.Texture2D.MipSlice = 0u;
	}
	s_pDevice->CreateDepthStencilView(pDepthStencilTexture2D.Get(), &dpthStncViewDesc, &s_pDepthStencilView);

	SetupViewPort();
	SetRenderTargets();

	ImGui_ImplDX11_Init(s_pDevice, s_pDeviceContext);
}
void Renderer::Terminate() noexcept {	
	ImGui_ImplDX11_Shutdown();

	delete s_pCamera;

	if (s_pDepthStencilView != nullptr) {
		s_pDepthStencilView->Release();
		s_pDepthStencilView = nullptr;
	}

	if (s_pRenderTargetView != nullptr) {
		s_pRenderTargetView->Release();
		s_pRenderTargetView = nullptr;
	}

	if (s_pSwapChain != nullptr) {
		s_pSwapChain->Release();
		s_pSwapChain = nullptr;
	}

	if (s_pDeviceContext != nullptr) {
		s_pDeviceContext->Release();
		s_pDeviceContext = nullptr;
	}

	if (s_pDevice != nullptr) {
		s_pDevice->Release();
		s_pDevice = nullptr;
	}

	if (s_pFactory != nullptr) {
		s_pFactory->Release();
		s_pFactory = nullptr;
	}

	m_availableDevices.clear();
}

void Renderer::Render() noexcept {
	
}

void Renderer::Recreate(int indexOfAdapter) noexcept {
	ImGui_ImplDX11_Shutdown();

	delete s_pCamera;

	if (s_pDepthStencilView != nullptr) {
		s_pDepthStencilView->Release();
		s_pDepthStencilView = nullptr;
	}

	if (s_pRenderTargetView != nullptr) {
		s_pRenderTargetView->Release();
		s_pRenderTargetView = nullptr;
	}

	if (s_pSwapChain != nullptr) {
		s_pSwapChain->Release();
		s_pSwapChain = nullptr;
	}

	if (s_pDeviceContext != nullptr) {
		s_pDeviceContext->Release();
		s_pDeviceContext = nullptr;
	}

	if (s_pDevice != nullptr) {
		s_pDevice->Release();
		s_pDevice = nullptr;
	}

	DXGI_SWAP_CHAIN_DESC swDesc{};
	{
		swDesc.BufferDesc.Width = 0;//0 - means get it from window
		swDesc.BufferDesc.Height = 0;
		swDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swDesc.BufferDesc.RefreshRate.Numerator = 0;
		swDesc.BufferDesc.RefreshRate.Denominator = 0;
		swDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swDesc.SampleDesc.Count = 1u;
		swDesc.SampleDesc.Quality = 0u;

		swDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swDesc.BufferCount = 1;//So we want one back buffer (double buffering one front and one back)
		swDesc.OutputWindow = AppWindow::s_hWnd;
		swDesc.Windowed = TRUE;
		swDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//vanila (gives you the best perfomance at many cases
		swDesc.Flags = 0;
	}

	IDXGIAdapter* pAdapter = m_availableDevices[indexOfAdapter].adapter;

	D3D11CreateDeviceAndSwapChain(
		pAdapter,//video adapter device 0 - use system setings
		D3D_DRIVER_TYPE_UNKNOWN,// Driver.
		nullptr,//handle to dll with software driver
		0,//layers
		nullptr,//------features
		0,//features count
		D3D11_SDK_VERSION,
		&swDesc,//swap chain description (input)
		&s_pSwapChain,// (output)
		&s_pDevice,// (output)
		nullptr,// feature level (output)
		&s_pDeviceContext// (output)//
	);

	//get access to texture subresource in swap chain (back buffer)
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	s_pSwapChain->GetBuffer(0/*0 - is a back buffer*/, __uuidof(ID3D11Resource), &pBackBuffer);
	s_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &s_pRenderTargetView);//Set this parameter to NULL to create a view that accesses all of the subresources in mipmap level 0.

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	D3D11_DEPTH_STENCIL_DESC dptStnDesc{};
	{
		dptStnDesc.DepthEnable = TRUE;
		dptStnDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dptStnDesc.DepthFunc = D3D11_COMPARISON_LESS;
	}

	s_pDevice->CreateDepthStencilState(&dptStnDesc, &pDepthStencilState);
	s_pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture2D;
	D3D11_TEXTURE2D_DESC dpthStnTextureDesc{};
	{
		dpthStnTextureDesc.Width = APP_WINDOW_WIDTH;
		dpthStnTextureDesc.Height = APP_WINDOW_HEIGHT;
		dpthStnTextureDesc.MipLevels = 1u;
		dpthStnTextureDesc.ArraySize = 1u;
		dpthStnTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dpthStnTextureDesc.SampleDesc.Count = 1u;
		dpthStnTextureDesc.SampleDesc.Quality = 0u;
		dpthStnTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		dpthStnTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}
	s_pDevice->CreateTexture2D(&dpthStnTextureDesc, nullptr, &pDepthStencilTexture2D);


	D3D11_DEPTH_STENCIL_VIEW_DESC dpthStncViewDesc{};
	{
		dpthStncViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dpthStncViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dpthStncViewDesc.Texture2D.MipSlice = 0u;
	}
	s_pDevice->CreateDepthStencilView(pDepthStencilTexture2D.Get(), &dpthStncViewDesc, &s_pDepthStencilView);

	SetupViewPort();
	SetRenderTargets();

	ImGui_ImplDX11_Init(s_pDevice, s_pDeviceContext);
}


IDXGIAdapter* Renderer::GetMostPowerfulGPUAdapter() noexcept {
	int bestScore = 0;
	IDXGIAdapter* retpAdapter = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	for (int i = 0; s_pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		int score = 0;
		AvailableAdapterDesc desc;
		DXGI_ADAPTER_DESC adapterDesc;

		pAdapter->GetDesc(&adapterDesc);
		for (int j = 0; j < 16; j++) {
			desc.name[j] = adapterDesc.Description[j];
		}
		desc.adapter = pAdapter;

		std::wstring description(adapterDesc.Description);

		if (description.find(L"RTX") != std::wstring::npos || description.find(L"GTX") != std::wstring::npos || description.find(L"RX") != std::wstring::npos) {
			score += 5;
		}
		score += (int)(adapterDesc.DedicatedVideoMemory / 1000000000);

		if (score > bestScore) {
			retpAdapter = pAdapter;
			bestScore = score;
		}

		m_availableDevices.push_back(desc);
	}

	return retpAdapter;
}
void Renderer::SetupViewPort() noexcept {
	D3D11_VIEWPORT viewPort{};
	{
		viewPort.TopLeftX = 0.f;
		viewPort.TopLeftY = 0.f;
		viewPort.Width = APP_WINDOW_WIDTH;
		viewPort.Height = APP_WINDOW_HEIGHT;
		viewPort.MinDepth = 0.f;
		viewPort.MaxDepth = 1.f;
	}
	s_pDeviceContext->RSSetViewports(1, &viewPort);//WE need a viewport for rasterization
}
void Renderer::SetRenderTargets() noexcept {
	s_pDeviceContext->OMSetRenderTargets(1u, &s_pRenderTargetView, s_pDepthStencilView);//sprecify stensilDepth
}
void Renderer::ClearRenderTarget() noexcept {
	const float color[] = { 0.15f, 0.15f, 0.15f, 1.0f };
	s_pDeviceContext->ClearRenderTargetView(s_pRenderTargetView, color);
	s_pDeviceContext->ClearDepthStencilView(s_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
}
void Renderer::PresentRenderTargets() noexcept {
	s_pSwapChain->Present(1u, 0u);
}

ID3D11Buffer* Renderer::CreateConstBuffer(const char* pConstBufferStruct, unsigned int sizeOfStruct) noexcept {
	ID3D11Buffer* pConstBuffer;

	D3D11_BUFFER_DESC constBuffDesc{};
	{
		constBuffDesc.ByteWidth = sizeOfStruct;
		constBuffDesc.Usage = D3D11_USAGE_DYNAMIC;// D3D11_USAGE_IMMUTABLE can gives more fps
		constBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//
		constBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constBuffDesc.MiscFlags = 0u;// miscellaneous
		constBuffDesc.StructureByteStride = 0u;
	}

	D3D11_SUBRESOURCE_DATA constBuffSubResData{};
	{
		constBuffSubResData.pSysMem = pConstBufferStruct;
	}

	s_pDevice->CreateBuffer(&constBuffDesc, &constBuffSubResData, &pConstBuffer);

	return pConstBuffer;
}
void Renderer::UpdateConstBuffer(const Microsoft::WRL::ComPtr<ID3D11Buffer>& pConstBuff, void* pConstBufferStruct, int sizeOfStruct) noexcept {
	D3D11_MAPPED_SUBRESOURCE constBuffMappedSRes;
	s_pDeviceContext->Map(pConstBuff.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &constBuffMappedSRes);
	memcpy(constBuffMappedSRes.pData, pConstBufferStruct, sizeOfStruct);
	s_pDeviceContext->Unmap(pConstBuff.Get(), 0u);
}