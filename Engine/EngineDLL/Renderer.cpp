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

	planes = new Plane[6];
	ExtractPlanes();

	return true;
}

bool Renderer::Stop()
{
	delete[] planes;
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

	ExtractPlanes();
	SetMVP();
}

void Renderer::RotateCamera(vec3 rot)
{
	viewMatrix = rotate(viewMatrix, rot.y, vec3(-1.0f, 0.0f, 0.0f));
	viewMatrix = rotate(viewMatrix, rot.x, vec3(0.0f, 1.0f, 0.0f));
	viewMatrix = rotate(viewMatrix, rot.z, vec3(0.0f, 0.0f, 1.0f));
	modelMatrix= scale (mat4(1.0f), vec3(0.5f));

	ExtractPlanes();
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

	ExtractPlanes();
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

	ExtractPlanes();
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

	ExtractPlanes();
	SetMVP();
}

void Renderer::SetCameraEyePosition(mat4 newPosition)
{
	viewMatrix = newPosition;

	ExtractPlanes();
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

	ExtractPlanes();
	SetMVP();
}

void Renderer::SetHeadUpPosition(mat4 newPosition)
{
	viewMatrix = newPosition;

	ExtractPlanes();
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

	ExtractPlanes();
	SetMVP();
}

void Renderer::SetProjOrtho(float left, float right, float bottom, float top)
{
	projectionMatrix = glm::ortho(left, right, bottom, top);

	ExtractPlanes();
	SetMVP();
}

void Renderer::SetProjOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
	
	ExtractPlanes();
	SetMVP();
}

void Renderer::SetProjPersp(float fovy, float aspect, float zNear, float zFar)
{
	projectionMatrix = glm::perspective(fovy, aspect, zNear, zFar);

	ExtractPlanes();
	SetMVP();
}

void Renderer::ExtractPlanes()
{
	mat4 comboMatrix = viewMatrix * projectionMatrix;

	/*cout << "ComboMatrix" << endl;
	cout << comboMatrix[0][0] << " " << comboMatrix[0][1] << " " << comboMatrix[0][2] << " " << comboMatrix[0][3] << endl;
	cout << comboMatrix[1][0] << " " << comboMatrix[1][1] << " " << comboMatrix[1][2] << " " << comboMatrix[1][3] << endl;
	cout << comboMatrix[2][0] << " " << comboMatrix[2][1] << " " << comboMatrix[2][2] << " " << comboMatrix[2][3] << endl;
	cout << comboMatrix[3][0] << " " << comboMatrix[3][1] << " " << comboMatrix[3][2] << " " << comboMatrix[3][3] << endl;
	cout << endl;

	cout << "ViewMatrix" << endl;
	cout << viewMatrix[0][0] << " " << viewMatrix[0][1] << " " << viewMatrix[0][2] << " " << viewMatrix[0][3] << endl;
	cout << viewMatrix[1][0] << " " << viewMatrix[1][1] << " " << viewMatrix[1][2] << " " << viewMatrix[1][3] << endl;
	cout << viewMatrix[2][0] << " " << viewMatrix[2][1] << " " << viewMatrix[2][2] << " " << viewMatrix[2][3] << endl;
	cout << viewMatrix[3][0] << " " << viewMatrix[3][1] << " " << viewMatrix[3][2] << " " << viewMatrix[3][3] << endl;
	cout << endl;

	cout << "ProjectionMatrix" << endl;
	cout << projectionMatrix[0][0] << " " << projectionMatrix[0][1] << " " << projectionMatrix[0][2] << " " << projectionMatrix[0][3] << endl;
	cout << projectionMatrix[1][0] << " " << projectionMatrix[1][1] << " " << projectionMatrix[1][2] << " " << projectionMatrix[1][3] << endl;
	cout << projectionMatrix[2][0] << " " << projectionMatrix[2][1] << " " << projectionMatrix[2][2] << " " << projectionMatrix[2][3] << endl;
	cout << projectionMatrix[3][0] << " " << projectionMatrix[3][1] << " " << projectionMatrix[3][2] << " " << projectionMatrix[3][3] << endl;
	cout << endl;*/

	// Left clipping plane
	planes[0].a = comboMatrix[3][0] + comboMatrix[0][0];
	planes[0].b = comboMatrix[3][1] + comboMatrix[0][1];
	planes[0].c = comboMatrix[3][2] + comboMatrix[0][2];
	planes[0].d = comboMatrix[3][3] + comboMatrix[0][3];

	// Right clipping plane
	planes[1].a = comboMatrix[3][0] - comboMatrix[0][0];
	planes[1].b = comboMatrix[3][1] - comboMatrix[0][1];
	planes[1].c = comboMatrix[3][2] - comboMatrix[0][2];
	planes[1].d = comboMatrix[3][3] - comboMatrix[0][3];

	// Top clipping plane
	planes[2].a = comboMatrix[3][0] - comboMatrix[1][0];
	planes[2].b = comboMatrix[3][1] - comboMatrix[1][1];
	planes[2].c = comboMatrix[3][2] - comboMatrix[1][2];
	planes[2].d = comboMatrix[3][3] - comboMatrix[1][3];

	// Bottom clipping plane
	planes[3].a = comboMatrix[3][0] + comboMatrix[1][0];
	planes[3].b = comboMatrix[3][1] + comboMatrix[1][1];
	planes[3].c = comboMatrix[3][2] + comboMatrix[1][2];
	planes[3].d = comboMatrix[3][3] + comboMatrix[1][3];

	// Near clipping plane
	planes[4].a = comboMatrix[3][0] + comboMatrix[2][0];
	planes[4].b = comboMatrix[3][1] + comboMatrix[2][1];
	planes[4].c = comboMatrix[3][2] + comboMatrix[2][2];
	planes[4].d = comboMatrix[3][3] + comboMatrix[2][3];

	// Far clipping plane
	planes[5].a = comboMatrix[3][0] - comboMatrix[2][0];
	planes[5].b = comboMatrix[3][1] - comboMatrix[2][1];
	planes[5].c = comboMatrix[3][2] - comboMatrix[2][2];
	planes[5].d = comboMatrix[3][3] - comboMatrix[2][3];
}

void Renderer::NormalizePlanes()
{
	for (int i = 0; i < 6; i++)
	{
		float mag;
		mag = sqrt(planes[i].a * planes[i].a + planes[i].b * planes[i].b + planes[i].c * planes[i].c);
		planes[i].a = planes[i].a / mag;
		planes[i].b = planes[i].b / mag;
		planes[i].c = planes[i].c / mag;
		planes[i].d = planes[i].d / mag;
	}
	
}

Halfspace Renderer::ClassifyPoint(const Plane& plane, const vec4& vertex)
{
	float distToPlane = plane.a * vertex.x + plane.b * vertex.y + plane.c * vertex.z + plane.d;

	return distToPlane >= 0.0f ? POSITIVE : NEGATIVE;
}