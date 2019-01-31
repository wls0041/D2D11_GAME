#include "stdafx.h"
#include "Effect.h"
#include "../Scene/Component/Animator.h"
#include "../Scene/Component/Transform.h"

Effect::Effect(Context * context) : IResource(context), bExist(false)
{
	GeometryUtility::CreateQuad(geometry);
	
	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create<VertexTexture>(geometry.GetVertices());

	indexBuffer = new IndexBuffer(context);
	indexBuffer->Create(geometry.GetIndices());

	vertexShader = new VertexShader(context);
	vertexShader->Create("Animation.hlsl");

	pixelShader = new PixelShader(context);
	pixelShader->Create("Animation.hlsl");

	inputLayout = new InputLayout(context);
	inputLayout->Create(vertexShader->GetBlob());

	spriteBuffer = new ConstantBuffer(context);
	spriteBuffer->Create<AnimationData>();

	worldBuffer = new ConstantBuffer(context);
	worldBuffer->Create<WorldData>();

	animator = new Animator(context);
	transform = new Transform(context);
}

Effect::~Effect()
{
}

void Effect::SaveToFile(const string & filePath)
{
}

void Effect::LoadFromFile(const string & filePath)
{
}

void Effect::SetIsExist(const bool & bExist)
{
}

void Effect::SetPosition(const Vector3 & position)
{
}

void Effect::SetScale(const Vector3 & scale)
{
}

void Effect::SetTexture(const string & filePath)
{
}

void Effect::SetAnimation(const string & filePath)
{
}

void Effect::Update()
{
}

void Effect::Render()
{
}
