#pragma once

#include "Exports.h"
#include "Renderer.h"
#include "Component.h"

enum CameraType
{
	Orthogonal = 0,
	Perspective
};

struct sOrthogonal
{
	float left;
	float right;
	float bottom;
	float top;
	float zNear;
	float zFar;

	sOrthogonal()
		: left(0.0f), right(1024.0f),
		bottom(0.0f), top(768.0f),
		zNear(0.0f), zFar(4000.0f)
	{
	};
};

struct sPerspective
{
	float fovy;
	float aspect;
	float zNear;
	float zFar;

	sPerspective()
		: fovy(glm::radians(45.0f)), aspect(4.0 / 3.0f),
		  zNear(0.1f), zFar(1000.0f)
	{
	};
};

class ENGINEDLL_API Camera : public Component
{
	Renderer* renderer;

	sOrthogonal ortho;
	sPerspective persp;
	CameraType type;

	mat4 vMatrix;
	vec4 right;
	vec4 up;
	vec4 fwd;
	vec4 pos;

	bool isMainCamera;

public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	/// <summary>Affects on the 'z' axis
	/// <para>Camera goes 'forward'</para>
	/// <param name="mount">How much the camera advance</param>
	/// </summary>
	void Walk(float mountX, float mountZ);

	/// <summary>Affects on the 'x' axis
	/// <para>Camera rotates</para>
	/// <param name="degrees">How much the camera rotates</param>
	/// </summary>
	void Pitch(float degrees);

	/// <summary>Affects on the 'y' axis
	/// <para>Camera rotates</para>
	/// <param name="degrees">How much the camera rotates</param>
	/// </summary>
	void Yaw(float degrees);

	/// <summary>Affects on the 'z' axis
	/// <para>Camera rotates</para>
	/// <param name="degrees">How much the camera rotates</param>
	/// </summary>
	void Roll(float degrees);

	void SetAsMainCamera();
	void RemoveAsMainCamera();
	void SetCameraType(CameraType type);

	/// <summary>Returns the position</summary>
	vec3 GetPosition() { return vec3(pos.x, pos.y, pos.z); };

	Camera();
	~Camera();
};