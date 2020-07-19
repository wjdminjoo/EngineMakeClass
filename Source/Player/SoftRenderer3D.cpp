#include "Precompiled.h"
#include "SoftRenderer.h"

// 그리드 그리기
void SoftRenderer::DrawGrid3D()
{
	
}

// 게임 로직
void SoftRenderer::Update3D(float InDeltaSeconds)
{
	InputManager input = _GameEngine3D.GetInputManager();
	Transform3D& cameraTransform = _GameEngine3D.GetCamera()->GetTransform();
	auto& Body = _GameEngine3D.GameObjectFinder("Body")->GetTransform();
	auto& LLeg = _GameEngine3D.GameObjectFinder("LLeg")->GetTransform();
	auto& RLeg = _GameEngine3D.GameObjectFinder("RLeg")->GetTransform();
	auto& LArm = _GameEngine3D.GameObjectFinder("LArm")->GetTransform();
	auto& RArm = _GameEngine3D.GameObjectFinder("RArm")->GetTransform();
	auto& Head = _GameEngine3D.GameObjectFinder("Head")->GetTransform();

	Quaternion q1(Rotator(0, 0, 0));
	Quaternion q2(Rotator(0, 0, -60));
	Quaternion q3(Rotator(0, 0, 60));
	Quaternion q4(Rotator(0, 20, 0));
	Quaternion q5(Rotator(0, -20, 0));
	Quaternion q6(Rotator(0, 0, -30));
	Quaternion q7(Rotator(0, 0, 30));

	if (isCheckTime)
	{
		time += InDeltaSeconds;
		time2 -= InDeltaSeconds;
		if (time >= 1.f)
			isCheckTime = false;
	}
	else
	{
		time -= InDeltaSeconds;
		time2 += InDeltaSeconds;
		if (time <= 0.f)
			isCheckTime = true;
	}

	LLeg.SetRotation(Quaternion::Slerp(q1, q6, time));
	RLeg.SetRotation(Quaternion::Slerp(q1, q7, time));
	RArm.SetRotation(Quaternion::Slerp(q1, q2, time));
	LArm.SetRotation(Quaternion::Slerp(q1, q3, time));
	Head.SetRotation(Quaternion::Slerp(q4, q5, time));

	_CurrentColor = input.SpacePressed() ? LinearColor::Red : LinearColor::Blue;

}

void SoftRenderer::Render3D()
{

	DrawGrid3D();
	InputManager input = _GameEngine3D.GetInputManager();
	Camera3D* cam = _GameEngine3D.GetCamera();
	auto& object = _GameEngine3D.GetObject();
	Matrix4x4 viewMat = _GameEngine3D.GetCamera()->GetViewMatrix();
	Matrix4x4 projectionMat = _GameEngine3D.GetCamera()->GetProjectionMatrix(_ScreenSize.X, _ScreenSize.Y);

	
	for (int i = 0; i < object.size(); i++)
	{
		const Mesh3D* mesh = object[i]->GetMesh();
		Sphere sphereBound = mesh->GetSphereBound();
		
		Transform3D& transform = object[i]->GetTransform();
		Matrix4x4 ProjectionfinalMat = projectionMat * viewMat * transform.GetWorldModelingMatrix();
		Matrix4x4 finalMat = viewMat * transform.GetModelingMatrix();

		sphereBound.Center = finalMat * sphereBound.Center;
		sphereBound.Radius = sphereBound.Radius * object[i]->GetTransform().GetLocalScale().Max();

		if(input.SpacePressed())
		if (!cam->GetViewSpaceFrustum(sphereBound)) {
			continue;
		}

		if (!input.SpacePressed())
		if (!cam->GetProjectioneFrustum(sphereBound)) {
			continue;
		}

		size_t vertexCount = mesh->_Vertices.size();
		size_t indexCount = mesh->_Indices.size();
		size_t triangleCount = indexCount / 3;

		// 렌더러가 사용할 정점 버퍼와 인덱스 버퍼 생성
		Vector4* vertices = new Vector4[vertexCount];
		int* indices = new int[indexCount];
		std::memcpy(indices, mesh->_Indices.data(), sizeof(int) * indexCount);
		// 각 정점에 행렬을 적용
		for (int vi = 0; vi < vertexCount; ++vi)
		{
			vertices[vi] = Vector4(mesh->_Vertices[vi], true);
			vertices[vi] = finalMat * vertices[vi];
		}

		// r * cos + (1 - cos) * (r * n) * (n) + sin * (n x r)
		Vector3 n = Vector3(5.f, 10.f, 15.f).Normalize();
		float cosRadi = cos(radius);
		for (int i = 0; i < vertexCount; ++i)
		{
			vertices[i] = Vector4((vertices[i] * cosRadi).ToVector3() + n * (1 - cosRadi) * vertices[i].ToVector3().Dot(n) +
				Vector3(n).Cross(vertices[i].ToVector3()) * sinf(radius), 0);

			vertices[i].W = 1.f;
		}

		// 변환된 정점을 잇는 선 그리기
		for (int ti = 0; ti < triangleCount; ++ti)
		{
			int bi = ti * 3;
			if (object[0]) {
				std::vector<Vector4> tp = { vertices[indices[bi]] , vertices[indices[bi + 1]] , vertices[indices[bi + 2]] };
				for (int i = 0; i < 3; ++i)
				{
					float invW = 1.f / tp[i].W;
					tp[i].X *= invW;
					tp[i].Y *= invW;
					tp[i].Z *= invW;
					tp[i].W = invW;
				}

				Vector3 edge1 = (vertices[indices[bi + 1]] - vertices[indices[bi]]).ToVector3();
				Vector3 edge2 = (vertices[indices[bi + 2]] - vertices[indices[bi]]).ToVector3();
				Vector3 faceNormal = edge1.Cross(edge2).Normalize();
				if (faceNormal.Dot(-Vector3::UnitZ) > 0.f)
				{
					continue;
				}

				for (int i = 0; i < 3; ++i)
				{
					tp[i].X *= (_ScreenSize.X * 0.5f);
					tp[i].Y *= (_ScreenSize.Y * 0.5f);
				}
			}

			_RSI->DrawLine(vertices[indices[bi]].ToVector2(), vertices[indices[bi + 1]].ToVector2(), LinearColor::Blue);
			_RSI->DrawLine(vertices[indices[bi]].ToVector2(), vertices[indices[bi + 2]].ToVector2(), LinearColor::Blue);
			_RSI->DrawLine(vertices[indices[bi + 1]].ToVector2(), vertices[indices[bi + 2]].ToVector2(), LinearColor::Blue);

		}
		delete[] vertices;
		delete[] indices;
	}

	//_RSI->PushStatisticText("viewSpaceFrustum" + std::to_string(viewSpaceFrustum));
	//_RSI->PushStatisticText("ProjectionFrustum" + std::to_string(ProjectionFrustum));
}

