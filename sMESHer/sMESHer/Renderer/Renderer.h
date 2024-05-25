#pragma once
#include <string>
#include <vector>

#include <wrl.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3d11.h>

#include "../GlobalDefs.h"
#include "../AppWindow/AppWindow.h"
#include "../Camera/Camera.h"
#include <DirectXMath.h>

class Renderer {
	//friend class Mesh;
public:
	struct AvailableAdapterDesc {
		char name[16]{};
		IDXGIAdapter* adapter = nullptr;
	};
public:
	Renderer() = delete;
	~Renderer() = delete;
	Renderer(const Renderer& other) = delete;

private:
	static IDXGIAdapter* GetMostPowerfulGPUAdapter() noexcept;
	static void SetupViewPort() noexcept;
	

public:
	static void Initialize(int indexOfAdapter = 0) noexcept;
	static void Terminate() noexcept;

	static void Render() noexcept;

	static void Recreate(int indexOfAdapter = 0) noexcept;

	static void SetRenderTargets() noexcept;

	static void ClearRenderTarget() noexcept;
	static void PresentRenderTargets() noexcept;

	static ID3D11Buffer* CreateConstBuffer(const char* pConstBufferStruct, unsigned int sizeOfStruct) noexcept;
	static void UpdateConstBuffer(const Microsoft::WRL::ComPtr<ID3D11Buffer>& pConstBuff, void* pConstBufferStruct, int sizeOfStruct) noexcept;

	static void SetCamera() noexcept {
		s_pCamera = new Camera(GraphicsFundament::Vector3D(0.0f, 0.0f, -10.0), GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0));
	}

public:
	static IDXGIFactory* s_pFactory;
	static std::vector<AvailableAdapterDesc> m_availableDevices;

	static ID3D11Device* s_pDevice;
	static ID3D11DeviceContext* s_pDeviceContext;
	static IDXGISwapChain* s_pSwapChain;
	static ID3D11RenderTargetView* s_pRenderTargetView;
	static ID3D11DepthStencilView* s_pDepthStencilView;

	static Camera* s_pCamera;
};