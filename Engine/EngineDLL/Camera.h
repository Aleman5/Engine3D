#pragma once

#include "Exports.h"
#include "Renderer.h"
#include "Component.h"

class ENGINEDLL_API Camera : public Component
{
	Renderer* renderer;

	mat4 vMatrix;
	vec4 right;
	vec4 up;
	vec4 forward;
	vec4 pos;

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

	/// <summary>Returns the position</summary>
	vec3 GetPosition() { return vec3(pos.x, pos.y, pos.z); };

	Camera(Renderer* renderer);
	~Camera();
};