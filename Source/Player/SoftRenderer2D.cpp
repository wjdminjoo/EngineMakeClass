#include "Precompiled.h"
#include "SoftRenderer.h"

// 그리드 그리기
void SoftRenderer::DrawGrid2D()
{
	// 그리드 색상
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
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	InputManager input = _GameEngine2D.GetInputManager();
	Transform2D& cameraTransform = _GameEngine2D.GetCamera()->GetTransform();
	Transform2D& playerTransform = _GameEngine2D.GameObjectFinder("Player")->GetTransform();
	Transform2D& childTransform = _GameEngine2D.GameObjectFinder("Child")->GetTransform();
	Transform2D& childTwoTransform = _GameEngine2D.GameObjectFinder("ChildTwo")->GetTransform();
	
	Vector2 deltaPosition = Vector2(input.GetXAxis(), input.GetYAxis()) * _MoveSpeed * InDeltaSeconds;

	cameraTransform.AddLocalPosition(deltaPosition);
	playerTransform.SetLocalPosition(cameraTransform.GetLocalPosition());

	// TEST
	playerTransform.AddWorldRotation(InDeltaSeconds * 50);
	childTransform.AddWorldRotation(InDeltaSeconds * 100);
	childTwoTransform.AddWorldRotation(InDeltaSeconds * 500);

	_CurrentColor = input.SpacePressed() ? LinearColor::Red : LinearColor::Blue;
}

void SoftRenderer::Render2D()
{


	// 격자 그리기
	DrawGrid2D();

	////////////////////// 월드 공간 //////////////////////
	Matrix3x3 viewMatrix = _GameEngine2D.GetCamera()->GetViewMatrix();
	Matrix3x3 finalMat;
	auto& object = _GameEngine2D.GetObject();
	const auto& playerOjbect = _GameEngine2D.GameObjectFinder("Player");
	const auto& childObject = _GameEngine2D.GameObjectFinder("Child");
	const auto& childTwoObject = _GameEngine2D.GameObjectFinder("ChildTwo");

	//TEST 
	// Player
	// Position
	int LocalPosX = playerOjbect->GetTransform().GetLocalPosition().X;
	int LocalPosY = playerOjbect->GetTransform().GetLocalPosition().Y;
	// Rotation
	float LocalRotation = playerOjbect->GetTransform().GetLocalRotation();
	// Scale
	unsigned short LocalScaleX = playerOjbect->GetTransform().GetLocalScale().X;
	unsigned short LocalScaleY = playerOjbect->GetTransform().GetLocalScale().Y;
	// WPosition
	int WorldPosX = playerOjbect->GetTransform().GetWorldPosition().X;
	int WorldPosY = playerOjbect->GetTransform().GetWorldPosition().Y;
	// WRotation
	float WorldRotation = playerOjbect->GetTransform().GetWorldRotation();
	// WScale
	unsigned short WorldScaleX = playerOjbect->GetTransform().GetWorldScale().X;
	unsigned short WorldScaleY = playerOjbect->GetTransform().GetWorldScale().Y;

	// Child
	// Position
	int childLocalPosX = childObject->GetTransform().GetLocalPosition().X;
	int childLocalPosY = childObject->GetTransform().GetLocalPosition().Y;
	// Rotation
	float childLocalRotation = childObject->GetTransform().GetLocalRotation();
	// Scale
	unsigned short childLocalScaleX = childObject->GetTransform().GetLocalScale().X;
	unsigned short childLocalScaleY = childObject->GetTransform().GetLocalScale().Y;
	// WPosition
	int childWorldPosX = childObject->GetTransform().GetWorldPosition().X;
	int childWorldPosY = childObject->GetTransform().GetWorldPosition().Y;
	// WRotation
	float childWorldRotation = childObject->GetTransform().GetWorldRotation();
	// WScale
	unsigned short childWorldScaleX = childObject->GetTransform().GetWorldScale().X;
	unsigned short childWorldScaleY = childObject->GetTransform().GetWorldScale().Y;


	// ChildTwo
	// Position
	int childTwoLocalPosX = childTwoObject->GetTransform().GetLocalPosition().X;
	int childTwoLocalPosY = childTwoObject->GetTransform().GetLocalPosition().Y;
	// Rotation
	float childTwoLocalRotation = childTwoObject->GetTransform().GetLocalRotation();
	// Scale
	unsigned short childTwoLocalScaleX = childTwoObject->GetTransform().GetLocalScale().X;
	unsigned short childTwoLocalScaleY = childTwoObject->GetTransform().GetLocalScale().Y;
	// WPosition
	int childTwoWorldPosX = childTwoObject->GetTransform().GetWorldPosition().X;
	int childTwoWorldPosY = childTwoObject->GetTransform().GetWorldPosition().Y;
	// WRotation
	float childTwoWorldRotation = childTwoObject->GetTransform().GetWorldRotation();
	// WScale
	unsigned short childTwoWorldScaleX = childTwoObject->GetTransform().GetWorldScale().X;
	unsigned short childTwoWorldScaleY = childTwoObject->GetTransform().GetWorldScale().Y;

	// Draw Object
	for (int i = 0; i < object.size(); i++) {
		Transform2D& objectTransform = object[i]->GetTransform();

		const Mesh2D* objectMesh = object[i]->GetMesh();

		size_t vertexCount = objectMesh->_Vertices.size();
		size_t indexCount = objectMesh->_Indices.size();
		size_t triangleCount = indexCount / 3;

		// 렌더러가 사용할 정점 버퍼와 인덱스 버퍼 생성
		Vector2* vertices = new Vector2[vertexCount];
		std::memcpy(vertices, &objectMesh->_Vertices[0], sizeof(Vector2) * vertexCount);
		int* indices = new int[indexCount];
		std::memcpy(indices, &objectMesh->_Indices[0], sizeof(int) * indexCount);

		finalMat = viewMatrix * objectTransform.GetWorldModelingMatrix();

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

	// 출력을 로컬, 월드로 표시
	//_RSI->PushStatisticText("Parent");
	//_RSI->PushStatisticText("Local Position : " + std::to_string(LocalPosX) + ", " + std::to_string(LocalPosY));
	//_RSI->PushStatisticText("Local Scale : " + std::to_string(LocalScaleX) + ", " + std::to_string(LocalScaleY));
	//_RSI->PushStatisticText("Local Rotation : " + std::to_string(LocalRotation));
	//_RSI->PushStatisticText("World Position : " + std::to_string(WorldPosX) + ", " + std::to_string(WorldPosY));
	//_RSI->PushStatisticText("World Scale : " + std::to_string(WorldScaleX) + ", " + std::to_string(WorldScaleY));
	//_RSI->PushStatisticText("World Rotation : " + std::to_string(WorldRotation));

	//_RSI->PushStatisticTextLine("");

	//_RSI->PushStatisticText("Child");
	//_RSI->PushStatisticText("Local Position : " + std::to_string(childLocalPosX) + ", " + std::to_string(childLocalPosY));
	//_RSI->PushStatisticText("Local Scale : " + std::to_string(childLocalScaleX) + ", " + std::to_string(childLocalScaleY));
	//_RSI->PushStatisticText("Local Rotation : " + std::to_string(childLocalRotation));
	//_RSI->PushStatisticText("World Position : " + std::to_string(childWorldPosX) + ", " + std::to_string(childTwoWorldPosY));
	//_RSI->PushStatisticText("World Scale : " + std::to_string(childWorldScaleX) + ", " + std::to_string(childTwoWorldScaleY));
	//_RSI->PushStatisticText("World Rotation : " + std::to_string(childWorldRotation));

	//_RSI->PushStatisticTextLine("");

	//_RSI->PushStatisticText("ChildTwo");
	//_RSI->PushStatisticText("Local Position : " + std::to_string(childTwoLocalPosX) + ", " + std::to_string(childTwoLocalPosY));
	//_RSI->PushStatisticText("Local Scale : " + std::to_string(childTwoLocalScaleX) + ", " + std::to_string(childTwoLocalScaleY));
	//_RSI->PushStatisticText("Local Rotation : " + std::to_string(childTwoLocalRotation));
	//_RSI->PushStatisticText("World Position : " + std::to_string(childTwoWorldPosX) + ", " + std::to_string(childTwoWorldPosY));
	//_RSI->PushStatisticText("World Scale : " + std::to_string(childTwoWorldScaleX) + ", " + std::to_string(childTwoWorldScaleY));
	//_RSI->PushStatisticText("World Rotation : " + std::to_string(childTwoWorldRotation));

}

