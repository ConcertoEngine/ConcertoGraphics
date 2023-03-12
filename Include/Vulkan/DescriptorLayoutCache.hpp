//
// Created by arthur on 16/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_DESCRIPTORLAYOUTCACHE_HPP_
#define CONCERTOGRAPHICS_INCLUDE_DESCRIPTORLAYOUTCACHE_HPP_

#include <vulkan/vulkan.h>
#include <unordered_map>
#include "Vulkan/wrapper/DescriptorSetLayout.hpp"
#include "Vulkan/wrapper/Device.hpp"

namespace Concerto::Graphics
{
//	template<typename HashMap = std::unordered_map<void, void>>
	class DescriptorLayoutCache
	{
	 public:
		struct DescriptorLayoutInfo
		{
			std::vector<VkDescriptorSetLayoutBinding> bindings;

			bool operator==(const DescriptorLayoutInfo& other) const
			{
				if (other.bindings.size() != bindings.size())
				{
					return false;
				}
				else
				{
					for (int i = 0; i < bindings.size(); i++)
					{
						if (other.bindings[i].binding != bindings[i].binding)
						{
							return false;
						}
						if (other.bindings[i].descriptorType != bindings[i].descriptorType)
						{
							return false;
						}
						if (other.bindings[i].descriptorCount != bindings[i].descriptorCount)
						{
							return false;
						}
						if (other.bindings[i].stageFlags != bindings[i].stageFlags)
						{
							return false;
						}
					}
					return true;
				}
			}

			[[nodiscard]] std::size_t hash() const
			{
				std::size_t result = std::hash<std::size_t>()(bindings.size());
				for (const VkDescriptorSetLayoutBinding& b : bindings)
				{
					std::size_t
						binding_hash = b.binding | b.descriptorType << 8 | b.descriptorCount << 16 | b.stageFlags << 24;
					result ^= std::hash<std::size_t>()(binding_hash);
				}
				return result;
			}
		};
	 private:
		struct DescriptorLayoutHash
		{
			std::size_t operator()(const DescriptorLayoutInfo& descriptorLayoutInfo) const
			{
				return descriptorLayoutInfo.hash();
			}
		};
	 public:
		explicit DescriptorLayoutCache(Wrapper::Device& device) : _device(&device)
		{
		}

		Wrapper::DescriptorSetLayoutPtr GetLayout(const VkDescriptorSetLayoutCreateInfo& createInfo)
		{
			DescriptorLayoutInfo layoutInfo;
			layoutInfo.bindings.reserve(createInfo.bindingCount);
			bool isSorted = true;
			int lastBinding = -1;

			for (int i = 0; i < createInfo.bindingCount; i++)
			{
				layoutInfo.bindings.push_back(createInfo.pBindings[i]);

				if (createInfo.pBindings[i].binding > lastBinding)
				{
					lastBinding = createInfo.pBindings[i].binding;
				}
				else
				{
					isSorted = false;
				}
			}
			if (!isSorted)
			{
				std::sort(layoutInfo.bindings.begin(),
					layoutInfo.bindings.end(),
					[](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b)
					{
					  return a.binding < b.binding;
					});
			}

			auto it = _layoutsCache.find(layoutInfo);
			if (it != _layoutsCache.end())
			{
				return (*it).second;
			}
			auto [elementIt, _] =
				_layoutsCache.emplace(layoutInfo, Wrapper::MakeDescriptorSetLayout(*_device, layoutInfo.bindings));
			return elementIt->second;
		}

	 private:
		Wrapper::Device* _device;
		std::unordered_map<DescriptorLayoutInfo, Wrapper::DescriptorSetLayoutPtr, DescriptorLayoutHash> _layoutsCache;
	};
}
#endif //CONCERTOGRAPHICS_INCLUDE_DESCRIPTORLAYOUTCACHE_HPP_