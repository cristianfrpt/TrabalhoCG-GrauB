#include "SceneManager.hpp"



Camera* camera;
Shader* shader;
Shader* shaderSimpleObject;

int objectSelected = -1;
int transformationType = -1;
int direction = -1;

void SceneManager::initialize() {
	glfwInit();
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho A", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, &SceneManager::key_callback);
	glfwSetCursorPosCallback(window, &SceneManager::mouse_callback);
	glfwSetScrollCallback(window, &SceneManager::mouse_scroll_callback);
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	

	loadConfig("../../Config/config.txt");

	// GLAD: carrega todos os ponteiros d fun??es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	glm::mat4 startProjection = glm::perspective(glm::radians(fov), (float)width / (float)height, zNear, zFar);

	camera = new Camera(startPosition, startFront, startUp, fov, zNear, zFar, startView, startProjection, cameraSpeed);
	reader = new ObjectReader();
	shader = new Shader("Phong.vs", "Phong.fs");
	objects = new Object[20];


	initializeObjects();
	printf("Objetos carregados\n");

	initializeShader(shader, width, height);


	bezier.setControlPoints(circlePoints);
	bezier.setShader(shader);
	bezier.generateCurve(1000);
	curvePoints = bezier.getNbCurvePoints();
	currentCurvePoint = 1600;

	glEnable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->ID, "colorBuffer"), 0);

	return;
};

int SceneManager::run() {
	while (!glfwWindowShouldClose(window)) {
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun??es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.9f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);


		updateSun();
		updateCamera();
		doTransformation();
		updateObjects();

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

void SceneManager::loadConfig(string filepath) {
	ifstream inputFile;
	ifstream inputMTLFile;
	string mtlPath;
	bool end = false;
	inputFile.open(filepath.c_str());
	if (inputFile.is_open())
	{
		char line[200];
		string sline;
		
		while (!inputFile.eof() && !end)
		{
			
			inputFile.getline(line, 200);
			sline = line;

			//cout << sline << "\n";
			string word;

			istringstream ssline(line);
			ssline >> word;
			if (word == "stPos") {
				string values;
				ssline >> values;
				cout << word;
				startPosition = readVec3(values);
			}

			if (word == "stFrnt") {
				string values;
				ssline >> values;
				cout << word;
				startFront = readVec3(values);
			}

			if (word == "stUp") {
				string values;
				ssline >> values;
				cout << word;
				startUp = readVec3(values);
			}

			if (word == "stView") {
				string values;
				ssline >> values;
				cout << word;
				startView = readMat4View(values);
			}

			if (word == "fov") {
				string value;
				ssline >> value;
				fov = std::stof(value);
				cout << word << " " << value<< " \n";

			}

			if (word == "zNear") {
				string value;
				ssline >> value;
				zNear = std::stof(value);
				cout << word << " " << value << " \n";
			}

			if (word == "zFar") {
				string value;
				ssline >> value;
				zFar = std::stof(value);
				cout << word << " " << value << " \n";
			}

			if (word == "camSpd") {
				string value;
				ssline >> value;
				cameraSpeed = std::stof(value);
				cout << word << " " << value << " \n";
			}
			if (word == "end") {
				end = true;
			}

			if (word == "circle") {
				string values;
				ssline >> values;
				circlePoints = readCircle(values);

			}
		}
	}
	else
	{
		cout << "Problema ao encontrar o arquivo " << filepath << endl;
	}

	inputFile.close();
}

glm::vec3 SceneManager::readVec3(string s) {
	std::string delimiter = "/";
	float values[3] = {0,0,0};
	int k = 0;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		values[k] = std::stof(token);
		s.erase(0, pos + delimiter.length());
		k++;
	}
	printf(" %f-%f-%f\n", values[0], values[1], values[2]);
	return glm::vec3(values[0], values[1], values[2]);
}

glm::mat4 SceneManager::readMat4View(string s) {
	std::string delimiter = "/";
	float values[9] = { 0,0,0,0,0,0,0,0,0 };
	int k = 0;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		values[k] = std::stof(token);
		s.erase(0, pos + delimiter.length());
		k++;
	}
	printf(" %f-%f-%f-%f-%f-%f-%f-%f-%f\n", values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8]);
	return glm::lookAt(glm::vec3(values[0], values[1], values[2]), glm::vec3(values[3], values[4], values[5]), glm::vec3(values[6], values[7], values[8]));
}

std::vector<glm::vec3> SceneManager::readCircle(string s) {
	std::string delimiter = "/";
	float values[13*3] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int j = 0;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		values[j] = std::stof(token);
		s.erase(0, pos + delimiter.length());
		j++;
	}

	vector <glm::vec3> points;

	for (int i = 1; i < 13 * 3; i += 3)
	{
		glm::vec3 point;
		point.x = 0.0;
		point.y = values[i];
		point.z = values[i + 1];

		points.push_back(point);
	}

	return points;
}

void SceneManager::updateSun() {
	glm::vec3 currentPointOnCurve = bezier.getPointOnCurve(currentCurvePoint);
	currentSunPos = glm::vec3(50.0, 50.0 * currentPointOnCurve.y, 50* currentPointOnCurve.z);

	controlCurveAceleration++;
		currentCurvePoint = (currentCurvePoint + 1) % curvePoints;
		controlCurveAceleration = 0;

	objects[SUN_ID].setPosition(currentSunPos);
	shader->setVec3("lightPos", currentSunPos.x, currentSunPos.y, currentSunPos.z);
}

void SceneManager::updateCamera() {
	glm::mat4 model = glm::mat4(1);
	shader->setMat4("model", glm::value_ptr(model));
	camera->updateView();

	glm::mat4 view = camera->getView();
	shader->setMat4("view", glm::value_ptr(view));
	shader->setVec3("cameraPos", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
}

void SceneManager::updateObjects() {
	for(int i=0; i<12; i++){
			objects[i].update();
			objects[i].draw();
	}
}

void SceneManager::initializeObjects() {
	initializeFloor();
	initializeLeftWall();
	initializeRightWall();
	initializeBackWall();
	initializeFrontWall();
	initializeWindow();
	initializeCeiling();
	initializeBed();
	initializeSkybox();
	initializeSun();
	initializeWardrobe();
	initializeChair();
}

void SceneManager::initializePikachu() {
	string texPaths[] = { "../../3D_models/Pokemon/textures/PikachuMouthDh.png",
						 "../../3D_models/Pokemon/textures/PikachuDh.png",
						 "../../3D_models/Pokemon/textures/PikachuHohoDh.png",
						 "../../3D_models/Pokemon/textures/PikachuEyeDh.png",
						 "../../3D_models/Pokemon/textures/PikachuDh.png" };

	objects[PIKACHU_ID].initialize("../../3D_models/Pokemon/Pikachu.obj", texPaths, shader);
}

void SceneManager::initializeFloor() {
	string texPaths[] = { "../../3D_models/Cube/textures/WoodFloor.jpg" };
	objects[FLOOR_ID].initialize("../../3D_models/Cube/Cube.obj", texPaths, shader);
	objects[FLOOR_ID].setScale(glm::vec3(16, 0.02, 10));
	objects[FLOOR_ID].setPosition(glm::vec3(0, 0, 0));
}

void SceneManager::initializeLeftWall() {
	string texPaths[] = { "../../3D_models/Cube/textures/whitewall.png" };
	objects[LEFT_WALL_ID].initialize("../../3D_models/Cube/Cube.obj", texPaths, shader);
	objects[LEFT_WALL_ID].setScale(glm::vec3(0.1, 10, 10));
	objects[LEFT_WALL_ID].setPosition(glm::vec3(-8, 5, 0));
}

void SceneManager::initializeRightWall() {
	string texPaths[] = { "../../3D_models/Cube/textures/whitewall.png" };
	objects[RIGHT_WALL_ID].initialize("../../3D_models/Cube/Cube.obj", texPaths, shader);
	objects[RIGHT_WALL_ID].setScale(glm::vec3(0.1, 10, 10));
	objects[RIGHT_WALL_ID].setPosition(glm::vec3(8, 5, 0));
}

void SceneManager::initializeBackWall() {
	string texPaths[] = {"../../3D_models/Cube/textures/whitewall.png"};
	objects[BACK_WALL_ID].initialize("../../3D_models/Cube/Cube.obj", texPaths, shader);
	objects[BACK_WALL_ID].setScale(glm::vec3(16, 10, 0.02));
	objects[BACK_WALL_ID].setPosition(glm::vec3(0, 5, 5));
}

void SceneManager::initializeFrontWall() {
	string texPaths[] = {"../../3D_models/Cube/textures/bluewall.jpg"};
	objects[FRONT_WALL_ID].initialize("../../3D_models/Janela/frontwall.obj", texPaths, shader);
	objects[FRONT_WALL_ID].setPosition(glm::vec3(-0.75, 3.7, -2.9));
}

void SceneManager::initializeWindow() {
	string texPaths[] = { "../../3D_models/Cube/textures/whitewall.png" };
	objects[WINDOW_ID].initialize("../../3D_models/Janela/janela9.obj", texPaths, shader);
	objects[WINDOW_ID].setPosition(glm::vec3(-0.75, 3.7, -2.9));
}

void SceneManager::initializeCeiling(){
	string texPaths[] = { "../../3D_models/Cube/textures/whitewall.png" };
	objects[CEILING_ID].initialize("../../3D_models/Cube/Cube.obj", texPaths, shader);
	objects[CEILING_ID].setScale(glm::vec3(16, 0.02, 10));
	objects[CEILING_ID].setPosition(glm::vec3(0, 10, 0));
}

void SceneManager::initializeBed() {
	string texPaths[] = { "../../3D_models/Bed/textures/bed_albedo.jpeg",
							 "../../3D_models/Bed/textures/blanket_albedo.jpeg",
							 "../../3D_models/Bed/textures/Matress_albedo.jpeg" ,
							 "../../3D_models/Bed/textures/Pillow_albedo.jpeg" };
	objects[BED_ID].initialize("../../3D_models/Bed/bed4.obj", texPaths, shader);
	objects[BED_ID].setPosition(glm::vec3(-3.2, 0, -2.4));
	objects[BED_ID].setScale(glm::vec3(1, 1, 1));
}

void SceneManager::initializeSkybox() {
	string texPaths[] = { "../../3D_models/Skybox/textures/skybox2.jpg" };
	objects[SKYBOX_ID].initialize("../../3D_models/Skybox/skybox.obj", texPaths, shader);
	objects[SKYBOX_ID].setScale(glm::vec3(55, 55, 55));
}

void SceneManager::initializeSun() {
	string texPaths[] = { "../../3D_models/Sun/textures/sun.jpg" };
	objects[SUN_ID].initialize("../../3D_models/Sun/sol.obj", texPaths, shader);
	objects[SUN_ID].setScale(glm::vec3(2, 2, 2));
	objects[SUN_ID].setPosition(glm::vec3(50, 50, 0));
}

void SceneManager::initializeWardrobe() {
	string texPaths[] = { "../../3D_models/Wardrobe/textures/wardrobe.jpg",
							 "../../3D_models/Wardrobe/textures/whitewall.png",
							 "../../3D_models/Wardrobe/textures/whitewall.png"  };
	objects[WARDROBE_ID].initialize("../../3D_models/Wardrobe/wardrobe.obj", texPaths, shader);
	objects[WARDROBE_ID].setScale(glm::vec3(1, 1,0.99));
	objects[WARDROBE_ID].setPosition(glm::vec3(-3.25, 0, -2.45));
}

void SceneManager::initializeChair() {
	string texPaths[] = { "../../3D_models/Chair/textures/white.png",
							 "../../3D_models/Chair/textures/black.jpg",
							 "../../3D_models/Chair/textures/black.jpg",
							 "../../3D_models/Chair/textures/black.jpg",
							 "../../3D_models/Chair/textures/black.jpg",
							 "../../3D_models/Chair/textures/black.jpg",
							 "../../3D_models/Chair/textures/black.jpg",
							 "../../3D_models/Chair/textures/black.jpg",
							 "../../3D_models/Chair/textures/black.jpg",
							 "../../3D_models/Chair/textures/black.jpg",
							 "../../3D_models/Chair/textures/black.jpg",
							 "../../3D_models/Chair/textures/black.jpg" };
	objects[CHAIR_ID].initialize("../../3D_models/Chair/chair.obj", texPaths, shader);
	objects[CHAIR_ID].setPosition(glm::vec3(-3.25, 0, -2.45));
	objects[CHAIR_ID].setScale(glm::vec3(1, 1, 1));
}

void SceneManager::doTransformation() {
	float angle = (GLfloat)glfwGetTime();
	if (objectSelected >= 0) {
		if (transformationType == 0) {
			if (direction == 0) {
				objects[objectSelected].setPosition(glm::vec3((objects[objectSelected].getPosition().x * 1.01), objects[objectSelected].getPosition().y, objects[objectSelected].getPosition().z));
			} else if (direction == 1) {
				objects[objectSelected].setPosition(glm::vec3(objects[objectSelected].getPosition().x, objects[objectSelected].getPosition().y, (objects[objectSelected].getPosition().z * 1.01)));
			} else if (direction == 2) {
				objects[objectSelected].setPosition(glm::vec3(objects[objectSelected].getPosition().x, objects[objectSelected].getPosition().y, (objects[objectSelected].getPosition().z * 0.99)));
			} else if (direction == 3) {
				objects[objectSelected].setPosition(glm::vec3((objects[objectSelected].getPosition().x * 0.99), objects[objectSelected].getPosition().y, objects[objectSelected].getPosition().z));
			}
		}
		else if (transformationType == 1) {
			
			if (direction == 0) {
				objects[objectSelected].setAngle(angle);
				objects[objectSelected].setAxis(glm::vec3(0.0, 1.0, 0.0));
			}
			else if (direction == 1) {
				objects[objectSelected].setAngle(angle);
				objects[objectSelected].setAxis(glm::vec3(-1.0, 0.0, 0.0));
			}
			else if (direction == 2) {
				objects[objectSelected].setAngle(angle);
				objects[objectSelected].setAxis(glm::vec3(1.0, 0.0, 0.0));
			}
			else if (direction == 3) {
				objects[objectSelected].setAngle(angle);
				objects[objectSelected].setAxis(glm::vec3(0.0, -1.0, 0.0));
			}
			else
			{
				objects[objectSelected].setAngle(0);
			}
		}
		else if (transformationType == 2) {
			if (direction == 1) {
				objects[objectSelected].setScale(glm::vec3((objects[objectSelected].getScale().x * 1.01), (objects[objectSelected].getScale().y * 1.01), (objects[objectSelected].getScale().z * 1.01)));
			}
			else if (direction == 2) {
				objects[objectSelected].setScale(glm::vec3((objects[objectSelected].getScale().x * 0.99), (objects[objectSelected].getScale().z * 0.99), (objects[objectSelected].getScale().z * 0.99)));
			}
		}
	}
}

void SceneManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE) {
		camera->moveUp();
	}
	if (key == GLFW_KEY_LEFT_CONTROL) {
		camera->moveDown();
	}
	if (key == GLFW_KEY_W) {
		camera->moveFoward();
	}
	if (key == GLFW_KEY_S) {
		camera->moveBack();
	}
	if (key == GLFW_KEY_A) {
		camera->moveLeft();
	}
	if (key == GLFW_KEY_D) {
		camera->moveRight();
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		if(objectSelected < 0){
			objectSelected = CHAIR_ID;
			direction = -1;
		}
		else if (objectSelected == CHAIR_ID) {
			objectSelected = BED_ID;
			direction = -1;
		}
		else {
			objectSelected = -1;
		}
		cout << "objectSelected: " << objectSelected << "\n";
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		if (transformationType != 0) {
			transformationType = 0;
			direction = -1;
		} else {
			transformationType = -1;
		}
		cout << "transformationType: " << transformationType << "\n";
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		if (transformationType != 1) {
			transformationType = 1;
			direction = -1;
		}
		else {
			transformationType = -1;
		}
		cout << "transformationType: " << transformationType << "\n";
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		if (transformationType != 2) {
			transformationType = 2;
			direction = -1;
		}
		else {
			transformationType = -1;
		}
		cout << "objectSelected: " << objectSelected << "\n";
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		if (direction != 0) {
			direction = 0;
		} else {
			direction = -1;
		}
		cout << "Direction: " << direction << "\n";
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (direction != 1) {
			direction = 1;
		}
		else {
			direction = -1;
		}
		cout << "Direction: " << direction << "\n";
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		if (direction != 2) {
			direction = 2;
		}
		else {
			direction = -1;
		}
		cout << "Direction: " << direction << "\n";
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		if (direction != 3) {
			direction = 3;
		}
		else {
			direction = -1;
		}
		cout << "Direction: " << direction << "\n";
	}
}

void SceneManager::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera->moveMouse(xpos, ypos);
}

void SceneManager::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0) {
		camera->moveFoward();
	}
	else {
		camera->moveBack();
	}
}

void SceneManager::initializeShader(Shader* shader, int width, int height) {

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shader->ID, "model");
	//Definindo a matriz de view (posi??o e orienta??o da c?mera)
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 6.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
	GLint viewLoc = glGetUniformLocation(shader->ID, "view");
	//Definindo a matriz de proje??o perpectiva
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");

	glUseProgram(shader->ID);
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	//Definindo as propriedades do material da superficie
	shader->setFloat("ka", 0.8);
	shader->setFloat("kd", 0.5);
	shader->setFloat("ks", 0.5);
	shader->setFloat("q", 5.0);
	//Definindo a fonte de luz pontual

	shader->setVec3("lightPos", 30, 30, 0);
	shader->setVec3("lightColor", 1.0, 1.0, 0.9);
}