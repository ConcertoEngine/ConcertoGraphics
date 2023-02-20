//
// Created by arthur on 23/05/22.
//

#ifndef CONCERTO_AWINDOW_HPP
#define CONCERTO_AWINDOW_HPP

#include <string>
#include <functional>
#include <optional>

namespace Concerto::Graphics
{
	//TODO : custom enum for keys
	enum class Key
	{

	};
	//TODO : Remove template because it's breaking the abstraction
	template<typename T>
	class AWindow
	{
	public:
		AWindow(const std::string& title, int width, int height) : _title(title), _width(width), _height(height)
		{

		}

		AWindow() = delete;

		AWindow(AWindow&&) noexcept = default;

		AWindow(const AWindow&) = delete;

		AWindow& operator=(AWindow&&) noexcept = default;

		AWindow& operator=(const AWindow&) = delete;

		virtual ~AWindow() = default;

		/**
		* @brief Get the raw window
		* @return The raw window as a void pointer
		*/
		virtual void* GetRawWindow() = 0;
		/**
		 * @return The cursor position in the window
		 */
		//virtual Math::Vector2d getCursorPosition() = 0;
		/**
		 * @return The width of the window
		 */
		virtual std::uint32_t GetWidth() = 0;

		/**
		 * @return The height of the window
		 */
		virtual std::uint32_t GetHeight() = 0;
		/**
		 * @return The window clear color
		 */
		//		[[nodiscard]] virtual const Math::Vector4f& getClearColor() const = 0;
		/**
		 * @brief Set the window clear color
		 * @param color The new clear color
		 */
		//		virtual void setClearColor(const Math::Vector4f& color) = 0;
		/**
		 * @brief Set the window clear color
		 * @param r The red component of the new clear color
		 * @param g The green component of the new clear color
		 * @param b The blue component of the new clear color
		 * @param a The alpha component of the new clear color
		 */
		//		virtual void setClearColor(float r, float g, float b, float a) = 0;
		/**
		 * @bried Set the window title
		 * @param title The new title
		 */
		virtual void SetTitle(const std::string& title) = 0;

		/**
		 * @brief Set the window icon
		 * @param path The path to the icon
		 */
		virtual void SetIcon(const std::string& path) = 0;

		/**
		 * @brief Set the cursor visibility
		 * @param visible True if the cursor should be visible, false otherwise
		 */
		virtual void SetCursorVisible(bool visible) = 0;

		/**
		 * @brief Set the cursor position
		 * @param x The x position of the cursor
		 * @param y The y position of the cursor
		 */
		virtual void SetCursorPosition(int x, int y) = 0;

		/**
		 * @brief Set the cursor icon
		 * @param path The path to the cursor icon
		 */
		virtual void SetCursorIcon(const std::string& path) = 0;

		/**
		 * @brief The cursor always go back to the center of the window, and becomes invisible
		 * @param disabled True if the cursor should be disabled, false otherwise
		 */
		virtual void SetCursorDisabled(bool disabled) = 0;

		/**
		 * @brief Get an event
		 * @return an empty optional if the key is not pressed, a filled optional otherwise
		 */
		virtual std::optional<Key> PopEvent() = 0;

		/**
		 * @brief Check if the window should close
		 * @return True if the window should close, false otherwise
		 */
		virtual bool ShouldClose() = 0;

		/**
		 * @brief Register a callback function that will be called when the window is resized
		 * @param callback The callback function
		 */
		virtual void RegisterResizeCallback(std::function<void(AWindow& window)> callback) = 0;

		/**
		 * @brief Register a callback function that will be called when a key is pressed
		 * @param callback The callback function
		 */
		virtual void RegisterKeyCallback(std::function<void(AWindow& window, Key button, int scancode, int action, int mods)> callback) = 0;

		virtual void RegisterMouseButtonCallback(std::function<void(AWindow& window, int button, int action, int mods)> callback) = 0;

		virtual void RegisterCursorPosCallback(std::function<void(AWindow& window, double xpos, double ypos)> callback) = 0;
	protected:
		std::string _title;
		std::size_t _width;
		std::size_t _height;
		std::unique_ptr<T, std::function<void(T*)>> _window;
		std::function<void(AWindow& window)> _resizeCallback;
		std::function<void(AWindow& window, Key key, int scancode, int action, int mods)> _keyCallback;
		std::function<void(AWindow& window, int button, int action, int mods)> _mouseButtonCallback;
		std::function<void(AWindow& window, double xpos, double ypos)> _cursorPosCallback;
	};
	template<typename T>
	using AWindowPtr = std::unique_ptr<AWindow<T>>;
}

#endif //CONCERTO_AWINDOW_HPP