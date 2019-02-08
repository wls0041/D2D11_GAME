#include "stdafx.h"
#include "BallManager.h"
#include "../Scene/Component/Transform.h"
#include "Ball.h"

BallManager::BallManager(Context * context) : context(context), Level(0), position(0.0f), moveDir(0.0f), bCheat(false), number(0)
{
}

BallManager::~BallManager()
{
	for (auto ball : Balls) SAFE_DELETE(ball.second);
}

const bool & BallManager::IsClear()
{
	int i = 0;
	for (auto ball : Balls) i++;

	return i == 0 ? true : false;
}

void BallManager::SetFirstBall()
{
	Balls[number] = new Ball(context);
	Balls[number]->GetTransform()->SetScale({ 40.0f * Level, 40.0f * Level, 1 });
	Balls[number]->GetTransform()->SetPosition(position);
	Balls[number]->SetFloorSpeed(floorSpeed[Level - 1]);
	Balls[number]->SetNumber(number);
	Balls[number]->SetLevel(Level);
	Balls[number]->SetCollider();
	number++;
}

void BallManager::Split(const int &index)
{
	auto ball = Balls.find(index);

	Vector3 pScale = ball->second->GetTransform()->GetScale();
	Vector3 pPosition = ball->second->GetTransform()->GetPosition();
	Vector3 jumpSpeed = ball->second->GetJumpSpeed();
	int level = ball->second->GetLevel() - 1;

	Balls[number] = new Ball(context);
	Balls[number]->GetTransform()->SetScale({ pScale.x * 0.5f, pScale.y * 0.5f , 1 });
	Balls[number]->GetTransform()->SetPosition({ pPosition.x - pScale.x * 0.5f, pPosition.y, 0 });
	Balls[number]->SetJumpSpeed({-jumpSpeed.x * Math::Sign(jumpSpeed.x), -jumpSpeed.y * Math::Sign(jumpSpeed.y), jumpSpeed.z});
	Balls[number]->SetFloorSpeed(floorSpeed[level - 1]);
	Balls[number]->SetNumber(number);
	Balls[number]->SetLevel(level);
	Balls[number]->SetCollider();
	number++;

	Balls[number] = new Ball(context);
	Balls[number]->GetTransform()->SetScale({ pScale.x * 0.5f, pScale.y * 0.5f , 1 });
	Balls[number]->GetTransform()->SetPosition({ pPosition.x + pScale.x * 0.5f, pPosition.y, 0 });
	Balls[number]->SetFloorSpeed(floorSpeed[level - 1]);
	Balls[number]->SetJumpSpeed({ jumpSpeed.x * Math::Sign(jumpSpeed.x), -jumpSpeed.y * Math::Sign(jumpSpeed.y), jumpSpeed.z });
	Balls[number]->SetNumber(number);
	Balls[number]->SetLevel(level);
	Balls[number]->SetCollider();
	number++;

	//for (auto iter = effects.begin(); iter != effects.end();) { //resourceManager등록 안했다면. 사용 안하면 erase전에 delete해야함
	//	auto effect = *iter;
	//	effect->Update();
	//
	//	if (!effect->IsExist()) {
	//		SAFE_DELETE(effect);
	//		iter = effects.erase(iter); //iter -> 지우고 그 자리를 반환
	//	}
	//	else iter++;
	//} //동적할당이 빈번하면 빈공간이 생기고 그 공간을 활용하지 못하는 병목현상이 생길 수 있음
	////따라서 미리 데이터공간을 할당하는 데이터풀링이 필요함
	//
	//int i = index;
	//SAFE_DELETE(Balls[i]); 
	Balls.erase(index);
}

void BallManager::Update()
{
	vector<Ball*> results;
	auto input = context->GetSubsystem<Input>();

	for (auto &ball : Balls) {
		ball.second->Update();

		auto colliderMgr = context->GetSubsystem<ColliderManager>();
		colliderMgr->HitCheck_AABB_Bullet("Bullet", to_string(ball.second->GetNumber()), CheckCase::Rect_Circle); //부딪히면 양쪽의 event 모두 실행
		colliderMgr->HitCheck_AABB_Circle("Back", to_string(ball.second->GetNumber()), CheckCase::Back_Circle); //Ball이 배경 밖으로 나가는가
		colliderMgr->HitCheck_AABB_Circle("Block", to_string(ball.second->GetNumber()), CheckCase::Circle_Rect); //Ball Block충돌
		
		if (input->KeyDown('R')) bCheat = !bCheat;
		if(!bCheat) colliderMgr->HitCheck_AABB_Player(to_string(ball.second->GetNumber()), "Player", ball.second->GetTransform()->GetPosition().x); //Ball Player충돌

		if (ball.second->GetCollResult()) results.push_back(ball.second);
	}
	for (auto &result : results) {
		if (result->GetLevel() != 1) Split(result->GetNumber());
		else Balls.erase(result->GetNumber());
	}

	results.clear();
	results.shrink_to_fit();
}

void BallManager::Render()
{
	for (auto &ball : Balls) {
		ball.second->Render();
	}

	if (bCheat) {
		auto dw = context->GetSubsystem<DirectWrite>();
		dw->Text_Middle(L"<< CHEAT_ON >>", Vector2(650, 780), 50.0f, Color(1, 1, 1, 1));
	}
}
