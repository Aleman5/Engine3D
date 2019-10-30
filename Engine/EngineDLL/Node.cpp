#include "Node.h"
#include "BSP.h"

Node::Node(string name)
	: name(name), myMesh(NULL), shouldDraw(true)
{
	Start();
}
Node::Node(string name, Node* parent)
	: name(name), myMesh(NULL), shouldDraw(true)
{
	Start();
	SetParent(parent);
}
Node::~Node()
{
	Delete();
}

void Node::Delete()
{
	while (components.size() > 0)
	{
		Component* component = components.back();
		components.pop_back();
		delete component;
	}

	while (nodeChilds.size() > 0)
	{
		Node* nChild = nodeChilds.back();
		nodeChilds.pop_back();
		delete nChild;
	}

	if (parent)
	{
		parent->RemoveChild(this);
		parent = NULL;
	}
}

void Node::CheckPlanes()
{
	if (!shouldDraw) return;

	if (fcData.initialized)
	{
		vec4* planes = renderer->GetPlanes();

		for (int i = 0; i < 6; i++)
			if (IsBehindPlane(planes[i]))
				shouldDraw = false;
	}
}

bool Node::IsBehindPlane(vec4& plane, Halfspace halfspace)
{
	for (int j = 0; j < 8; j++)
		if (renderer->ClassifyPoint(plane, renderer->GetModelMatrix() * vec4(fcData.vertex[j], 1.0f)) == halfspace)
			return false;

	return true;
}

void Node::Start()
{
	parent = NULL;
	isActive = true;

	renderer = Renderer::getInstance();
	fcData = FCCubeData();

	transform = new Transform();
	AddComponent(transform);
}

void Node::Update()
{
	if (isActive)
	{
		shouldDraw = true;

		for (int i = 0; i < components.size(); i++)
			components[i]->Update();

		for (int i = 0; i < nodeChilds.size(); i++)
			nodeChilds[i]->Update();

		if (parent)
			if (parent->fcData.initialized)
				parent->fcData.CompareData(fcData);
	}
}

void Node::Draw()
{
	if (isActive)
	{
		mat4 currentModelMatrix = renderer->GetModelMatrix();
		renderer->MultiplyModelMatrix(transform->GetModelMatrix());

		CheckPlanes();

		if (shouldDraw)
		{
			for (int i = 0; i < components.size(); i++)
				components[i]->Draw();

			for (unsigned int i = 0; i < nodeChilds.size(); i++)
				nodeChilds[i]->Draw();
		}

		if (fcData.initialized)
		{
			fcData.UpdateData();

			if (shouldDraw)
			{
				//cout << "Should Draw" << endl;
				if (myMesh)
				{
					cout << "Mesh" << endl;
					if (myMesh->GetDebugMode())
					{
						cout << "Holiguis" << endl;
						myMesh->DrawFCData(fcData);
					}
				}
			}
		}

		//shouldDraw = true;

		renderer->SetModelMatrix(currentModelMatrix);
	}
}

void Node::SetParent(Node* parent)
{
	if(this->parent)
		this->parent->RemoveChild(this);
	this->parent = parent;
	this->parent->AddChild(this);

	this->transform->SetParent(this->parent->transform);
}

void Node::AddChild(Node* node)
{
	nodeChilds.push_back(node);
}

void Node::RemoveChild(Node* child)
{
	for (int i = 0; i < nodeChilds.size(); i++)
		if (child == nodeChilds[i])
		{
			nodeChilds.erase(nodeChilds.begin() + i);
			return;
		}

	cout << child->name << " isn't a child of this Node" << endl;
}

void Node::RemoveChild(unsigned int index)
{
	if (index > nodeChilds.size())
	{
		printf("Index out of range");
		return;
	}

	nodeChilds.erase(nodeChilds.begin() + index);
}

Component* Node::AddComponent(Component* component)
{
	for (int i = 0; i < components.size(); i++)
		if (component->GetName() == components[i]->GetName())
		{
			cout << "A " << component->GetName() << " already exists in " << name << endl;
			delete component;
			return components[i];
		}

	components.push_back(component);

	if (component->ReqTransform())
		component->SetTransform(transform);

	return component;
}

void Node::RemoveComponent(Component* component)
{
	for (int i = 0; i < components.size(); i++)
		if (component->GetName() == components[i]->GetName())
		{
			delete components[i];
			components.erase(components.begin() + i);
			return;
		}

	cout << "A " << component->GetName() << " doesn't exist in " << name << endl;
}

void Node::RemoveComponent(string type)
{
	for (int i = 0; i < components.size(); i++)
		if (components[i]->GetName() == type)
		{
			delete components[i];
			components.erase(components.begin() + i);
			return;
		}

	cout << "A " << type << " doesn't exist in " << name << endl;
}

bool Node::IsActive()
{
	return isActive;
}

void Node::ActivateNode()
{
	isActive = true;
}

void Node::DesactivateNode()
{
	isActive = false;
}

void Node::ActivateCameraDebugMode()
{
	vector<Component*> comps = GetComponents("Camera");

	for (int i = 0; i < comps.size(); i++)
	{
		Camera* camera = (Camera*)comps[i];
		camera->DebugModeOn();
		camera->UpdateRendererPos();
	}
}

void Node::DesactivateCameraDebugMode()
{
	vector<Component*> comps = GetComponents("Camera");

	for (int i = 0; i < comps.size(); i++)
	{
		Camera* camera = (Camera*)comps[i];
		camera->DebugModeOff();
		camera->UpdateRendererPos();
	}
}

void Node::ActivateMeshDebugMode()
{
	myMesh = (Mesh*)GetComponent("Mesh");
	
	if (!myMesh)
	{
		cout << "A Mesh doesn't exist in node " << name << endl;
		return;
	}
	
	if (!fcData.initialized)
	{
		cout << "This mesh isn't the root" << endl;
		return;
	}

	myMesh->ActivateDebugMode();
}

void Node::DesactivateMeshDebugMode()
{
	if (!myMesh)
	{
		cout << "Mesh Debug Mode wasn't activated" << endl;
		return;
	}

	if (!fcData.initialized)
	{
		cout << "This mesh isn't the root" << endl;
		return;
	}

	myMesh->DesactivateDebugMode();
	myMesh = NULL;
}

void Node::CheckHalfspace(BSP* bsp)
{
	mat4 currentModelMatrix = renderer->GetModelMatrix();
	renderer->MultiplyModelMatrix(transform->GetModelMatrix());

	if (fcData.initialized)
	{
		for (int i = 0; i < 8; i++)
			if (IsBehindPlane(bsp->plane, bsp->halfspace))
			{
				shouldDraw = false;
				break;
			}
	}

	if (shouldDraw)
	{
		for (int i = 0; i < nodeChilds.size(); i++)
			nodeChilds[i]->CheckHalfspace(bsp);
	}

	renderer->SetModelMatrix(currentModelMatrix);
}

string Node::GetName()
{
	return name;
}

int Node::GetChildCount()
{
	return nodeChilds.size();
}

Node* Node::GetChild(unsigned int index)
{
	if (index > nodeChilds.size())
	{
		printf("Index out of range");
		return NULL;
	}

	return nodeChilds[index];
}

Component* Node::GetComponent(string type)
{
	for (int i = 0; i < components.size(); i++)
		if (components[i]->GetName() == type)
			return components[i];

	cout << name << " doesn't have a component of type: " << type << endl;

	return NULL;
}

Component* Node::GetComponentInChildren(string type)
{
	Component* comp = NULL;

	for (int i = 0; i < nodeChilds.size(); i++)
	{

		comp = nodeChilds[i]->GetComponent(type);
		if (comp) return comp;
	}

	cout << name << " doesn't have a component of type: " << type << endl;

	return NULL;
}

vector<Component*> Node::GetComponents(string type)
{
	vector<Component*> comps;

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->GetName() == type)
			comps.push_back(components[i]);
	}

	for (int j = 0; j < nodeChilds.size(); j++)
	{
		vector<Component*> compsChild = nodeChilds[j]->GetComponents(type);
		if (compsChild.size() > 0)
			comps.insert(comps.end(), compsChild.begin(), compsChild.end());
	}

	if (comps.size() == 0)
	{
		cout << name << " doesn't have a component of type: " << type << endl;
	}

	return comps;
}