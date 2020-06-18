#include "Precompiled.h"
#include "SoftRenderer.h"

// 그리드 그리기
void SoftRenderer::DrawGrid3D()
{
	
}


// 게임 로직
void SoftRenderer::Update3D(float InDeltaSeconds)
{
	
}

void SoftRenderer::Render3D()
{
	// 격자 그리기
	DrawGrid3D();
	auto& object = _GameEngine3D.GetObject();
	const auto& playerOjbect = _GameEngine3D.GameObjectFinder("Player");
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

