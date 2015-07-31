#pragma once

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>


namespace globjects
{
	class Buffer;
}

namespace gloperate
{
	class AbstractTargetFramebufferCapability;
	class AbstractViewportCapability;
	class AbstractPerspectiveProjectionCapability;
	class AbstractCameraCapability;
	class TexturedObject;
}

class TexturingOptions;

/**
*  @brief
*    An example relying heavily on gloperate::TexturedObject to show the usage of different texture types.
*    
*    In particular, color textures (supporting transparency), normal textures and/or height textures can be
*    applied to a quad. The normal texture is used for diffuse lighting and the height texture is used for 
*    per pixel displacement mapping 
*    (see "GPU Gems 2", chapter 8; http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter08.html).
*
*    The various parameters for the displacement mapping can be set by the user and the use of each of the
*    textures can be turned on or off individually.
*    Different sets of textures are provided.
*/
class Texturing : public gloperate::Painter
{
public:
	Texturing(gloperate::ResourceManager & resourceManager);
	virtual ~Texturing();

protected:
	virtual void onInitialize() override;
	virtual void onPaint() override;

	void createAndSetupTexture();
	void createAndSetupGeometry();
	void setupProjection();

protected:
	gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
	gloperate::AbstractViewportCapability * m_viewportCapability;
	gloperate::AbstractPerspectiveProjectionCapability * m_projectionCapability;
	gloperate::AbstractCameraCapability * m_cameraCapability;

	globjects::ref_ptr<globjects::Buffer> m_positionsBuffer;
	globjects::ref_ptr<globjects::Buffer> m_tangentsBuffer;
	globjects::ref_ptr<globjects::Buffer> m_binormalsBuffer;
	globjects::ref_ptr<globjects::Buffer> m_normalsBuffer;
	globjects::ref_ptr<globjects::Buffer> m_texCoordsBuffer;

	std::unique_ptr<TexturingOptions> m_options;
	globjects::ref_ptr<gloperate::TexturedObject> m_texturedDrawable;
};
