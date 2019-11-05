#pragma once


struct GameValues
{
	class Camera*		Camera;
	class Perspective*	Perspective;
	class Viewport*		Viewport;
};


class GameNode
{
public:
	GameNode(GameValues * values)
		: values(values)
	{

	}

	virtual ~GameNode() {}

	virtual void Initialize() = 0;
	virtual void Update() = 0;

	virtual void PreRender() = 0;		//	��ó�� ����
	virtual void Render() = 0;			//	�Ϲ����� ����
	virtual void PostRender() = 0;		//	��� ������ �Ŀ� ������(UI)

	virtual void ResizeScreen() = 0;
protected:
	GameValues * values;
};