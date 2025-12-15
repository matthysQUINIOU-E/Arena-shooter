#pragma once

class ScoreManager
{
	static int m_amount;

public:
	static int GetScore() { return m_amount; }
	static void Reset() { m_amount = 0; }
	static void Add(float val) { m_amount += (int)val; }
	static void Multiply(float val) { m_amount *= val; }
};

