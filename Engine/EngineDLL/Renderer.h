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

enum CameraType
{
	Orthogonal = 0,
	Perspective
};

class ENGINEDLL_API Renderer
{
	CameraType cameraType;

	Window* window;				// Reference to the actual window

	GLuint VertexArrayID;		// Id of the Vertex Array

	vec3 cameraPosition;	// View of the Camera
	vec3 eyePosition;		// Actual position of the Camera
	vec3 headUpPosition;	// Head Up of the Camera

	mat4 modelMatrix;		// Position of the entity based on the origin
	mat4 viewMatrix;		// Position of the entity based on the camera
	mat4 projectionMatrix;	// Position of the entity based on the frustum of the camera
	mat4 MVP;				// The final position of the entity in world space

	mat4 orthoMatrix;
	mat4 perspMatrix;

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
		vec3 newPos			// Position added to the actual View Matrix position
	);

	void RotateCamera(vec3 rot);

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

	// MVP View parameter Set functions
	void SetCameraPosition(vec3 position);
	void SetCameraPosition(float x, float y, float z);

	void SetCameraEyePosition(vec3 newEyePosition);
	void SetCameraEyePosition(float x, float y, float z);

	void SetHeadUpPosition(vec3 newEyePosition);
	void SetHeadUpPosition(float x, float y, float z);

	// MVP Projection parameter Set functions
	void SetProjOrtho(float left, float right, float bottom, float top);
	void SetProjOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetProjPersp(float fovy, float aspect, float zNear, float zFar);

	glm::mat4& GetMVP();
	unsigned int GetWindowWidht()  { return window->GetWidth();  };
	unsigned int GetWindowHeight() { return window->GetHeight(); };
	glm::vec3 GetCameraPosition()  { return eyePosition;		 };

	Renderer();
	~Renderer();
};

