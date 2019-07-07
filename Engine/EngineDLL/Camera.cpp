#include "Camera.h"

Camera::Camera()
{
	Start();
}
Camera::~Camera()
{
}

void Camera::Start()
{
	name = "Camera";
	reqTransform = false;

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

	speed = 10.0f;
}

void Camera::Update()
{
	
}

void Camera::Draw()
{

}

void Camera::SetTransform(Transform* transform)
{

}

void Camera::UpdateRendererPos()
{
	if (!debugMode)
	{
		if (isMainCamera)
		{
			renderer->SetCameraPosition(vMatrix);
			renderer->ExtractPlanes(vMatrix);
		}
	}
	else
	{
		if (isMainCamera)
		{
			renderer->ExtractPlanes(vMatrix);
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

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	UpdateRendererPos();
}

void Camera::Move(float mountX, float mountY, float mountZ)
{
	pos = translate(mat4(1.0f), (vec3)((right * mountX) + (up * mountY) + (fwd * mountZ))) * pos;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	UpdateRendererPos();
}

void Camera::WalkFront(Direction dir)
{
	float mountZ = dir * speed * Defs::getInstance()->deltaTime;

	pos = translate(mat4(1.0f), (vec3)(fwd * mountZ)) * pos;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	UpdateRendererPos();
}

void Camera::WalkSideWays(Direction dir)
{
	float mountX = dir * speed * Defs::getInstance()->deltaTime;

	pos = translate(mat4(1.0f), (vec3)(right * mountX)) * pos;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	UpdateRendererPos();
}

void Camera::Rise(Direction dir)
{
	float mountY = dir * speed * Defs::getInstance()->deltaTime;

	pos = translate(mat4(1.0f), (vec3)(up * mountY)) * pos;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	UpdateRendererPos();
}

void Camera::Pitch(float degrees)
{
	fwd = rotate(mat4(1.0f), degrees, vec3(right.x, right.y, right.z)) * fwd;
	up = rotate(mat4(1.0f), degrees, vec3(right.x, right.y, right.z)) * up;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	UpdateRendererPos();
}

void Camera::Yaw(float degrees)
{
	fwd = rotate(mat4(1.0f), degrees, vec3(up.x, up.y, up.z)) * fwd;
	right = rotate(mat4(1.0f), degrees, vec3(up.x, up.y, up.z)) * right;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	UpdateRendererPos();
}

void Camera::Roll(float degrees)
{
	right = rotate(mat4(1.0f), degrees, vec3(fwd.x, fwd.y, fwd.z)) * right;
	up = rotate(mat4(1.0f), degrees, vec3(fwd.x, fwd.y, fwd.z)) * up;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	UpdateRendererPos();
}

void Camera::DebugModeOn()
{
	debugMode = true;
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