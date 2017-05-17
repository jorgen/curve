#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>

#include <string>

#include "renderer.h"
#include "testitem.h"

struct Window
{
	Window(int width, int height, const std::string &title)
		: handle(glfwCreateWindow(width,height, title.c_str(), 0, 0))
		, line_1(width * 0.1, height * 0.1)
		, line_2(width * 0.9, height * 0.9)
	{
		if (handle) {
			glfwSetWindowUserPointer(handle, this);
			glfwSetKeyCallback(handle, key_callback);
			glfwSetMouseButtonCallback(handle, mouse_button_callback);
			glfwSetCursorPosCallback(handle, mouse_pos_callback);
		}
	}
	~Window()
	{
		glfwDestroyWindow(handle);
	}

	GLFWwindow *handle;
	double x;
	double y;
	glm::vec2 line_1;
	glm::vec2 line_2;
private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	static void mouse_button_callback(GLFWwindow* w, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			Window *window = static_cast<Window *>(glfwGetWindowUserPointer(w));
			glm::vec2 pos(window->x, window->y);
			glm::vec2 distance = glm::abs(pos - window->line_1);
			if (distance.x < 10 && distance.y < 10)
			{
				fprintf(stderr, "hello world\n");
			}
			else {
				fprintf(stderr, "distance %f %f\n", distance.x, distance.y);
			}
		}
	}

	static void mouse_pos_callback(GLFWwindow *handle, double x, double y)
	{
		Window *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));
		window->x = x;
		window->y = y;
		fprintf(stderr, "mousecallback %f %f\n", x, y);
	}
};

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	Window window(640, 480, "Simple example");
    if (!window.handle)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window.handle);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

	RenderManager renderer;
	TestItem item;
	renderer.addItem(&item);
    while (!glfwWindowShouldClose(window.handle))
    {
        int width, height;
        glfwGetFramebufferSize(window.handle, &width, &height); 
		renderer.setViewport(float(width), float(height));
		renderer.prepareRender();
		renderer.render();

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
