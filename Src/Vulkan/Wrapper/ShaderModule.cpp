//
// Created by arthur on 10/06/22.
//

#include <cassert>
#include <fstream>
#include <iostream>

#include "Vulkan/wrapper/ShaderModule.hpp"
#include "Vulkan/wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{

	ShaderModule::ShaderModule(Device& device, const std::string& shaderPath) : Object<VkShaderModule>(device, [this](Device &device, VkShaderModule handle)
	{ vkDestroyShaderModule(*device.Get(), handle, nullptr); })
	{
		loadShaderModule(shaderPath);
		createShaderModule();
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
		if (vkCreateShaderModule(*_device->Get(), &_shaderModuleCreateInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module");
		}
	}
}