#include "GpuApi/Dx12/Dx12App.h"

#include "Core/Logger.h"
#include "Core/Error.h"

#include "GpuApi/Dx12/Dx12CommandManager.h"
#include "GpuApi/Dx12/Dx12Device.h"
#include "GpuApi/Dx12/Dx12GpuApi.h"
#include "GpuApi/Dx12/Dx12Queue.h"
#include "GpuApi/Dx12/Dx12ResourceManager.h"

#include "Renderer/Vertex.h"
#include "Renderer/Dx12/Dx12Renderer.h"
#include "Renderer/Dx12/Dx12Vertex.h"

#include <functional>
#include <vector>

namespace dxe
{
	// Dx12App

	void Dx12App::Initialize()
	{
		Logger::Initialize();
		Logger::Info("Initializing DX12 Engine...");

		eventRegistry = std::make_shared<EventRegistry>();
		eventRegistry->RegisterCallback<dxe::WindowCloseCallbackData>(
			std::bind(&Dx12App::OnWindowClose, this, std::placeholders::_1));

		InitializeWindow();
		InitializeDx12();

		LoadAssets();

		Logger::Info("DX12 Engine initialization complete!");
	}
	void Dx12App::Terminate()
	{
		TerminateDx12();
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

	void Dx12App::LoadAssets()
	{
		Dx12GpuData* gpuData = GetDx12GpuData();

		// Create Vertex and Index buffers

		std::vector<VertexPC> vertices
		{
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			{ {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } }
		};

		std::shared_ptr<Dx12Mesh> mesh = std::make_shared<Dx12Mesh>();
		mesh->CreateMesh(gpuData->device->GetDevice(), vertices);

		meshId = gpuData->resourceManager->meshes.AddResource(mesh);

		// Create Root Signature

		std::shared_ptr<Dx12RootSignature> rootSignature = std::make_shared<Dx12RootSignature>();
		rootSignature->CreateRootSignature(gpuData->device->GetDevice());

		rootSignatureId = gpuData->resourceManager->rootSignatures.AddResource(rootSignature);

		// Create Graphics Pipeline State Object

		std::shared_ptr<Dx12GraphicsPSO> graphicsPSO = std::make_shared<Dx12GraphicsPSO>();

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

		graphicsPSO->CreateGraphicsPSO(gpuData->device->GetDevice());
		
		graphicsPSOId = gpuData->resourceManager->graphicsPSOs.AddResource(graphicsPSO);
	}

	void Dx12App::Render()
	{
		RenderData renderData{};
		renderData.meshId = meshId;
		renderData.rootSignatureId = rootSignatureId;
		renderData.graphicsPSOId = graphicsPSOId;

		Dx12GpuData* gpuData = GetDx12GpuData();
		gpuData->renderer->Render(renderData);

		gpuData->commandManager->GetDirectQueue()->FlushQueue();
	}

	void Dx12App::OnWindowClose(const WindowCloseCallbackData& callbackData)
	{
		Logger::Info("Closing the application...");
		appIsRunning = false;
	}
}