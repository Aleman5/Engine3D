#include "Node.h"

Node::Node()
{
	Start();
}
Node::~Node()
{
}

void Node::Start()
{
	transform = new Transform();
}

void Node::Update()
{


	for (list<Node*>::iterator it = nodeChilds.begin(); it != nodeChilds.end(); ++it)
		(*it)->Update();
}

void Node::Draw()
{
	mat4 currentT;

	for (unsigned int i = 0; i < nodeChilds.size(); i++)
	{
		nodeChilds[i]->Draw();
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
	if (nodeChilds.size() > index)
	{
		return nodeChilds[index];
	}
	return NULL;
}