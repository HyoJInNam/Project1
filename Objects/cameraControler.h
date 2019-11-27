#pragma once

class CAMERA;

class CameraControl :public CAMERA
{
public:
	CameraControl(const char* name, float moveSpeed = 15.0f, float rotSpeed = 6.5f);
	CameraControl(const CameraControl& other);
    ~CameraControl();

	virtual void Update() override;
	void Rotate();
	void Translate();

private:
	float moveSpeed;
	float rotSpeed;
};