#include "Renderer/Dx12/Dx12Shader.h"

#include "Core/Error.h"

#include <d3dcompiler.h>

#include <fstream>

using namespace Microsoft::WRL;

namespace dxe
{
	void Dx12ShaderFactory::CreateShader(Dx12ShaderData& shader)
	{
#if defined(_DEBUG)
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif

		ComPtr<ID3DBlob> errorBlob;

		/*
		HRESULT hr = D3DCompileFromFile(
			shader.shaderPath.c_str(),
			nullptr, nullptr,
			shader.entryPoint.c_str(), "vs_5_0",
			compileFlags, 0,
			shader.shader.ReleaseAndGetAddressOf(),
			errorBlob.ReleaseAndGetAddressOf());

		if (!SUCCEEDED(hr) && errorBlob && errorBlob->GetBufferSize() != 0)
		{
			THROW_DX12_ERROR(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}
		*/

		DX12_THROW_IF_NOT_SUCCESS(
			D3DCompileFromFile(
				shader.shaderPath.c_str(),
				nullptr, nullptr,
				shader.entryPoint.c_str(), "vs_5_0",
				compileFlags, 0,
				shader.shader.ReleaseAndGetAddressOf(),
				errorBlob.ReleaseAndGetAddressOf()),
			"Failed to compile a shader!");
	}

	void Dx12ShaderFactory::CreateVertexShader(Dx12ShaderData& shader)
	{
#if defined(_DEBUG)
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif

		ComPtr<ID3DBlob> errorBlob;

		/*
		DX12_THROW_IF_NOT_SUCCESS(
			D3DCompileFromFile(
				shader.shaderPath.c_str(),
				nullptr, nullptr,
				shader.entryPoint.c_str(), "vs_5_0",
				compileFlags, 0,
				shader.shader.ReleaseAndGetAddressOf(),
				errorBlob.ReleaseAndGetAddressOf()),
			"Failed to compile a shader!");
		*/

		HRESULT hr = D3DCompileFromFile(
			shader.shaderPath.c_str(),
			nullptr, nullptr,
			shader.entryPoint.c_str(), "vs_5_0",
			compileFlags, 0,
			shader.shader.ReleaseAndGetAddressOf(),
			errorBlob.ReleaseAndGetAddressOf());

		if (!SUCCEEDED(hr) && errorBlob && errorBlob->GetBufferSize() != 0)
		{
			THROW_DX12_ERROR(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}
	}
	void Dx12ShaderFactory::CreatePixelShader(Dx12ShaderData& shader)
	{
#if defined(_DEBUG)
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif

		ComPtr<ID3DBlob> errorBlob;

		/*
		DX12_THROW_IF_NOT_SUCCESS(
			D3DCompileFromFile(
				shader.shaderPath.c_str(),
				nullptr, nullptr,
				shader.entryPoint.c_str(), "ps_5_0",
				compileFlags, 0,
				shader.shader.ReleaseAndGetAddressOf(),
				errorBlob.ReleaseAndGetAddressOf()),
			"Failed to compile a shader!");
		*/

		HRESULT hr = D3DCompileFromFile(
			shader.shaderPath.c_str(),
			nullptr, nullptr,
			shader.entryPoint.c_str(), "ps_5_0",
			compileFlags, 0,
			shader.shader.ReleaseAndGetAddressOf(),
			errorBlob.ReleaseAndGetAddressOf());

		if (!SUCCEEDED(hr) && errorBlob && errorBlob->GetBufferSize() != 0)
		{
			THROW_DX12_ERROR(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}
	}

	void Dx12ShaderFactory::DestroyShader(Dx12ShaderData& shader)
	{
		shader.shader.Reset();
	}

	std::vector<char> Dx12ShaderFactory::ReadShaderFile(const std::filesystem::path& shaderFilePath)
	{
		std::ifstream shaderFile(shaderFilePath, std::ifstream::ate | std::ifstream::binary);

		if (!shaderFile.is_open())
		{
			std::string errMsg{ "Couldn't open the shader file: " + shaderFilePath.string() };
			throw std::runtime_error{ errMsg };
		}

		size_t fileSize = shaderFile.tellg();
		std::vector<char> shaderBuf(fileSize);

		shaderFile.seekg(0);
		shaderFile.read(shaderBuf.data(), fileSize);

		shaderFile.close();

		return shaderBuf;
	}
}