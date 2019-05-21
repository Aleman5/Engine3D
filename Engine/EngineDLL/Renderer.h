#pragma once

#include "Exports.h"
#include "Window.h"
#include "Material.h"
#include "Definitions.h"
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

	Window* window;			// Reference to the actual window

	unsigned int VertexArrayID;	// Id of the Vertex Array

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

	unsigned int GenElementBuffer(
		vector<unsigned int> indices
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
	void BindElementBuffer(
		unsigned int bufferId		// Buffer to use.
	);
	void BindTexture(
		unsigned int bufferId,		// Buffer to use.
		unsigned int width,			// Width of the image
		unsigned int height,		// Height of the image
		unsigned char* data			// Data of the file
	);
	void DrawBuffer(
		unsigned int attributeId,	// Location to fill in
		int size,					// Total of vertices to draw
		GLenum mode					// Draw mode
	);
	void DrawElementBuffer(
		unsigned int indices
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

	/// <summary>Updates the position of the camera</summary>
	void SetCameraPosition(mat4 position);
	/// <summary>Updates the position of the camera</summary>
	void SetCameraPosition(float x, float y, float z);

	/// <summary>Updates the position of the camera</summary>
	void SetCameraEyePosition(mat4 newEyePosition);
	/// <summary>Updates the position of the camera</summary>
	void SetCameraEyePosition(float x, float y, float z);
	
	/// <summary>Updates the position of the camera</summary>
	void SetHeadUpPosition(mat4 newEyePosition);
	/// <summary>Updates the position of the camera</summary>
	void SetHeadUpPosition(float x, float y, float z);

	/// <summary>Updates the values of the orthographic projection</summary>
	void SetProjOrtho(float left, float right, float bottom, float top);
	/// <summary>Updates the values of the orthographic projection</summary>
	void SetProjOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	/// <summary>Updates the values of the perspective projection</summary>
	void SetProjPersp(float fovy, float aspect, float zNear, float zFar);

	unsigned int GetWindowWidht()  { return window->GetWidth();  };
	unsigned int GetWindowHeight() { return window->GetHeight(); };
	vec3 GetCameraPosition()	   { return eyePosition;		 };
	mat4& GetProjMatrix()		   { return projectionMatrix;	 };
	mat4& GetViewMatrix()		   { return viewMatrix;			 };
	mat4& GetModelMatrix()		   { return modelMatrix;		 };
	mat4& GetMVP();

	Renderer();
	~Renderer();
};

