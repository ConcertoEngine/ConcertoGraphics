//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTO_GRAPHICS_RENDEROBJECT_HPP
#define CONCERTO_GRAPHICS_RENDEROBJECT_HPP

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
#endif //CONCERTO_GRAPHICS_RENDEROBJECT_HPP
