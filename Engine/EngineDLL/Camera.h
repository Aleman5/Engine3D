#pragma once

#include "Exports.h"
#include "Entity.h"

class ENGINEDLL_API Camera
{
	Renderer* renderer;

	mat4 vMatrix;
	vec4 x;
	vec4 y;
	vec4 z;
	vec4 pos;

public:
	/// <summary>Affects on the 'z' axis.
	/// <para>Camera goes 'forward'</para>
	/// <param name="mount">How much the camera advance</param>
	/// </summary>
	void Walk(float mount);

	/// <summary>Affects on the 'x' axis.
	/// <para>Camera goes to the sides</para>
	/// <param name="mount">How much the camera advance</param>
	/// </summary>
	void Strafe(float mount);

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