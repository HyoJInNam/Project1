#pragma once

enum anchors {
	ANCHORS_TOP_LEFT,
	ANCHORS_TOP,
	ANCHORS_TOP_RIGHT,
	ANCHORS_LEFT,
	ANCHORS_CENTER,
	ANCHORS_RIGHT,
	ANCHORS_BOTTOM_LEFT,
	ANCHORS_BOTTOM,
	ANCHORS_BOTTOM_RIGHT,
};
class TEXTURESHADER;
class LOADIMGFILE;

class BitmapClass : public Transform <BitmapClass>
{
public:
	BitmapClass(const char* name);
	BitmapClass(const BitmapClass&);
	~BitmapClass();
	virtual bool ViewTransform() override;

	bool Initialize(WCHAR*);
	void Shutdown();
	bool Render(RNDMATRIXS& matrixs);

	bool Reposition(float, float);
	bool FullScreen();
	bool Resize(float, float);


private:
	UINT screenWidth;
	UINT screenHeight;

	LOADIMGFILE* ImgFile;
	TEXTURESHADER* texture;

	float m_bitmapWidth;
	float m_bitmapHeight;

	float left, right, top, bottom;
};