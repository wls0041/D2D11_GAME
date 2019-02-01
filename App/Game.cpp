#include "stdafx.h"
#include "Game.h"
#include "./Framework/Rendering/Rect.h"
#include "./Framework/Scene/Component/Camera.h"

void Game::Initialize()
{
	timer = context->GetSubsystem<Timer>();
	input = context->GetSubsystem<Input>();
	audio = context->GetSubsystem<Audio>();
	sceneMgr = context->GetSubsystem<SceneManager>();
	effectMgr = context->GetSubsystem<EffectManager>();

	//RTT
	camera = new Camera(context);

	renderTexture = new RenderTexture(context);
	renderTexture->Create(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));

	GeometryUtility::CreateScreenQuad(geometry);

	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create<VertexTexture>(geometry.GetVertices());

	indexBuffer = new IndexBuffer(context);
	indexBuffer->Create(geometry.GetIndices());

	vertexShader = new VertexShader(context);
	vertexShader->Create("../_Assets/Shader/GrayScale.hlsl");

	pixelShader = new PixelShader(context);
	pixelShader->Create("../_Assets/Shader/GrayScale.hlsl");

	inputLayout = new InputLayout(context);
	inputLayout->Create(vertexShader->GetBlob());

	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	worldBuffer = new ConstantBuffer(context);
	worldBuffer->Create<WorldData>();
}

void Game::Update()
{
	timer->Update();
	input->Update();
	audio->Update();
	effectMgr->Update();
	sceneMgr->Update();
	
	renderTexture->SetTarget();
	renderTexture->ClearTarget();

	effectMgr->Render();
	sceneMgr->Render();

	if (input->KeyDown(VK_SPACE)) renderTexture->Save("Test.png");
}

void Game::Render()
{
	camera->Update();

	auto cameraData = static_cast<CameraData*>(cameraBuffer->Map());
	cameraData->View = camera->GetViewMatrix();
	cameraData->Projection = camera->GetProjectionMatrix();
	cameraBuffer->Unmap();

	auto worldData = static_cast<WorldData*>(worldBuffer->Map());
	worldData->World = Matrix::Identity;
	worldBuffer->Unmap();

	auto graphics = context->GetSubsystem<Graphics>();

	vertexBuffer->BindPipeline();
	indexBuffer->BindPipeline();
	vertexShader->BindPipeline();
	pixelShader->BindPipeline();
	inputLayout->BindPipeline();
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	worldBuffer->BindPipeline(ShaderType::VS, 1);

	auto srv = renderTexture->GetShaderResourceView();
	graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphics->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
	graphics->GetDeviceContext()->DrawIndexed(geometry.GetIndexCount(), 0, 0);
}

void Game::Destroy()
{
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(cameraBuffer);
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(pixelShader);
	SAFE_DELETE(vertexShader);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(renderTexture);
	SAFE_DELETE(camera);
}
