#pragma once


class Texturing;

enum class DemoLightDirection { FromTop, FromSide, Diagonally };
enum class DemoSubject { Stones, Nvidia, Fan, Text };

class TexturingOptions
{
public:
	TexturingOptions(Texturing & painter);
	~TexturingOptions();

	unsigned short iterations() const;
	void setIterations(unsigned short iterations);

	DemoLightDirection lightDirection() const;
	void setLightDirection(DemoLightDirection lightDirection);

	DemoSubject subject() const;
	void setSubject(DemoSubject subject);

	bool useNormals() const;
	void setUseNormals(bool useNormals);

	bool useColors() const;
	void setUseColors(bool useColors);

	bool useHeights() const;
	void setUseHeights(bool useHeights);

	float inverseBumpDepth() const;
	void setInverseBumpDepth(float inverseBumpDepth);

	unsigned short depthLayers() const;
	void setDepthLayers(unsigned short depthLayers);

	bool subjectChanged() const;

	bool depthLayersChanged() const;

protected:
	Texturing & m_painter;

	unsigned short m_iterations;
	DemoLightDirection m_lightDirection;
	DemoSubject m_subject;
	bool m_useNormals;
	bool m_useColors;
	bool m_useHeights;
	float m_inverseBumpDepth;
	unsigned short m_depthLayers;
	mutable bool m_subjectChanged;
	mutable bool m_depthLayersChanged;
};
