#include "Camera.h"
#include "GlobalDefs.h"
#include "Transform.h"

Camera::Camera() : material(Material::GenerateMaterial(TEXTURE_VERTEX_SHADER, TEXTURE_FRAGMENT_SHADER)),
		controlledByMouse(false), input(Input::getInstance()), rotationSpeed(90.0f)
{
	Start();
}
Camera::Camera(bool controlledByMouse) : material(Material::GenerateMaterial(TEXTURE_VERTEX_SHADER, TEXTURE_FRAGMENT_SHADER)), 
		controlledByMouse(controlledByMouse), input(Input::getInstance()), rotationSpeed(90.0f)
{
	Start();
}
Camera::~Camera()
{
	delete material;
}

void Camera::Start()
{
	name = "Camera";
	reqTransform = true;

	ortho = sOrthogonal{};
	persp = sPerspective{};
	type  = Perspective;

	right = vec4(1, 0, 0, 0);
	up	  = vec4(0, 1, 0, 0);
	fwd	  = vec4(0, 0, 1, 0);
	pos   = vec4(0, 0, 0, 1);

	debugMode = false;
	isMainDebugCamera = false;
	isMainCamera = false;

	vMatrix = lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	renderer = Renderer::getInstance();

	ortho.right = (float)renderer->GetWindowWidht();
	ortho.top = (float)renderer->GetWindowHeight();

	renderer->ExtractPlanes(pos, fwd, right, up, persp.zNear, persp.zFar, persp.aspect, persp.fovy);

	speed = 10.0f;
}

void Camera::Update()
{
	if (!isMainCamera) return;

	if (controlledByMouse)
	{
		float horRotation = input->GetAxis(Axis::HORIZONTAL) * rotationSpeed * Defs::getInstance()->deltaTime;
		float verRotation = input->GetAxis(Axis::VERTICAL)   * rotationSpeed * Defs::getInstance()->deltaTime;

		Rotate(horRotation, verRotation);
	}

	vec3 globalPos = transform->GetGlobalPosition();
	vec3 viewDir   = transform->GetLocalForward();

	if (globalPosition != globalPos || viewDirection != viewDir)
	{
		globalPosition = globalPos;
		viewDirection = viewDir;

		vec3 center = globalPosition + viewDirection;
		vec3 upVector = transform->GetLocalUp();
		vec3 fwdVector = transform->GetLocalForward();
		vec3 rightVector = transform->GetLocalRight();

		vMatrix = lookAt(globalPosition, center, upVector);

		renderer->SetCameraPosition(vMatrix);
		renderer->ExtractPlanes(globalPosition, fwdVector, rightVector, upVector,
								persp.zNear, persp.zFar, persp.aspect, persp.fovy);
	}
}

void Camera::Draw()
{
	if (debugMode && isMainCamera)
	{
		float* vertex = new float[12 * 3]
		{
			 persp.zNear, persp.zNear, persp.zNear,
			 persp.zNear,-persp.zNear, persp.zNear,
			 persp.zFar, -persp.zFar,  persp.zFar,

			 persp.zNear, persp.zNear, persp.zNear,
			 persp.zFar,  persp.zFar,  persp.zFar,
			 persp.zFar, -persp.zFar,  persp.zFar,

			-persp.zNear, persp.zNear, persp.zNear,
			-persp.zNear,-persp.zNear, persp.zNear,
			-persp.zFar, -persp.zFar,  persp.zFar,

			-persp.zNear, persp.zNear, persp.zNear,
			-persp.zFar,  persp.zFar,  persp.zFar,
			-persp.zFar, -persp.zFar,  persp.zFar,
		};

		float* verticesColorData = new float[3 * 12]{
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
		};

		vector<unsigned int> indices{
			0, 1, 2,
			3, 4, 5,
			6, 7, 8,
			9,10,11
		};

		if (material != NULL)
		{
			material->Bind();
			mat4 newMVP = renderer->GetProjMatrix() * vMatrix * renderer->GetModelMatrix();
			material->SetMatrixProperty("MVP", newMVP);
			material->BindTexture();
		}

		unsigned int id = renderer->GenBuffer(vertex, sizeof(float) * 12 * 3);
		unsigned int colorId = renderer->GenBuffer(verticesColorData, sizeof(float) * 12 * 3);
		unsigned int elementsId = renderer->GenElementBuffer(indices);

		renderer->EnableAttributes(0);
		renderer->EnableAttributes(1);

		renderer->BindBuffer(id, 0);
		renderer->BindTextureBuffer(colorId, 1);
		renderer->BindElementBuffer(elementsId);
		renderer->DrawElementBuffer(elementsId);

		renderer->DisableAttributes(0);
		renderer->DisableAttributes(1);
	}
}

mat4 Camera::CalculateModel()
{
	mat4 model = mat4(1.0f);

	model *= glm::translate(mat4(1.0f), (vec3)pos);

	vec3 vecAxisX;

	vecAxisX[1] = vecAxisX[2] = 0.0f;
	vecAxisX[0] = 1.0f;

	return model;
}

void Camera::SetTransform(Transform* transform)
{
	this->transform = transform;

	globalPosition = this->transform->GetGlobalPosition();
	viewDirection = this->transform->GetLocalForward();
}

void Camera::UpdateRendererPos()
{
	vMatrix = lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	if (!debugMode)
	{
		if (isMainCamera)
		{
			cout << "Hola" << endl;
			renderer->SetCameraPosition(vMatrix);
			renderer->ExtractPlanes(pos, fwd, right, up, persp.zNear, persp.zFar, persp.aspect, persp.fovy);
		}
	}
	else
	{
		if (isMainCamera)
		{
			renderer->ExtractPlanes(pos, fwd, right, up, persp.zNear, persp.zFar, persp.aspect, persp.fovy);
		}
		if (isMainDebugCamera)
		{
			renderer->SetCameraPosition(vMatrix);
		}
	}
}

void Camera::Teleport(float mountX, float mountY, float mountZ)
{
	pos.x = mountX;
	pos.y = mountY;
	pos.z = mountZ;

	UpdateRendererPos();
}

void Camera::Move(float mountX, float mountY, float mountZ)
{
	pos = translate(mat4(1.0f), (vec3)((right * mountX) + (up * mountY) + (fwd * mountZ))) * pos;

	UpdateRendererPos();
}

void Camera::WalkFront(Direction dir)
{
	float mountZ = dir * speed * Defs::getInstance()->deltaTime;

	pos = translate(mat4(1.0f), (vec3)(fwd * mountZ)) * pos;

	UpdateRendererPos();
}

void Camera::WalkSideWays(Direction dir)
{
	float mountX = dir * speed * Defs::getInstance()->deltaTime;

	pos = translate(mat4(1.0f), (vec3)(right * mountX)) * pos;

	UpdateRendererPos();
}

void Camera::Rise(Direction dir)
{
	float mountY = dir * speed * Defs::getInstance()->deltaTime;

	pos = translate(mat4(1.0f), (vec3)(up * mountY)) * pos;

	UpdateRendererPos();
}

void Camera::Pitch(float degrees)
{
	fwd = rotate(mat4(1.0f), radians(degrees), vec3(right.x, right.y, right.z)) * fwd;
	up = rotate(mat4(1.0f), radians(degrees), vec3(right.x, right.y, right.z)) * up;

	UpdateRendererPos();
}

void Camera::Yaw(float degrees)
{
	fwd = rotate(mat4(1.0f), radians(degrees), vec3(up.x, up.y, up.z)) * fwd;
	right = rotate(mat4(1.0f), radians(degrees), vec3(up.x, up.y, up.z)) * right;

	UpdateRendererPos();
}

void Camera::Roll(float degrees)
{
	right = rotate(mat4(1.0f), radians(degrees), vec3(fwd.x, fwd.y, fwd.z)) * right;
	up = rotate(mat4(1.0f), radians(degrees), vec3(fwd.x, fwd.y, fwd.z)) * up;

	UpdateRendererPos();
}

void Camera::Rotate(float horRotation, float verRotation)
{
	verRotation = clamp(verRotation, -VERTICAL_RANGE, VERTICAL_RANGE);

	Pitch(verRotation);
	Yaw(horRotation);

	up = vec4(0, 1, 0, 0);
}

void Camera::SetViewDirection(vec3 sViewDirection)
{
	viewDirection = normalize(sViewDirection);
	transform->ForceLocalForward(sViewDirection);

	vec3 center = globalPosition + viewDirection;
	vec3 upVector = transform->GetLocalUp();
	vec3 fwdVector = transform->GetLocalForward();
	vec3 rightVector = transform->GetLocalRight();

	vMatrix = lookAt(globalPosition, center, upVector);

	renderer->SetCameraPosition(vMatrix);
	renderer->ExtractPlanes(globalPosition, fwdVector, rightVector, upVector,
							persp.zNear, persp.zFar, persp.aspect, persp.fovy);
}

void Camera::DebugModeOn()
{
	debugMode = true;
}

void Camera::DebugModeOff()
{
	debugMode = false;
}

void Camera::SetAsMainDebugCamera()
{
	isMainDebugCamera = true;
}

void Camera::SetAsMainCamera()
{
	if (!isMainCamera)
	{
		isMainCamera = true;

		renderer->SetProjPersp(persp.fovy, persp.aspect, persp.zNear, persp.zFar);
		UpdateRendererPos();
	}
}

void Camera::RemoveAsMainCamera()
{
	isMainCamera = false;
}

void Camera::SetCameraType(const CameraType type)
{
	if (this->type != type)
	{
		this->type = type;

		if (type == Orthogonal)
		{
			if (isMainCamera)
				renderer->SetProjOrtho(ortho.left, ortho.right, ortho.bottom, ortho.top, ortho.zNear, ortho.zFar);
		}
		else
		{
			if (isMainCamera)
				renderer->SetProjPersp(persp.fovy, persp.aspect, persp.zNear, persp.zFar);
		}
	}
}

void Camera::SetSpeed(float speed)
{
	this->speed = speed;
}