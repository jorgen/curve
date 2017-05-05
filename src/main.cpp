#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>

#include <string>

static const struct
{
    float x, y;
    float u, v;
} vertices[3] =
{
    { -0.8f, -0.8f, 0.f, 0.f },
    {  0.f, 1.f, 0.5f, 0.f },
    {  0.8f,  -0.8f, 1.f, 1.f }
};

static const char* vertex_shader_text = R"shader(
uniform mat4 MVP;
attribute vec2 vUv;
attribute vec2 vPos;
varying vec2 uv;
void main()
{
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    uv = vUv;
};
)shader";
static const char* fragment_shader_text = R"shader(
varying vec2 uv;
void main()
{
	float factor = uv.x * uv.x - uv.y;
	if (factor > 0)
		discard;
	gl_FragColor = vec4(uv, 1.0, 1.0);
};
)shader";

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
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;
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
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vUv");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*) (sizeof(float) * 2));
    while (!glfwWindowShouldClose(window.handle))
    {
        float ratio;
        int width, height;
		glm::mat4 m;
        glfwGetFramebufferSize(window.handle, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
		m = glm::rotate(m, (float)glfwGetTime(), glm::vec3(0, 1, 0));

		glm::mat4 p = glm::ortho(0, width, 0, height);
		//glm::mat4 p = width > height ? glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f)
		//						     : glm::ortho(-1.f, 1.f, -ratio, ratio, 1.f, -1.f);
		glm::mat4 v;
		//glm::mat4 p = glm::perspective(45.f, ratio, 1.f, -1.f);
		//glm::mat4 v = glm::lookAt(glm::vec3(0.7f, 1.f, 1.5f),
		//	glm::vec3(0, -0.3f, 0),
		//	glm::vec3(0, 1, 0));
		glm::mat4 mvp = p * v * m;
        glUseProgram(program);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
