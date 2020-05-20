
#include "Precompiled.h"
#include "SoftRenderer.h"

// �׸��� �׸���
void SoftRenderer::DrawGrid2D()
{
	// �׸��� ����
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// ���� ���� ���
	Vector2 viewPos = _GameEngine.GetCamera()->GetTransform().GetPosition();
	Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

	// ���� �ϴܿ������� ���� �׸���
	int xGridCount = _ScreenSize.X / _Grid2DUnit;
	int yGridCount = _ScreenSize.Y / _Grid2DUnit;

	// �׸��尡 ���۵Ǵ� ���ϴ� ��ǥ �� ���
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

	// ������ ����
	ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(_ScreenSize, -viewPos);
	_RSI->DrawFullHorizontalLine(worldOrigin.Y, LinearColor::Red);
	_RSI->DrawFullVerticalLine(worldOrigin.X, LinearColor::Green);
}


// ���� ����
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	InputManager input = _GameEngine.GetInputManager();
	Transform2D& playerTransform = _GameEngine.GetPlayer()->GetTransform();
	Transform2D& cameraTransform = _GameEngine.GetCamera()->GetTransform();
	Vector2 deltaPosition = Vector2(input.GetXAxis(), input.GetYAxis()) * _MoveSpeed * InDeltaSeconds;
	
	cameraTransform.AddPosition(deltaPosition);
	playerTransform.SetPosition(cameraTransform.GetPosition());
	_CurrentColor = input.SpacePressed() ? LinearColor::Red : LinearColor::Blue;
}

void SoftRenderer::Render2D()
{
	// ���� �׸���
	DrawGrid2D();

	////////////////////// ���� ���� //////////////////////
	Transform2D& playerTransform = _GameEngine.GetPlayer()->GetTransform();
	Matrix3x3 viewMatrix = _GameEngine.GetCamera()->GetViewMatrix();

	auto& object = _GameEngine.GetObject();
	// ���� �������� ������ �ǹ� ��ġ�� ���
	_RSI->PushStatisticText(playerTransform.GetPosition().ToString());
	Matrix3x3 finalMat;
	const Mesh* mesh = _GameEngine.GetPlayer()->GetMesh();
	size_t vertexCount = mesh->_Vertices.size();
	size_t indexCount = mesh->_Indices.size();
	size_t triangleCount = indexCount / 3;

	for (int i = 0; i < object.size(); i++) {
		Transform2D& objectTransform = object[i]->GetTransform();

		const Mesh* objectMesh = object[i]->GetMesh();

		size_t vertexCount2 = objectMesh->_Vertices.size();
		size_t indexCount2 = mesh->_Indices.size();
		size_t triangleCount2 = indexCount2 / 3;

		// �������� ����� ���� ���ۿ� �ε��� ���� ����
		Vector2* vertices2 = new Vector2[vertexCount2];
		std::memcpy(vertices2, &mesh->_Vertices[0], sizeof(Vector2) * vertexCount2);
		int* indices2 = new int[indexCount2];
		std::memcpy(indices2, &mesh->_Indices[0], sizeof(int) * indexCount2);

		finalMat = viewMatrix * objectTransform.GetModelingMatrix();

		for (int vi = 0; vi < vertexCount; ++vi)
		{
			vertices2[vi] = finalMat * vertices2[vi];
		}

		// ��ȯ�� ������ �մ� �� �׸���
		for (int ti = 0; ti < triangleCount2; ++ti)
		{
			int bi = ti * 3;
			_RSI->DrawLine(vertices2[indices2[bi]], vertices2[indices2[bi + 1]], _CurrentColor);
			_RSI->DrawLine(vertices2[indices2[bi]], vertices2[indices2[bi + 2]], _CurrentColor);
			_RSI->DrawLine(vertices2[indices2[bi + 1]], vertices2[indices2[bi + 2]], _CurrentColor);
		}
		delete[] vertices2;
		delete[] indices2;
	}
	finalMat = viewMatrix * playerTransform.GetModelingMatrix();
	// �������� ����� ���� ���ۿ� �ε��� ���� ����
	Vector2* vertices = new Vector2[vertexCount];
	std::memcpy(vertices, &mesh->_Vertices[0], sizeof(Vector2) * vertexCount);
	int* indices = new int[indexCount];
	std::memcpy(indices, &mesh->_Indices[0], sizeof(int) * indexCount);

	

	// �� ������ ����� ����
	for (int vi = 0; vi < vertexCount; ++vi)
	{
		vertices[vi] = finalMat * vertices[vi];
	}

	// ��ȯ�� ������ �մ� �� �׸���
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

