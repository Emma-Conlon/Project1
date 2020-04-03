#include <GameObject.h>

GameObject::GameObject(int t_type) :
	model(1.0f)
{
	type = t_type;

	for (int i = 0; i < 72; i++)
	{
		vertexRec[i] = verticesRec[i];
	}
	// Copy the Cube contents into GameObject
	if (t_type == 1)
	{
		memcpy(this->vertex, vertices, sizeof(this->vertex));
	}
	else if (t_type == 2)
	{
		memcpy(vertexRec, vertexRec, sizeof(vertexRec));
	}
	memcpy(this->color, colors, sizeof(this->color));
	memcpy(this->uv, uvs, sizeof(this->uv));

	// Copy UV's to all faces
	for (int i = 1; i < 6; i++)
		memcpy(&uv[i * 4 * 2], &uv[0], 2 * 4 * sizeof(GLfloat));

	memcpy(this->index, indices, sizeof(this->index));

	this->position = vec3();
}

GameObject::~GameObject()
{
}

vec3 GameObject::getPosition() {
	return this->position;
}
void GameObject::setSize(float t_size)
{
	m_size = t_size;
}
void GameObject::setPosition(vec3 position) { this->position = position; }

void GameObject::setMove(int t_move)
{
	m_move = t_move;
}

// Returns the first element of the Vertex array
GLfloat* GameObject::getVertex() {
	if (type == 1)
	{
		return this->vertex;
	}
	else if (type == 2)
	{
		return this->vertexRec;
	}
}
// 3 Vertices
int GameObject::getVertexCount() { return ARRAY_SIZE(vertex) / 3; }

int GameObject::getMove()
{
	return m_move;
}

// Returns the first element of the Color array
GLfloat* GameObject::getColor() { return this->color; }
// 4 Colors RGBA
int GameObject::getColorCount() { return ARRAY_SIZE(color) / 4; }

// Returns the first element of the UV array
GLfloat* GameObject::getUV() { return this->uv; }
// 3 Colors RGB
int GameObject::getUVCount() { return ARRAY_SIZE(uv); }

// Returns the first element of the Index array
GLfloat* GameObject::getIndex() { return this->index; }
// 3 Colors RGB
int GameObject::getIndexCount() { return ARRAY_SIZE(index) / 3; }