#pragma once

class BallManager
{
public:
	BallManager(class Context *context);
	virtual ~BallManager();

	const int &GetNumber() const { return number; }
	const bool &IsClear();

	void SetPosition(const Vector3 &position) { this->position = position; }
	void SetMoveDir(const Vector3 &moveDir) { this->moveDir = moveDir; }
	void SetLevel(const int &Level) { this->Level = Level; }
	void SetNumber(const int &number) { this->number = number * 100; }
	void SetFirstBall();
	
	void Split(const int &index);
	void Update();
	void Render();

private:
	class Context *context;
	map<int, class Ball*> Balls;

	Vector3 position;
	Vector3 moveDir;
	bool bCheat;

	int Level;
	int number;
	float floorSpeed[4] = { 28.0f, 29.0f, 30.0f, 32.0f };
};