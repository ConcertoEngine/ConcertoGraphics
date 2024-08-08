	Texture::Texture(Device& device, const std::string& file, CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue,
			VkImageAspectFlags aspectFlags) :
			_image(device, file, commandBuffer, uploadContext, queue),
			_imageView(device, _image, aspectFlags)
	{

	}