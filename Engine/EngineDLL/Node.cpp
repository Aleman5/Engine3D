#include "Node.h"

Node::Node()
{
	Start();
}
Node::Node(Node* parent)
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
	parent = NULL;
}

void Node::Update()
{
	if (isActive)
	{
		for (int i = 0; i < components.size(); i++)
			components[i]->Update();

		for (int i = 0; nodeChilds.size(); i++)
			nodeChilds[i]->Update();
	}
}

void Node::Draw()
{
	if (isActive)
	{
		mat4 currentT;

		for (int i = 0; i < components.size(); i++)
			components[i]->Draw();

		for (unsigned int i = 0; i < nodeChilds.size(); i++)
			nodeChilds[i]->Draw();
	}
}

void Node::SetParent(Node* parent)
{
	if(this->parent)
		this->parent->RemoveChild(this);
	this->parent = parent;
}

void Node::RemoveChild(Node* child)
{
	for (int i = 0; i < nodeChilds.size(); i++)
		if (child == nodeChilds[i])
		{
			nodeChilds.erase(nodeChilds.begin() + i);
			return;
		}

	cout << "This child is not in this Node" << endl;
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

void Node::AddComponent(Component* component)
{
	for (int i = 0; i < components.size(); i++)
		if (component->GetId() == components[i]->GetId())
		{
			cout << "A " << component->GetId() << " already exists in this Node" << endl;
			return;
		}

	components.push_back(component);
}

void Node::RemoveComponent(Component* component)
{
	for (int i = 0; i < components.size(); i++)
		if (component->GetId() == components[i]->GetId())
		{
			delete components[i];
			components.erase(components.begin() + i);
			return;
		}

	cout << "A " << component->GetId() << " doesn't exist in this Node" << endl;
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