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

	projectionMatrix = glm::ortho(0.0f, 1024.0f, 0.0f, 768.0f, 0.0f, 100.0f);

	cameraPosition = glm::vec3(0, 0, 3);
	eyePosition = glm::vec3(0, 0, 0);

	viewMatrix = glm::lookAt(
		cameraPosition,		// Camera is at (0, 0, 3), is World Space
		eyePosition,		// Looks at the origin
		glm::vec3(0, 1, 0)  // Head is up to (0, 1, 0)
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

void Renderer::DrawBuffer(unsigned int attributeId, int size, GLenum mode)
{
	glDrawArrays(mode, 0, size);
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

void Renderer::MoveCamera(glm::vec3 newPos)
{
	cameraPosition += glm::vec3(newPos.x, newPos.y, 0);
	eyePosition += newPos;

	viewMatrix = glm::lookAt(
		cameraPosition,
		eyePosition,
		glm::vec3(0, 1, 0)  // Head is up to (0, 1, 0)
	);
}

void Renderer::ResetCamera(float x, float y)
{
	glm::vec3 newPos = glm::vec3(x, y, 3);

	cameraPosition = glm::vec3(newPos.x, newPos.y, newPos.z);
	eyePosition = glm::vec3(0.0f, newPos.y, 0.0f);

	viewMatrix = glm::lookAt(
		cameraPosition,
		eyePosition,
		glm::vec3(0, 1, 0)  // Head is up to (0, 1, 0)
	);
}

void Renderer::loadIdentityMatrix()
{
	modelMatrix = glm::mat4(1.0f);

	SetMVP();
}

void Renderer::SetModelMatrix(glm::mat4 model)
{
	modelMatrix = model;

	SetMVP();
}


void Renderer::MultiplyModelMatrix(glm::mat4 model)
{
	modelMatrix *= model;

	SetMVP();
}

void Renderer::SetMVP()
{
	MVP = projectionMatrix * viewMatrix * modelMatrix;
}

glm::mat4& Renderer::GetMVP()
{
	return MVP;
}