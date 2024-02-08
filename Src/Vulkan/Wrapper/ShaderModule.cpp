//
// Created by arthur on 10/06/22.
//

#include <cassert>
#include <fstream>
#include <iostream>

#include "Concerto/Graphics/Vulkan/Wrapper/ShaderModule.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{

	ShaderModule::ShaderModule(Device& device, const std::string& shaderPath, VkShaderStageFlagBits stageFlags, std::string entryPoint /*= "main"*/) :
		Object<VkShaderModule>(device, [this](Device& device, VkShaderModule handle)
			{ vkDestroyShaderModule(*device.Get(), handle, nullptr); }),
		_stageFlags(stageFlags),
		_entryPoint(std::move(entryPoint))
	{
		LoadShaderModule(shaderPath);
		CreateShaderModule();
	}

	ShaderModule::ShaderModule(Device& device, const std::vector<UInt32>& bytes, VkShaderStageFlagBits stageFlags, std::string entryPoint /*= "main"*/) :
		Object<VkShaderModule>(device, [this](Device& device, VkShaderModule handle)
														{ vkDestroyShaderModule(*device.Get(), handle, nullptr); }),
		_stageFlags(stageFlags),
		_entryPoint(std::move(entryPoint))
	{
		_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		_shaderModuleCreateInfo.pNext = nullptr;
		_shaderModuleCreateInfo.codeSize = bytes.size() * sizeof(UInt32);
		_shaderModuleCreateInfo.pCode = reinterpret_cast<const UInt32*>(bytes.data());
		CreateShaderModule();
	}

	VkPipelineShaderStageCreateInfo ShaderModule::GetPipelineShaderStageCreateInfo() const
	{
		return VkPipelineShaderStageCreateInfo {
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			{},
			_stageFlags,
			_handle,
			_entryPoint.c_str(),
			nullptr
		};
	}

	void ShaderModule::LoadShaderModule(const std::string& shaderPath)
	{
		std::ifstream file(shaderPath, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("Failed to open the file " + shaderPath);
		}
		std::streamsize fileSize = file.tellg();
		_buffer.resize(fileSize / sizeof(UInt32));
		file.seekg(0);
		file.read(reinterpret_cast<char*>(_buffer.data()), fileSize);
		file.close();
		_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		_shaderModuleCreateInfo.pNext = nullptr;
		_shaderModuleCreateInfo.codeSize = _buffer.size() * sizeof(UInt32);
		_shaderModuleCreateInfo.pCode = _buffer.data();
	}

	void ShaderModule::CreateShaderModule()
	{
		if (vkCreateShaderModule(*_device->Get(), &_shaderModuleCreateInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("Failed to create shader module");
		}
	}
}