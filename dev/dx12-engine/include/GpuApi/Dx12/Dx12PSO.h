#pragma once

#include "GpuApi/Dx12/Dx12RootSignature.h"

#include "Renderer/Dx12/Dx12Shader.h"

#include <d3d12.h>

#include <wrl/client.h>

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace dxe
{
	class Dx12PSO
	{
	public:

		ID3D12PipelineState* GetPipelineState() const;
		ID3D12RootSignature* GetRootSignature() const;

	protected:

		D3D12_SHADER_BYTECODE CreateShaderBytecode(const Dx12ShaderData& shaderData) const;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	};

	class Dx12GraphicsPSO : public Dx12PSO
	{
	public:

		static D3D12_RASTERIZER_DESC CreateDefaultRasterizerState();
		static D3D12_BLEND_DESC CreateDefaultBlendState();

		static DXGI_SAMPLE_DESC CreateSingleSampleSampleDesc();

		static DXGI_SAMPLE_DESC CreateMultiSample2XSampleDesc();
		static DXGI_SAMPLE_DESC CreateMultiSample4XSampleDesc();
		static DXGI_SAMPLE_DESC CreateMultiSample8XSampleDesc();

		static D3D12_DEPTH_STENCIL_DESC CreateNoDepthNoStencilDepthStencilDesc();
		static D3D12_DEPTH_STENCIL_DESC CreateDefaultDepthNoStencilDepthStencilDesc();

		void SetInputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout);

		void SetVertexShader(const Dx12ShaderData& vertexShader);
		void SetPixelShader(const Dx12ShaderData& pixelShader);

		void SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc);
		void SetBlendState(const D3D12_BLEND_DESC& blendDesc);

		void SetRootSignature(std::shared_ptr<Dx12RootSignature> rootSignature);

		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopology);

		void SetRTVCount(uint32_t rtvCount);
		void SetRTVFormat(DXGI_FORMAT rtvFormat, uint32_t rtvIdx);

		void SetMultiSampleState(const DXGI_SAMPLE_DESC& sampleDesc);

		void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);

		void CreateGraphicsPSO(ID3D12Device* device);

	private:

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

		std::optional<Dx12ShaderData> vertexShader;
		std::optional<Dx12ShaderData> pixelShader;

		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

		std::shared_ptr<Dx12RootSignature> rootSignature;
	};

	class Dx12ComputePSO : public Dx12PSO
	{
	public:

	private:

		D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc{};
	};
}