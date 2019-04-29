#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
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

	SetProjOrtho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), 0.0f, 4000.0f);
	//projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

	cameraType = Perspective;

	//					   Default values
	cameraPosition	= vec3(0, 0, 30);
	eyePosition		= vec3(0, 0, 0);
	headUpPosition	= vec3(0, 1, 0);

	viewMatrix = glm::lookAt(
		cameraPosition,
		eyePosition,
		headUpPosition
	);

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

unsigned int Renderer::GenElementBuffer(vector<unsigned int> indices, float* buffer, int size)
{
	unsigned int elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	return elementbuffer;
}

unsigned int Renderer::GenTexture(unsigned int width, unsigned int height, unsigned char* data)
{
	// Identificar el vertex buffer
	unsigned int vertexbuffer;
	glGenTextures(1, &vertexbuffer);

	// Se "Ata" la nueva textura : Todas las futuras funciones de texturas van a modificar esta textura
	glBindTexture(GL_TEXTURE_2D, vertexbuffer);

	// Se le pasa la imagen a OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return vertexbuffer;
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

void Renderer::DrawBuffer(unsigned int attributeId, int size, GLenum mode)
{
	glDrawArrays(mode, 0, size);
}

void Renderer::DrawElementBuffer(vector<unsigned int> indices)
{
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
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
	glm::vec3 newPos = vec3(x, y, 3);

	cameraPosition = vec3(newPos.x, newPos.y, newPos.z);
	eyePosition = vec3(0.0f, newPos.y, 0.0f);
	headUpPosition = vec3(0.0f, 1.0f, 0.0f);

	viewMatrix = glm::lookAt(
		cameraPosition,
		eyePosition,
		headUpPosition
	);
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

void Renderer::SetCameraEyePosition(mat4 newPosition)
{
	viewMatrix = newPosition;
	SetMVP();
}

void Renderer::SetCameraEyePosition(float x, float y, float z)
{
	eyePosition += vec3(x, y, z);

	viewMatrix = glm::lookAt(
		cameraPosition,
		eyePosition,
		headUpPosition
	);

	SetMVP();
}

void Renderer::SetHeadUpPosition(mat4 newPosition)
{
	viewMatrix = newPosition;
	SetMVP();
}

void Renderer::SetHeadUpPosition(float x, float y, float z)
{
	headUpPosition += vec3(x, y, z);

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

glm::mat4& Renderer::GetMVP()
{
	return MVP;
}