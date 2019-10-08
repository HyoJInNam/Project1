#include "../stdafx.h"
#include "camera.h"



CAMERA::CAMERA()
	: position(0.0f, 0.0f, 0.0f)
	, rotation(0.0f, 0.0f, 0.0f)
{
	//cameraRender = new R_TRANSFORMATION;
	//cameraRender->Initialize();
}

CAMERA::CAMERA(const CAMERA& other) {}
CAMERA::~CAMERA() {}

void CAMERA::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = this->position.x;
	position.y = this->position.y;
	position.z = this->position.z;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&matrix.view, &position, &lookAt, &up);
}

void CAMERA::GetViewMatrix(D3DXMATRIX & viewMatrix)
{
	viewMatrix = matrix.view;
	return;
}

