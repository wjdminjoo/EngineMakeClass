
#include "Precompiled.h"
#include "Windows/WindowsRSI.h"
#include "Windows/WindowsGDI.h"

WindowsRSI::~WindowsRSI()
{
}

bool WindowsRSI::Init(const ScreenPoint& InScreenSize)
{
	return InitializeGDI(InScreenSize);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	FillBuffer(InClearColor.ToColor32());
	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{
}

void WindowsRSI::EndFrame()
{
	SwapBuffer();
}

void WindowsRSI::DrawFullVerticalLine(int InX, const LinearColor & InColor)
{
	if (InX < 0 || InX >= _ScreenSize.X)
	{
		return;
	}

	for (int y = 0; y < _ScreenSize.Y; ++y)
	{
		SetPixel(ScreenPoint(InX, y), InColor);
	}
}

void WindowsRSI::DrawFullHorizontalLine(int InY, const LinearColor & InColor)
{
	if (InY < 0 || InY >= _ScreenSize.Y)
	{
		return;
	}

	for (int x = 0; x < _ScreenSize.X; ++x)
	{
		SetPixel(ScreenPoint(x, InY), InColor);
	}
}

void WindowsRSI::DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor)
{
	SetPixel(ScreenPoint::ToScreenCoordinate(_ScreenSize, InVectorPos), InColor);
}

void WindowsRSI::DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor)
{
	Vector2 clippedStart = InStartPos;
	Vector2 clippedEnd = InEndPos;
	Vector2 screenExtend = Vector2(_ScreenSize.X, _ScreenSize.Y) * 0.5f;
	Vector2 minScreen = -screenExtend;
	Vector2 maxScreen = screenExtend;
	if (!CohenSutherlandLineClip(clippedStart, clippedEnd, minScreen, maxScreen))
	{
		return;
	}

	ScreenPoint startPosition = ScreenPoint::ToScreenCoordinate(_ScreenSize, clippedStart);
	ScreenPoint endPosition = ScreenPoint::ToScreenCoordinate(_ScreenSize, clippedEnd);


	int width = endPosition.X - startPosition.X;
	int height = endPosition.Y - startPosition.Y;

	bool isGradualSlope = (Math::Abs(width) >= Math::Abs(height));
	int dx = (width >= 0) ? 1 : -1;
	int dy = (height > 0) ? 1 : -1;
	int fw = dx * width;
	int fh = dy * height;

	int f = isGradualSlope ? fh * 2 - fw : 2 * fw - fh;
	int f1 = isGradualSlope ? 2 * fh : 2 * fw;
	int f2 = isGradualSlope ? 2 * (fh - fw) : 2 * (fw - fh);
	int x = startPosition.X;
	int y = startPosition.Y;

	if (isGradualSlope)
	{
		while (x != endPosition.X)
		{
			SetPixel(ScreenPoint(x, y), InColor);

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				y += dy;
			}

			x += dx;
		}
	}
	else
	{
		while (y != endPosition.Y)
		{
			SetPixel(ScreenPoint(x, y), InColor);

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				x += dx;
			}

			y += dy;
		}
	}

	
}

void WindowsRSI::PushStatisticText(std::string && InText)
{
	_StatisticTexts.emplace_back(InText);
}

void WindowsRSI::PushStatisticTexts(std::vector<std::string> && InTexts)
{
	std::move(InTexts.begin(), InTexts.end(), std::back_inserter(_StatisticTexts));
}

const int INSIDE = 0;    // 0000
const int LEFT = 1;    // 0001
const int RIGHT = 2;    // 0010
const int BOTTOM = 4;    // 0100
const int UP = 8;    // 1000

int WindowsRSI::TestRegion(const Vector2& InVectorPos, const Vector2& InMinPos, const Vector2& InMaxPos)
{
	int result = INSIDE;

	if (InVectorPos.X < InMinPos.X) result = LEFT;
	else if (InVectorPos.X > InMaxPos.X) result = RIGHT;

	if (InVectorPos.Y < InMinPos.Y) result = BOTTOM;
	else if (InVectorPos.Y > InMaxPos.Y) result = UP;

	return result;
}


 
bool WindowsRSI::CohenSutherlandLineClip(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos)
{

	int testStart = TestRegion(InOutStartPos, InMinPos, InMaxPos);
	int testEnd = TestRegion(InOutEndPos, InMinPos, InMaxPos);

	float width = (InOutEndPos.X - InOutStartPos.X);
	float height = (InOutEndPos.Y - InOutStartPos.Y);

	while (true) {
		// 두점이 내부에 있다는 것으로 판단.
		if (testStart == 0 && testEnd == 0) {
			return true;
		}
		// 두 점이 외부에 있다는 판단.
		else if (testStart & testEnd) {
			return false;
		}
		// 클리핑 영역 파악후 개선및 저장
		else {
			Vector2 clippedPos = Vector2::Zero;

			int current = 0;

			if (testStart == 0) current = testEnd;
			else current = testStart;
			

			if (current < BOTTOM) {
				if (current & LEFT) clippedPos.X = InMinPos.X;
				else clippedPos.X = InMaxPos.X;

				if (height == INSIDE) clippedPos.Y = InOutStartPos.Y;
				else clippedPos.Y = InOutStartPos.Y + height * (clippedPos.X - InOutStartPos.X) / width;

			}
			else {
				if (current & BOTTOM) clippedPos.Y = InMinPos.Y;
				else clippedPos.Y = InMaxPos.Y;

				if (width == INSIDE)	clippedPos.X = InOutStartPos.X;
				else clippedPos.X = InOutStartPos.X + width * (clippedPos.Y - InOutStartPos.Y) / height;
			}

			if (current == testStart) {
				InOutStartPos = clippedPos;
				testStart = TestRegion(InOutStartPos, InMinPos, InMaxPos);
			}
			else {
				InOutEndPos = clippedPos;
				testEnd = TestRegion(InOutEndPos, InMinPos, InMaxPos);
			}
		}
	}
	
	return true;
}
