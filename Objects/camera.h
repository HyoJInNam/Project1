#pragma once

#include <Utility.h>
#include <input.h>
#include <timeclass.h>
#include "objectstdafx.h"

class CAMERA : public Transform<CAMERA>
{
protected:
	D3DXVECTOR3 lookAt;  // target

	D3DXVECTOR3 forward;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;

	D3DXMATRIX rotationMatrix;
	D3DXMATRIX viewMatrix;


public:
	CAMERA(const char* name);
	CAMERA(const CAMERA&);
	virtual ~CAMERA();

	void Initialize();
	virtual void Update() = 0;
	void Render();
	
	void SetPosition(float x, float y, float z) { position.x = x; position.y = y; position.z = z; }
	void SetPosition(D3DXVECTOR3 pos) { position = pos; }
	void SetRotation(float x, float y, float z) { rotation.x = x; rotation.y = y; rotation.z = z; }
	void SetRotation(D3DXVECTOR3 rot) { rotation = rot; }


	D3DXVECTOR3 GetPosition() { return position; }
	D3DXVECTOR3 GetRotation() { return rotation; }
	void GetViewMatrix(D3DXMATRIX & viewMatrix) { viewMatrix = this->viewMatrix; }
	void Forward(D3DXVECTOR3* val) { *val = forward; }
	void Right(D3DXVECTOR3* val) { *val = right; }
	void Up(D3DXVECTOR3* val) { *val = up; }

private:
	void Rotation();
	void View();


};

