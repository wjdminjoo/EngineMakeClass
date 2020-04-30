
#include "Precompiled.h"
#include "SoftRenderer.h"

// 그리드 그리기
void SoftRenderer::DrawGrid2D()
{
	// 그리드 색상
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// 가로 세로 라인 그리기
	ScreenPoint screenHalfSize = _ScreenSize.GetHalf();

	for (int x = screenHalfSize._X; x <= _ScreenSize._X; x += _Grid2DUnit)
	{
		_RSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize._X)
		{
			_RSI->DrawFullVerticalLine(2 * screenHalfSize._X - x, gridColor);
		}
	}

	for (int y = screenHalfSize._Y; y <= _ScreenSize._Y; y += _Grid2DUnit)
	{
		_RSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize._Y)
		{
			_RSI->DrawFullHorizontalLine(2 * screenHalfSize._Y - y, gridColor);
		}
	}

	// 월드 축 그리기
	_RSI->DrawFullHorizontalLine(screenHalfSize._Y, LinearColor::Red);
	_RSI->DrawFullVerticalLine(screenHalfSize._X, LinearColor::Green);
}



// 게임 로직
void SoftRenderer::Update2D(float InDeltaSeconds)
{

	// 매트릭스 사용 금지

	static float time = 0.0f;
	static bool isTime = false;
	if (isTime) {
		time -= InDeltaSeconds;
		
	}
	else if(!isTime) {
		time += InDeltaSeconds;
	}

	if (time > 2) {
		isTime = true;
	}else if (time < -1) {
		isTime = false;
	}
	
	
	if (!isTime) {
		_transform.AddPosition(Vector2(time, time * 0.5f));
	}
	else {
		_transform.AddPosition(Vector2(-time, -time * 0.5f));
	}

	_CurrentPosition = _transform.GetPosition();
	_CurrentColor = isTime ? LinearColor::Red : LinearColor::Blue;
}

// 렌더링 로직
void SoftRenderer::Render2D()
{

	// 매트릭스는 렌더러에서만 사용
	// 격자 그리기
	DrawGrid2D();

	static Vector2 originVector(100.f, 0.f);

	// 10도 간격으로 36개의 점을 그린다.
	for (int i = 0; i < 36; i++)
	{
		float rotateRadian = Math::Deg2Rad(i * 10.f);
		float sin = sinf(rotateRadian);
		float cos = cosf(rotateRadian);
		Matrix3x3 rotMat(Vector3(cos, sin, 0.f), Vector3(-sin, cos, 0.f), Vector3(0.f, 0.f, 1.f));
		Vector2 newPosition = rotMat * _transform.GetModelingMatrix() * _CurrentPosition;

		// 지정된 위치에 지정한 색상으로 점 찍기
		_RSI->DrawPoint(newPosition, _CurrentColor);
		_RSI->DrawPoint(newPosition + Vector2::UnitX, _CurrentColor);
		_RSI->DrawPoint(newPosition - Vector2::UnitX, _CurrentColor);
		_RSI->DrawPoint(newPosition + Vector2::UnitY, _CurrentColor);
		_RSI->DrawPoint(newPosition - Vector2::UnitY, _CurrentColor);
	}
}

