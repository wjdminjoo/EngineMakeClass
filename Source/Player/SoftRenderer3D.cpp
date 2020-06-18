#include "Precompiled.h"
#include "SoftRenderer.h"

// �׸��� �׸���
void SoftRenderer::DrawGrid3D()
{
	
}


// ���� ����
void SoftRenderer::Update3D(float InDeltaSeconds)
{
	
}

void SoftRenderer::Render3D()
{
	// ���� �׸���
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

		// �������� ����� ���� ���ۿ� �ε��� ���� ����
		Vector4* vertices = new Vector4[vertexCount];
		for (int vi = 0; vi < vertexCount; ++vi)
		{
			vertices[vi] = Vector4(mesh->_Vertices[vi], true);
		}

		int* indices = new int[indexCount];
		std::memcpy(indices, mesh->_Indices.data(), sizeof(int) * indexCount);

		// �� ������ ����� ����
		for (int vi = 0; vi < vertexCount; ++vi)
		{
			vertices[vi] = finalMat * vertices[vi];
		}

		// ��ȯ�� ������ �մ� �� �׸���
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

