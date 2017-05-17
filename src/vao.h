#pragma once

#include <glad/glad.h>
#include <stdint.h>
struct Vao
{
	typedef void (*BindVaoFunc)(GLuint array);
	typedef void (*DeleteVaoFunc)(GLsizei n, const GLuint *arrays);
	typedef void (*GenVaoFunc)(GLsizei n, GLuint *arrays);

	Vao()
	{
		if (GLAD_GL_OES_vertex_array_object)
		{
			bindFunc = glBindVertexArrayOES;
			deleteFunc = glDeleteVertexArraysOES;
			glGenVertexArraysOES(1, &id);
		}
		else if (GLAD_GL_ARB_vertex_array_object)
		{
			bindFunc = glBindVertexArray;
			deleteFunc = glDeleteVertexArrays;
			glGenVertexArrays(1, &id);
		}
		else if (GLAD_GL_APPLE_vertex_array_object)
		{
			bindFunc = glBindVertexArrayAPPLE;
			deleteFunc = glDeleteVertexArraysAPPLE;
			glGenVertexArraysAPPLE(1, &id);
		}

	}

	~Vao()
	{
		deleteFunc(1, &id);
	}

	void bind()
	{
		bindFunc(id);
	}

	void release()
	{
		bindFunc(0);
	}

	struct Binder
	{
		Binder(Vao &vao)
			: vao(vao)
		{
			vao.bind();
		}
		~Binder()
		{
			vao.release();
		}
		Vao &vao;
	};
	uint32_t id;
	BindVaoFunc bindFunc;
	DeleteVaoFunc deleteFunc;
};