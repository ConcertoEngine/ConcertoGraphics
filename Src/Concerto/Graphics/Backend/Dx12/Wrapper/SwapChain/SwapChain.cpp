//
// Created by arthur on 01/09/2025.
//

#include "Concerto/Graphics/Backend/Dx12/Wrapper/SwapChain/SwapChain.hpp"
#include "Concerto/Graphics/Backend/Dx12/Wrapper/PhysicalDevice/PhysicalDevice.hpp"
#include "Concerto/Graphics/Backend/Dx12/Wrapper/Device/Device.hpp"
#include "Concerto/Graphics/Backend/Dx12/Wrapper/Factory/Factory.hpp"

namespace cct::gfx::dx12
{
	SwapChain::SwapChain(Device& device, Window& window)
	{
		if (FAILED(Create(device, window)))
			throw Dx12Exception(m_lastResult);
	}

	HRESULT SwapChain::Create(Device& device, Window& window)
	{
		m_device = &device;
		m_window = &window;

		m_lastResult = m_directCommandQueue.Create(*m_device, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_QUEUE_FLAG_NONE);
		if (FAILED(m_lastResult))
			return m_lastResult;

		Factory& factory = m_device->GetPhysicalDevice().GetFactory();
		auto nativeWindow = window.GetNativeWindow();

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = ImageCount;
		swapChainDesc.Width = window.GetWidth();
		swapChainDesc.Height = window.GetHeight();
		swapChainDesc.Format = Factory::PixelFormatToDXGI(window.GetFormat());
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Flags = 0u;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
		m_lastResult = factory->CreateSwapChainForHwnd(m_directCommandQueue.Get(),
														static_cast<HWND>(nativeWindow.window),
														&swapChainDesc,
														nullptr,
														nullptr,
														&swapChain1);
		CCT_ASSERT(SUCCEEDED(m_lastResult), "ConcertoGraphics: CreateSwapChainForHwnd failed HRESULT={}", m_lastResult);
		if (FAILED(m_lastResult))
			return m_lastResult;

		m_lastResult = swapChain1.As(&m_handle);
		if (FAILED(m_lastResult))
			return m_lastResult;

		m_lastResult = m_renderTargetViewHeap.Create(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, ImageCount, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
		if (FAILED(m_lastResult))
			return m_lastResult;

		UInt32 rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle = m_renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
		for (UInt32 n = 0; n < ImageCount; n++)
		{
			m_lastResult = m_handle->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
			if (FAILED(m_lastResult))
				return m_lastResult;
			device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, cpuDescriptorHandle);
			m_renderTargetViewHandles[n] = cpuDescriptorHandle;
			cpuDescriptorHandle.ptr += rtvDescriptorSize;
		}

		return S_OK;
	}

	Vector2u SwapChain::GetExtent() const
	{
		return Vector2u{m_window->GetWidth(), m_window->GetHeight()};
	}
} // namespace cct::gfx::dx12