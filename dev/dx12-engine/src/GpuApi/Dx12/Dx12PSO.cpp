#include "GpuApi/Dx12/Dx12PSO.h"

#include "Core/Error.h"

using namespace Microsoft::WRL;

namespace dxe
{
	ID3D12PipelineState* Dx12PSO::GetPipelineState() const
	{
		return pipelineState.Get();
	}
	ID3D12RootSignature* Dx12PSO::GetRootSignature() const
	{
		return rootSignature.Get();
	}

    D3D12_SHADER_BYTECODE Dx12PSO::CreateShaderBytecode(const Dx12ShaderData& shaderData) const
    {
        D3D12_SHADER_BYTECODE shaderBytecode{};
        shaderBytecode.BytecodeLength = shaderData.shader->GetBufferSize();
        shaderBytecode.pShaderBytecode = shaderData.shader->GetBufferPointer();
        return shaderBytecode;
    }

    D3D12_RASTERIZER_DESC Dx12GraphicsPSO::CreateDefaultRasterizerState()
    {
        D3D12_RASTERIZER_DESC rasterizerDesc{};
        rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
        rasterizerDesc.FrontCounterClockwise = TRUE;
        rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rasterizerDesc.DepthClipEnable = TRUE;
        rasterizerDesc.MultisampleEnable = FALSE;
        rasterizerDesc.AntialiasedLineEnable = FALSE;
        rasterizerDesc.ForcedSampleCount = 0;
        rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        return rasterizerDesc;
    }

    D3D12_BLEND_DESC Dx12GraphicsPSO::CreateDefaultBlendState()
    {
        D3D12_BLEND_DESC blendState{};
        blendState.AlphaToCoverageEnable = FALSE;
        blendState.IndependentBlendEnable = FALSE;

        const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
        {
            FALSE,FALSE,
            D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
            D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE_ALL,
        };

        for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        {
            blendState.RenderTarget[i] = defaultRenderTargetBlendDesc;
        }

        return blendState;
    }

    DXGI_SAMPLE_DESC Dx12GraphicsPSO::CreateSingleSampleSampleDesc()
    {
        DXGI_SAMPLE_DESC sampleDesc{};
        sampleDesc.Count = 1;
        sampleDesc.Quality = 0; // What should this value be?
        return sampleDesc;
    }

    DXGI_SAMPLE_DESC Dx12GraphicsPSO::CreateMultiSample2XSampleDesc()
    {
        DXGI_SAMPLE_DESC sampleDesc{};
        sampleDesc.Count = 2;
        sampleDesc.Quality = 0; // What should this value be?
        return sampleDesc;
    }
    DXGI_SAMPLE_DESC Dx12GraphicsPSO::CreateMultiSample4XSampleDesc()
    {
        DXGI_SAMPLE_DESC sampleDesc{};
        sampleDesc.Count = 4;
        sampleDesc.Quality = 0; // What should this value be?
        return sampleDesc;
    }
    DXGI_SAMPLE_DESC Dx12GraphicsPSO::CreateMultiSample8XSampleDesc()
    {
        DXGI_SAMPLE_DESC sampleDesc{};
        sampleDesc.Count = 8;
        sampleDesc.Quality = 0; // What should this value be?
        return sampleDesc;
    }

    D3D12_DEPTH_STENCIL_DESC Dx12GraphicsPSO::CreateNoDepthNoStencilDepthStencilDesc()
    {
        D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

        depthStencilDesc.DepthEnable = FALSE;
        depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
        depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;

        depthStencilDesc.StencilEnable = TRUE;
        depthStencilDesc.StencilReadMask = 0x0;
        // https://www.reddit.com/r/Unity3D/comments/mz7b4h/what_does_stencil_readmask_writemask_do/
        depthStencilDesc.StencilWriteMask = 0x0;

        depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

        depthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

        return depthStencilDesc;
    }

    D3D12_DEPTH_STENCIL_DESC Dx12GraphicsPSO::CreateDefaultDepthNoStencilDepthStencilDesc()
    {
        D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

        depthStencilDesc.DepthEnable = TRUE;
        depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;    

        depthStencilDesc.StencilEnable = TRUE;
        depthStencilDesc.StencilReadMask = 0x0;
        // https://www.reddit.com/r/Unity3D/comments/mz7b4h/what_does_stencil_readmask_writemask_do/
        depthStencilDesc.StencilWriteMask = 0x0;

        depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

        depthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

        return depthStencilDesc;
    }

    void Dx12GraphicsPSO::SetInputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout)
    {
        this->inputLayout = inputLayout;

        D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
        inputLayoutDesc.pInputElementDescs = this->inputLayout.data();
        inputLayoutDesc.NumElements = static_cast<UINT>(this->inputLayout.size());

        psoDesc.InputLayout = inputLayoutDesc;
    }

    void Dx12GraphicsPSO::SetVertexShader(const Dx12ShaderData& vertexShader)
    {
        this->vertexShader = vertexShader;
        psoDesc.VS = CreateShaderBytecode(vertexShader);
    }
    void Dx12GraphicsPSO::SetPixelShader(const Dx12ShaderData& pixelShader)
    {
        this->pixelShader = pixelShader;
        psoDesc.PS = CreateShaderBytecode(pixelShader);
    }

    void Dx12GraphicsPSO::SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc)
    {
        this->psoDesc.RasterizerState = rasterizerDesc;
    }
    void Dx12GraphicsPSO::SetBlendState(const D3D12_BLEND_DESC& blendDesc)
    {
        this->psoDesc.BlendState = blendDesc;
        this->psoDesc.SampleMask = UINT_MAX;
    }

    void Dx12GraphicsPSO::SetRootSignature(std::shared_ptr<Dx12RootSignature> rootSignature)
    {
        this->rootSignature = rootSignature;
        psoDesc.pRootSignature = this->rootSignature->GetRootSignature();
    }

    void Dx12GraphicsPSO::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopology)
    {
        psoDesc.PrimitiveTopologyType = primitiveTopology;
    }

    void Dx12GraphicsPSO::SetRTVCount(uint32_t rtvCount)
    {
        psoDesc.NumRenderTargets = rtvCount;
    }
    void Dx12GraphicsPSO::SetRTVFormat(DXGI_FORMAT rtvFormat, uint32_t rtvIdx)
    {
        psoDesc.RTVFormats[rtvIdx] = rtvFormat;
    }

    void Dx12GraphicsPSO::SetMultiSampleState(const DXGI_SAMPLE_DESC& sampleDesc)
    {
        this->psoDesc.SampleDesc = sampleDesc;
    }

    void Dx12GraphicsPSO::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
    {
        this->psoDesc.DepthStencilState = depthStencilDesc;
    }

    void Dx12GraphicsPSO::CreateGraphicsPSO(ID3D12Device* device)
	{
        DX12_THROW_IF_NOT_SUCCESS(
            device->CreateGraphicsPipelineState(
                &psoDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf())),
            "Failed to create a Graphics Pipeline State Object!");
	}
}