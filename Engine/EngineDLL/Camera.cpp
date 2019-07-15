#include "Camera.h"

Camera::Camera(Material* material)
	: material(material)
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

	renderer->ExtractPlanes(pos, fwd, right, up, persp.zNear, persp.zFar, persp.aspect, persp.fovy);

	speed = 10.0f;
}

void Camera::Update()
{
	
}

void Camera::Draw()
{
	if (debugMode && isMainCamera)
	{
		float* vertex = new float[12 * 3]
		{
			/*fcNew.vertex[0].x, fcNew.vertex[0].y, fcNew.vertex[0].z,
			fcNew.vertex[1].x, fcNew.vertex[1].y, fcNew.vertex[1].z,
			fcNew.vertex[4].x, fcNew.vertex[4].y, fcNew.vertex[4].z,

			fcNew.vertex[1].x, fcNew.vertex[1].y, fcNew.vertex[1].z,
			fcNew.vertex[4].x, fcNew.vertex[4].y, fcNew.vertex[4].z,
			fcNew.vertex[5].x, fcNew.vertex[5].y, fcNew.vertex[5].z,

			fcNew.vertex[2].x, fcNew.vertex[2].y, fcNew.vertex[2].z,
			fcNew.vertex[3].x, fcNew.vertex[3].y, fcNew.vertex[3].z,
			fcNew.vertex[6].x, fcNew.vertex[6].y, fcNew.vertex[6].z,

			fcNew.vertex[3].x, fcNew.vertex[3].y, fcNew.vertex[3].z,
			fcNew.vertex[6].x, fcNew.vertex[6].y, fcNew.vertex[6].z,
			fcNew.vertex[7].x, fcNew.vertex[7].y, fcNew.vertex[7].z,*/

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
			
			/*1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,*/
		};

		vector<unsigned int> indices{
			0, 1, 2,
			3, 4, 5,
			6, 7, 8,
			9,10,11
		};

		if (material != NULL)
		{
			material->Bind("myTextureSampler", 2);
			//mat4 model = CalculateModel();
			mat4 newMVP = renderer->GetProjMatrix() * vMatrix * renderer->GetModelMatrix();
			material->SetMatrixProperty("MVP", newMVP/*renderer->GetMVP()*/);
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

	//model *= rotate(mat4(1.0f), right, vecAxisX);

	return model;
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
	fwd = rotate(mat4(1.0f), glm::radians(degrees), vec3(right.x, right.y, right.z)) * fwd;
	up = rotate(mat4(1.0f), glm::radians(degrees), vec3(right.x, right.y, right.z)) * up;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	UpdateRendererPos();
}

void Camera::Yaw(float degrees)
{
	fwd = rotate(mat4(1.0f), glm::radians(degrees), vec3(up.x, up.y, up.z)) * fwd;
	right = rotate(mat4(1.0f), glm::radians(degrees), vec3(up.x, up.y, up.z)) * right;

	vMatrix = glm::lookAt(
		(vec3)pos,
		(vec3)(pos + fwd),
		(vec3)up
	);

	UpdateRendererPos();
}

void Camera::Roll(float degrees)
{
	right = rotate(mat4(1.0f), glm::radians(degrees), vec3(fwd.x, fwd.y, fwd.z)) * right;
	up = rotate(mat4(1.0f), glm::radians(degrees), vec3(fwd.x, fwd.y, fwd.z)) * up;

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