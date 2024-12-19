//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_ENUMS_HPP
#define CONCERTO_GRAPHICS_RHI_ENUMS_HPP

#include <type_traits>
#include <Concerto/Core/Types.hpp>

namespace cct::gfx::rhi
{
	enum class DeviceType : UInt8
	{
		Other,      //Unknown device type
		Integrated, //GPU integrated to the CPU
		Dedicated,  //GPU dedicated to the graphics
		Virtual,    //Virtual GPU provided by a virtualization system
		Software    //CPU software renderer
	};

	enum class ValidationLevel : UInt8
	{
		None = 0,
		Info = 1,
		Warning = 2,
		Error = 3,
		All = 4
	};

	enum class PixelFormat : UInt8
	{
		R8uNorm,
		R8ui,
		RG8uNorm,
		RG8ui,
		RGB8uNorm,
		RGB8ui,
		BGRuNorm,
		BGRui,
		RGBA8uNorm,
		RGBA8ui,
		BGRA8uNorm,
		BGRA8ui,
		R8sNorm,
		R8i,
		R8_SRGB,
		R8G8sNorm,
		R8G8i,
		R8G8_SRGB,
		RGB8sNorm,
		RGB8i,
		RGB8_SRGB,
		BGRsNorm,
		BGRi,
		BGR_SRGB,
		RGBA8sNorm,
		RGBA8i,
		RGBA8_SRGB,
		BGRA8sNorm,
		BGRA8i,
		BGRA8_SRGB,
		R16uNorm,
		R16G16uNorm,
		R16ui,
		R16G16ui,
		RGBuNorm,
		RGBui,
		RGBA16uNorm,
		RGBA16ui,
		R16sNorm,
		R16i,
		R16G16sNorm,
		R16G16i,
		RGBsNorm,
		RGBi,
		RGBA16sNorm,
		RGBA16i,
		R16f,
		R16G16f,
		RGBf,
		RGBA16f,
		R32ui,
		RG32u,
		RGB32ui,
		RGBA32ui,
		R32i,
		RG32i,
		RGB32i,
		RGBA32i,
		R32f,
		RG32f,
		RGB32f,
		RGBA32f,
		R64ui,
		RG64ui,
		RGB64ui,
		RGBA64ui,
		R64i,
		RG64i,
		RGB64i,
		RGBA64i,
		R64f,
		RG64f,
		RGB64f,
		RGBA64f,
		D32f
	};

	enum class AttachmentLoadOp : UInt8
	{
		Load,
		Clear,
		DontCare
	};

	enum class AttachmentStoreOp : UInt8
	{
		Store,
		DontCare
	};

	enum class ImageLayout : UInt8
	{
		Undefined,
		General,
		ColorAttachmentOptimal,
		DepthStencilAttachmentOptimal,
		DepthStencilReadOnlyOptimal,
		ShaderReadOnlyOptimal,
		TransferSrcOptimal,
		TransferDstOptimal,
		Preinitialized,
		DepthReadOnlyStencilAttachmentOptimal,
		DepthAttachmentStencilReadOnlyOptimal,
		DepthAttachmentOptimal,
		DepthReadOnlyOptimal,
		StencilAttachmentOptimal,
		StencilReadOnlyOptimal,
		ReadOnlyOptimal,
		AttachmentOptimal,
		PresentSrcKhr,
	};

	enum class PipelineStage : UInt32
	{
		Pipe = 0x00000001,
		DrawIndirect = 0x00000002,
		VertexInput = 0x00000004,
		VertexShader = 0x00000008,
		TessellationControlShader = 0x00000010,
		TessellationEvaluationShader = 0x00000020,
		GeometryShader = 0x00000040,
		FragmentShader = 0x00000080,
		EarlyFragmentTests = 0x00000100,
		LateFragmentTests = 0x00000200,
		ColorAttachmentOutput = 0x00000400,
		ComputeShader = 0x00000800,
		Transfer = 0x00001000,
		BottomOfPipe = 0x00002000
	};
	using PipelineStageFlags = std::underlying_type_t<PipelineStage>;

	enum class MemoryAccess : UInt32
	{
		IndirectCommandRead = 0x00000001,
		IndexRead = 0x00000002,
		VertexAttributeRead = 0x00000004,
		UniformRead = 0x00000008,
		InputAttachmentRead = 0x00000010,
		ShaderRead = 0x00000020,
		ShaderWrite = 0x00000040,
		ColorAttachmentRead = 0x00000080,
		ColorAttachmentWrite = 0x00000100,
		DepthStencilAttachmentRead = 0x00000200,
		DepthStencilAttachmentWrite = 0x00000400,
		TransferRead = 0x00000800,
		TransferWrite = 0x00001000,
		HostRead = 0x00002000,
		HostWrite = 0x00004000,
		MemoryRead = 0x00008000,
		MemoryWrite = 0x00010000
	};
	using MemoryAccessFlags = std::underlying_type_t<MemoryAccess>;

	struct AttachmentReference
	{
		UInt32 attachmentIndex;
		ImageLayout imageLayout;
	};

	enum class QueueFamily : UInt8
	{
		Compute,  /**< Compute queue for parallel computation. */
		Graphics, /**< Graphics queue for rendering operations. */
		Transfer  /**< Transfer queue for memory operations. */
	};

	enum class BufferUsage : UInt32
	{
		Uniform = 0x00000001,
		Vertex = 0x00000002,
		Storage = 0x00000004,
		TransferSrc = 0x00000008,
		TransferDst = 0x00000010,
		Indirect = 0x00000020
	};
	using BufferUsageFlags = std::underlying_type_t<BufferUsage>;

	inline std::size_t PadUniformBuffer(std::size_t size, std::size_t minUniformBufferOffsetAlignment)
	{
		if (size > 0)
			return (size + minUniformBufferOffsetAlignment - 1) & ~(minUniformBufferOffsetAlignment - 1);
		return size;
	}
}

#endif //CONCERTO_GRAPHICS_RHI_ENUMS_HPP