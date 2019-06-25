#include "Node.h"

Node::Node(string name)
	: name(name)
{
	Start();
}
Node::Node(string name, Node* parent)
	: name(name)
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

void Node::Start()
{
	parent = NULL;
	isActive = true;

	renderer = Renderer::getInstance();

	transform = new Transform();
	AddComponent(transform);
}

void Node::Update()
{
	if (isActive)
	{
		for (int i = 0; i < components.size(); i++)
			components[i]->Update();

		for (int i = 0; i < nodeChilds.size(); i++)
			nodeChilds[i]->Update();
	}
}

void Node::Draw()
{
	if (isActive)
	{
		mat4 currentModelMatrix = renderer->GetModelMatrix();
		renderer->MultiplyModelMatrix(transform->GetModelMatrix());

		for (int i = 0; i < components.size(); i++)
			components[i]->Draw();

		for (unsigned int i = 0; i < nodeChilds.size(); i++)
			nodeChilds[i]->Draw();

		renderer->SetModelMatrix(currentModelMatrix);
	}
}

void Node::SetParent(Node* parent)
{
	if(this->parent)
		this->parent->RemoveChild(this);
	this->parent = parent;
	this->parent->AddChild(this);
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

string Node::GetName()
{
	return name;
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

	cout << name << "doesn't have a component of type: " << type << endl;
}