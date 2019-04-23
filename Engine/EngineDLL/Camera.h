#pragma once

#include "Exports.h"
#include "Entity.h"

class ENGINEDLL_API Camera
{
	Renderer* renderer;

	mat4 vMatrix;
	vec4 right;
	vec4 up;
	vec4 forward;
	vec4 pos;

public:
	/// <summary>Affects on the 'z' axis.
	/// <para>Camera goes 'forward'</para>
	/// <param name="mount">How much the camera advance</param>
	/// </summary>
	void Walk(float mountX, float mountZ);

	/// <summary>Affects on the 'x' axis.
	/// <para>Camera rotates</para>
	/// <param name="degrees">How much the camera rotates</param>
	/// </summary>
	void Pitch(float degrees);

	/// <summary>Affects on the 'y' axis.
	/// <para>Camera rotates</para>
	/// <param name="degrees">How much the camera rotates</param>
	/// </summary>
	void Yaw(float degrees);

	/// <summary>Affects on the 'z' axis.
	/// <para>Camera rotates</para>
	/// <param name="degrees">How much the camera rotates</param>
	/// </summary>
	void Roll(float degrees);

	Camera(Renderer* renderer);
	~Camera();
};