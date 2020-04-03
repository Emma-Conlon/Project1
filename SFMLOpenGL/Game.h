#ifndef GAME_H

#define GAME_H



#include <string>

#include <sstream>



#include <iostream>

#include <GL/glew.h>

#include <GL/wglew.h>





#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>



#include <SFML/Graphics.hpp>

#include <SFML/OpenGL.hpp>



#include <Debug.h>



#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>



#include <GameObject.h>



using namespace std;

using namespace sf;

using namespace glm;



class Game

{

public:

	Game();

	Game(sf::ContextSettings settings);

	~Game();

	void run();



private:

	RenderWindow window;

	Clock clock;

	Time time;

	bool animate = false;

	vec3 animation = vec3(0.0f);

	float rotation = 0.0f;

	bool isRunning = false;

	void initialize();

	void update();

	void render();

	void unload();

	sf::Sprite m_backgroundSprite;

	sf::Texture m_backgroundTexture;

	sf::Sprite m_lightningSprite;

	sf::Texture m_lightningTexture;

	int timer = 100;

	Vector2i mouseClick{};

	sf::VertexArray m_lightning{ sf::Lines, 2U };



	// Functions to keep the code readable and easy to navigate

	void handleMovement();

	void setupEnemies();



	// Checks mouse colision with the cubes

	bool checkCollision(GameObject& gbj);



	// Game objects ------------------------

	// Value decides shape, 1 = Cube, 2 = Rectangle

	GameObject m_block[50]{ {1},{1},{1},{1},{1}, {1},{1},{1},{1},{1},

	{1},{1},{1},{1},{1}, {1},{1},{1},{1},{1} ,

	{1},{1},{1},{1},{1}, {1},{1},{1},{1},{1} ,

	{1},{1},{1},{1},{1}, {1},{1},{1},{1},{1} ,

	{1},{1},{1},{1},{1}, {1},{1},{1},{1},{1} };

	GameObject m_ground[4]{ 2,2,2,2 };





};
#endif
