//
// Created by arthur on 10/06/22.
//


#include <fstream>
#include <exception>
#include "wrapper/ShaderModule.hpp"

namespace Concerto::Graphics::Wrapper
{

	ShaderModule::ShaderModule(const std::string& shaderPath, VkDevice device) : _shaderModule(VK_NULL_HANDLE)
	{
		loadShaderModule(shaderPath);
		createShaderModule();
	}

	ShaderModule::~ShaderModule()
	{
		vkDestroyShaderModule(_device, _shaderModule, nullptr);
	}

	void ShaderModule::loadShaderModule(const std::string& shaderPath)
	{
		std::ifstream file(shaderPath, std::ios::ate | std::ios::binary);
		if (!file.is_open())
			throw std::runtime_error("Failed to open the file " + shaderPath);
		std::streamsize fileSize = file.tellg();
		_buffer.resize(fileSize / sizeof(std::uint32_t));
		file.seekg(0);
		file.read(reinterpret_cast<char*>(_buffer.data()), fileSize);
		file.close();
		_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		_shaderModuleCreateInfo.pNext = nullptr;
		_shaderModuleCreateInfo.codeSize = _buffer.size() * sizeof(std::uint32_t);
		_shaderModuleCreateInfo.pCode = _buffer.data();
	}

	void ShaderModule::createShaderModule()
	{
		VkShaderModule shaderModule;
		if (vkCreateShaderModule(_device, &_shaderModuleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module");
		}
	}

	VkShaderModule ShaderModule::getShaderModule()
	{
		return _shaderModule;
	}
}