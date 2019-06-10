#include "Node.h"

Node::Node()
{
	Start();
}
Node::Node(Node* parent)
{
	Start();
}
Node::~Node()
{
	Delete();
}

void Node::Delete()
{
	while (components.size() > 0)
	{
		delete components[components.size() - 1];
		components.pop_back();
	}

	while (nodeChilds.size() > 0)
	{
		delete nodeChilds[nodeChilds.size() - 1];
		nodeChilds.pop_back();
	}

	parent = NULL;
}

void Node::Start()
{
	//transform = new Transform();
}

void Node::Update()
{
	if (isActive)
	{
		// Update components

		for (int i = 0; nodeChilds.size(); i++)
			nodeChilds[i]->Update();
	}
}

void Node::Draw()
{
	if (isActive)
	{
		mat4 currentT;

		for (unsigned int i = 0; i < nodeChilds.size(); i++)
		{
			nodeChilds[i]->Draw();
		}
	}
}

void Node::SetParent(Node* parent)
{
	this->parent->RemoveChild(this);
	this->parent = parent;
}

void Node::RemoveChild(Node* child)
{
	for (int i = 0; i < nodeChilds.size(); i++)
		if (child == nodeChilds[i])
		{
			nodeChilds.erase(nodeChilds.begin() + i);
			break;
		}
}

void Node::AddChild(Node* node)
{
	nodeChilds.push_back(node);
}

void Node::RemoveChild(unsigned int index)
{
	nodeChilds.erase(nodeChilds.begin() + index);
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

void Node::AddComponent(ComponentType componentType)
{
	if (!components[componentType])
	{
		switch (componentType)
		{
		case cTransform:
			//components[componentType] = new Transform();
			break;
		case cMesh:
			//components[componentType] = new Mesh();
			break;
		case cCamera:
			break;
		case cSprite:
			break;
		case cAnimation:
			break;
		case cTilemap:
			break;
		case cTriangule:
			break;
		case cRectangle:
			break;
		case cCircle:
			break;
		case cCount:
			break;
		default:
			break;
		}
	}
}

void Node::RemoveComponent(ComponentType componentType)
{
	if (components[componentType])
		delete components[componentType];
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