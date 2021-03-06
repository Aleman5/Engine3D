#pragma once

#include <vector>
#include "Exports.h"
#include "Enums.h"
#include "Window.h"
//#include "Material.h"
#include "Definitions.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\glm.hpp>

using namespace glm;

class BSP;
class Node;

enum class Planes
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	NEAR,
	FAR,
	COUNT
};

enum Halfspace
{
	NEGATIVE = -1,
	POSITIVE =  1
};

class ENGINEDLL_API Renderer
{
	static Renderer* instance;

	Window* window;			// Reference to the actual window

	unsigned int VertexArrayID;	// Id of the Vertex Array

	vec3 cameraGlobalPosition;  // View of the Camera
	vec3 eyePosition;			// Actual position of the Camera
	vec3 headUpPosition;		// Head Up of the Camera
	vec3 cameraFwd;				// Camera Forward

	mat4 modelMatrix;		// Position of the transform based on the origin
	mat4 viewMatrix;		// Position of the transform based on the camera
	mat4 projectionMatrix;	// Position of the transform based on the frustum of the camera
	mat4 MVP;				// The final position of the entity in world space

	vector<BSP*> bSPs;
	vec4 planes[(int)Planes::COUNT];

	mat4 orthoMatrix;
	mat4 perspMatrix;

	float fov;
	float radianLimit;
	

public:
	int counter;

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
		unsigned int imageFormat,	// Format of the image
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
	void BindNormalBuffer(
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
		int size,					// Total of vertices to draw
		GLenum mode					// Draw mode
	);
	void DrawBuffer(
		int size,					// Total of vertices to draw
		PrimitiveType mode			// Draw mode
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
		mat4 model			// Model matrix of the entity
	);
	void MultiplyModelMatrix(
		mat4 model			// Model matrix of the entity
	); 
	void SetMVP();
	void ExtractPlanes(vec3 globalPos, vec3 fwd, vec3 right, vec3 up, float zNear, float zFar, float aspRatio, float fovy);
	void ExtractPlanes();

	/// <summary>Updates the position of the camera</summary>
	void SetCameraPosition(mat4 position);
	/// <summary>Updates the position of the camera</summary>
	void SetCameraPosition(float x, float y, float z);

	/// <summary>Updates the values of the orthographic projection</summary>
	void SetProjOrtho(float left, float right, float bottom, float top);
	/// <summary>Updates the values of the orthographic projection</summary>
	void SetProjOrtho(float left, float right, float bottom, float top, float zNear, float zFar);

	/// <summary>Updates the values of the perspective projection</summary>
	void SetProjPersp(float fovy, float aspect, float zNear, float zFar);

	vec4 CreatePlane(const vec3& normal, const vec3& point);
	void NormalizePlane(vec4& plane);
	void MakeBSPClean(Node* scene);
	Halfspace ClassifyPoint(const vec4& plane, const vec4& vertex);

	void AddBSP(BSP* bsp);

	unsigned int GetWindowWidht()  { return window->GetWidth();  };
	unsigned int GetWindowHeight() { return window->GetHeight(); };
	float GetFov()				   { return fov;				 };
	float GetRadianLimit()		   { return radianLimit;		 };
	vec4* GetPlanes()			   { return planes;				 };
	vec3 GetCameraGlobalPosition() { return cameraGlobalPosition;};
	vec3 GetCameraFwd()			   { return cameraFwd;			 };
	mat4 GetProjMatrix()		   { return projectionMatrix;	 };
	mat4 GetViewMatrix()		   { return viewMatrix;			 };
	mat4 GetModelMatrix()		   { return modelMatrix;		 };
	mat4& GetMVP()				   { return MVP;				 };

	static Renderer* getInstance()
	{
		if (instance == NULL) instance = new Renderer();
		return instance;
	}
	Renderer(Renderer const&) = delete;
	void operator = (Renderer const&) = delete;
private:
	Renderer();
};