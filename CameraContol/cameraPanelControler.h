#pragma once

class CAMERA;

class CameraPanelControler :public CAMERA
{
public:
	CameraPanelControler();
	~CameraPanelControler();

	virtual void Update() override;
	void Rotate();
	void Translate();
};