/* ------------------- PROYECTO FINAL LAB. CGeIHC -------------------
*  -------------------       SEMESTRE 2023-1      -------------------
*  ALUMNOS Y NO. DE CUENTA:
*  César Yair Calderón Guevara		316277211
*  Miguel Angel Luna Colmenares		316060370
*  Jaime Moreno Duran				316064378
*  Tania Lizeth Peñaloza Lugo		316013929
*/
#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>


// --------------- REPRODUCCION MUSICAL --------------------------

// Pragma para reproducir audio .wav o mp3
#pragma comment(lib, "winmm.lib")
bool music = true, // Variable para activar musica
current_song = false; // Cancion, se vuelve True si se reproduce bien

// Funcion para reproducir musica
void play_music() {
	if (music) {
		current_song = PlaySound(L"NavidadSong.wav", NULL, SND_LOOP | SND_ASYNC);
		music = false; // Debe ser activada de nuevo para permitir reproduccion
	}
} // play_music()

// --------------- FIN PARTE MUSICAL ------------------------------


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
int var33 = 0;
float   trineoz = -300.0f, trineox = -300.0f, trineodir = 900, aux = 0;
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		open = 0.0f,
		bandera1 = 0.0f,
		orientaT = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		abrir = false,
		recorrido4 = false;
// Variables de control para animaciones

bool anim_auto = false, // Variables para animacion de auto en garage
anim_a1 = false,
anim_a2 = false,
anim_a3 = false,
anim_a4 = false,
anim_a5 = false;

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

#define MAX_FRAMES 40
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;
	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
}


void animate(void)
{
	// Variables para animacion de trineo
	if (var33 == 0)
	{
		trineox += 3.0f;
		trineoz = trineoz;
		orientaT = 180.0f;
		if (trineox >= 300.0f)
		{
			var33 = 1;

		}
	}
	if (var33 == 1)
	{
		trineox = trineox;
		trineoz += 3.0f;
		orientaT = 90.0f;
		if (trineoz >= 300.0f)
		{
			var33 = 2;

		}
	}
	if (var33 == 2)
	{
		trineox -= 3.0f;
		trineoz = trineoz;
		orientaT = 0.0f;
		if (trineox <= -300.0f)
		{
			var33 = 3;

		}
	}
	if (var33 == 3)
	{
		trineox = trineox;
		trineoz -= 3.0f;
		orientaT = -90.0f;
		if (trineoz <= -300.0f)
		{
			var33 = 0;

		}

	}

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}
	//Vehículo
	if (animacion)
	{
		movAuto_z += 3.0f;
	}
	// Vehículo Y animacion de Garage
	if (anim_auto) {
		if (abrir) {
			if (bandera1 == 0)
			{
				aux += 0.003;
				open = 300 * sin(aux);
			}
			if (aux >= 0.3f)
			{
				bandera1 = 1;
				open = 300 * sin(aux);

				//abrir = false;
			} // bandera
		} // abrir
		if (anim_a1) { // Comienza movimiento del coche
			if (aux >= 0.3f) {
				movAuto_z += 3.0f;
				movAuto_x = 0.0f;
				orienta = 0.0f;
				if (movAuto_z >= 250.0f) {
					anim_a1 = false;
					anim_a2 = true; // Comienza segunda parte
					printf("a1 reach");
				}
			}
		} // anim_a1
		if (anim_a2) {
			orienta = -90.0f;
			movAuto_z = 250.0f;
			movAuto_x += -3.0f;
			if (movAuto_x <= -150.0f) {
				anim_a2 = false;
				anim_a3 = true;
				printf("a2 reach");
			}
		} // anim_a2
		if (anim_a3) {
			orienta = -180.0f;
			movAuto_z -= 2.0f;
			movAuto_x = -150.0f;
			if (movAuto_z <= 100.0f) {
				anim_a3 = false;
				anim_a4 = true;
				printf("a3 reach");
			}
		}// anim_a3
		if (anim_a4) {
			orienta = -270.0f;
			movAuto_x += 3.0f;
			movAuto_z = 100.0f;
			if (movAuto_x >= 0.0f) {
				anim_a5 = true;
				anim_a4 = false;
				printf("a4 reach");
			}
		}// anim_a4
		if (anim_a5) { // Se mete en reversa, bien chido
			orienta = 0.0f;
			movAuto_z -= 1.0f;
			movAuto_x = 0.0f;
			if (movAuto_z <= 0.0f) {
				movAuto_z = 0.0f;

				// Cerrando garaje
				if (bandera1 == 1) {
					aux -= 0.003;
					open = 300 * sin(aux);
				}
				if (aux <= 0.0f) { // termina de cerrarse
					anim_a5 = false;
					//aux = 0.0f;
					bandera1 = 0;
					open = 300 * sin(aux);
					anim_auto = false;
					abrir = false;
				}
			}
		} // anim_a5
	} // anim_auto
}

void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}




int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model casaPro("resources/objects/CasaProyecto/CASABIEN.obj");
	Model pasto("resources/objects/piso/pastos.obj");


	//Modelos Miguel 
	Model Rega("resources/objects/parteMiguel/Regadera/regadera.obj");
	Model cama("resources/objects/parteMiguel/Cama/CAMA.obj");
	Model MuebleP("resources/objects/parteMiguel/Mueblepeque/Mueblepeque.obj");
	Model MuebleRope("resources/objects/parteMiguel/Muebleropero/Muebleropero.obj");
	Model MesaMedia("resources/objects/parteMiguel/MesaMedia/Mesamedia.obj");
	Model Taza("resources/objects/parteMiguel/Taza/Taza.obj");
	Model Laba("resources/objects/parteMiguel/laba/Laba.obj");
	Model Planta("resources/objects/parteMiguel/planta/planta.obj");
	Model MesaCoci("resources/objects/parteMiguel/Mesa/Mesa.obj");
	Model MesaCen("resources/objects/parteMiguel/MesaCen/MesaCen.obj");
	Model GuitarraC("resources/objects/parteMiguel/GuitarraC/GuitarraC.obj");
	Model MuebleEx1("resources/objects/parteMiguel/MuebleEx1/MuebleEx1.obj");
	Model MuebleEx2("resources/objects/parteMiguel/MuebleEx2/MuebleEx2.obj");
	Model Tina("resources/objects/parteMiguel/Tina/Tina.obj");
	Model Trineo("resources/objects/parteMiguel/Trineo/Trineo.obj");
	Model Cochera("resources/objects/parteMiguel/Cochera/Cochera.obj");


	//Modelos Jaime
	Model Arbol1("resources/objects/PatioJ/Arbol1/Arbolnavidad.obj");
	Model Arbol2("resources/objects/PatioJ/Arbol2/arboln2.obj");
	Model SillaP("resources/objects/PatioJ/PatioN/Patiojmd.obj");
	Model Arbol3("resources/objects/PatioJ/Arbol3/Arbol3.obj");
	Model CamaC("resources/objects/CuartoJ/Cama/Camac.obj");
	Model Escritorio("resources/objects/CuartoJ/Escritorio/Escritorio.obj");
	Model Pc("resources/objects/CuartoJ/Pc/pc.obj");
	Model Tele("resources/objects/CuartoJ/tv/tv.obj");
	Model LabaJ("resources/objects/BanioJ/laba/Laba.obj");
	Model TazaJ("resources/objects/BanioJ/Taza/Taza.obj");
	Model RegaJ("resources/objects/BanioJ/Rega/regadera.obj");
	Model TinaJ("resources/objects/BanioJ/Tina/Tina.obj");

	// Elementos del GARAJE
	Model GarajeCarro("resources/objects/lambo/carroceria.obj");
	Model GCarroLlanta("resources/objects/lambo/Wheel.obj");
	Model GarajeUtCart("resources/objects/Garaje/AuxTable/auxTable.obj");
	Model GarajeEscalera("resources/objects/Garaje/escaleras/escaleras.obj");
	Model GarajeMoto("resources/objects/Garaje/moto/moto_blanca.obj");
	Model GarajeCarroFix("resources/objects/Garaje/coche_averiado/coche_roto.obj");
	// Elementos Cocina
	Model CocinaBase("resources/objects/Cocina/cocina_base.obj");
	// Estante
	Model Estante("resources/objects/estante/estante.obj");
	//Snowman
	Model Snowmans("resources/objects/muneco_nieve/snowmans.obj");

	// Modelos Tania
	Model PatExterno("resources/objects/tania/patio-trasero/patio.obj");


	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}
	

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight.position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight.constant", 1.0f);
		staticShader.setFloat("spotLight.linear", 0.0009f);
		staticShader.setFloat("spotLight.quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, -1.50f, -150.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		casaPro.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);


		// ---------------------- INSTANCIAS DEL GARAJE --------------------
		// UtilityCart
		model = glm::translate(model, glm::vec3(25.0f, 0.0f, -8.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		GarajeUtCart.Draw(staticShader);

		//escaleras
		model = glm::translate(glm::mat4(1.0f), glm::vec3(165.0f, -1.0f, -45.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		GarajeEscalera.Draw(staticShader);

		// Motocicleta
		model = glm::translate(glm::mat4(1.0f), glm::vec3(235.0f, -1.0f, 25.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		staticShader.setMat4("model", model);
		GarajeMoto.Draw(staticShader);

		// Coche descompuesto
		model = glm::translate(glm::mat4(1.0f), glm::vec3(205.0f, -1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", model);
		GarajeCarroFix.Draw(staticShader);

		// GarajeCarro - Lambo
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(205.0f + movAuto_x, -1.0f, 80.0f + movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		GarajeCarro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		GCarroLlanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		GCarroLlanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		GCarroLlanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		GCarroLlanta.Draw(staticShader);	//Izq trase

		// Instancias Banio Garaje
		model = glm::translate(glm::mat4(1.0f), glm::vec3(157.0f, -2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		Taza.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(107.0f, -2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Laba.Draw(staticShader);

		//Instancias snowman
		model = glm::translate(glm::mat4(1.0f), glm::vec3(157.0f, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Snowmans.Draw(staticShader);

		// Instancias Cocina
		model = glm::translate(glm::mat4(1.0f), glm::vec3(79.0f, -2.0f, -40.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 7.0f));
		staticShader.setMat4("model", model);
		CocinaBase.Draw(staticShader);

		// Estante
		model = glm::translate(glm::mat4(1.0f), glm::vec3(115.0f, -2.0f, 23.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		Estante.Draw(staticShader);

		// ------------------------------------ JMD
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.50f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, -1.50f, 160.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arbol2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, -1.50f, 160.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arbol2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, -1.50f, 160.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arbol2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, 1.5f, 180.0f));
		model = glm::scale(model, glm::vec3(0.001f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		SillaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 1.5f, 180.0f));
		model = glm::scale(model, glm::vec3(0.001f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		SillaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, -1.5f, 250.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arbol3.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, -1.5f, 300.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arbol3.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.5f, 250.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arbol3.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-67.0f, 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		CamaC.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-120.0f, -1.3f, -55.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Escritorio.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-126.0f, 4.5f, -55.0f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Pc.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-167.0f, -2.5f, -110.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Tele.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-240.0f, -2.5f, 18.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		LabaJ.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-280.0f, -2.5f, 47.0f));
		model = glm::scale(model, glm::vec3(0.75f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		TazaJ.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-297.0f, -1.5f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		RegaJ.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-225.0f, -2.5f, -10.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		TinaJ.Draw(staticShader);

		// ------------------------------------ PARTE DE MIGUEL 


		model = glm::translate(glm::mat4(1.0f), glm::vec3(trineox, 150.0f, trineoz));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(orientaT), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Trineo.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(214.5f, 15.50f, 122.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(open), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		Cochera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 3.1f, -100.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, -0.2f, -100.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		GuitarraC.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-220.0f, -3.0f, -100.0f));
		model = glm::scale(model, glm::vec3(1.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		MuebleP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-295.0f, -2.0f, -130.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		MesaMedia.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-210.0f, -2.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Taza.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-210.0f, -2.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Planta.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-230.0f, -2.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		MuebleEx1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-190.0f, -2.0f, 117.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Laba.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-180.0f, -2.0f, 70.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		MuebleEx2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-290.0f, -15.0f, 117.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Rega.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-270.0f, -2.0f, 56.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Tina.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, -100.0f));
		model = glm::scale(model, glm::vec3(0.36f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		MesaCoci.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.5f, -100.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		MesaCen.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-220.0f, -1.5f, -55.0f));
		model = glm::scale(model, glm::vec3(0.40f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		MuebleRope.Draw(staticShader);

		// -------------------- Tania models
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, -1.0f, -300.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		staticShader.setMat4("model", model);
		PatExterno.Draw(staticShader);

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Movement and Project manipulation
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;
	// Animacion Cochera y Auto
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		anim_auto = true;
		abrir = true;
		anim_a1 = true;
	}


	// Reproduccion Musical
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		play_music();


	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}