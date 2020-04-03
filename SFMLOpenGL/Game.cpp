#include <Game.h>

#include <Cube.h>

#include <Easing.h>



// Helper to convert Number to String for HUD

template <typename T>

string toString(T number)

{

	ostringstream oss;

	oss << number;

	return oss.str();

}



GLuint	vsid,		// Vertex Shader ID

fsid,		// Fragment Shader ID

progID,		// Program ID

vao = 0,	// Vertex Array ID

vbo,		// Vertex Buffer ID

vib,		// Vertex Index Buffer

to[1];		// Texture ID

GLint	positionID,	// Position ID

colorID,	// Color ID

textureID,	// Texture ID

uvID;		// UV ID



//Please see .//Assets//Textures// for more textures

const string filename = "pattern.tga";





int width;						// Width of texture

int height;						// Height of texture

int comp_count;					// Component of texture



unsigned char* img_data;		// image data

unsigned char* img_dataPat;		// image data



mat4 mvp, projection, view(1.f), model;			// View, Projection



Font font;						// Game font



Game::Game() :

	window(VideoMode(800, 600),

		"Introduction to OpenGL Texturing")

{



}



Game::Game(sf::ContextSettings settings) :

	window(VideoMode(800, 600),

		"Introduction to OpenGL Texturing",

		sf::Style::Default,

		settings)

{

	if (!m_backgroundTexture.loadFromFile(".//Assets//Backgrounds//OIP.jpg"))

	{

		std::cout << "Error loading Background texture" << std::endl;

	}

	if (!m_lightningTexture.loadFromFile(".//Assets//Backgrounds//Lightning.jpg"))

	{

		std::cout << "Error loading Lightning texture" << std::endl;

	}

	m_backgroundSprite.setTexture(m_backgroundTexture);

	m_lightningSprite.setTexture(m_lightningTexture);

}



Game::~Game()

{

}



// Constant game loop used so it will have the rate of movement on whatever machine it runs on

void Game::run()

{

	initialize();

	sf::Clock gameClock;											// Game clock

	sf::Time timeTakenForUpdate = sf::Time::Zero;					// No lag on first input

	sf::Time timePerFrame = sf::seconds(1.f / 60.f);				// 60 frames per second

	Event event;

	while (window.isOpen())									// Loop

	{

		while (window.pollEvent(event))

		{

			if (event.type == Event::Closed)

			{

				isRunning = false;

			}		// Zoom function used in some of the views

			if (event.mouseButton.button == sf::Mouse::Left)

			{

				if (sf::Event::MouseButtonPressed == event.type)

				{

					mouseClick = sf::Mouse::getPosition(window); // get the mouse click position in the window



					m_lightning[0].position = (sf::Vector2f{ static_cast<float>(window.getSize().x / 2), 10.0f }); // the first cell of the array starts at the top of window

					m_lightning[1].position = (sf::Vector2f{ mouseClick }); // the second cell of the array ends at the mouse click



				}

			}

		}// Check for input

		timeTakenForUpdate += gameClock.restart();					// Returns time take to do the loop

		while (timeTakenForUpdate > timePerFrame)					// Update enough times to catch up on updates missed during the lag time

		{

			timeTakenForUpdate -= timePerFrame;						// Decrement lag

			//processInput();											// More checks, the more accurate input to display will be

			update();									// Update

		}

		render();

	}

}







void Game::initialize()

{

	isRunning = true;

	GLint isCompiled = 0;

	GLint isLinked = 0;



	if (!(!glewInit())) { DEBUG_MSG("glewInit() failed"); }



	// Copy UV's to all faces

	for (int i = 1; i < 6; i++)

		memcpy(&uvs[i * 4 * 2], &uvs[0], 2 * 4 * sizeof(GLfloat));



	// Vertex Array Buffer

	glGenBuffers(1, &vbo);		// Generate Vertex Buffer

	glBindBuffer(GL_ARRAY_BUFFER, vbo);



	// Vertices (3) x,y,z , Colors (4) RGBA, UV/ST (2)

	glBufferData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS) + (2 * UVS)) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);



	glGenBuffers(1, &vib); //Generate Vertex Index Buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);



	// Indices to be drawn

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);



	// NOTE: uniforms values must be used within Shader so that they 

	// can be retreived

	const char* vs_src =

		"#version 400\n\r"

		""

		"in vec3 sv_position;"

		"in vec4 sv_color;"

		"in vec2 sv_uv;"

		""

		"out vec3 position;"

		"out vec4 color;"

		"out vec2 uv;"

		""

		"uniform mat4 sv_mvp;"

		"uniform float sv_x_offset;"

		"uniform float sv_y_offset;"

		"uniform float sv_z_offset;"

		""



		"uniform mat4 ModelMatrix;"

		"uniform mat4 ViewMatrix;"

		"uniform mat4 ProjectionMatrix;"



		"void main() {"

		"   position = vec4(ModelMatrix * vec4(sv_position, 1.f)).xyz;"

		"	color = sv_color;"

		"	uv = sv_uv;"

		"	gl_Position = ProjectionMatrix * ViewMatrix *  ModelMatrix  * vec4(sv_position, 1.f);"

		"}"; //Vertex Shader Src



	DEBUG_MSG("Setting Up Vertex Shader");



	vsid = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL);

	glCompileShader(vsid);



	// Check is Shader Compiled

	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);



	if (isCompiled == GL_TRUE) {

		DEBUG_MSG("Vertex Shader Compiled");

		isCompiled = GL_FALSE;

	}

	else

	{

		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");

	}



	const char* fs_src =

		"#version 400\n\r"

		""

		"uniform sampler2D f_texture;"

		""

		"in vec4 color;"

		"in vec2 uv;"

		""

		"out vec4 fColor;"

		""

		"void main() {"

		"	fColor = color - texture2D(f_texture, uv);"

		""

		"}"; //Fragment Shader Src



	fsid = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);

	glCompileShader(fsid);



	// Check is Shader Compiled

	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);



	progID = glCreateProgram();

	glAttachShader(progID, vsid);

	glAttachShader(progID, fsid);

	glLinkProgram(progID);



	// Check is Shader Linked

	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);



	// Set image data

	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);





	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &to[0]);

	glBindTexture(GL_TEXTURE_2D, to[0]);



	// Wrap around

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);



	// Filtering

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	// Bind to OpenGL

	glTexImage2D(

		GL_TEXTURE_2D,			// 2D Texture Image

		0,						// Mipmapping Level 

		GL_RGBA,				// GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 

		width,					// Width

		height,					// Height

		0,						// Border

		GL_RGBA,				// Bitmap

		GL_UNSIGNED_BYTE,		// Specifies Data type of image data

		img_data				// Image Data

	);



	// Projection Matrix 

	projection = glm::perspective(

		glm::radians(45.0f),					// Field of View 45 degrees

		4.0f / 3.0f,			// Aspect ratio

		0.1f,					// Display Range Min : 0.1f unit

		1500.0f					// Display Range Max : 100.0f unit

	);



	// Camera Matrix

	view = glm::lookAt(

		glm::vec3(0.0f, 10.0f, 20.0f),	// Camera (x,y,z), in World Space

		glm::vec3(0.0f, 0.0f, 0.0f),		// Camera looking at origin

		glm::vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up

	);





	// enemy blocks

	for (int i = 0; i < 50; i++)

	{

		m_block[i].objectPosition = vec3{ 12.f + i * 200,-8.f,0.f };

		m_block[i].model = glm::translate(m_block[i].model, m_block[i].objectPosition);

		m_block[i].objectRotation = { 60.f, 0.f, 10.f };

		m_block[i].model = glm::rotate(m_block[i].model, glm::radians(m_block[i].objectRotation.x), glm::vec3(1.f, 0.f, 0.f));

		m_block[i].model = glm::rotate(m_block[i].model, glm::radians(m_block[i].objectRotation.y), glm::vec3(0.f, 1.0f, 0.f));

		m_block[i].model = glm::rotate(m_block[i].model, glm::radians(m_block[i].objectRotation.z), glm::vec3(0.f, 0.f, 1.f));

	}



	// Ground rectangle block

	for (int i = 0; i < 4; i++)

	{

		m_ground[i].model = glm::mat4(1.f);

		m_ground[i].objectRotation = { 0.f, -180.f, -180.f };

		m_ground[i].objectPosition = vec3{ 0.f + i * 200,-11.f,0.f };

		m_ground[i].m_size = 100;

		m_ground[i].model = glm::translate(m_ground[i].model, m_ground[i].objectPosition);

		m_ground[i].model = glm::rotate(m_ground[i].model, glm::radians(m_ground[i].objectRotation.x), glm::vec3(1.f, 0.f, 0.f));

		m_ground[i].model = glm::rotate(m_ground[i].model, glm::radians(m_ground[i].objectRotation.y), glm::vec3(0.f, 1.0f, 0.f));

		m_ground[i].model = glm::rotate(m_ground[i].model, glm::radians(m_ground[i].objectRotation.z), glm::vec3(0.f, 0.f, 1.f));

	}





	// Enable Depth Test

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);



	// Load Font

	font.loadFromFile("BAUHS93.ttf");

}



void Game::update()

{

#if (DEBUG >= 2)

	DEBUG_MSG("Updating...");

#endif





	// Controls all input for the game except camera changes

	handleMovement();



}



void Game::render()

{

	//window.clear(sf::Color(255.f, 255.f, 255.f, 255.f));		//   Reduces it to its base color (funky)

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save current OpenGL render states

	window.pushGLStates();



	m_backgroundSprite.setPosition(0.0f, 0.0f);

	window.draw(m_backgroundSprite);



	window.draw(m_lightning); // draw the line for the pool cue



	// Find mouse position using sf::Mouse

	int x = Mouse::getPosition(window).x;

	int y = Mouse::getPosition(window).y;



	string hud = "Your position in window ["

		+ string(toString(x))

		+ "]["

		+ string(toString(y))

		+ "]";



	Text text(hud, font);



	text.setFillColor(sf::Color(255, 255, 255, 170));

	text.setPosition(50.f, 50.f);



	window.draw(text);







	// Restore OpenGL render states

	// https://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderTarget.php#a8d1998464ccc54e789aaf990242b47f7



	window.popGLStates();



	// Rebind Buffers and then set SubData

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);



	// Use Progam on GPU

	glUseProgram(progID);



	// Find variables within the shader

	positionID = glGetAttribLocation(progID, "sv_position");

	if (positionID < 0) { DEBUG_MSG("positionID not found"); }



	colorID = glGetAttribLocation(progID, "sv_color");

	if (colorID < 0) { DEBUG_MSG("colorID not found"); }



	uvID = glGetAttribLocation(progID, "sv_uv");

	if (uvID < 0) { DEBUG_MSG("uvID not found"); }



	textureID = glGetUniformLocation(progID, "f_texture");

	if (textureID < 0) { DEBUG_MSG("textureID not found"); }





	// Set Active Texture .... 32 GL_TEXTURE0 .... GL_TEXTURE31

	glActiveTexture(GL_TEXTURE0);

	glUniform1i(textureID, 0); // 0 .... 31



	// Set the X, Y and Z offset (this allows for multiple cubes via different shaders)

	// Experiment with these values to change screen positions

	// Set pointers for each parameter (with appropriate starting positions)

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, (VOID*)(3 * VERTICES * sizeof(GLfloat)));

	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (VOID*)(((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat)));



	// Enable Arrays

	glEnableVertexAttribArray(positionID);

	glEnableVertexAttribArray(colorID);

	glEnableVertexAttribArray(uvID);



	// View and projection updated once

	glUniformMatrix4fv(glGetUniformLocation(progID, "ViewMatrix"), 1, GL_FALSE, &view[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(progID, "ProjectionMatrix"), 1, GL_FALSE, &projection[0][0]);



	// Drawing object blocks

	for (int i = 0; i < 50; i++)

	{

		glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), m_block[i].getVertex());	// They are all the same size

		glUniformMatrix4fv(glGetUniformLocation(progID, "ModelMatrix"), 1, GL_FALSE, &m_block[i].model[0][0]);

		glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);

	}



	// Drawing Ground rectangles

	for (int i = 0; i < 4; i++)

	{

		glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), m_ground[i].getVertex());	// They are all the same size

		glUniformMatrix4fv(glGetUniformLocation(progID, "ModelMatrix"), 1, GL_FALSE, &m_ground[i].model[0][0]);

		glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);

	}



	//for (int i = 0; i < 4; i++)

	//{

	//	glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), m_block[i].getVertex());	// They are all the same size

	//	glUniformMatrix4fv(glGetUniformLocation(progID, "ModelMatrix"), 1, GL_FALSE, &m_block[i].model[0][0]);

	//	glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);

	//}

	window.display();



	// Disable Arrays

	glDisableVertexAttribArray(positionID);

	glDisableVertexAttribArray(colorID);

	glDisableVertexAttribArray(uvID);



	// Unbind Buffers with 0 (Resets OpenGL States...important step)

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	// Reset the Shader Program to Use

	glUseProgram(0);

}



void Game::unload()

{

	glDetachShader(progID, vsid);	// Shader could be used with more than one progID

	glDetachShader(progID, fsid);	// ..

	glDeleteShader(vsid);			// Delete Vertex Shader

	glDeleteShader(fsid);			// Delete Fragment Shader

	glDeleteProgram(progID);		// Delete Shader

	glDeleteBuffers(1, &vbo);		// Delete Vertex Buffer

	glDeleteBuffers(1, &vib);		// Delete Vertex Index Buffer

	stbi_image_free(img_data);		// Free image stbi_image_free(..)

}



void Game::handleMovement()

{

	int x = 10;

	for (int i = 0; i < 50; )

	{

		m_block[i].objectPosition = vec3{ m_block[i].objectPosition.x + x, m_block[i].objectPosition.y, m_block[i].objectPosition.z };

		timer--;

		if (timer <= 0)

		{

			timer = 100;

			i++;

		}

		break;

	}



}









void Game::setupEnemies()

{

	for (int i = 0; i < 50; i++)

	{

		m_block[i].model = glm::mat4(1.f);

		m_block[i].model = glm::translate(m_block[i].model, m_block[i].objectPosition);

		m_block[i].model = glm::rotate(m_block[i].model, glm::radians(m_block[i].objectRotation.x), glm::vec3(1.f, 0.f, 0.f));

		m_block[i].model = glm::rotate(m_block[i].model, glm::radians(m_block[i].objectRotation.y), glm::vec3(0.f, 1.0f, 0.f));

		m_block[i].model = glm::rotate(m_block[i].model, glm::radians(m_block[i].objectRotation.z), glm::vec3(0.f, 0.f, 1.f));

		m_block[i].model = glm::scale(m_block[i].model, m_block[i].objectScale);

	}

}







bool Game::checkCollision(GameObject& gbj)

{

	int x = Mouse::getPosition(window).x;

	int y = Mouse::getPosition(window).y;

	if (x >= gbj.objectPosition.x - 10 || y <= gbj.objectPosition.y + 10)

	{

		return true;

	}

	else

		return false;

}