#include "GpuApi/Dx12/Dx12App.h"

#include "Core/Logger.h"
#include "Core/Error.h"

#include "GpuApi/Dx12/Dx12Barrier.h"
#include "Renderer/Dx12/Dx12Shader.h"
#include "Renderer/Dx12/Dx12Vertex.h"

#include <functional>
#include <vector>

using namespace Microsoft::WRL;

namespace dxe
{
	// Dx12App

	void Dx12App::Initialize()
	{
		Logger::Initialize();
		Logger::Info("Initializing DX12 Engine...");

		using namespace std::placeholders;

		eventRegistry = std::make_shared<EventRegistry>();
		eventRegistry->RegisterCallback<dxe::WindowCloseCallbackData>(
			std::bind(&Dx12App::OnWindowClose, this, _1));

		InitializeWindow();
		InitializeDx12();

		Logger::Info("DX12 Engine initialization complete!");
	}
	void Dx12App::Terminate()
	{
		swapChain.reset();

		device->Terminate();
		device.reset();

		window.reset();
		eventRegistry.reset();
		Logger::Terminate();
	}

	void Dx12App::Run()
	{
		appIsRunning = true;
		while (appIsRunning)
		{
			window->Tick();
			eventRegistry->Tick();

			Render();
			directQueue->FlushQueue();
		}

		// Logger::Info("Done!");
	}

	void Dx12App::InitializeWindow()
	{
		Logger::Info("Initializing Win32 window...");

		dxe::WindowSettings defWindowSettings{};
		window = std::make_shared<WindowWin32>(defWindowSettings);

		window->SetEventRegistry(eventRegistry.get());
		window->InitializeWin32WindowParams();
		window->CreateWin32Window();
		window->SetWindowVisibility(true);

		Logger::Info("Win32 window initialization complete!");
	}

	void Dx12App::InitializeDx12()
	{
		device = std::make_unique<Dx12Device>();
		device->Initialize();

		CreateCommandObjects();

		CreateSwapChain();

		ResizeViewport();
		ResizeScissors();

		LoadAssets();
	}

	void Dx12App::Render()
	{
		DX12_THROW_IF_NOT_SUCCESS(
			commandAllocator->Reset(),
			"Failed to reset the command allocator!");

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
			graphicsCommandList->Reset(commandAllocator.Get(), nullptr),
			"Failed to reset the graphics command list!");
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

		ID3D12CommandList* ppCommandLists[] = { graphicsCommandList.Get() };
		directQueue->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		swapChain->Present();
	}

	void Dx12App::CreateCommandObjects()
	{
		directQueue = device->CreateDirectQueue();
		commandAllocator = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT);
		graphicsCommandList = device->CreateGraphicsCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get());
	}

	void Dx12App::CreateSwapChain()
	{
		swapChain = std::make_unique<Dx12SwapChain>(2);
		swapChain->CreateSwapChain(
			device->GetDevice(),
			device->GetFactory(),
			directQueue->GetCommandQueue(),
			window.get());
	}

	void Dx12App::LoadAssets()
	{
		// Create Vertex and Index buffers

		std::vector<VertexPC> vertices
		{
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			{ {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } }
		};

		mesh = std::make_unique<Dx12Mesh>();
		mesh->CreateMesh(device->GetDevice(), vertices);

		// Create Root Signature

		rootSignature = std::make_shared<Dx12RootSignature>();
		rootSignature->CreateRootSignature(device->GetDevice());

		// Create Graphics Pipeline State Object

		graphicsPSO = std::make_shared<Dx12GraphicsPSO>();

		graphicsPSO->SetRootSignature(rootSignature);

		// Shaders

		Dx12ShaderData vertexShader{};
		vertexShader.shaderPath = std::filesystem::path{ L"resource/shaders/color_shader.hlsl" };
		vertexShader.entryPoint = std::string{ "VSMain" };
		vertexShader.shaderType = SHADER_TYPE::VERTEX_SHADER;

		Dx12ShaderFactory::CreateVertexShader(vertexShader);

		Dx12ShaderData fragmentShader;
		fragmentShader.shaderPath = std::filesystem::path{ "resource/shaders/color_shader.hlsl" };
		fragmentShader.entryPoint = std::string{ "PSMain" };
		fragmentShader.shaderType = SHADER_TYPE::FRAGMENT_SHADER;

		Dx12ShaderFactory::CreatePixelShader(fragmentShader);

		// Input Layout

		graphicsPSO->SetInputLayout(VertexAttribLayoutToDx12AttribLayout(VertexPC::attributes));

		// Fixed function states

		graphicsPSO->SetVertexShader(vertexShader);
		graphicsPSO->SetPixelShader(fragmentShader);

		graphicsPSO->SetRasterizerState(Dx12GraphicsPSO::CreateDefaultRasterizerState());
		graphicsPSO->SetBlendState(Dx12GraphicsPSO::CreateDefaultBlendState());

		graphicsPSO->SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		graphicsPSO->SetRTVCount(1);
		graphicsPSO->SetRTVFormat(DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		graphicsPSO->SetDepthStencilState(Dx12GraphicsPSO::CreateNoDepthNoStencilDepthStencilDesc());

		graphicsPSO->SetMultiSampleState(Dx12GraphicsPSO::CreateSingleSampleSampleDesc());

		// Compile PSO

		graphicsPSO->CreateGraphicsPSO(device->GetDevice());
	}

	void Dx12App::ResizeViewport()
	{
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		viewport.Width = static_cast<float>(window->GetWindowWidth());
		viewport.Height = static_cast<float>(window->GetWindowHeight());

		viewport.MinDepth = D3D12_MIN_DEPTH;
		viewport.MaxDepth = D3D12_MAX_DEPTH;
	}
	void Dx12App::ResizeScissors()
	{
		scissorRect.left = 0;
		scissorRect.right = static_cast<LONG>(window->GetWindowWidth());
		scissorRect.top = 0;
		scissorRect.bottom = static_cast<LONG>(window->GetWindowHeight());
	}

	void Dx12App::OnWindowClose(const WindowCloseCallbackData& callbackData)
	{
		Logger::Info("Closing the application...");
		appIsRunning = false;
	}
}