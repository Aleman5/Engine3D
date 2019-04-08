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

	void Walk(float mount);
	void Strafe(float mount);
	void Pitch(float degrees);	// Rota sobre el eje x
	void Yaw(float degrees);		// Rota sobre el eje y
	void Roll(float degrees);	// Rota sobre el eje z

	Camera(
		Renderer* renderer,	// Renderer reference
		Material* material, // Material reference
		Layers tag			// Tag of the Entity
	);
	~Camera();
};