#pragma once

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>


namespace globjects
{
    class Program;
    class Texture;
}

namespace gloperate
{
    class AdaptiveGrid;
    class ResourceManager;
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;
    class AbstractPerspectiveProjectionCapability;
    class AbstractCameraCapability;
    class TypedRenderTargetCapability;
    class AbstractVirtualTimeCapability;
}

class HitmanBox;

class Platform : public gloperate::Painter
{
public:
    Platform(gloperate::ResourceManager & resourceManager);
    virtual ~Platform();

    void setupProjection();

protected:
    virtual void onInitialize() override;
    virtual void onPaint() override;
    virtual void onTargetFramebufferChanged();

protected:
    /* capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::AbstractPerspectiveProjectionCapability * m_projectionCapability;
    gloperate::TypedRenderTargetCapability * m_typedRenderTargetCapability;
    gloperate::AbstractCameraCapability * m_cameraCapability;
    gloperate::AbstractVirtualTimeCapability * m_timeCapability;

    /* members */
    globjects::ref_ptr<gloperate::AdaptiveGrid> m_grid;
    globjects::ref_ptr<globjects::Program> m_program;
    globjects::ref_ptr<globjects::Texture> m_boxTexture;
    globjects::ref_ptr<HitmanBox> m_platform;
    gl::GLint m_transformLocation;
};
