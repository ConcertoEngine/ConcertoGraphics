//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_RENDEROBJECT_HPP
#define CONCERTOGRAPHICS_RENDEROBJECT_HPP

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Vulkan/VkSubMesh.hpp"
#include "Concerto/Graphics/Vulkan/VkMaterial.hpp"

namespace Concerto::Graphics
{
	struct RenderObject
	{
		std::unique_ptr<VkSubMesh> mesh;
		glm::mat4 transformMatrix;
	};
	using RenderObjectPtr = std::shared_ptr<RenderObject>;
}
#endif //CONCERTOGRAPHICS_RENDEROBJECT_HPP
