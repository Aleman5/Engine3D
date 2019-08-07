#pragma once

#include <glm/vec3.hpp>
#include "Exports.h"
#include "Input.h"
#include "Definitions.h"
#include "Renderer.h"
#include "Material.h"
#include "Component.h"

using namespace glm;

class Transform;

enum CameraType
{
	Orthogonal = 0,
	Perspective
};

enum Direction
{
	BACK = -1,
	ADVANCE = 1
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
		zNear(0.0f), zFar(400.0f)
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
		: fovy(glm::radians(45.0f)), aspect(4.0f / 3.0f),
		  zNear(0.1f), zFar(400.0f)
	{
	};
};

class ENGINEDLL_API Camera : public Component
{
	Renderer* renderer;
	Transform* transform;
	Material* material;
	Input* input;

	sOrthogonal ortho;
	sPerspective persp;
	CameraType type;

	vec3 globalPosition;
	vec3 viewDirection;

	mat4 vMatrix;
	vec4 right;
	vec4 up;
	vec4 fwd;
	vec4 pos;

	const float VERTICAL_RANGE = 70.0f;

	float speed;
	float rotationSpeed;

	float horAngle;
	float verAngle;

	bool controlledByMouse;
	bool debugMode;
	bool isMainDebugCamera;
	bool isMainCamera;

	mat4 CalculateModel();

public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void SetTransform(Transform* transform) override;

	void UpdateRendererPos();

	/// <summary>Affects on position
	/// <para>Camera goes 'forward'</para>
	/// <para>Camera goes 'right'</para>
	/// <para>Camera goes 'up'</para>
	/// <param name="mount">How much the camera moves</param>
	/// </summary>
	void Teleport(float mountX, float mountY, float mountZ);

	/// <summary>Affects on position
	/// <para>Camera goes 'forward'</para>
	/// <para>Camera goes 'right'</para>
	/// <para>Camera goes 'up'</para>
	/// <param name="mount">How much the camera moves</param>
	/// </summary>
	void Move(float mountX, float mountY, float mountZ);

	/// <summary>Affects on the 'z' axis
	/// <para>Camera goes 'right'</para>
	/// <param name="mount">How much the camera advance</param>
	/// </summary>
	void WalkFront(Direction dir);

	/// <summary>Affects on the 'x' axis
	/// <para>Camera goes 'up'</para>
	/// <param name="mount">How much the camera advance</param>
	/// </summary>
	void WalkSideWays(Direction dir);

	/// <summary>Affects on the 'y' axis
	/// <para>Camera goes 'up'</para>
	/// <param name="mount">How much the camera rise</param>
	/// </summary>
	void Rise(Direction dir);

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

	void Rotate(float horRotation, float verRotation);
	void SetViewDirection(vec3 viewDirection);

	void DebugModeOn();
	void DebugModeOff();
	void SetAsMainDebugCamera();
	void SetAsMainCamera();
	void RemoveAsMainCamera();
	void SetCameraType(CameraType type);
	void SetSpeed(float speed);

	/// <summary>Returns the position</summary>
	vec3 GetPosition() { return vec3(pos.x, pos.y, pos.z); };

	Camera();
	Camera(bool controlledByMouse);
	~Camera();
};