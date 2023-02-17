//
// Created by arthur on 21/12/2022.
//

#ifndef CONCERTOGRAPHICS_IMGUI_HPP
#define CONCERTOGRAPHICS_IMGUI_HPP

#include "imgui.h"
#include "Vulkan/RenderingContext.hpp"
#include "Vulkan/Wrapper/DescriptorPool.hpp"

namespace Concerto::Graphics
{
	class GlfW3;
	namespace Wrapper
	{
		class CommandBuffer;
	}
	class ImGUI
	{
	public:
		explicit ImGUI(RenderingContext &context, GlfW3 &window);
		~ImGUI();

		/**
		 * @brief Begin a new Dear ImGui frame
		 */
		void NewFrame();
		/**
		 * @brief Render the Dear ImGui frame
		 * @info This function must be called after the rendering pass
		 * @param commandBuffer The command buffer to record the rendering commands
		 */
		void RenderDrawData(Wrapper::CommandBuffer &commandBuffer);
		/**
		 * @brief End the Dear ImGui frame, finalize the draw data
		 */
		void Draw();
	private:
		RenderingContext& _context;
		Wrapper::DescriptorPool _descriptorPool;
		GlfW3 &_window;
	};

} // Concerto::Graphics

#endif //CONCERTOGRAPHICS_IMGUI_HPP
