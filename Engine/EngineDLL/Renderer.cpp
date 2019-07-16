#include "Renderer.h"

Renderer * Renderer::instance = NULL;

Renderer::Renderer()
{
}

bool Renderer::Start(Window* win)
{
	window = win;

	glfwMakeContextCurrent((GLFWwindow*)window->GetContext());
	
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return false;
	}

	// Habilidad el test de profundidad
	glEnable(GL_DEPTH_TEST);

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);*/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Aceptar el fragmento si está más cerca de la cámara que el fragmento anterior
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//					   Default values
	cameraPosition	= vec3(0, 0, -5);
	eyePosition		= vec3(0, 0, 0);
	headUpPosition	= vec3(0, 1, 0);

	viewMatrix = glm::lookAt(
		cameraPosition,
		eyePosition,
		headUpPosition
	);

	modelMatrix = projectionMatrix = mat4(1.0f);

	return true;
}

bool Renderer::Stop()
{
	return true;
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::SwapBuffers()
{
	glfwSwapBuffers((GLFWwindow*)window->GetContext());
}

void Renderer::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DestroyBuffer(unsigned int bufferId)
{
	glDeleteBuffers(1, &bufferId);
}

unsigned int Renderer::GenBuffer(float* buffer, int size)
{
	// Identificar el vertex buffer
	unsigned int vertexbuffer;
	// Generar un buffer, poner el resultado en el vertexbuffer que acabamos de crear
	glGenBuffers(1, &vertexbuffer);
	// Los siguientes comandos le darán características especiales al 'vertexbuffer' 
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Darle nuestros vértices a  OpenGL.
	glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);

	return vertexbuffer;
}

unsigned int Renderer::GenElementBuffer(vector<unsigned int> indices)
{
	unsigned int elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	return elementbuffer;
}

unsigned int Renderer::GenTexture(unsigned int width, unsigned int height, unsigned int imageFormat, unsigned char* data)
{
	// Identificar el vertex buffer
	unsigned int vertexbuffer;
	glGenTextures(1, &vertexbuffer);

	// Se "Ata" la nueva textura : Todas las futuras funciones de texturas van a modificar esta textura
	glBindTexture(GL_TEXTURE_2D, vertexbuffer);

	// Se le pasa la imagen a OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	return vertexbuffer;

	/*unsigned int textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return textureID;*/
}

unsigned int Renderer::GenTilemapTexture(unsigned int width, unsigned int height, unsigned char* data)
{
	unsigned int vertexbuffer;

	glGenTextures(1, &vertexbuffer);

	glBindTexture(GL_TEXTURE_2D, vertexbuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return vertexbuffer;
}

void Renderer::EnableAttributes(unsigned int attributebId)
{
	glEnableVertexAttribArray(attributebId);
}

void Renderer::BindBuffer(unsigned int bufferId, unsigned int attributebId)
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glVertexAttribPointer(
		attributebId,       // debe corresponder en el shader.
		3,                  // tamaño
		GL_FLOAT,           // tipo
		GL_FALSE,           // normalizado?
		0,                  // corrimiento
		(void*)0            // desfase del buffer
	);
}

void Renderer::BindTextureBuffer(unsigned int bufferId, unsigned int attributebId)
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glVertexAttribPointer(
		attributebId,       // debe corresponder en el shader.
		2,                  // tamaño
		GL_FLOAT,           // tipo
		GL_FALSE,           // normalizado?
		0,                  // corrimiento
		(void*)0            // desfase del buffer
	);
}

void Renderer::BindElementBuffer(unsigned int bufferId)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
}

void Renderer::BindTexture(unsigned int bufferId, unsigned int width, unsigned int height, unsigned char* data)
{
	// Se "Ata" la nueva textura : Todas las futuras funciones de texturas van a modificar esta textura
	glBindTexture(GL_TEXTURE_2D, bufferId);

	// Se le pasa la imagen a OpenGL
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/
}

void Renderer::DrawBuffer(unsigned int attributeId, int size, GLenum mode)
{
	glDrawArrays(mode, 0, size);
}

void Renderer::DrawElementBuffer(unsigned int indices)
{
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices,    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);
}

void Renderer::DisableAttributes(unsigned int attributeId)
{
	glDisableVertexAttribArray(attributeId);
}

void Renderer::EnableBlend()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::DisableBlend()
{
	glDisable(GL_BLEND);
}

void Renderer::MoveCamera(vec3 newPos)
{
	cameraPosition += vec3(newPos.x, newPos.y, 0);
	eyePosition += newPos;

	viewMatrix = glm::lookAt(
		cameraPosition,
		eyePosition,
		headUpPosition
	);

	SetMVP();
}

void Renderer::RotateCamera(vec3 rot)
{
	viewMatrix = rotate(viewMatrix, rot.y, vec3(-1.0f, 0.0f, 0.0f));
	viewMatrix = rotate(viewMatrix, rot.x, vec3(0.0f, 1.0f, 0.0f));
	viewMatrix = rotate(viewMatrix, rot.z, vec3(0.0f, 0.0f, 1.0f));
	modelMatrix= scale (mat4(1.0f), vec3(0.5f));

	SetMVP();
}

void Renderer::ResetCamera(float x, float y)
{
	vec3 newPos = vec3(x, y, 3);

	cameraPosition = vec3(newPos.x, newPos.y, newPos.z);
	eyePosition = vec3(0.0f, newPos.y, 0.0f);
	headUpPosition = vec3(0.0f, 1.0f, 0.0f);

	viewMatrix = glm::lookAt(
		cameraPosition,
		eyePosition,
		headUpPosition
	);

	SetMVP();
}

void Renderer::loadIdentityMatrix()
{
	modelMatrix = mat4(1.0f);

	SetMVP();
}

void Renderer::SetModelMatrix(mat4 model)
{
	modelMatrix = model;

	SetMVP();
}


void Renderer::MultiplyModelMatrix(mat4 model)
{
	modelMatrix *= model;

	SetMVP();
}

void Renderer::SetMVP()
{
	MVP = projectionMatrix * viewMatrix * modelMatrix;
}

void Renderer::SetCameraPosition(mat4 newPosition)
{
	viewMatrix = newPosition;

	SetMVP();
}

void Renderer::SetCameraPosition(float x, float y, float z)
{
	cameraPosition = vec3(x, y, z);

	viewMatrix = glm::lookAt(
		cameraPosition,
		eyePosition,
		headUpPosition
	);

	SetMVP();
}

void Renderer::SetProjOrtho(float left, float right, float bottom, float top)
{
	projectionMatrix = glm::ortho(left, right, bottom, top);

	SetMVP();
}

void Renderer::SetProjOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
	
	SetMVP();
}

void Renderer::SetProjPersp(float fovy, float aspect, float zNear, float zFar)
{
	projectionMatrix = glm::perspective(fovy, aspect, zNear, zFar);

	SetMVP();
}

vec4 Renderer::CreatePlane(const vec3& normal, const vec3& point)
{
	vec4 plane;

	plane.x = normal.x;
	plane.y = normal.y;
	plane.z = normal.z;
	plane.w = -dot(normal, point);

	return plane;
}

void Renderer::ExtractPlanes()
{
	glm::mat4 comboMatrix = projectionMatrix * glm::transpose(viewMatrix);

	// Left clipping plane
	planes[0].x = comboMatrix[0][3] + comboMatrix[0][3];
	planes[0].y = comboMatrix[1][3] + comboMatrix[1][3];
	planes[0].z = comboMatrix[2][3] + comboMatrix[2][3];
	planes[0].w = comboMatrix[3][3] + comboMatrix[3][3];
	
	// Right clipping plane
	planes[1].x = comboMatrix[0][3] - comboMatrix[0][3];
	planes[1].y = comboMatrix[1][3] - comboMatrix[1][3];
	planes[1].z = comboMatrix[2][3] - comboMatrix[2][3];
	planes[1].w = comboMatrix[3][3] - comboMatrix[3][3];
	
	// Top clipping plane
	planes[2].x = comboMatrix[0][3] - comboMatrix[0][3];
	planes[2].y = comboMatrix[1][3] - comboMatrix[1][3];
	planes[2].z = comboMatrix[2][3] - comboMatrix[2][3];
	planes[2].w = comboMatrix[3][3] - comboMatrix[3][3];
	
	// Bottom clipping plane
	planes[3].x = comboMatrix[0][3] + comboMatrix[0][3];
	planes[3].y = comboMatrix[1][3] + comboMatrix[1][3];
	planes[3].z = comboMatrix[2][3] + comboMatrix[2][3];
	planes[3].w = comboMatrix[3][3] + comboMatrix[3][3];
	
	// Near clipping plane
	planes[4].x = comboMatrix[0][3] + comboMatrix[0][3];
	planes[4].y = comboMatrix[1][3] + comboMatrix[1][3];
	planes[4].z = comboMatrix[2][3] + comboMatrix[2][3];
	planes[4].w = comboMatrix[3][3] + comboMatrix[3][3];
	
	// Far clipping plane
	planes[5].x = comboMatrix[0][3] - comboMatrix[0][3];
	planes[5].y = comboMatrix[1][3] - comboMatrix[1][3];
	planes[5].z = comboMatrix[2][3] - comboMatrix[2][3];
	planes[5].w = comboMatrix[3][3] - comboMatrix[3][3];
}

void Renderer::ExtractPlanes(vec3 globalPos, vec3 fwd, vec3 right, vec3 up, float zNear, float zFar, float aspRatio, float fovy)
{
	vec3 nearCenter = globalPos + fwd * zNear;
	vec3 farCenter  = globalPos + fwd * zFar;

	float fovTan = tan(fovy);

	float nHeight = zNear * fovTan;
	float nWidth  = nHeight * aspRatio;

	vec3 leftPlaneVec	= (nearCenter - right * nWidth  * 0.5f) - globalPos;
	vec3 rightPlaneVec  = (nearCenter + right * nWidth  * 0.5f) - globalPos;
	vec3 topPlaneVec	= (nearCenter + up	  * nHeight * 0.5f) - globalPos;
	vec3 bottomPlaneVec = (nearCenter - up	  * nHeight * 0.5f) - globalPos;

	vec3 normalLeft   = -normalize(cross(leftPlaneVec, up));
	vec3 normalRight  =  normalize(cross(rightPlaneVec, up));
	vec3 normalTop	  = -normalize(cross(topPlaneVec, right));
	vec3 normalBottom =  normalize(cross(bottomPlaneVec, right));

	planes[(int)Planes::NEAR]	= CreatePlane(fwd, nearCenter);
	planes[(int)Planes::FAR]	= CreatePlane(-fwd, farCenter);
	planes[(int)Planes::LEFT]	= CreatePlane(normalLeft, globalPos);
	planes[(int)Planes::RIGHT]	= CreatePlane(normalRight, globalPos);
	planes[(int)Planes::TOP]	= CreatePlane(normalTop, globalPos);
	planes[(int)Planes::BOTTOM] = CreatePlane(normalBottom, globalPos);
}

void Renderer::NormalizePlanes()
{
	for (int i = 0; i < 6; i++)
	{
		float mag;
		mag = sqrt(planes[i].x * planes[i].x + planes[i].y * planes[i].y + planes[i].z * planes[i].z);
		planes[i].x = planes[i].x / mag;
		planes[i].y = planes[i].y / mag;
		planes[i].z = planes[i].z / mag;
		planes[i].w = planes[i].w / mag;
	}
	
}

Halfspace Renderer::ClassifyPoint(const vec4& plane, const vec4& vertex)
{
	float distToPlane = plane.x * vertex.x + plane.y * vertex.y + plane.z * vertex.z + plane.w;

	return distToPlane >= 0.0f ? POSITIVE : NEGATIVE;
}