//
// Created by arthur on 23/05/22.
//

#ifndef CONCERTO_GRAPHICS_WINDOW_HPP
#define CONCERTO_GRAPHICS_WINDOW_HPP

#include <string>
#include <functional>
#include <optional>
#include <vulkan/vulkan_core.h>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Window/Key.hpp"
#include "Concerto/Graphics/Window/Input.hpp"

struct GLFWwindow;

namespace cct::gfx
{

	class CONCERTO_GRAPHICS_API Window
	{
	public:
		Window(const std::string& title, int width, int height);

		Window() = delete;

		Window(Window&&) noexcept = default;

		Window(const Window&) = delete;

		Window& operator=(Window&&) noexcept = default;

		Window& operator=(const Window&) = delete;

		~Window();

		/**
		 * @brief Check if Vulkan is supported
		 * @return True if Vulkan is supported, false otherwise
		 */
		bool IsVulkanSupported();
		
		/**
		 * @brief Create a Vulkan surface
		 * @param instance The Vulkan instance
		 * @param surface The surface to draw on
		 * @return True if the surface is successfully created, false otherwise
		 */
		bool CreateVulkanSurface(VkInstance instance, VkSurfaceKHR& surface) const;

		/**
		* @brief Get the raw Window
		* @return The raw Window as a void pointer
		*/
		void* GetRawWindow();
		/**
		 * @return The cursor position in the Window
		 */
		//Math::Vector2d getCursorPosition();
		/**
		 * @return The width of the Window
		 */
		UInt32 GetWidth();

		/**
		 * @return The height of the Window
		 */
		UInt32 GetHeight();

		/**
		 * @bried Set the Window title
		 * @param title The new title
		 */
		void SetTitle(const std::string& title);

		/**
		 * @brief Set the Window icon
		 * @param path The path to the icon
		 */
		void SetIcon(const std::string& path);

		/**
		 * @brief Set the cursor visibility
		 * @param visible True if the cursor should be visible, false otherwise
		 */
		void SetCursorVisible(bool visible);

		/**
		 * @brief Set the cursor position
		 * @param x The x position of the cursor
		 * @param y The y position of the cursor
		 */
		void SetCursorPosition(int x, int y);

		/**
		 * @brief Set the cursor icon
		 * @param path The path to the cursor icon
		 */
		void SetCursorIcon(const std::string& path);

		/**
		 * @brief The cursor always go back to the center of the Window, and becomes invisible
		 * @param disabled True if the cursor should be disabled, false otherwise
		 */
		void SetCursorDisabled(bool disabled);

		/**
		 * @brief Get an event
		 * @return an empty optional if the key is not pressed, a filled optional otherwise
		 */
		std::optional<Key> PopEvent();

		/**
		 * @brief Check if the Window should close
		 * @return True if the Window should close, false otherwise
		 */
		bool ShouldClose();

		/**
		 * @brief Register a callback function that will be called when the Window is resized
		 * @param callback The callback function
		 */
		void RegisterResizeCallback(std::function<void(Window& window)> callback);

		/**
		 * @brief Register a callback function that will be called when a key is pressed
		 * @param callback The callback function
		 */
		void RegisterKeyCallback(std::function<void(Window& window, Key button, int scancode, int action, int mods)> callback);

		/**
		 * @brief Register a callback function that will be called when a mouse button is pressed
		 * @param callback The callback function
		 */
		void RegisterMouseButtonCallback(std::function<void(Window& window, int button, int action, int mods)> callback);

		/**
		 * @brief Register a callback function that will be called when the mouse cursor has moved
		 * @param callback The callback function
		 */
		void RegisterCursorPosCallback(std::function<void(Window& window, double xpos, double ypos)> callback);

		Input& GetInputManager();
	protected:
		void RegisterInputCallbacks();
		std::string _title;
		std::size_t _width;
		std::size_t _height;
		GLFWwindow* _window;
		Input _input;
		std::function<void(Window& window)> _resizeCallback;
		std::function<void(Window& window, Key key, int scancode, int action, int mods)> _keyCallback;
		std::function<void(Window& window, int button, int action, int mods)> _mouseButtonCallback;
		std::function<void(Window& window, double xpos, double ypos)> _cursorPosCallback;
	};
}

#endif //CONCERTO_GRAPHICS_WINDOW_HPP