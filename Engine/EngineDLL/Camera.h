#pragma once

#include "Exports.h"
#include "Entity.h"

class ENGINEDLL_API Camera : public Entity
{
public:
	void Draw() override;
	void ShouldDispose() override;

	virtual unsigned int SetVertices(
		float* vertices,	// Vertices data
		int count			// Total of vertices
	) override;

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
	void Roll(float degrees);	// Rota sobre el eje z

	Camera(
		Renderer* renderer,	// Renderer reference
		Material* material, // Material reference
		Layers tag			// Tag of the Entity
	);
	~Camera();
};