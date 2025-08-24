	Texture::Texture(Device& device, const std::string& file, CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue,
			VkImageAspectFlags aspectFlags) :
			m_image(device, file, commandBuffer, uploadContext, queue),
			m_imageView(device, m_image, aspectFlags)
	{

	}