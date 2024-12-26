#include "GpuApi/Dx12/Dx12RootSignature.h"

#include "Core/Error.h"

using namespace Microsoft::WRL;

namespace dxe
{
	void Dx12RootSignature::CreateRootSignature(ID3D12Device* device)
	{
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 0;
		rootSignatureDesc.pParameters = nullptr;
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;

		DX12_THROW_IF_NOT_SUCCESS(
			D3D12SerializeRootSignature(
				&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
			"Failed to serialize a Root Signature!");

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateRootSignature(
				0,
				signature->GetBufferPointer(),
				signature->GetBufferSize(),
				IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf())),
			"");
	}

	ID3D12RootSignature* Dx12RootSignature::GetRootSignature() const
	{
		return rootSignature.Get();
	}
}