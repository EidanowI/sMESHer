#pragma once
#include <string>
#include <vector>

#include <d3d12.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include "../hi.h"
#include "d3dcompiler.h"

#include "../GlobalDefs.h"
#include "../AppWindow/AppWindow.h"
#include <DirectXMath.h>

class Renderer {
public:
	struct AvailableAdapterDesc {
		char name[16]{};
		IDXGIAdapter1* adapter = nullptr;
	};
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4 offset;
		float padding[60]; // Padding so the constant buffer is 256-byte aligned.
	};
	static_assert((sizeof(SceneConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

	Renderer() = delete;
	~Renderer() = delete;
	Renderer(const Renderer& other) = delete;

private:
	static IDXGIAdapter1* GetMostPowerfulGPUAdapter(IDXGIFactory4* pFactory) noexcept;
	static void LoadAssets() noexcept;
	static void WaitForPreviousFrame() noexcept;
	static void PopulateCommandList() noexcept;

public:
	static void Initialize() noexcept;
	static void Terminate() noexcept;

	static void Render() noexcept;
	static void Update() noexcept;

private:
	//pipeline
	static std::vector<AvailableAdapterDesc> m_availableDevices;

#define NUM_OF_BACK_BUFFERS 2 
	static ID3D12Device* m_pDevice;
	static ID3D12CommandQueue* m_pCmdQueue;
	static ID3D12CommandAllocator* m_pCmdAllocator;
	static IDXGISwapChain4* m_pSwapChain;
	static D3D12_VIEWPORT m_viewport;
	static D3D12_RECT m_surfaceSize;

	static	ID3D12Resource* m_ppRenderTargets[NUM_OF_BACK_BUFFERS];
	static ID3D12DescriptorHeap* m_pRtvDescriptorHeap;	
	static unsigned int m_rtvDescriptorIncrementSize;
	static D3D12_CPU_DESCRIPTOR_HANDLE m_phRtvCpuDescriptorHandles[NUM_OF_BACK_BUFFERS];

	static ID3D12DescriptorHeap* m_pCbvDescriptorHeap;


	static ID3D12RootSignature* m_pRootSignature;
	static ID3D12PipelineState* m_pPipelineState;
	static ID3D12GraphicsCommandList* m_pCmdList;


	 // App resources.
	static ID3D12Resource* m_pVertexBuffer;
	static D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	static ID3D12Resource* m_pConstantBuffer;
	static SceneConstantBuffer m_constantBufferData;
	static UINT8* m_pCbvDataBegin;

	// Synchronization objects.
	static unsigned int m_currentFrameIndex;
	static HANDLE m_hFenceEvent;
	static ID3D12Fence* m_pFence;
	static UINT64 m_fenceValue;

};