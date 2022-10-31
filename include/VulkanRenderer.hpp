//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTOGRAPHICS_VULKANRENDERER_HPP
#define CONCERTOGRAPHICS_VULKANRENDERER_HPP

namespace Concerto::Graphics
{
	class VulkanRenderer
	{
	public:
		VulkanRenderer() = default;
		~VulkanRenderer() = default;
		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer(VulkanRenderer&&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(VulkanRenderer&&) = delete;
		VulkanRenderer* Instance();
	private:
		VulkanRenderer* _instance;
	};

} // Concerto::Graphics

#endif //CONCERTOGRAPHICS_VULKANRENDERER_HPP
