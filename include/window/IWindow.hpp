//
// Created by arthur on 23/05/22.
//

#ifndef CONCERTO_IWINDOW_HPP
#define CONCERTO_IWINDOW_HPP
#include <string>
#include <memory>

namespace Concerto
{


	class IWindow
	{
	public:
		virtual ~IWindow() = default;
		/**
		 *
		 * @return
		 */
		virtual void *getRawWindow() = 0;
		/**
		 * @return The cursor position in the window
		 */
		//virtual Math::Vector2d getCursorPosition() = 0;
		/**
		 * @return The width of the window
		 */
		virtual std::size_t getWidth() = 0;
		/**
		 * @return The height of the window
		 */
		virtual std::size_t getHeight() = 0;
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
		virtual void setTitle(const std::string &title) = 0;
		/**
		 * @brief Set the window icon
		 * @param path The path to the icon
		 */
		virtual void setIcon(const std::string &path) = 0;
		/**
		 * @brief Set the cursor visibility
		 * @param visible True if the cursor should be visible, false otherwise
		 */
		virtual void setCursorVisible(bool visible) = 0;
		/**
		 * @brief Set the cursor position
		 * @param x The x position of the cursor
		 * @param y The y position of the cursor
		 */
		virtual void setCursorPosition(int x, int y) = 0;
		/**
		 * @brief Set the cursor icon
		 * @param path The path to the cursor icon
		 */
		virtual void setCursorIcon(const std::string &path) = 0;
		/**
		 * @brief The cursor always go back to the center of the window, and becomes invisible
		 * @param disabled True if the cursor should be disabled, false otherwise
		 */
		virtual void setCursorDisabled(bool disabled) = 0;
	};
	using IWindowPtr = std::unique_ptr<IWindow>;
}

#endif //CONCERTO_IWINDOW_HPP