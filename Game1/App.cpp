#include "App.hpp"
#include <utility>
#include "camera.hpp"
#include "input.hpp"
#include <glm/gtc/constants.inl>
#include "buffer.hpp"
#include "model.hpp"
#include <memory>
#include <glm/gtc/matrix_transform.inl>
#include "texture.hpp"
#include "image.hpp"
#include "Shapes/ShapeFactory.hpp"

App::App(std::string name) : _name(std::move(name)), _width(1280), _height(720), _clearColor(glm::vec4(0.f))
{
	_window = nullptr;
}

App::~App()
{
	glfwTerminate();
};

bool App::init()
{
	if (glfwInit() != GLFW_TRUE)
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
	if (window == nullptr)
	{
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	glewInit();
	if (glfwInit() != GLFW_TRUE)
	{
		fprintf(stderr, "Failed to initialize GLFW with context\n");
		return false;
	}

	_window = window;


	return true;
}

void App::drawGui()
{

	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&_clearColor)); // Edit 3 floats representing a color
	double s = _frameTime.smoothDelta();

	//ImGui::Button("Reload shaders", ImVec2(10, 10));


}

void App::run()
{
	_camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), _worldUp, _width, _height);
	_input = std::make_shared<Input>(_window);
	glfwSetFramebufferSizeCallback(_window, resize);
	//engine::PrintOpenGLInfo();

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	ImGui_ImplGlfwGL3_Init(_window, true);



	std::filesystem::path modelPath = "../Assets/Models/cube.obj";
	je::Model cube;
	cube.load(modelPath);
	modelPath = "../Assets/Models/ShaderBall.obj";
	je::Model shaderBall;
	shaderBall.load(modelPath);
	modelPath = "../Assets/Models/Floor.obj";
	je::Model floor;
	floor.load(modelPath);
	modelPath = "../Assets/Models/arrow.obj";
	je::Model arrow;
	arrow.load(modelPath);

	je::Mesh cube2 = createCube();

	std::filesystem::path vertPath = "../Assets/Shaders/forward.vert";
	std::filesystem::path fragPath = "../Assets/Shaders/forward.frag";
	auto forward = je::Program("Forward", vertPath, fragPath); // Default constructor does not work

	vertPath = "../Assets/Shaders/fullscreen.vert";
	fragPath = "../Assets/Shaders/copy.frag";
	auto copyProgram = je::Program("Copy", vertPath, fragPath);


	//auto depthTexture = je::Create2DTexture(GL_TEXTURE_2D, GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, GL_UNSIGNED_BYTE, _width, _height);
	//auto colorTexture = je::Create2DTexture(GL_TEXTURE_2D, GL_RGBA, GL_RGBA16, GL_UNSIGNED_BYTE, _width, _height);
	//auto frameBuffer = je::FrameBuffer();
	//frameBuffer.attachDepthAttachment(depthTexture);
	//frameBuffer.attachColorAttachment({colorTexture});

	auto image = je::LDRImage();
	image.load("../Assets/Images/RockyDirt.png");

	je::Texture texture = je::Texture(GL_TEXTURE_2D, GL_RGBA, GL_RGB, GL_RGB8, image);
	texture.bind(0);
	

	auto cameraBuffer = je::Buffer(GL_UNIFORM_BUFFER, &_camera->cameraData, sizeof _camera->cameraData);
	forward.bindUniformBuffer("Camera", cameraBuffer);
	
	struct material_t
	{
		glm::vec4 color = glm::vec4(1, 1, 1, 1);
		float metalness = 0.f;
		float rougness = 1.f;
	} materialData;

	auto materialBuffer = je::Buffer(GL_UNIFORM_BUFFER, &materialData, sizeof materialData);
	materialBuffer.update(&materialData, sizeof(materialData));
	forward.bindUniformBuffer("Material", materialBuffer);

	_clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
	while (_input->pressed("quit") != GLFW_PRESS && glfwWindowShouldClose(_window) == 0)
	{
		int width, height;
		glfwGetFramebufferSize(_window, &width, &height);
		if (width != _width || height != _height)
		{
			_width = width; _height = height;
			glViewport(0, 0, _width, _height);
			_camera->resize(width, height);
			//resize fbos, textures etc.
		}
		
		ImGui_ImplGlfwGL3_NewFrame();

		_frameTime.update(static_cast<float>(glfwGetTime()));
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", _frameTime.deltaTime * 1000.f, 1 / _frameTime.deltaTime);
		
		_input->update();
		_camera->update(*_input, _frameTime.deltaTime); // Made should share the pointer to the input with everything that wants to read it

		cameraBuffer.update(&_camera->cameraData, sizeof _camera->cameraData);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		forward.use();
		
		glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f));
		modelMatrix = glm::rotate(modelMatrix, static_cast<float>(_frameTime.currentTime), _worldUp);
		forward.setUniform("modelMatrix", modelMatrix);
		cube.draw();

		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -1.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, -glm::half_pi<float>(), _worldUp);
		forward.setUniform("modelMatrix", modelMatrix);

		floor.draw();


		
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}


void App::resize(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
}