#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

#include <stdio.h>
#include <string.h>
#include <Cube.h>
#include <Floor.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class GameObject
{
private:
	// Cube Elements
	GLfloat vertex[ARRAY_SIZE(vertices)];
	GLfloat vertexRec[72];
	GLfloat color[ARRAY_SIZE(colors)];
	GLfloat uv[ARRAY_SIZE(uvs)];
	GLfloat index[ARRAY_SIZE(indices)];
	vec3 position;
	int m_move;

public:
	GameObject(int t_type);
	~GameObject();

	// Cube info
	float m_size{ 2 };
	bool isHit{ false };
	int type{ 1 };

	// Cube transformations
	vec3 objectPosition{ 0.f };
	vec3 objectRotation{ 0.f };
	vec3 objectScale{ 1.f };

	mat4 model;

	vec3 getPosition();
	void setSize(float t_size);
	void setPosition(vec3 position);
	void setMove(int t_move);

	// Returns the first element of the array
	GLfloat* getVertex();
	// 3 Vertices
	int getVertexCount();
	int getMove();
	// Returns the first element of the array
	GLfloat* getColor();
	// 3 Colors RGB
	int getColorCount();

	// Returns the first element of the array
	GLfloat* getUV();
	// 3 Colors RGB
	int getUVCount();

	// Returns the first element of the array
	GLfloat* getIndex();
	// 3 Colors RGB
	int getIndexCount();
};

#endif // !GAME_OBJECT_H
