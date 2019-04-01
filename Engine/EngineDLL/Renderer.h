#pragma once

#include "Exports.h"
#include "Window.h"
#include "Material.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\glm.hpp>

class ENGINEDLL_API Renderer
{
	Window* window;				// Reference to the actual window

	GLuint VertexArrayID;		// Id of the Vertex Array

	glm::vec3 eyePosition;		// Actual position of the Camera
	glm::vec3 cameraPosition;	// View of the Camera

	glm::mat4 modelMatrix;		// Position of the entity based on the origin
	glm::mat4 viewMatrix;		// Position of the entity based on the camera
	glm::mat4 projectionMatrix;	// Position of the entity based on the frustum of the camera
	glm::mat4 MVP;				// The final position of the entity in world space

public:
	bool Start(
		Window* window	// Actual window
	);
	bool Stop();

	void SetClearColor(
		float r,	// Red
		float g,	// Green
		float b,	// Blue
		float a		// Transparency
	);
	void ClearScreen();
	void SwapBuffers();
	void DestroyBuffer(
		unsigned int bufferId		// Buffer to destroy
	);

	unsigned int GenBuffer(
		float* buffer,				// Data to fill in the buffer
		int size					// Size of the data
	);

	unsigned int GenTexture(
		unsigned int width,			// Width of the image
		unsigned int height,		// Height of the image
		unsigned char* data			// Data of the file
		);

	unsigned int GenTilemapTexture(
		unsigned int width,			// Width of the image
		unsigned int height,		// Height of the image
		unsigned char* data			// Data of the file
	);

	void EnableAttributes(
		unsigned int attributeId	// Location to fill in
	);
	void BindBuffer(
		unsigned int bufferId,		// Buffer to use.
		unsigned int attributeId	// Location to fill in
	);
	void BindTextureBuffer(
		unsigned int bufferId,		// Buffer to use.
		unsigned int attributeId	// Location to fill in
	);
	void DrawBuffer(
		unsigned int attributeId,	// Location to fill in
		int size,					// Total of vertices to draw
		GLenum mode					// Draw mode
	);
	void DisableAttributes(
		unsigned int attributeId	// Location to fill in
	);

	void EnableBlend();
	void DisableBlend();

	void MoveCamera(
		glm::vec3 newPos			// Position added to the actual View Matrix position
	);

	void ResetCamera(
		float x,
		float y
	);

	void loadIdentityMatrix();
	void SetModelMatrix(
		glm::mat4 model				// Model matrix of the entity
	);
	void MultiplyModelMatrix(
		glm::mat4 model				// Model matrix of the entity
	);
	void SetMVP();

	glm::mat4& GetMVP();
	unsigned int GetWindowWidht()  { return window->GetWidth();  };
	unsigned int GetWindowHeight() { return window->GetHeight(); };
	glm::vec3 GetCameraPosition()  { return eyePosition;		 };

	Renderer();
	~Renderer();
};

