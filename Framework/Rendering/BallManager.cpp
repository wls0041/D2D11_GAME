#include "stdafx.h"
#include "BallManager.h"
#include "../Scene/Component/Transform.h"
#include "../Scene/Component/Collider.h"
#include "Ball.h"

BallManager::BallManager(Context * context, const int &Level, const Vector3 &position, const Vector3 &moveDir) : context(context), Level(Level), position(position), moveDir(moveDir), number(0)
{
	pBall = new Ball(context);
	pBall->GetTransform()->SetScale({ 40.0f * Level, 40.0f * Level, 1 });
	pBall->GetTransform()->SetPosition({ position.x, position.y, 0 });
	pBall->SetMoveDir(moveDir);
	pBall->SetNumber(number);
	pBall->SetCollider();
	Balls.insert(make_pair(number, pBall));
	
	number++;

}

BallManager::~BallManager()
{
	//if (Level != 1) {
	//	SAFE_DELETE(cBall_B);
	//	SAFE_DELETE(cBall_A);
	//}
	SAFE_DELETE(pBall);
}

void BallManager::Split(const int &index)
{
	auto ball = Balls.find(index);

	Vector3 pScale = ball->second->GetTransform()->GetScale();

	Ball *cBall_A = new Ball(context);
	cBall_B->GetTransform()->SetScale({ pScale.x * 0.5f, pScale.y * 0.5f , 1 });
	cBall_B->GetTransform()->SetPosition({ position.x - pScale.x * 0.25f, position.y, 0 });
	cBall_B->SetMoveDir({ -moveDir.x, moveDir.y, moveDir.z });
	cBall_B->SetCollider();

	Ball *cBall_B = new Ball(context);
	cBall_B->GetTransform()->SetScale({ pScale.x * 0.5f, pScale.y * 0.5f , 1 });
	cBall_B->GetTransform()->SetPosition({ position.x - pScale.x * 0.25f, position.y, 0 });
	cBall_B->SetMoveDir({ -moveDir.x, moveDir.y, moveDir.z });
	cBall_B->SetCollider();

}
