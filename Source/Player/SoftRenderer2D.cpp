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
	isCheckQuadTree = input.SpacePressed() ? false : true;
}

void SoftRenderer::Render2D()
{
	size_t culling = 0;
	size_t renderObject = 0;
	size_t numCircle = 0;
	size_t numCircleDraw = 0;
	size_t numRect = 0;
	size_t numRectDraw = 0;
	// 격자 그리기
	DrawGrid2D();

	////////////////////// 월드 공간 //////////////////////
	Matrix3x3 viewMatrix = _GameEngine.GetCamera()->GetViewMatrix();
	Matrix3x3 finalMat;
	auto& object = _GameEngine.GetObject();
	Camera2D* camera = _GameEngine.GetCamera();

	Circle circleCamera = camera->GetCircleBound();
	circleCamera.Center += camera->GetTransform().GetPosition();
	const auto& playerOjbect = _GameEngine.GameObjectFinder("Player");

	Vector2 min = Vector2(camera->GetTransform().GetPosition() + camera->GetRectBound().Min);
	Vector2 max = Vector2(camera->GetTransform().GetPosition() + camera->GetRectBound().Max);

	Rectangle RectCamera = Rectangle(min, max);

	if (!isCheckQuadTree) {
		size_t total = _GameEngine.GetObject().size();

		_RSI->PushStatisticText("Total : " + std::to_string(total));

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
			const Circle& cameraBound = _GameEngine.GetCamera()->GetCircleBound();

			Circle gameObjectBound(playerOjbect->GetMesh()->GetCircleBound());

			gameObjectBound.Center = finalMat * gameObjectBound.Center;
			gameObjectBound.Radius = gameObjectBound.Radius * playerOjbect->GetTransform().GetScale().Max();


			if (!cameraBound.Intersect(gameObjectBound)) {
				culling++;
				continue;
			}
			Vector2 min = objectMesh->GetRectBound().Min * object[i]->GetTransform().GetScale().Max();
			Vector2 max = objectMesh->GetRectBound().Max * object[i]->GetTransform().GetScale().Max();
			min = min + object[i]->GetTransform().GetPosition();
			max = max + object[i]->GetTransform().GetPosition();

			Rectangle ObjectRectBound = Rectangle(min, max);

			if (!RectCamera.Intersect(ObjectRectBound))
			{
				++numRect;
				continue;
			}
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
		_RSI->PushStatisticText("culling : " + std::to_string(culling));
		int sum = total - culling;
		_RSI->PushStatisticText("renderObject : " + std::to_string(sum));

	}

	// QUADTREE
	else {
		size_t total = _GameEngine.GetObject().size();

		_RSI->PushStatisticText("Total : " + std::to_string(total));
		std::vector<GameObject2D*> quadTree;
		
		for (int i = 0; i < object.size(); i++) {
			if(camera->GetRectBound().Max.X > object[i]->GetTransform().GetPosition().X)
				if (camera->GetRectBound().Min.X < object[i]->GetTransform().GetPosition().X)
					if (camera->GetRectBound().Max.Y > object[i]->GetTransform().GetPosition().Y)
						if (camera->GetRectBound().Min.Y < object[i]->GetTransform().GetPosition().Y)
								quadTree.push_back(object[i].get());

		}

		_GameEngine.SetQuadTree(RectCamera, quadTree);
		_GameEngine.GetQuadTree()->GetSectorObject(RectCamera, quadTree); //GameEngine에 있는 메모리를 읽을 수 없음

		for (int i = 0; i < quadTree.size(); i++) {

			const Mesh* objectMesh = quadTree[i]->GetMesh();
			if (!objectMesh) continue;

			Vector2 min = objectMesh->GetRectBound().Min * quadTree[i]->GetTransform().GetScale().Max();
			Vector2 max = objectMesh->GetRectBound().Max * quadTree[i]->GetTransform().GetScale().Max();
			min = min + quadTree[i]->GetTransform().GetPosition();
			max = max + quadTree[i]->GetTransform().GetPosition();

			Rectangle ObjectRect = Rectangle(min, max);

			if (!RectCamera.Intersect(ObjectRect))
			{
				++numRect;
				continue;
			}

			Circle ObjectCircle = objectMesh->GetCircleBound();
			ObjectCircle.Center += quadTree[i]->GetTransform().GetPosition();
			ObjectCircle.Radius = ObjectCircle.Radius * quadTree[i]->GetTransform().GetScale().Max();

			if (!circleCamera.Intersect(ObjectCircle))
			{
				++numCircle;
				continue;
			}

			size_t vertexCount = objectMesh->_Vertices.size();
			size_t indexCount = objectMesh->_Indices.size();
			size_t triangleCount = indexCount / 3;
			// 렌더러가 사용할 정점 버퍼와 인덱스 버퍼 생성
			Vector2* vertices = new Vector2[vertexCount];
			std::memcpy(vertices, &objectMesh->_Vertices[0], sizeof(Vector2) * vertexCount);
			int* indices = new int[indexCount];
			std::memcpy(indices, &objectMesh->_Indices[0], sizeof(int) * indexCount);

			finalMat = viewMatrix * quadTree[i]->GetTransform().GetModelingMatrix();

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
		_RSI->PushStatisticText("Rect culling : " + std::to_string(numRect));
		numRectDraw = total - numRect;
		_RSI->PushStatisticText("renderObject : " + std::to_string(numRectDraw));

		_RSI->PushStatisticText("Circle culling : " + std::to_string(numCircle));
		numCircleDraw = total - numCircle;
		_RSI->PushStatisticText("renderObject : " + std::to_string(numCircleDraw));
	}


}

