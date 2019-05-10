#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Mesh* mesh1();
	Mesh* mesh2();
	Mesh* mesh3();
	Texture* texture(); //load texture
	Texture* texture1(); //load texture
	Overlay* overlay(); //load texture
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 

	texture.init("..\\res\\bricks.jpg"); //load texture
	texture1.init("..\\res\\water.jpg"); //load texture

	shaderSkybox.init("..\\res\\shaderSky.vert", "..\\res\\shaderSky.frag", "");

	shaderExplosion.init("..\\res\\shaderExplosion.vert", "..\\res\\shaderExplosion.frag", "..\\res\\shaderExplosion.geom");
	shaderMapping.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag", "");
	customShader.init("..\\res\\customShader.vert", "..\\res\\customShader.frag", "..\\res\\customShader.geom");
	//customShader.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag", "");

	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\monkey3.obj");
	mesh3.loadModel("..\\res\\monkey3.obj");

	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
	
	vector<std::string> faces
	{
		"..\\res\\skybox\\bluecloud_ft.jpg",
		"..\\res\\skybox\\bluecloud_bk.jpg",
		"..\\res\\skybox\\bluecloud_up.jpg",
		"..\\res\\skybox\\bluecloud_dn.jpg",
		"..\\res\\skybox\\bluecloud_rt.jpg",
		"..\\res\\skybox\\bluecloud_lf.jpg"
	};
	cubemapTexture = skybox.loadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures

	float skyboxVertices[] = {
		// positions          
		-8.0f,  8.0f, -8.0f,
		-8.0f, -8.0f, -8.0f,
		8.0f, -8.0f, -8.0f,
		8.0f, -8.0f, -8.0f,
		8.0f,  8.0f, -8.0f,
		-8.0f,  8.0f, -8.0f,

		-8.0f, -8.0f,  8.0f,
		-8.0f, -8.0f, -8.0f,
		-8.0f,  8.0f, -8.0f,
		-8.0f,  8.0f, -8.0f,
		-8.0f,  8.0f,  8.0f,
		-8.0f, -8.0f,  8.0f,

		8.0f, -8.0f, -8.0f,
		8.0f, -8.0f,  8.0f,
		8.0f,  8.0f,  8.0f,
		8.0f,  8.0f,  8.0f,
		8.0f,  8.0f, -8.0f,
		8.0f, -8.0f, -8.0f,

		-8.0f, -8.0f,  8.0f,
		-8.0f,  8.0f,  8.0f,
		8.0f,  8.0f,  8.0f,
		8.0f,  8.0f,  8.0f,
		8.0f, -8.0f,  8.0f,
		-8.0f, -8.0f,  8.0f,

		-8.0f,  8.0f, -8.0f,
		8.0f,  8.0f, -8.0f,
		8.0f,  8.0f,  8.0f,
		8.0f,  8.0f,  8.0f,
		-8.0f,  8.0f,  8.0f,
		-8.0f,  8.0f, -8.0f,

		-8.0f, -8.0f, -8.0f,
		-8.0f, -8.0f,  8.0f,
		8.0f, -8.0f, -8.0f,
		8.0f, -8.0f, -8.0f,
		-8.0f, -8.0f,  8.0f,
		8.0f, -8.0f,  8.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}	
}

void MainGame::Skybox()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shaderSkybox.Bind();
	shaderSkybox.setInt("skybox", 0);
	//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	shaderSkybox.setMat4("view", myCamera.GetView());
	shaderSkybox.setMat4("projection", myCamera.GetProjection());
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	Skybox();

	//----------------------------------
	transform.SetPos(glm::vec3(4, 3, 1));
	transform.SetRot(glm::vec3(0.0, counter * 0.5, 0.0));
	transform.SetScale(glm::vec3(0.8, 0.8, 0.8));

	explosionCounter += 0.01f;
	shaderExplosion.Bind();
	shaderExplosion.setMat4("transform", myCamera.GetProjection() * myCamera.GetView() * transform.GetModel());
	shaderExplosion.setFloat("time", explosionCounter);
	texture.Bind(0);
	mesh1.draw();
	
	//--------------------------------

	transform.SetPos(glm::vec3(4, -2.0, 1));
	transform.SetRot(glm::vec3(0.0, counter * 0.5, 0.0));
	transform.SetScale(glm::vec3(1.5, 1.5, 1.5));

	shaderMapping.Bind();
	shaderMapping.setMat4("model", transform.GetModel());
	shaderMapping.setMat4("view", myCamera.GetView());
	shaderMapping.setMat4("projection", myCamera.GetProjection());
	shaderMapping.setVec3("cameraPos", myCamera.getPos());
	mesh2.draw();


	//--------------------------------

	transform.SetPos(glm::vec3(0.4, 0, 0));
	transform.SetRot(glm::vec3(0, counter * 0.5 , 0));
	transform.SetScale(glm::vec3(0.3, 0.3, 0.3));

	customShader.Bind();
	customShader.setMat4("model", transform.GetModel());
	customShader.setMat4("view", myCamera.GetView());
	customShader.setMat4("projection", myCamera.GetProjection());
	texture1.Bind(0);
	mesh3.draw();

	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 