/**
*
*	Numero de cuenta: 316060497
*	Proyecto Final de laboratorio de Computación Gráfica e Interación Humano Computadora
*	Grupo: 07
*
**/

// Incluyendo librerias basicas para el funcionamiento de C++ 
#include <iostream>
#include <cmath>

// Incluyendo la libreria basica para el funcionamiento de GLEW
#include <GL/glew.h>

// Incluyendo la libreria basica para el funcionamiento de GLFW
#include <GLFW/glfw3.h>

// Otras librerias 
#include "stb_image.h"

// Incluyendo librerias matematicas para  para el funcionamiento de GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Archivo de encabezado para la carga de modelos
#include "SOIL2/SOIL2.h"

// Archivos de encabezados para los shaders, camara y modelos 
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Prototipos de las funciones 
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Dimensiones de la ventana 
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-80.0f, 10.0f, 170.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Atributos de luz
glm::vec3 lightPos(1.0f, 1.0f, 1.0f);
bool active;



// Variables para la animacion de la trajinera
bool animTrajinera = false; // Bandera para activar la animacion de la trajinera
bool desplazaTrajineraBandera = false; // controla si se esta desplazandose
bool animTrajineraRev = false; // Bandera para reiniciar la animacion
float desplazaTrajinera = 0.0f; // guarda la distancia del desplazamiento

// Varibales para controlar los focos de las casas
bool luzCasa = false; // bandera que activar la luz de las casas
bool luzCasaRev = false; // Bandera para desactivar la luz de las casas

// Varibales para la aniamcion de la puerta (Puerta de principal):
bool animPuertaPrincipal = false; // bandera que activa la animacion de la puerta
bool BanderaPuertaFachada = false;  // controla si la puerta esta rotando
bool animPuertaPrincipalRev = false; // Bandera que sirve para controlar el reinicio de la animacion de la puerta
float rotaPuertaFacha = 0.0f; // guarda el angulo de rotacion de la puerta

// Variables para la animacion compleja 05 (Movimiento del pasto)
float tiempo = 1.0f;
float speed = 2.0f;

// Posiciones de los 4 pointlight
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f, 145.0f),
	glm::vec3(0.0f,0.0f, 145.0f),
	glm::vec3(0.0f,0.0f, 145.0f),
	glm::vec3(0.0f,0.0f, 145.0f)
};

glm::vec3 spotlightPositions(6.0f, 2.33f, -7.0f);// Posicion de la spotlight 
glm::vec3 spotlightDirection(0.0f, -8.0f, 0.0f);// Direccion de la spotlight



float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0.0f, 0.0f, 0.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Tiempo entre el frame actual y el último frame
GLfloat lastFrame = 0.0f;  	// Tiempo del último frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Crea un objeto GLFWwindow que podamos usar para las funciones de GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Establecer las funciones de devolución de llamada requeridas
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// Opciones de GLFW
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Se establece en verdadero para que GLEW sepa utilizar un enfoque moderno para recuperar punteros de funciones y extensiones.
	glewExperimental = GL_TRUE;
	// Inicializa GLEW para configurar los punteros de la función OpenGL
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define las dimesiones del viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp2.vs", "Shaders/lamp2.frag");
	Shader Anim01("Shaders/anim.vs", "Shaders/anim.frag");
	Shader Anim02("Shaders/anim2.vs", "Shaders/anim2.frag");

	// Modelos
	Model Mar((char*)"Models/Mundo/Agua/Agua_HV.obj");
	Model Mar2((char*)"Models/Mundo/Agua/Agua_AVATAR.obj");
	Model Piso((char*)"Models/Mundo/Piso/PastoyPiso.obj");
	Model ArbolAlgodon((char*)"Models/Mundo/ArbolAlgodon/ArbolAzucar.obj");
	Model CasaArbol((char*)"Models/Mundo/CasaArbol/Casa_arbol_HV.obj");
	Model BarcoRoto((char*)"Models/Mundo/BarcoMaderaRoto/Barco_roto.obj");
	Model Carreta((char*)"Models/Mundo/Carretas/Carreta.obj");
	Model TrajineraEstacionada01((char*)"Models/Mundo/Trajineras/Trajinera_estacionda/Trajinera_estacionada01.obj");
	Model TrajineraEstacionada02((char*)"Models/Mundo/Trajineras/Trajinera_estacionda/Trajinera_estacionada02.obj");
	Model TrajineraEstacionada03((char*)"Models/Mundo/Trajineras/Trajinera_estacionda/Trajinera_estacionada03.obj");
	Model TrajineraEstacionada04((char*)"Models/Mundo/Trajineras/Trajinera_estacionda/Trajinera_estacionada04.obj");
	Model TrajineraMoviendose((char*)"Models/Mundo/Trajineras/Trajinera_moviendose/Trajinera_movimiento.obj");
	Model PurplePentapus((char*)"Models/Mundo/PurplePentapus/Purple_pentapus.obj");
	Model CasaHV((char*)"Models/Mundo/CasaHV/CasaHV.obj");
	Model CasaAVATAR((char*)"Models/Mundo/CasaAVATAR/CasaAvatar.obj");
	Model Muralla((char*)"Models/Mundo/Muralla/Muralla.obj");
	Model MurallapuertaDER((char*)"Models/Mundo/Muralla/puerta_der.obj");
	Model MurallapuertaIZQ((char*)"Models/Mundo/Muralla/puerta_izq.obj");
	Model Lirio01((char*)"Models/Mundo/Flores/FloresTextura01.obj");
	Model Lirio02((char*)"Models/Mundo/Flores/FloresTextura02.obj");
	Model Lirio03((char*)"Models/Mundo/Flores/FloresTextura03.obj");
	Model Lirio04((char*)"Models/Mundo/Flores/FloresTextura04.obj");
	Model jake((char*)"Models/Mundo/AvatarJake/Modelo_jake_el_perro.obj");

	// Primero, se configura el VAO del contenedor (y VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Atributo de posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Atributo normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Establece las unidades de textura
	lightingShader.Use();

	// Establece el tipo de proyeccion
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop (Principal)
	while (!glfwWindowShouldClose(window))
	{

		// Calcula deltatime del frame actual
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Comprueba si se ha activado algún evento (tecla pulsada, movimiento del ratón, etc.) y llama a las funciones de respuesta correspondientes.
		glfwPollEvents();
		DoMovement();

		// Limpia el colorbuffer
		glClearColor(0.5294f, 0.8078f, 0.9216f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Opciones de OpenGL
		glEnable(GL_DEPTH_TEST);



		// Carga de modelos


		// Utiliza un Shades correspondient al configurar uniforms y al dibujar objetos
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Direcciones de la luz
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);

		// Point light 1
		glm::vec3 lightColor; // definimos el vector que contendra los valores para determinar el color de luz del Point light 
		lightColor.x = Light1.x;
		lightColor.y = Light1.y;
		lightColor.z = Light1.z;

		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f); // --> Color de la Pointlight
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 1.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 3.0f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 1.8f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 1.8f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 1.8f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), spotlightPositions.x, spotlightPositions.y, spotlightPositions.z); // Separamos la posicion de la  SpotLight de la camara
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), spotlightDirection.x, spotlightDirection.y, spotlightDirection.z); // Separamos la direccion de la  SpotLight de la camara
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f); // SIEMPRE ES UNO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 1.8f); 
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f))); // el angulo de adentro debe ser mas peqeño que el de afuera

		// Colocando las propiedades de los materiales
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Obtiene las ubicaciones de los uniforms
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pasa la matriz al shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		// Empieza la divertido: Carga y dibujo de los modelos propuestos
		view = camera.GetViewMatrix();
		

		// Carga y dibujo de la PointLight
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		

		// Modelos
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
		Piso.Draw(lightingShader);
		ArbolAlgodon.Draw(lightingShader);
		CasaArbol.Draw(lightingShader);
		BarcoRoto.Draw(lightingShader);
		Carreta.Draw(lightingShader);
		TrajineraEstacionada01.Draw(lightingShader);	
		TrajineraEstacionada02.Draw(lightingShader);
		TrajineraEstacionada03.Draw(lightingShader);
		TrajineraEstacionada04.Draw(lightingShader);
		CasaHV.Draw(lightingShader);
		CasaAVATAR.Draw(lightingShader);
		Muralla.Draw(lightingShader);
		Lirio01.Draw(lightingShader);
		Lirio02.Draw(lightingShader);
		Lirio03.Draw(lightingShader);
		Lirio04.Draw(lightingShader);

		// carga del modelo de la trajinera en movimiento
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(desplazaTrajinera, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
		TrajineraMoviendose.Draw(lightingShader);


		// carga del mdoelo de las puertas de la muralla
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-76.5f, 1.0f, 155.0f));
		model = glm::rotate(model, glm::radians(rotaPuertaFacha), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
		MurallapuertaDER.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-76.5f, 1.0f, 155.0f));
		model = glm::rotate(model, glm::radians(rotaPuertaFacha), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
		MurallapuertaIZQ.Draw(lightingShader);

		// Carga del modelo del avatar
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-50.0f, 2.5f, 120.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
		jake.Draw(lightingShader);

		// Carga del pulpo en movimiento
		Anim02.Use();
		tiempo = glfwGetTime() * speed;
		modelLoc = glGetUniformLocation(Anim02.Program, "model");
		viewLoc = glGetUniformLocation(Anim02.Program, "view");
		projLoc = glGetUniformLocation(Anim02.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(Anim02.Program, "time"), tiempo);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//PurplePentapus.Draw(Anim02);


		// Carga y dibujo de la animacion de movimiento del agua
		Anim01.Use();
		tiempo = glfwGetTime() * speed;
		modelLoc = glGetUniformLocation(Anim01.Program, "model");
		viewLoc = glGetUniformLocation(Anim01.Program, "view");
		projLoc = glGetUniformLocation(Anim01.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(Anim01.Program, "time"), tiempo);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mar.Draw(Anim01);
		Mar2.Draw(Anim01);
		glBindVertexArray(0);

		// Intercambiamos los buffers de pantalla
		glfwSwapBuffers(window);
	}


	// Finaliza el proceso de GLFW, borrando todos los recursos asignados por GLFW.
	glfwTerminate();
	return 0;
}

// Mueve/altera las posiciones de la cámara según la entrada del usuario.
void DoMovement()
{

	// Controles de camara
	if (keys[GLFW_KEY_UP]) // Tecla flecha de arriba: hace un acercamiento
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_DOWN]) // Tecla flecha de abajo: hace un alejamiento
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_LEFT]) // Tecla flecha de la izquierda: se desplaza hacia la izquierda
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_RIGHT]) // Tecla flecha de la dercha: se desplaza hacia la derecha
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}


	////INICIO SECCION ANIMACION :: Trajinera Moviendose
	if (animTrajinera == true) { // Si anim02 == true realiza lo siguiente
		if (desplazaTrajineraBandera == true) {
			desplazaTrajinera += 0.01f * glfwGetTime();
			if (desplazaTrajinera >= 20.0f) {
				desplazaTrajinera = 20.0f;
				desplazaTrajineraBandera = false;
				animTrajinera = false;
				animTrajineraRev = false;
			}
		}
	}
	if (animTrajineraRev == true) { // Si anim02 == true realiza lo siguiente
		if (desplazaTrajineraBandera == false) {
			desplazaTrajinera -= 0.01f * glfwGetTime();
			if (desplazaTrajinera <= 0.0f) {
				desplazaTrajinera = 0.0f;
			}
		}
	}
	////FIN SECCION ANIMACION :: Trajinera moviendose
	
	////INICIO SECCION :: Pointlight casas 
	if (luzCasa == true) { // Si anim03 == true realiza lo siguiente
		Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
	}
	if (luzCasaRev == true) {
		Light1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	////FIN SECCION:: Pointlight casas 

	////INICIO SECCION :: Animacion (Puerta de la muralla) 
	if (animPuertaPrincipal == true) { // Si es true realiza lo siguiente
		if (BanderaPuertaFachada == true) { // Verifica si la Bandera para la puerta es verdarera 
			rotaPuertaFacha += 0.1f * glfwGetTime(); // Si lo es comienza a incrementar el valor del angulo de rotacion de la puerta
			if (rotaPuertaFacha >= 90.0f) { // Si el valor de este angulo en mayor o igual a 90.0 asiga ese valor y detiene la animacion
				rotaPuertaFacha = 90.0f;
				BanderaPuertaFachada = false;
				animPuertaPrincipal = false;
				animPuertaPrincipalRev = false;
			}
		}
	}
	if (animPuertaPrincipalRev == true) {
		if (BanderaPuertaFachada == false) { // Verifica si la Bandera para la puerta es falsa 
			rotaPuertaFacha -= 0.1f * glfwGetTime(); // Si lo es comienza a decrementar el valor del angulo de rotacion de la puerta
			if (rotaPuertaFacha <= 0.0f) { // Si el valor de este angulo en menor o igual a 0.0 asiga ese valor y detiene la animacion
				rotaPuertaFacha = 0.0f;
			}
		}
	}
	////FIN SECCION :: Animacion 04 (Puerta de la fachada)

}

// Funcion que se llama cada vez que se presiona/suelta una tecla a través de GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	
	////INICIO SECCION ANIMACION :: Teclas para activar la animacion de la trajinera
		// Se debe teclear la tecla 'E' para que la animacion se active
		if (key == GLFW_KEY_Y && action == GLFW_PRESS && animTrajinera == false && desplazaTrajineraBandera == false && desplazaTrajinera == 0.0f){
			desplazaTrajineraBandera = true;
			animTrajinera = true; // Inicia la animación 02
		}
		// Se debe teclear la tecla 'D' para que la animacion 02 se reinicie
		if (key == GLFW_KEY_U && action == GLFW_PRESS && animTrajinera == false && desplazaTrajineraBandera == false && desplazaTrajinera == 20.0f) {
			animTrajineraRev = true; // Reinicia la animacion 02
		}
	////FIN SECCION ANIMACION 02:: Teclas para activar la animacion 02 (Silla)

	////INICIO SECCION ANIMACION 03:: Pointlight casas 
		// Se debe teclear la tecla 'W' para que la animacion 03 se active
		if (key == GLFW_KEY_K && action == GLFW_PRESS && luzCasa == false)
		{
			luzCasa = true;
		}
		// Se debe teclear la tecla 'S' para que la animacion 04 se reinicie 
		if (key == GLFW_KEY_J && action == GLFW_PRESS && luzCasa == true) {
			luzCasaRev = true; // Reinicia la animacion 03
		}
	////FIN SECCION ANIMACION03 :: Pointlight casas 


	////INICIO SECCION ANIMACION :: Teclas para activar la animacion (Puerta de la muralla)
		// Se debe teclear la tecla 'Q' para que la animacion 04 se active
		if (key == GLFW_KEY_O && action == GLFW_PRESS && animPuertaPrincipal == false && BanderaPuertaFachada == false && rotaPuertaFacha == 0.0f)
		{
			BanderaPuertaFachada = true;
			animPuertaPrincipal = true; // Inicia la animación 04
		}
		// Se debe teclear la tecla 'A' para que la animacion 04 se reinicie 
		if (key == GLFW_KEY_C && action == GLFW_PRESS && animPuertaPrincipal == false && BanderaPuertaFachada == false && rotaPuertaFacha == 90.0f) {
			animPuertaPrincipalRev = true; // Reinicia la animacion 04
		}
	////FIN SECCION ANIMACION 04 :: Teclas para activar la animacion 04 (Puerta de la fachada)

}


void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}