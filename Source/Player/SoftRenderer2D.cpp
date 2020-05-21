#include "Precompiled.h"
#include "SoftRenderer.h"

// 그리드 그리기
void SoftRenderer::DrawGrid2D()
{
	// 그리드 색상
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// 뷰의 영역 계산
	Vector2 viewPos = _GameEngine.GetCamera()->GetTransform().GetPosition();
	Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

	// 좌측 하단에서부터 격자 그리기
	int xGridCount = _ScreenSize.X / _Grid2DUnit;
	int yGridCount = _ScreenSize.Y / _Grid2DUnit;

	// 그리드가 시작되는 좌하단 좌표 값 계산
	Vector2 minPos = viewPos - extent;
	Vector2 minGridPos = Vector2(ceilf(minPos.X / (float)_Grid2DUnit), ceilf(minPos.Y / (float)_Grid2DUnit)) * (float)_Grid2DUnit;
	ScreenPoint gridBottomLeft = ScreenPoint::ToScreenCoordinate(_ScreenSize, minGridPos - viewPos);

	for (int ix = 0; ix < xGridCount; ++ix)
	{
		_RSI->DrawFullVerticalLine(gridBottomLeft.X + ix * _Grid2DUnit, gridColor);
	}

	for (int iy = 0; iy < yGridCount; ++iy)
	{
		_RSI->DrawFullHorizontalLine(gridBottomLeft.Y - iy * _Grid2DUnit, gridColor);
	}

	// 월드의 원점
	ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(_ScreenSize, -viewPos);
	_RSI->DrawFullHorizontalLine(worldOrigin.Y, LinearColor::Red);
	_RSI->DrawFullVerticalLine(worldOrigin.X, LinearColor::Green);
}


// 게임 로직
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	InputManager input = _GameEngine.GetInputManager();
	Transform2D& cameraTransform = _GameEngine.GetCamera()->GetTransform();
	Transform2D& playerTransform = _GameEngine.GameObjectFinder("Player")->GetTransform();
	Vector2 deltaPosition = Vector2(input.GetXAxis(), input.GetYAxis()) * _MoveSpeed * InDeltaSeconds;
	
	cameraTransform.AddPosition(deltaPosition);
	playerTransform.SetPosition(cameraTransform.GetPosition());
	_CurrentColor = input.SpacePressed() ? LinearColor::Red : LinearColor::Blue;
}

void SoftRenderer::Render2D()
{
	// 격자 그리기
	DrawGrid2D();

	////////////////////// 월드 공간 //////////////////////
	Matrix3x3 viewMatrix = _GameEngine.GetCamera()->GetViewMatrix();
	Matrix3x3 finalMat;
	auto& object = _GameEngine.GetObject();

	for (int i = 0; i < object.size(); i++) {
		Transform2D& objectTransform = object[i]->GetTransform();

		const Mesh* objectMesh = object[i]->GetMesh();

		size_t vertexCount = objectMesh->_Vertices.size();
		size_t indexCount = objectMesh->_Indices.size();
		size_t triangleCount = indexCount / 3;

		// 렌더러가 사용할 정점 버퍼와 인덱스 버퍼 생성
		Vector2* vertices = new Vector2[vertexCount];
		std::memcpy(vertices, &objectMesh->_Vertices[0], sizeof(Vector2) * vertexCount);
		int* indices = new int[indexCount];
		std::memcpy(indices, &objectMesh->_Indices[0], sizeof(int) * indexCount);

		finalMat = viewMatrix * objectTransform.GetModelingMatrix();

		for (int vi = 0; vi < vertexCount; ++vi)
		{
			vertices[vi] = finalMat * vertices[vi];
		}

		// 변환된 정점을 잇는 선 그리기
		for (int ti = 0; ti < triangleCount; ++ti)
		{
			int bi = ti * 3;
			_RSI->DrawLine(vertices[indices[bi]], vertices[indices[bi + 1]], _CurrentColor);
			_RSI->DrawLine(vertices[indices[bi]], vertices[indices[bi + 2]], _CurrentColor);
			_RSI->DrawLine(vertices[indices[bi + 1]], vertices[indices[bi + 2]], _CurrentColor);
		}
		delete[] vertices;
		delete[] indices;
	}
}

