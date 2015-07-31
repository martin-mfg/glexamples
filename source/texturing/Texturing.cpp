#include "Texturing.h"

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>
#include <globjects/Texture.h>
#include <globjects/VertexAttributeBinding.h>

#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/CameraCapability.h>
#include <gloperate/primitives/TexturedObject.h>
#include <gloperate/resources/ResourceManager.h>

#include "TexturingOptions.h"


using namespace gl;
using namespace glm;
using namespace globjects;

Texturing::Texturing(gloperate::ResourceManager & resourceManager)
	: Painter(resourceManager)
	, m_targetFramebufferCapability(addCapability(new gloperate::TargetFramebufferCapability()))
	, m_viewportCapability(addCapability(new gloperate::ViewportCapability()))
	, m_projectionCapability(addCapability(new gloperate::PerspectiveProjectionCapability(m_viewportCapability)))
	, m_cameraCapability(addCapability(new gloperate::CameraCapability()))
	, m_options(new TexturingOptions(*this))
	, m_texturedDrawable(nullptr)
	, m_positionsBuffer(nullptr)
	, m_tangentsBuffer(nullptr)
	, m_binormalsBuffer(nullptr)
	, m_normalsBuffer(nullptr)
	, m_texCoordsBuffer(nullptr)
{
}

Texturing::~Texturing() = default;

void Texturing::setupProjection()
{
	static const auto zNear = 0.3f, zFar = 15.f, fovy = 50.f;

	m_projectionCapability->setZNear(zNear);
	m_projectionCapability->setZFar(zFar);
	m_projectionCapability->setFovy(radians(fovy));
}

void Texturing::onInitialize()
{
	globjects::init();

	glClearColor(0.85f, 0.87f, 0.91f, 1.0f);

	setupProjection();

	createAndSetupGeometry();
}

void Texturing::onPaint()
{
	glEnable(GL_DEPTH_TEST);

	const auto transform = m_projectionCapability->projection() * m_cameraCapability->view();
	const auto eye = m_cameraCapability->eye();

	if (m_options->subjectChanged()) {
		createAndSetupTexture();
	}

	glm::vec3 lightPosition;
	switch (m_options->lightDirection()){
	case DemoLightDirection::FromTop:
		lightPosition = glm::vec3(0.0, - 2.0, 3.0);
		break;
	case DemoLightDirection::FromSide:
		lightPosition = glm::vec3(2.0, 0.0, 3.0);
		break;
	default:
		lightPosition = glm::vec3(2.0, - 2.0, 4.0);
		break;
	}

	m_texturedDrawable->setTransform(transform);
	m_texturedDrawable->setEye(eye);
	m_texturedDrawable->setLightPosition(lightPosition);
	m_texturedDrawable->setUseColors(m_options->useColors());
	m_texturedDrawable->setUseNormals(m_options->useNormals());
	m_texturedDrawable->setUseHeights(m_options->useHeights());
	m_texturedDrawable->setIterations(m_options->iterations());
	m_texturedDrawable->setInverseBumpDepth(m_options->inverseBumpDepth());
	m_texturedDrawable->setDepthLayers(m_options->depthLayers());

	m_texturedDrawable->draw();
}

void Texturing::createAndSetupTexture()
{
	std::string subjectName;
	switch (m_options->subject()) {
	case DemoSubject::Fan:
		subjectName = "fan";
		break;
	case DemoSubject::Nvidia:
		subjectName = "nveye";
		break;
	case DemoSubject::Stones:
		subjectName = "stone";
		break;
	default:
		subjectName = "text";
		break;
	}

	auto m_normalTexture = m_resourceManager.load<globjects::Texture>("data/texturing/" + subjectName + "-normal.png");
	auto m_colorTexture = m_resourceManager.load<globjects::Texture>("data/texturing/" + subjectName + "-color.png");

	m_colorTexture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
	m_colorTexture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);
	m_colorTexture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
	m_colorTexture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);
	m_colorTexture->setParameter(gl::GL_TEXTURE_WRAP_R, gl::GL_CLAMP_TO_EDGE);

	m_normalTexture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
	m_normalTexture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);
	m_normalTexture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
	m_normalTexture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);
	m_normalTexture->setParameter(gl::GL_TEXTURE_WRAP_R, gl::GL_CLAMP_TO_EDGE);

	Texture * texture = m_resourceManager.load<globjects::Texture>("data/texturing/" + subjectName + "-height.png");

	m_texturedDrawable->setColorTexture(m_colorTexture);
	m_texturedDrawable->setNormalTexture(m_normalTexture);
	m_texturedDrawable->setHeightTexture(texture);
}

void Texturing::createAndSetupGeometry()
{
	static const std::array<glm::vec3, 4> positions{ {
			glm::vec3(+1.f, -1.f, 0.0f)
			, glm::vec3(+1.f, +1.f, 0.0f)
			, glm::vec3(-1.f, -1.f, 0.0f)
			, glm::vec3(-1.f, +1.f, 0.0f)
		} };

	static const std::array<glm::vec3, 4> normals{ {
			glm::vec3(0.0f, 0.0f, 1.0f)
			, glm::vec3(0.0f, 0.0f, 1.0f)
			, glm::vec3(0.0f, 0.0f, 1.0f)
			, glm::vec3(0.0f, 0.0f, 1.0f)
		} };

	static const std::array<glm::vec2, 4> texCoords{ {
			glm::vec2(1.0f, 0.0f)
			, glm::vec2(1.0f, 1.0f)
			, glm::vec2(0.0f, 0.0f)
			, glm::vec2(0.0f, 1.0f)
		} };

	static const std::array<glm::vec3, 4> tangents{ {
			glm::vec3(1.0f, 0.0f, 0.0f)
			, glm::vec3(1.0f, 0.0f, 0.0f)
			, glm::vec3(1.0f, 0.0f, 0.0f)
			, glm::vec3(1.0f, 0.0f, 0.0f)
		} };

	static const std::array<glm::vec3, 4> binormals{ {
			glm::vec3(0.0f, 1.0f, 0.0f)
			, glm::vec3(0.0f, 1.0f, 0.0f)
			, glm::vec3(0.0f, 1.0f, 0.0f)
			, glm::vec3(0.0f, 1.0f, 0.0f)
		} };

	auto m_vao = new VertexArray;
	m_positionsBuffer = new Buffer();
	m_positionsBuffer->setData(positions, gl::GL_STATIC_DRAW); //needed for some drivers
	m_normalsBuffer = new Buffer();
	m_normalsBuffer->setData(normals, gl::GL_STATIC_DRAW); //needed for some drivers
	m_texCoordsBuffer = new Buffer();
	m_texCoordsBuffer->setData(texCoords, gl::GL_STATIC_DRAW); //needed for some drivers
	m_tangentsBuffer = new Buffer();
	m_tangentsBuffer->setData(tangents, gl::GL_STATIC_DRAW); //needed for some drivers
	m_binormalsBuffer = new Buffer();
	m_binormalsBuffer->setData(binormals, gl::GL_STATIC_DRAW); //needed for some drivers

	auto binding = m_vao->binding(0);
	binding->setAttribute(0);
	binding->setBuffer(m_positionsBuffer, 0, sizeof(glm::vec3));
	binding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);

	binding = m_vao->binding(1);
	binding->setAttribute(1);
	binding->setBuffer(m_normalsBuffer, 0, sizeof(glm::vec3));
	binding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);

	binding = m_vao->binding(2);
	binding->setAttribute(2);
	binding->setBuffer(m_texCoordsBuffer, 0, sizeof(glm::vec2));
	binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);

	binding = m_vao->binding(3);
	binding->setAttribute(3);
	binding->setBuffer(m_tangentsBuffer, 0, sizeof(glm::vec3));
	binding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);

	binding = m_vao->binding(4);
	binding->setAttribute(4);
	binding->setBuffer(m_binormalsBuffer, 0, sizeof(glm::vec3));
	binding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);

	m_vao->enable(0);
	m_vao->enable(1);
	m_vao->enable(2);
	m_vao->enable(3);
	m_vao->enable(4);

	m_texturedDrawable = make_ref<gloperate::TexturedObject>(m_targetFramebufferCapability, m_viewportCapability, m_vao, 4);
}
