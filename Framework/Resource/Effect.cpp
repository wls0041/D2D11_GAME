#include "stdafx.h"
#include "Effect.h"
#include "../Scene/Component/Animator.h"
#include "../Scene/Component/Transform.h"

Effect::Effect(Context * context) : IResource(context), texture(nullptr), bExist(false)
{
	GeometryUtility::CreateQuad(geometry);

	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create<VertexTexture>(geometry.GetVertices());

	indexBuffer = new IndexBuffer(context);
	indexBuffer->Create(geometry.GetIndices());

	vertexShader = new VertexShader(context);
	vertexShader->Create("../_Assets/Shader/Animation.hlsl");

	pixelShader = new PixelShader(context);
	pixelShader->Create("../_Assets/Shader/Animation.hlsl");

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
	SAFE_DELETE(transform);
	SAFE_DELETE(animator);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(spriteBuffer);
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(pixelShader);
	SAFE_DELETE(vertexShader);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void Effect::SaveToFile(const string & filePath)
{
}

void Effect::LoadFromFile(const string & filePath)
{
}

void Effect::SetPosition(const Vector3 & position)
{
	transform->SetPosition(position);
}

void Effect::SetScale(const Vector3 & scale)
{
	transform->SetScale(scale);
}

void Effect::SetTexture(const string & filePath)
{
	texture = resourceMgr->Load<Texture>(filePath);
}

void Effect::SetAnimation(const string & filePath)
{
	animator->RegisterAnimation(filePath);
	animator->SetCurrentAnimation("Idle");
	animator->Play();
}

void Effect::Update()
{
	if (bExist)
	{
		animator->Update();

		auto keyframe = animator->GetCurrentkeyframe();

		auto spriteData = static_cast<AnimationData*>(spriteBuffer->Map());
		spriteData->SpriteOffset = keyframe->offset;
		spriteData->SpriteSize = keyframe->size;
		spriteData->TextureSize = texture ? texture->GetSize() : Vector2(1.0f);
		spriteBuffer->Unmap();

		auto worldData = static_cast<WorldData*>(worldBuffer->Map());
		worldData->World = transform->GetWorldMatrix();
		worldBuffer->Unmap();

		//반복적이지 않기에 끝나면 이펙트 없애주려면 
		if (!animator->IsPlay())
			bExist = false;

	}
}

void Effect::Render()
{
	if (bExist)
	{
		auto graphics = context->GetSubsystem<Graphics>();

		vertexBuffer->BindPipeline();
		indexBuffer->BindPipeline();
		vertexBuffer->BindPipeline();
		pixelShader->BindPipeline();
		inputLayout->BindPipeline();
		worldBuffer->BindPipeline(ShaderType::VS, 1);
		spriteBuffer->BindPipeline(ShaderType::VS, 2);
		texture->BindPipeline(ShaderType::PS, 0);

		graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		graphics->GetDeviceContext()->DrawIndexed(geometry.GetIndexCount(), 0, 0);
	}
}