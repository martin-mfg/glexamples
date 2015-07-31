#include "TexturingOptions.h"
#include "Texturing.h"


TexturingOptions::TexturingOptions(Texturing & painter)
	: m_painter(painter)
	, m_iterations(16)
	, m_lightDirection(DemoLightDirection::Diagonally)
	, m_subject(DemoSubject::Stones)
	, m_useNormals(true)
	, m_useColors(true)
	, m_useHeights(true)
	, m_inverseBumpDepth(0.5f)
	, m_depthLayers(16)
	, m_subjectChanged(true)
	, m_depthLayersChanged(true)
{
	painter.addProperty<unsigned short>("iterations", this,
		&TexturingOptions::iterations,
		&TexturingOptions::setIterations)->setOptions({
			{ "minimum", 0 },
			{ "maximum", 1000 },
			{ "step", 1 } });
	
	painter.addProperty<DemoLightDirection>("light_direction", this,
		&TexturingOptions::lightDirection,
		&TexturingOptions::setLightDirection)->setStrings({
			{ DemoLightDirection::FromSide, "from the side" },
			{ DemoLightDirection::FromTop, "from from above" },
			{ DemoLightDirection::Diagonally, "diagonally" } });

	painter.addProperty<DemoSubject>("subject", this,
		&TexturingOptions::subject,
		&TexturingOptions::setSubject)->setStrings({
			{ DemoSubject::Stones, "stones" },
			{ DemoSubject::Nvidia, "Nvidia logo" },
			{ DemoSubject::Fan, "fan" },
			{ DemoSubject::Text, "text" } });

	painter.addProperty<bool>("use_normal_texture", this,
		&TexturingOptions::useNormals,
		&TexturingOptions::setUseNormals);
	
	painter.addProperty<bool>("use_color_texture", this,
		&TexturingOptions::useColors,
		&TexturingOptions::setUseColors);

	painter.addProperty<bool>("use_height_texture", this,
		&TexturingOptions::useHeights,
		&TexturingOptions::setUseHeights);
	
	painter.addProperty<float>("inverse_bump_depth", this,
		&TexturingOptions::inverseBumpDepth, 
		&TexturingOptions::setInverseBumpDepth)->setOptions({
			{ "minimum", 0.01f },
			{ "maximum", 1000.0f },
			{ "step", 0.2f },
			{ "precision", 2u } });

	painter.addProperty<unsigned short>("depth_layers", this,
		&TexturingOptions::depthLayers,
		&TexturingOptions::setDepthLayers)->setOptions({
			{ "minimum", 1 },
			{ "maximum", 1000 },
			{ "step", 2 } });
}

TexturingOptions::~TexturingOptions() = default;

unsigned short TexturingOptions::iterations() const
{
	return m_iterations;
}

void TexturingOptions::setIterations(unsigned short iterations)
{
	m_iterations = iterations;
}

DemoLightDirection TexturingOptions::lightDirection() const
{
	return m_lightDirection;
}

void TexturingOptions::setLightDirection(DemoLightDirection lightDirection)
{
	m_lightDirection = lightDirection;
}

DemoSubject TexturingOptions::subject() const
{
	return m_subject;
}

void TexturingOptions::setSubject(DemoSubject subject)
{
	m_subject = subject;
	m_subjectChanged = true;
}

bool TexturingOptions::useNormals() const
{
	return m_useNormals;
}

void TexturingOptions::setUseNormals(bool useNormals)
{
	m_useNormals = useNormals;
}

bool TexturingOptions::useColors() const
{
	return m_useColors;
}

void TexturingOptions::setUseColors(bool useColors)
{
	m_useColors = useColors;
}

bool TexturingOptions::useHeights() const
{
	return m_useHeights;
}

void TexturingOptions::setUseHeights(bool useHeights)
{
	m_useHeights = useHeights;
}

float TexturingOptions::inverseBumpDepth() const
{
	return m_inverseBumpDepth;
}

void TexturingOptions::setInverseBumpDepth(float inverseBumpDepth)
{
	m_inverseBumpDepth = inverseBumpDepth;
}

unsigned short TexturingOptions::depthLayers() const
{
	return m_depthLayers;
}

void TexturingOptions::setDepthLayers(unsigned short depthLayers)
{
	m_depthLayers = depthLayers;
	m_depthLayersChanged = true;
}

bool TexturingOptions::subjectChanged() const
{
	const auto changed = m_subjectChanged;
	m_subjectChanged = false;
	return changed;
}

bool TexturingOptions::depthLayersChanged() const
{
	const auto changed = m_depthLayersChanged;
	m_depthLayersChanged = false;
	return changed;
}
