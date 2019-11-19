#include "../../Utility/stdafx.h"
#include "../../Utility/renafx.h"
#include "camera.h"



CAMERA::CAMERA()
	: position(0.0f, 0.0f, -10.0f), rotation(0.0f, 0.0f, 0.0f), lookAt(0.0f, 0.0f, 1.0f)
	, forward(0.0f, 0.0f, 0.0f), right(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f)
{
	Rotation(); View();
}


CAMERA::~CAMERA() {}
void CAMERA::Initialize() {}

void CAMERA::Render()
{
	Rotation();
	View();
	return;

	float yaw, pitch, roll;
	
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);

}

void CAMERA::Rotation()
{
	D3DXMATRIX X, Y, Z;
	D3DXMatrixRotationX(&X, rotation.x);
	D3DXMatrixRotationY(&Y, rotation.y);
	D3DXMatrixRotationZ(&Z, rotation.z);

	rotationMatrix = X * Y * Z;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y * 0.0174532925f, rotation.x * 0.0174532925f, rotation.z * 0.0174532925f);
	D3DXVec3TransformNormal(&forward, &D3DXVECTOR3(0, 0, 1), &rotationMatrix);
	D3DXVec3TransformNormal(&right, &D3DXVECTOR3(1, 0, 0), &rotationMatrix);
	D3DXVec3TransformNormal(&up, &D3DXVECTOR3(0, 1, 0), &rotationMatrix);
	return;
}

void CAMERA::View()
{
	D3DXMatrixLookAtLH(&viewMatrix, &position, &(position + forward), &up);
}