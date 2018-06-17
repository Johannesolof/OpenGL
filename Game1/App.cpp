#include "App.hpp"
#include <utility>
#include "camera.hpp"
#include "input.hpp"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/constants.inl>
#include "buffer.hpp"
#include "model.hpp"
#include <memory>

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
	ImGui_ImplGlfwGL3_NewFrame();

	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&_clearColor)); // Edit 3 floats representing a color
	double s = _frameTime.smoothDelta();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", s * 1000.f, 1 / s);

	//ImGui::Button("Reload shaders", ImVec2(10, 10));

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::run()
{
	_camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), _worldUp);
	_input = std::make_shared<Input>(_window);
	glfwSetFramebufferSizeCallback(_window, resize);
	//engine::PrintOpenGLInfo();

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	ImGui_ImplGlfwGL3_Init(_window, true);



	fs::path modelPath = "../Assets/Models/cube.obj";
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

	fs::path vertPath = "../Assets/Shaders/test.vert";
	fs::path fragPath = "../Assets/Shaders/test.frag";
	auto _gbuffer = je::Program("GBuffer", vertPath, fragPath); // Default constructor does not work

	vertPath = "../Assets/Shaders/fullscreen.vert";
	fragPath = "../Assets/Shaders/copy.frag";
	auto copyProgram = je::Program("Copy", vertPath, fragPath);


	//auto depthTexture = je::Create2DTexture(GL_TEXTURE_2D, GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, GL_UNSIGNED_BYTE, _width, _height);
	//auto colorTexture = je::Create2DTexture(GL_TEXTURE_2D, GL_RGBA, GL_RGBA16, GL_UNSIGNED_BYTE, _width, _height);
	//auto frameBuffer = je::FrameBuffer();
	//frameBuffer.attachDepthAttachment(depthTexture);
	//frameBuffer.attachColorAttachment({colorTexture});

	
	struct cameraData_t
	{
		glm::mat4 proj;
		glm::mat4 invProj;
		glm::mat4 view;
		glm::mat4 invView;
		glm::mat4 viewProj;
		glm::mat4 invViewProj;
		glm::vec4 wsPosition;	
	} cameraData;


	auto cameraBuffer = je::Buffer(GL_UNIFORM_BUFFER, &cameraData, sizeof(cameraData));

	_gbuffer.bindUniformBuffer("Camera", cameraBuffer);
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
			//resize fbos, textures etc.
		}

		_frameTime.update(glfwGetTime());
		_input->update();
		_camera->update(*_input, _frameTime.deltaTime); // Made should share the pointer to the input with everything that wants to read it

		cameraData.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(_width) / static_cast<float>(_height), _camera->nearPlane,
		                                  _camera->farPlane);
		cameraData.invProj = glm::inverse(cameraData.proj);
		cameraData.view = _camera->getViewMatrix();
		cameraData.invView = glm::inverse(cameraData.view);
		cameraData.viewProj = cameraData.proj * cameraData.view;
		cameraData.invViewProj = glm::inverse(cameraData.viewProj);
		cameraData.wsPosition = glm::vec4(_camera->getPosition(), 1.f);

		glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f));
		modelMatrix = glm::rotate(modelMatrix, static_cast<float>(_frameTime.currentTime), _worldUp);

		cameraBuffer.update(&cameraData, sizeof(cameraData));
		_gbuffer.setUniform("modelMatrix", modelMatrix);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//frameBuffer.bind();

		_gbuffer.use();
		//shaderBall.draw();

		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -1.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, -glm::half_pi<float>(), _worldUp);
		_gbuffer.setUniform("modelMatrix", modelMatrix);

		floor.draw();

		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 1.f, 0.f));
		_gbuffer.setUniform("modelMatrix", modelMatrix);
		arrow.draw();
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.0f, 0.f));
		_gbuffer.setUniform("modelMatrix", modelMatrix);
		arrow.draw();
		//modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.5f));
		modelMatrix = glm::rotate(modelMatrix, glm::half_pi<float>(), glm::vec3(1.f, 0.f, 0.f));
		_gbuffer.setUniform("modelMatrix", modelMatrix);
		arrow.draw();
		//modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, -glm::half_pi<float>(), glm::vec3(0.f, 0.f, 1.f));
		_gbuffer.setUniform("modelMatrix", modelMatrix);

		arrow.draw();

		//je::FrameBuffer::unBind();
		//copyProgram.use();
		//copyProgram.bindSampler("inTexture", frameBuffer.getColorAttachments()[0]->handle);

		drawGui();
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}


void App::resize(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}