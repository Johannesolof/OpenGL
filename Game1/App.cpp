#include "App.hpp"
#include <utility>
#include "camera.hpp"
#include "input.hpp"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/constants.inl>
#include "buffer.hpp"


App::App(std::string name) : _name(std::move(name)), _width(1280), _height(720), _clearColor(glm::vec4(0.f))
{
	_window = engine::InitWindow(_name, _width, _height, 4, 5);
}

App::~App() = default;

void App::drawGui()
{
	ImGui_ImplGlfwGL3_NewFrame();

	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&_clearColor)); // Edit 3 floats representing a color
	double s = _frameTime.smoothDelta();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", s * 1000.f, 1 / s);

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::run()
{
	_camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), engine::worldUp);
	_input = std::make_shared<Input>(_window);
	glfwSetFramebufferSizeCallback(_window, engine::FramebufferSizeCallback);
	engine::PrintOpenGLInfo();

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	ImGui_ImplGlfwGL3_Init(_window, true);



	fs::path modelPath = "../Assets/Models/cube.obj";
	engine::Model cube;
	cube.load(modelPath);
	modelPath = "../Assets/Models/ShaderBall.obj";
	engine::Model shaderBall;
	shaderBall.load(modelPath);
	modelPath = "../Assets/Models/Floor.obj";
	engine::Model floor;
	floor.load(modelPath);

	fs::path vertPath = "../Assets/Shaders/test.vert";
	fs::path fragPath = "../Assets/Shaders/test.frag";
	auto simpleProgram = engine::Program("Simple", vertPath, fragPath);
	
	struct cameraData_t
	{
		glm::mat4 proj;
		glm::mat4 view;
	} cameraData;


	auto cameraBuffer = engine::Buffer(GL_UNIFORM_BUFFER, &cameraData, sizeof(cameraData));

	simpleProgram.bindUniformBuffer("Camera", cameraBuffer);
	_clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
	while (_input->pressed("quit") != GLFW_PRESS && glfwWindowShouldClose(_window) == 0)
	{
		glfwGetFramebufferSize(_window, &_width, &_height);

		_frameTime.update(glfwGetTime());
		_input->update();
		_camera->update(*_input, _frameTime.deltaTime); // Made sould share the pointer to the input with everything that wants to read it

		cameraData.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(_width) / static_cast<float>(_height), _camera->nearPlane,
		                                  _camera->farPlane);
		cameraData.view = _camera->getViewMatrix();
		glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 1.f));

		cameraBuffer.update(&cameraData, sizeof(cameraData));
		simpleProgram.setUniform("modelMatrix", modelMatrix);

		glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		simpleProgram.use();
		shaderBall.draw(simpleProgram);

		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -1.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, -glm::half_pi<float>(), engine::worldUp);
		simpleProgram.setUniform("modelMatrix", modelMatrix);

		floor.draw(simpleProgram);


		drawGui();
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	engine::Terminate();
}
