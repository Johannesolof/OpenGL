#include "engine.hpp"


GLFWwindow* engine::InitWindow(std::string name, int width, int height, int opengl_major, int opengl_minor, int multi_samples) {

	if (glfwInit() != GLFW_TRUE)
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return nullptr;
	}

	glfwWindowHint(GLFW_SAMPLES, multi_samples);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_minor);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	if (window == nullptr)
	{
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	glewInit();
	if (glfwInit() != GLFW_TRUE)
	{
		fprintf(stderr, "Failed to initialize GLFW with context\n");
		return nullptr;
	}

	return window;
}

void engine::PrintOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", reinterpret_cast<char const*>(renderer));
	printf("OpenGL version supported %s\n", reinterpret_cast<char const*>(version));
}

void engine::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void engine::Terminate()
{
	glfwTerminate();
}
