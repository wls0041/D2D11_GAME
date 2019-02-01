#pragma once
#include "App.h"

//pipeline����
class Game final : public App
{
//�θ𿡼� Run���� �������� �ڵ带 ��� ���� �ʿ��� �͸� ���ָ� �˾Ƽ� ��
public:
	Game(){}
	~Game(){}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Destroy() override;

private:
	class Timer *timer;
	class Input *input;
	class Audio *audio;
	class SceneManager *sceneMgr;
	class EffectManager *effectMgr;

	//RTT
	class Camera *camera;
	class RenderTexture *renderTexture;

	class Geometry<VertexTexture> geometry;
	class VertexBuffer *vertexBuffer;
	class IndexBuffer *indexBuffer;
	class InputLayout *inputLayout;
	class VertexShader *vertexShader;
	class PixelShader *pixelShader;
	class Texture *texture;
	class ConstantBuffer *cameraBuffer;
	class ConstantBuffer *worldBuffer;
};