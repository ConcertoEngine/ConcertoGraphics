//
// Created by arthur on 23/05/22.
//

#ifndef CONCERTO_AWINDOW_HPP
#define CONCERTO_AWINDOW_HPP

#include "IWindow.hpp"

namespace Concerto
{
	class AWindow : public IWindow
	{
	public:
		AWindow(const std::string& title, unsigned int width, unsigned int height);

		AWindow() = delete;

		AWindow(AWindow&&) = default;

		//[[nodiscard]] const Math::Vector4f& getClearColor() const final;

		//void setClearColor(const Math::Vector4f& color) final;

//		void setClearColor(float r, float g, float b, float a) final;

		AWindow(const AWindow&) = delete;

		AWindow& operator=(AWindow&&) = default;

		AWindow& operator=(const AWindow&) = delete;

		~AWindow() override = default;

	protected:
		std::string _title;
		std::size_t _width;
		std::size_t _height;
		//Math::Vector4f _clearColor;
	};
}

#endif //CONCERTO_AWINDOW_HPP