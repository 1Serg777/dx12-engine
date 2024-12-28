#pragma once

#include "GpuApi/Dx12/Dx12SwapChain.h"
#include "GpuApi/Dx12/Dx12Fence.h"

#include <d3d12.h>
#include <dxgi1_6.h>

#include <cstdint>
#include <memory>
#include <vector>

#include <wrl/client.h>

namespace dxe
{
	class Dx12Device
	{
	public:

		void Initialize();
		void Terminate();

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CreateCommandQueue(
			D3D12_COMMAND_LIST_TYPE queueType) const;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE allocatorType) const;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> CreateGraphicsCommandList(
			D3D12_COMMAND_LIST_TYPE cmdListType, ID3D12CommandAllocator* commandAllocator) const;

		std::shared_ptr<Dx12Fence> CreateFence() const;

		IDXGIFactory7* GetFactory() const;

		IDXGIAdapter4* GetAdapter() const;
		ID3D12Device10* GetDevice() const;

	private:

		void CreateFactory();

#if defined(DEBUG) || defined(_DEBUG)
		void InitializeDebugLayers();
		void InitializeDebugCallbackFunction();
#endif

		void PickAdapter();

		void LogAdapter(std::wstringstream& logStream, UINT adapterIdx, DXGI_ADAPTER_DESC3& adapterDesc);
		void LogAdapterOutputs(std::wstringstream& logStream, Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter);
		void LogOutputDisplayModes(std::wstringstream& logStream, Microsoft::WRL::ComPtr<IDXGIOutput6> output);

		void CreateDevice();

		static void DebugMessenger(
			D3D12_MESSAGE_CATEGORY Category,
			D3D12_MESSAGE_SEVERITY Severity,
			D3D12_MESSAGE_ID ID,
			LPCSTR pDescription,
			void* pContext);

#if defined(DEBUG) || defined(_DEBUG)
		Microsoft::WRL::ComPtr<ID3D12Debug6> debugLayer;
		Microsoft::WRL::ComPtr<ID3D12InfoQueue1> infoQueue;
#endif

		Microsoft::WRL::ComPtr<IDXGIFactory7> factory;

		Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter;
		Microsoft::WRL::ComPtr<ID3D12Device10> device;

		DWORD debugMsgCallbackCookie{ 0 };
	};
}