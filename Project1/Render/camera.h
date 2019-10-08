#pragma once

class CAMERA
{
public:
	CAMERA();
	~CAMERA();

	void Initialize();
	void Render();
	void GetViewMatrix(D3DXMATRIX&);
	
	void SetPosition(float x, float y, float z) { position.x = x; position.y = y; position.z = z; }
	void SetPosition(D3DXVECTOR3 pos) { position = pos; }
	void SetRotation(float x, float y, float z) { rotation.x = x; rotation.y = y; rotation.z = z; }
	void SetRotation(D3DXVECTOR3 rot) { rotation = rot; }

	D3DXVECTOR3 GetPosition() { return position; }
	D3DXVECTOR3 GetRotation() { return rotation; }


private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;

	D3DXMATRIX viewMatrix;
};

