//
// Created by arthur on 23/05/22.
//

#include <stdexcept>

#include <Concerto/Core/Logger.hpp>
#include <Concerto/Core/Assert.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "Concerto/Graphics/Window/Window.hpp"
#include "Concerto/Graphics/Window/Event.hpp"

namespace cct::gfx
{
	namespace
	{
		MouseButton::Button SDLButtonToConcerto(UInt8 btn)
		{
			if (btn == SDL_BUTTON_LEFT)
				return MouseButton::Button::Left;
			if (btn == SDL_BUTTON_RIGHT)
				return MouseButton::Button::Right;
			if (btn == SDL_BUTTON_MIDDLE)
				return MouseButton::Button::Middle;
			if (btn == SDL_BUTTON_X1)
				return MouseButton::Button::Button1;
			if (btn == SDL_BUTTON_X2)
				return MouseButton::Button::Button2;
			CCT_ASSERT_FALSE("ConcertoGraphics: Invalid button value");
			return MouseButton::Button::Button5;
		}

		Key SDLKeyToConcerto(SDL_Scancode scancode)
		{
			switch (scancode) {
			case SDL_SCANCODE_SPACE:
				return Key::Space;
			case SDL_SCANCODE_APOSTROPHE:
				return Key::Apostrophe;
			case SDL_SCANCODE_COMMA:
				return Key::Comma;
			case SDL_SCANCODE_MINUS:
				return Key::Minus;
			case SDL_SCANCODE_PERIOD:
				return Key::Period;
			case SDL_SCANCODE_SLASH:
				return Key::Slash;
			case SDL_SCANCODE_0:
				return Key::Zero;
			case SDL_SCANCODE_1:
				return Key::One;
			case SDL_SCANCODE_2:
				return Key::Two;
			case SDL_SCANCODE_3:
				return Key::Three;
			case SDL_SCANCODE_4:
				return Key::Four;
			case SDL_SCANCODE_5:
				return Key::Five;
			case SDL_SCANCODE_6:
				return Key::Six;
			case SDL_SCANCODE_7:
				return Key::Seven;
			case SDL_SCANCODE_8:
				return Key::Eight;
			case SDL_SCANCODE_9:
				return Key::Nine;
			case SDL_SCANCODE_SEMICOLON:
				return Key::Semicolon;
			case SDL_SCANCODE_EQUALS:
				return Key::Equal;
			case SDL_SCANCODE_A:
				return Key::A;
			case SDL_SCANCODE_B:
				return Key::B;
			case SDL_SCANCODE_C:
				return Key::C;
			case SDL_SCANCODE_D:
				return Key::D;
			case SDL_SCANCODE_E:
				return Key::E;
			case SDL_SCANCODE_F:
				return Key::F;
			case SDL_SCANCODE_G:
				return Key::G;
			case SDL_SCANCODE_H:
				return Key::H;
			case SDL_SCANCODE_I:
				return Key::I;
			case SDL_SCANCODE_J:
				return Key::J;
			case SDL_SCANCODE_K:
				return Key::K;
			case SDL_SCANCODE_L:
				return Key::L;
			case SDL_SCANCODE_M:
				return Key::M;
			case SDL_SCANCODE_N:
				return Key::N;
			case SDL_SCANCODE_O:
				return Key::O;
			case SDL_SCANCODE_P:
				return Key::P;
			case SDL_SCANCODE_Q:
				return Key::Q;
			case SDL_SCANCODE_R:
				return Key::R;
			case SDL_SCANCODE_S:
				return Key::S;
			case SDL_SCANCODE_T:
				return Key::T;
			case SDL_SCANCODE_U:
				return Key::U;
			case SDL_SCANCODE_V:
				return Key::V;
			case SDL_SCANCODE_W:
				return Key::W;
			case SDL_SCANCODE_X:
				return Key::X;
			case SDL_SCANCODE_Y:
				return Key::Y;
			case SDL_SCANCODE_Z:
				return Key::Z;
			case SDL_SCANCODE_LEFTBRACKET:
				return Key::LefBracket;
			case SDL_SCANCODE_BACKSLASH:
				return Key::BackSlash;
			case SDL_SCANCODE_RIGHTBRACKET:
				return Key::RightBracket;
			case SDL_SCANCODE_GRAVE:
				return Key::GraveAccent;
			case SDL_SCANCODE_ESCAPE:
				return Key::Escape;
			case SDL_SCANCODE_RETURN:
				return Key::Enter;
			case SDL_SCANCODE_TAB:
				return Key::TAB;
			case SDL_SCANCODE_BACKSPACE:
				return Key::Backspace;
			case SDL_SCANCODE_INSERT:
				return Key::Insert;
			case SDL_SCANCODE_DELETE:
				return Key::Delete;
			case SDL_SCANCODE_RIGHT:
				return Key::Right;
			case SDL_SCANCODE_LEFT:
				return Key::Left;
			case SDL_SCANCODE_DOWN:
				return Key::Down;
			case SDL_SCANCODE_UP:
				return Key::UP;
			case SDL_SCANCODE_PAGEUP:
				return Key::PageUp;
			case SDL_SCANCODE_PAGEDOWN:
				return Key::PageDown;
			case SDL_SCANCODE_HOME:
				return Key::Home;
			case SDL_SCANCODE_END:
				return Key::End;
			case SDL_SCANCODE_CAPSLOCK:
				return Key::CapsLock;
			case SDL_SCANCODE_SCROLLLOCK:
				return Key::ScrollLock;
			case SDL_SCANCODE_NUMLOCKCLEAR:
				return Key::NumLock;
			case SDL_SCANCODE_PRINTSCREEN:
				return Key::PrintScreen;
			case SDL_SCANCODE_PAUSE:
				return Key::Pause;
			case SDL_SCANCODE_F1:
				return Key::F1;
			case SDL_SCANCODE_F2:
				return Key::F2;
			case SDL_SCANCODE_F3:
				return Key::F3;
			case SDL_SCANCODE_F4:
				return Key::F4;
			case SDL_SCANCODE_F5:
				return Key::F5;
			case SDL_SCANCODE_F6:
				return Key::F6;
			case SDL_SCANCODE_F7:
				return Key::F7;
			case SDL_SCANCODE_F8:
				return Key::F8;
			case SDL_SCANCODE_F9:
				return Key::F9;
			case SDL_SCANCODE_F10:
				return Key::F10;
			case SDL_SCANCODE_F11:
				return Key::F11;
			case SDL_SCANCODE_F12:
				return Key::F12;
			case SDL_SCANCODE_F13:
				return Key::F13;
			case SDL_SCANCODE_F14:
				return Key::F14;
			case SDL_SCANCODE_F15:
				return Key::F15;
			case SDL_SCANCODE_F16:
				return Key::F16;
			case SDL_SCANCODE_F17:
				return Key::F17;
			case SDL_SCANCODE_F18:
				return Key::F18;
			case SDL_SCANCODE_F19:
				return Key::F19;
			case SDL_SCANCODE_F20:
				return Key::F20;
			case SDL_SCANCODE_F21:
				return Key::F21;
			case SDL_SCANCODE_F22:
				return Key::F22;
			case SDL_SCANCODE_F23:
				return Key::F23;
			case SDL_SCANCODE_F24:
				return Key::F24;
			case SDL_SCANCODE_KP_0:
				return Key::Kp0;
			case SDL_SCANCODE_KP_1:
				return Key::Kp1;
			case SDL_SCANCODE_KP_2:
				return Key::Kp2;
			case SDL_SCANCODE_KP_3:
				return Key::Kp3;
			case SDL_SCANCODE_KP_4:
				return Key::Kp4;
			case SDL_SCANCODE_KP_5:
				return Key::Kp5;
			case SDL_SCANCODE_KP_6:
				return Key::Kp6;
			case SDL_SCANCODE_KP_7:
				return Key::Kp7;
			case SDL_SCANCODE_KP_8:
				return Key::Kp8;
			case SDL_SCANCODE_KP_9:
				return Key::Kp9;
			case SDL_SCANCODE_KP_DECIMAL:
				return Key::KpDECIMAL;
			case SDL_SCANCODE_KP_DIVIDE:
				return Key::KpDIVIDE;
			case SDL_SCANCODE_KP_MULTIPLY:
				return Key::KpMULTIPLY;
			case SDL_SCANCODE_KP_MINUS:
				return Key::KpSUBTRACT;
			case SDL_SCANCODE_KP_PLUS:
				return Key::KpADD;
			case SDL_SCANCODE_KP_ENTER:
				return Key::KpENTER;
			case SDL_SCANCODE_KP_EQUALSAS400:
				return Key::KpEQUAL;
			case SDL_SCANCODE_KP_EQUALS:
				return Key::LeftShift;
			case SDL_SCANCODE_LSHIFT:
				return Key::LeftControl;
			case SDL_SCANCODE_LCTRL:
				return Key::LeftAlt;
			case SDL_SCANCODE_LALT:
				return Key::LeftSuper;
			case SDL_SCANCODE_LGUI:
				return Key::RightShift;
			case SDL_SCANCODE_RSHIFT:
				return Key::RightControl;
			case SDL_SCANCODE_RCTRL:
				return Key::RightAlt;
			case SDL_SCANCODE_RALT:
				return Key::RightSuper;
			case SDL_SCANCODE_RGUI:
				return Key::Menu;
			default:
				return Key::Undefined;
			}
		}

		int EventHandler(void* userdata, SDL_Event* event)
		{
			Window* window = static_cast<Window*>(userdata);
			if (window == nullptr)
				return 0;
			Event newEvent = {
				.type = Event::Type::Mouse,
				.name = "",
				.data = {}
			};
			switch (event->type)
			{
			case SDL_WINDOWEVENT:
				if (event->window.windowID != window->GetId())
					return 0;
				else if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					window->TriggerResize();
				else if (event->window.event == SDL_WINDOWEVENT_CLOSE)
					window->SetShouldQuit(true);
				else return 0; // Do not trigger event
				break;
			case SDL_MOUSEMOTION:
			{
				if (event->window.windowID != window->GetId())
					return 0;
				MouseEvent mouseEvent = {};
				mouseEvent.type = MouseEvent::Type::Moved;
				mouseEvent.mouseMove.x = event->motion.x;
				mouseEvent.mouseMove.y = event->motion.y;
				mouseEvent.mouseMove.deltaX = event->motion.xrel;
				mouseEvent.mouseMove.deltaY = event->motion.yrel;
				newEvent.data = mouseEvent;
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				if (event->window.windowID != window->GetId())
					return 0;
				MouseEvent mouseEvent = {};
				mouseEvent.type = MouseEvent::Type::Button;
				mouseEvent.button.button = SDLButtonToConcerto(event->button.button);
				mouseEvent.button.triggerType = TriggerType::Pressed;
				newEvent.data = mouseEvent;
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				if (event->window.windowID != window->GetId())
					return 0;
				MouseEvent mouseEvent = {};
				mouseEvent.type = MouseEvent::Type::Button;
				mouseEvent.button.button = SDLButtonToConcerto(event->button.button);
				mouseEvent.button.triggerType = TriggerType::Released;
				newEvent.data = mouseEvent;
				break;
			}
			case SDL_KEYDOWN:
			{
				if (event->window.windowID != window->GetId())
					return 0;
				KeyEvent keyEvent = {};
				newEvent.type = Event::Type::Key;
				keyEvent.key = SDLKeyToConcerto(event->key.keysym.scancode);
				keyEvent.triggerType = TriggerType::Pressed;
				newEvent.data = keyEvent;
				break;
			}
			case SDL_KEYUP:
			{

				if (event->window.windowID != window->GetId())
					return 0;
				KeyEvent keyEvent = {};
				newEvent.type = Event::Type::Key;
				keyEvent.key = SDLKeyToConcerto(event->key.keysym.scancode);
				keyEvent.triggerType = TriggerType::Released;
				newEvent.data = keyEvent;
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				if (event->window.windowID != window->GetId())
					return 0;
				MouseEvent wheelEvent = {};
				wheelEvent.mouseWheel.x = event->wheel.mouseX;
				wheelEvent.mouseWheel.y = event->wheel.mouseX;
				wheelEvent.mouseWheel.delta = event->wheel.preciseY;
				newEvent.type = Event::Type::Mouse;
				newEvent.data = wheelEvent;
				break;
			}
			default:
				return 0; //do not trigger event
			}
			window->GetInputManager().Trigger({ newEvent });
			return 0;
		}
	}

	Window::Window(Int32 displayIndex, const std::string& title, Int32 width, Int32 height) :
		_title(title),
		_width(width),
		_height(height),
		_window(nullptr),
		_windowID(0),
		_shouldQuit(false)
	{
		Uint32 flags = 0;
		flags |= SDL_WINDOW_RESIZABLE;
		_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), width, height, flags);
		if (_window == nullptr)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: Glfw initialization failed");
			throw std::runtime_error("GLFW3 initialization failed");
		}
		_windowID = SDL_GetWindowID(_window);

		if (_windowID == 0)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: Could not get window id message: {}", SDL_GetError());
			throw std::runtime_error(std::format("ConcertoGraphics: Could not get window id message: {}", SDL_GetError()));
		}

		SDL_AddEventWatch(EventHandler, this);
	}
	Window::~Window()
	{
		SDL_DelEventWatch(EventHandler, this);
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}

	void Window::SetTitle(const std::string& title)
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		SDL_SetWindowTitle(_window, title.c_str());
	}


	void Window::SetCursorVisible(bool visible)
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		Int32 result = SDL_ShowCursor(visible);
		if (result < 0)
			Logger::Warning("{}", SDL_GetError());
	}

	void Window::SetCursorIcon(const std::string& path)
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
	}

	void Window::SetCursorDisabled(bool disabled)
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		if (disabled)
		{
			Int32 result = SDL_ShowCursor(SDL_DISABLE);
			if (result < 0)
				Logger::Warning("{}", SDL_GetError());
			result = SDL_SetRelativeMouseMode(SDL_TRUE);
			if (result < 0)
				Logger::Warning("{}", SDL_GetError());
		}
		else
		{
			Int32 result = SDL_ShowCursor(SDL_ENABLE);
			if (result < 0)
				Logger::Warning("{}", SDL_GetError());
			result = SDL_SetRelativeMouseMode(SDL_FALSE);
			if (result < 0)
				Logger::Warning("{}", SDL_GetError());
		}
	}

	UInt32 Window::GetHeight() const
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		int w, h;
		SDL_GetWindowSize(_window, &w, &h);
		return static_cast<UInt32>(h);
	}

	UInt32 Window::GetWidth() const
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		int w, h;
		SDL_GetWindowSize(_window, &w, &h);
		return static_cast<UInt32>(w);
	}

	NativeWindow Window::GetNativeWindow() const
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version)
		NativeWindow nativeWindow;
		if (SDL_GetWindowWMInfo(_window, &wmInfo)) {
#if defined(CCT_PLATFORM_WINDOWS)
			nativeWindow.window = wmInfo.info.win.window;
			nativeWindow.hinstance = wmInfo.info.win.hinstance;
#elif defined(CCT_PLATFORM_MACOS))
			CCT_ASSERT_FALSE("Not implemented");
#elif defined(CCT_PLATFORM_LINUX)
			CCT_ASSERT_FALSE("Not implemented");
#endif
		}
		else {
			CCT_ASSERT_FALSE("ConcertoGraphics: Could not get native window handle message:", SDL_GetError());
			return {};
		}
		return nativeWindow;
	}

	bool Window::ShouldClose() const
	{
		return _shouldQuit;
	}

	void Window::RegisterResizeCallback(std::function<void(Window& window)> callback)
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_resizeCallback = std::move(callback);
	}

	void Window::RegisterKeyCallback(std::function<void(Window&, Key, int, int, int)> callback)
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_keyCallback = std::move(callback);
	}

	void Window::RegisterMouseButtonCallback(std::function<void(Window& window, int button, int action, int mods)> callback)
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_mouseButtonCallback = std::move(callback);
	}

	void Window::RegisterCursorPosCallback(std::function<void(Window& window, double xpos, double ypos)> callback)
	{
		CCT_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_cursorPosCallback = std::move(callback);
	}

	Input& Window::GetInputManager()
	{
		return _input;
	}

	UInt32 Window::GetId() const
	{
		return _windowID;
	}

	void Window::SetShouldQuit(bool value)
	{
		_shouldQuit = value;
	}

	void Window::TriggerResize()
	{
		if(_resizeCallback)
			_resizeCallback(*this);
	}
}
