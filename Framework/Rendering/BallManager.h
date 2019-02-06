#pragma once

class BallManager
{
public:
	BallManager(class Context *context, const int &Level, const Vector3 &position, const Vector3 &moveDir);
	virtual ~BallManager();

	const int &GetNumber() const { return number; }

	void SetPosition(const Vector3 &position) { this->position = position; }
	void SetLevel(const int &Level) { this->Level = Level; }
	void SetNumber(const int &number) { this->number = number; }
	
	void Split(const int &index);

private:
	class Context *context;

	map<int, class Ball*> Balls;
	map<int, class Ball*> tempBalls;
	class Ball* pBall;
	class Ball* cBall_A;
	class Ball* cBall_B;

	Vector3 position;
	Vector3 moveDir;

	int Level;
	int number;
	float jumpSpeed[4] = { 0.0f, 4.0f, 8.0f, 14.0f };
	float floorSpeed[4] = { 28.0f, 29.0f, 30.0f, 32.0f };
};