#pragma once

class CAMERA;

class CameraPanelControler :public CAMERA
{
public:
	CameraPanelControler(const char* name);
	CameraPanelControler(const CameraPanelControler& other);
	~CameraPanelControler();

	virtual void Update() override;
	void Rotate();
	void Translate();
};