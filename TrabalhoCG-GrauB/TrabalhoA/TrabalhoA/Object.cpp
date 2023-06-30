#include "Object.hpp"

void Object::initialize(string filePath, string* texPaths, Shader* shader, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis)
{
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->axis = axis;
	this->shader = shader;
	this->texPaths = texPaths;

	loadObj(filePath);
}


void Object::update()
{
	glm::mat4 model = glm::mat4(1);
	model = glm::rotate(model, angle, axis);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	shader->setMat4("model", glm::value_ptr(model));
}

void Object::draw()
{
	for (int i = 0; i < grupos.size(); i++)
	{
		grupos[i].draw();
	}
}

void Object::loadObj(string filePath)
{
	int i = 0;

	glm::vec3 color;
	color.r = 1.0;  color.g = 0.0;  color.b = 0.0;


	vector <glm::vec3> vertices;
	vector <glm::vec3> colors;
	vector <GLuint> indices;
	vector <glm::vec2> texCoords;
	vector <glm::vec3> normals;
	vector <GLfloat> vbuffer;

	ifstream inputFile;
	ifstream inputMTLFile;
	string mtlPath;

	inputFile.open(filePath.c_str());
	if (inputFile.is_open())
	{
		char line[100];
		string sline;

		bool inicioGrupo = true;

		while (!inputFile.eof())
		{
			inputFile.getline(line, 100);
			sline = line;

			string word;

			istringstream ssline(line);
			ssline >> word;

			//cout << word << " ";
			if (word == "mtllib") {
				int startNameIndex = filePath.rfind("/");
				mtlPath = filePath.substr(0, startNameIndex);
				ssline >> word;
				mtlPath.append("/");
				mtlPath.append(word);
				cout << mtlPath << "\n";
			}
			if (word == "usemtl") {
				inputMTLFile.open(mtlPath);
				ssline >> word;

				char mtlLine[100];
				string mtlSline;

				while (!inputMTLFile.eof()){
					inputMTLFile.getline(mtlLine, 100);
					mtlSline = mtlLine;

					string mtlWord;

					istringstream mtlSsline(mtlLine);
					mtlSsline >> mtlWord;
					//cout << mtlWord << "\n";
					if (mtlWord == "newmtl" && !inputMTLFile.eof()) {
						mtlSsline >> mtlWord;
						cout << mtlWord << "\n";
						if (strcmp(mtlWord.c_str(), word.c_str()) == 0) {
							inputMTLFile.getline(mtlLine, 100);
							mtlSline = mtlLine;
							istringstream mtlSsline(mtlLine);
							mtlSsline >> mtlWord; 
							int stuckCount = 0;
							string stuckWord;

							while (mtlWord != "newmtl" && !inputMTLFile.eof()) {
								stuckWord = mtlWord;
								//cout << mtlWord << "\n";
								if(inicioGrupo) {
									if (vbuffer.size())
									{
										inicioGrupo = false;

										Mesh grupo;
										GLuint texID = loadTexture(texPaths[i]);
										i++;
										int nVerts;
										GLuint VAO = generateVAO(vbuffer, nVerts);
										grupo.initialize(VAO, nVerts, shader, texID);

										grupos.push_back(grupo);

										//Limpar o array auxiliar do buffer de geometria
										vbuffer.clear();
									}
								}

								if (mtlWord == "Ka") {
									cout << mtlSline << "\n";
									int kIndex = mtlSline.find("Ka");
									string kWord = mtlSline.substr((kIndex + 3), mtlSline.size());
									string kValue;
									bool endValue = false;
									int valueSize = kWord.find(" ") == -1 ? kWord.size() : kWord.find(" ");
									kValue = kWord.substr(0, valueSize);
									cout << kValue << "\n";
									if (grupos.size() > i - 1)
										grupos[i-1].ka = std::stof(kValue);
								}

								if (mtlWord == "Kd") {
									cout << mtlSline << "\n";
									int kIndex = mtlSline.find("Kd");
									string kWord = mtlSline.substr((kIndex + 3), mtlSline.size());
									string kValue;
									bool endValue = false;
									int valueSize = kWord.find(" ") == -1 ? kWord.size() : kWord.find(" ");
									kValue = kWord.substr(0, valueSize);
									cout << kValue << "\n";
									if(grupos.size() > i - 1)
										grupos[i - 1].kd = std::stof(kValue);
								}

								if (mtlWord == "Ks") {
									cout << mtlSline << "\n";
									int kIndex = mtlSline.find("Ks");
									string kWord = mtlSline.substr((kIndex + 3), mtlSline.size());
									string kValue;
									bool endValue = false;
									int valueSize = kWord.find(" ") == -1 ? kWord.size() : kWord.find(" ");
									kValue = kWord.substr(0, valueSize);
									cout << kValue << "\n";
									if (grupos.size() > i - 1)
										grupos[i - 1].ks = std::stof(kValue);
								}
								inputMTLFile.getline(mtlLine, 100);
								mtlSline = mtlLine;
								istringstream newMtlSsline(mtlLine);
								newMtlSsline >> mtlWord;
								if (strcmp(stuckWord.c_str(), mtlWord.c_str()) == 0) {
									stuckCount++;
									if (stuckCount >= 4) {
										cout << mtlSline << "\n";
									}
								} else {
									stuckCount = 0;
								}
							}
						}
					}
				}
				inputMTLFile.close();
			}

			if (word == "v" || inputFile.eof())
			{
				if (inicioGrupo)
				{
					if (vbuffer.size())
					{
						inicioGrupo = false;

						Mesh grupo;
						GLuint texID = loadTexture(texPaths[i]);
						i++;
						int nVerts;
						GLuint VAO = generateVAO(vbuffer, nVerts);
						grupo.initialize(VAO, nVerts, shader, texID);

						grupos.push_back(grupo);

						//Limpar o array auxiliar do buffer de geometria
						vbuffer.clear();
					}
				}

				glm::vec3 v;
				ssline >> v.x >> v.y >> v.z;
				vertices.push_back(v);
				colors.push_back(color);
			}
			if (word == "vt")
			{
				glm::vec2 vt;

				ssline >> vt.s >> vt.t;

				texCoords.push_back(vt);
			}
			if (word == "vn")
			{
				glm::vec3 vn;

				ssline >> vn.x >> vn.y >> vn.z;

				normals.push_back(vn);
			}

			if (word == "g")
			{
				inicioGrupo = true;
			}

			if (word == "f")
			{
				string tokens[3];

				ssline >> tokens[0] >> tokens[1] >> tokens[2];

				for (int i = 0; i < 3; i++)
				{
					//Recuperando os indices de v
					int pos = tokens[i].find("/");
					string token = tokens[i].substr(0, pos);
					int index = atoi(token.c_str()) - 1;
					indices.push_back(index);

					vbuffer.push_back(vertices[index].x);
					vbuffer.push_back(vertices[index].y);
					vbuffer.push_back(vertices[index].z);
					vbuffer.push_back(colors[index].r);
					vbuffer.push_back(colors[index].g);
					vbuffer.push_back(colors[index].b);

					//Recuperando os indices de vts
					tokens[i] = tokens[i].substr(pos + 1);
					pos = tokens[i].find("/");
					token = tokens[i].substr(0, pos);
					index = atoi(token.c_str()) - 1;

					vbuffer.push_back(texCoords[index].s);
					vbuffer.push_back(texCoords[index].t);

					//Recuperando os indices de vns
					tokens[i] = tokens[i].substr(pos + 1);
					index = atoi(tokens[i].c_str()) - 1;

					vbuffer.push_back(normals[index].x);
					vbuffer.push_back(normals[index].y);
					vbuffer.push_back(normals[index].z);
				}
			}

		}

	}
	else
	{
		cout << "Problema ao encontrar o arquivo " << filePath << endl;
	}
	inputFile.close();

}


GLuint Object::generateVAO(vector<GLfloat> vbuffer, int& nVerts)
{
	GLuint VBO, VAO;

	nVerts = vbuffer.size() / 11; //Provisório

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, vbuffer.size() * sizeof(GLfloat), vbuffer.data(), GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo coordenada de textura (s, t)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Atributo normal do vértice (x, y, z)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);


	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

GLuint Object::loadTexture(string filePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
				data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}


Shader* Object::getShader() {
	return shader;
}
glm::vec3 Object::getPosition() {
	return position;
}
glm::vec3 Object::getScale() {
	return scale;
}
glm::vec3 Object::getAxis() {
	return axis;
}
float Object::getAngle() {
	return angle;
}

void Object::setShader(Shader* newShader) {
	shader = newShader;
}
void Object::setPosition(glm::vec3 newPosition) {
	position = newPosition;
}
void Object::setScale(glm::vec3 newScale) {
	scale = newScale;
}
void Object::setAxis(glm::vec3 newAxis) {
	axis = newAxis;
}
void Object::setAngle(float newAngle) {
	angle = newAngle;
}