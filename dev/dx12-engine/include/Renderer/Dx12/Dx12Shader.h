#pragma once

#include "Renderer/Shader.h"

#include <d3d12.h>

#include <wrl/client.h>

#include <filesystem>
#include <string>
#include <vector>

namespace dxe
{
	struct Dx12ShaderData
	{
		std::filesystem::path shaderPath;
		std::string entryPoint;

		Microsoft::WRL::ComPtr<ID3DBlob> shader;

		SHADER_TYPE shaderType{};
	};

	class Dx12ShaderFactory
	{
	public:

		static void CreateShader(Dx12ShaderData& shader);

		static void CreateVertexShader(Dx12ShaderData& shader);
		static void CreatePixelShader(Dx12ShaderData& shader);

		static void DestroyShader(Dx12ShaderData& shader);

	private:

		static std::vector<char> ReadShaderFile(const std::filesystem::path& shaderFilePath);

		// TODO?
		// Scanner, Parser, and so on?
	};
}