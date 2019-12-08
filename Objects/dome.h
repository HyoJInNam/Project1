#pragma once

class LOADDOMEFILE;
class DOMESHADER;

class DOME : public Transform <DOME>
{
	LOADDOMEFILE* file;
	DOMESHADER* skyDomeShader;

public:
	DOME(const char* name);
	DOME(const DOME&);
	~DOME();

	bool Initialize(char* modelFilename);
	void Shutdown();
	void Render(D3D* d3d, RNDMATRIXS& renderMatrixs);

	int GetIndexCount() { return file->GetIndexCount(); }

public:
	virtual bool ViewTransform() override;
	void SetTopColor(D3DXVECTOR4 _topColor) { this->color.topColor = _topColor; };
	D3DXVECTOR4 GetTopColor() { return color.topColor; };

	void SetCenterColor(D3DXVECTOR4 _centerColor) { this->color.centerColor = _centerColor; };
	D3DXVECTOR4 GetCenterColor() { return color.centerColor; };
	
	void SetBottomColor(D3DXVECTOR4 _bottomColor) { this->color.bottomColor = _bottomColor; };
	D3DXVECTOR4 GetBottomColor() { return color.bottomColor; };

private:
	GradientBufferType color;
};