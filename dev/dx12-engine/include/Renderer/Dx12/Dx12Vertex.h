#pragma once

#include "Renderer/Vertex.h"

#include <d3d12.h>
#include <dxgi1_6.h>

#include <string>

namespace dxe
{
	DXGI_FORMAT PickDx12VertexAttribFormat(VertexAttribFormat format, uint32_t dimension);

	DXGI_FORMAT PickFloat32Dx12VertexAttribFormat(uint32_t dimension);

	DXGI_FORMAT PickUint32Dx12VertexAttribFormat(uint32_t dimension);
	DXGI_FORMAT PickUint16Dx12VertexAttribFormat(uint32_t dimension);
	DXGI_FORMAT PickUint8Dx12VertexAttribFormat(uint32_t dimension);

	DXGI_FORMAT PickInt32Dx12VertexAttribFormat(uint32_t dimension);
	DXGI_FORMAT PickInt16Dx12VertexAttribFormat(uint32_t dimension);
	DXGI_FORMAT PickInt8Dx12VertexAttribFormat(uint32_t dimension);

	const std::string& GetSemanticsName(VertexAttribType attribType);

	std::vector<D3D12_INPUT_ELEMENT_DESC> VertexAttribLayoutToDx12AttribLayout(
		const std::vector<VertexAttribDescriptor>& attribLayout);

	std::vector<VertexAttribDescriptor> Dx12AttribLayoutToVertexAttribLayout(
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& dx12AttribLayout);
}