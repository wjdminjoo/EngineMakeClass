#include "Precompiled.h"
#include "SoftRenderer.h"

// 그리드 그리기
void SoftRenderer::DrawGrid3D()
{
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// 뷰의 영역 계산
	Vector2 viewPos = _GameEngine2D.GetCamera()->GetTransform().GetLocalPosition();
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
void SoftRenderer::Update3D(float InDeltaSeconds)
{
	InputManager input = _GameEngine3D.GetInputManager();
	Transform3D& playerTransform = _GameEngine3D.GameObjectFinder("Player")->GetTransform();
	Transform3D& cameraTransform = _GameEngine3D.GetCamera()->GetTransform();

	float deltaPosition = input.GetXAxis() * _MoveSpeed * InDeltaSeconds;
	playerTransform.AddPitchRoation(100.0f * InDeltaSeconds);
	playerTransform.AddYawRoation(100.0f * InDeltaSeconds);
	playerTransform.AddRollRoation(100.0f * InDeltaSeconds);


	_CurrentColor = input.SpacePressed() ? LinearColor::Red : LinearColor::Blue;
}

void SoftRenderer::Render3D()
{
	
	DrawGrid3D();
	auto& object = _GameEngine3D.GetObject();
	Matrix4x4 viewMat = _GameEngine3D.GetCamera()->GetViewMatrix();
	for (int i = 0; i < object.size(); i++)
	{
		const Mesh3D* mesh = object[0]->GetMesh();
		Transform3D& transform = object[0]->GetTransform();

		Matrix4x4 finalMat = viewMat * transform.GetModelingMatrix();

		size_t vertexCount = mesh->_Vertices.size();
		size_t indexCount = mesh->_Indices.size();
		size_t triangleCount = indexCount / 3;

		// 렌더러가 사용할 정점 버퍼와 인덱스 버퍼 생성
		Vector4* vertices = new Vector4[vertexCount];
		for (int vi = 0; vi < vertexCount; ++vi)
		{
			vertices[vi] = Vector4(mesh->_Vertices[vi], true);
		}

		int* indices = new int[indexCount];
		std::memcpy(indices, mesh->_Indices.data(), sizeof(int) * indexCount);

		// 각 정점에 행렬을 적용
		for (int vi = 0; vi < vertexCount; ++vi)
		{
			vertices[vi] = finalMat * vertices[vi];
		}

		// 변환된 정점을 잇는 선 그리기
		for (int ti = 0; ti < triangleCount; ++ti)
		{
			int bi = ti * 3;
			_RSI->DrawLine(vertices[indices[bi]].ToVector2(), vertices[indices[bi + 1]].ToVector2(), LinearColor::Blue);
			_RSI->DrawLine(vertices[indices[bi]].ToVector2(), vertices[indices[bi + 2]].ToVector2(), LinearColor::Blue);
			_RSI->DrawLine(vertices[indices[bi + 1]].ToVector2(), vertices[indices[bi + 2]].ToVector2(), LinearColor::Blue);
		}

		delete[] vertices;
		delete[] indices;
	}

	
}

