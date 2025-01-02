#include "Renderer/Dx12/Dx12Renderer.h"

#include "Core/Error.h"

#include "GpuApi/Dx12/Dx12Barrier.h"
#include "GpuApi/Dx12/Dx12CommandManager.h"
#include "GpuApi/Dx12/Dx12GpuApi.h"
#include "GpuApi/Dx12/Dx12Queue.h"
#include "GpuApi/Dx12/Dx12ResourceManager.h"
#include "GpuApi/Dx12/Dx12SwapChain.h"

namespace dxe
{
	void Dx12Renderer::Initialize()
	{
		// TODO
	}
	void Dx12Renderer::Terminate()
	{
		// TODO
	}

	void Dx12Renderer::Render(const RenderData& renderData) const
	{
		// Get Command Context

		Dx12GpuData* gpuData = GetDx12GpuData();

		Dx12CommandManager* commandManager = gpuData->commandManager;
		Dx12SwapChain* swapChain = gpuData->swapChain;

		const Dx12GraphicsCommandContext* commandContext =
			commandManager->GetGraphicsCommandContext(
				swapChain->GetCurrentBackBufferIndex());

		ID3D12CommandAllocator* commandAllocator = commandContext->commandAllocator.Get();
		ID3D12GraphicsCommandList7* graphicsCommandList = commandContext->graphicsCommandList.Get();

		// Get resources

		std::shared_ptr<Dx12Mesh> mesh =
			gpuData->resourceManager->meshes.GetResource(renderData.meshId);

		std::shared_ptr<Dx12RootSignature> rootSignature =
			gpuData->resourceManager->rootSignatures.GetResource(renderData.rootSignatureId);

		std::shared_ptr<Dx12GraphicsPSO> graphicsPSO =
			gpuData->resourceManager->graphicsPSOs.GetResource(renderData.graphicsPSOId);

		DX12_THROW_IF_NOT_SUCCESS(
			commandAllocator->Reset(),
			"Failed to reset the Command Allocator!");

		// 1.
		// 
		// It's allowed not to set a pipeline when resetting a command buffer
		// and there's little cost to doing so.
		/*
		DX12_THROW_IF_NOT_SUCCESS(
			graphicsCommandList->Reset(commandAllocator.Get(), graphicsPSO->GetPipelineState()),
			"Failed to reset the graphics command list!");
		*/

		// 2.
		// 
		// But when choosing the second option don't forget to set the pipeline state object
		// with a separate function call.

		DX12_THROW_IF_NOT_SUCCESS(
			graphicsCommandList->Reset(commandAllocator, nullptr),
			"Failed to reset the Graphics Command List!");

		graphicsCommandList->SetPipelineState(graphicsPSO->GetPipelineState());
		graphicsCommandList->SetGraphicsRootSignature(graphicsPSO->GetRootSignature());

		graphicsCommandList->RSSetViewports(1, &viewport);
		graphicsCommandList->RSSetScissorRects(1, &scissorRect);

		D3D12_RESOURCE_BARRIER renderTargetBarrier =
			Dx12Barrier::CreatePresentToRenderTargetTransitionBarrier(
				swapChain->GetCurrentBackBufferResource());
		graphicsCommandList->ResourceBarrier(1, &renderTargetBarrier);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = swapChain->GetCurrentBackBufferRTV();
		graphicsCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		graphicsCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		// Render the vertex buffer

		Dx12VertexBuffer* vb = mesh->GetVertexBuffer();
		D3D12_VERTEX_BUFFER_VIEW vbView = vb->GetVertexBufferView();
		graphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		graphicsCommandList->IASetVertexBuffers(0, 1, &vbView);
		graphicsCommandList->DrawInstanced(static_cast<UINT>(vb->GetVertexCount()), 1, 0, 0);

		/*
		graphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		graphicsCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		graphicsCommandList->DrawInstanced(3, 1, 0, 0);
		*/

		D3D12_RESOURCE_BARRIER presentBarrier =
			Dx12Barrier::CreateRenderTargetToPresentTransitionBarrier(
				swapChain->GetCurrentBackBufferResource());
		graphicsCommandList->ResourceBarrier(1, &presentBarrier);

		DX12_THROW_IF_NOT_SUCCESS(
			graphicsCommandList->Close(),
			"Failed to close the graphics command list!");

		Dx12DirectQueue* directQueue = gpuData->commandManager->GetDirectQueue();

		ID3D12CommandList* ppCommandLists[] = { graphicsCommandList };
		directQueue->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		swapChain->Present();
	}

	void Dx12Renderer::SetViewport(uint32_t width, uint32_t height)
	{
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);

		viewport.MinDepth = D3D12_MIN_DEPTH;
		viewport.MaxDepth = D3D12_MAX_DEPTH;
	}
	void Dx12Renderer::SetScissors(uint32_t right, uint32_t bottom)
	{
		scissorRect.left = 0;
		scissorRect.right = static_cast<LONG>(right);
		scissorRect.top = 0;
		scissorRect.bottom = static_cast<LONG>(bottom);
	}
}