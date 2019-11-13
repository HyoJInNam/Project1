#include "../Utility/renafx.h"
#include "./camera.h"
#include "../inputController/Mouse.h"
#include "../inputController/Keyboard.h"
#include "cameraControler.h"

CameraControl::CameraControl(float moveSpeed, float rotSpeed)
	: CAMERA()
	, moveSpeed(moveSpeed), rotSpeed(rotSpeed)
{}

CameraControl::~CameraControl()
{
}

void CameraControl::Update()
{
	Rotate();
	Translate();
}

void CameraControl::Rotate()
{
	D3DXVECTOR3 rot = GetRotation();

	if (Mouse::GetInstance()->Press(1))
	{
		D3DXVECTOR3 val = Mouse::GetInstance()->GetMoveValue();

		rotation.x += val.y * rotSpeed * Time::Delta();
		rotation.y += val.x * rotSpeed * Time::Delta();
		rotation.z += val.z * rotSpeed * Time::Delta();

		SetRotation(rotation.x, rotation.y, rotation.z);
	}
}

void CameraControl::Translate()
{
	D3DXVECTOR3 forward, right, up;
	Forward(&forward);
	Right(&right);
	Up(&up);

	D3DXVECTOR3 position = GetPosition();

	{
		if (Keyboard::GetInstance()->Press('W'))
			position += forward * moveSpeed * Time::Delta();
		else if (Keyboard::GetInstance()->Press('S'))
			position += -forward * moveSpeed * Time::Delta();
		if (Keyboard::GetInstance()->Press('A'))
			position += -right * moveSpeed * Time::Delta();
		else if (Keyboard::GetInstance()->Press('D'))
			position += right * moveSpeed * Time::Delta();
		if (Keyboard::GetInstance()->Press('E'))
			position += up * moveSpeed * Time::Delta();
		else if (Keyboard::GetInstance()->Press('Q'))
			position += -up * moveSpeed * Time::Delta();

		SetPosition(position.x, position.y, position.z);
	}
}