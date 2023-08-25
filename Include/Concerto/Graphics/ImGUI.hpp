//
// Created by arthur on 21/12/2022.
//

#ifndef CONCERTOGRAPHICS_IMGUI_HPP
#define CONCERTOGRAPHICS_IMGUI_HPP

#include <imgui.h>
#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/RenderingContext.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorPool.hpp"

namespace Concerto::Graphics
{
	class Window;
	class CommandBuffer;

	class CONCERTO_GRAPHICS_API ImGUI
	{
	public:
		explicit ImGUI(RenderingContext &context, Window &window);
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
		void RenderDrawData(CommandBuffer &commandBuffer);

		/**
		 * @brief End the Dear ImGui frame, finalize the draw data
		 */
		void Draw();

		/**
		 * @brief Get the Dear ImGui context
		 * @return The Dear ImGui context
		 */
		[[nodiscard]] ImGuiContext* GetContext() const;

		/**
		 * @bief Queue a mouse position update
		 * @param x The x position
		 * @param y The y position
		 */
		void UpdateMousePosition(float x, float y);

		/**
		 * @bief Queue a mouse button update
		 * @param button The button index
		 * @param pressed The button state
		 */
		void UpdateMouseButton(int button, bool pressed);

	private:
		RenderingContext& _context;
		DescriptorPool _descriptorPool;
		Window &_window;
		ImGuiContext* _imGuiContext;
	};

} // Concerto::Graphics

#endif //CONCERTOGRAPHICS_IMGUI_HPP
