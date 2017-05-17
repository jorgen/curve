#pragma once

#include <stdint.h>
#include <glad/glad.h>
#include <string>

enum class ShaderType
{
	Vertex,
	Fragment
};

uint32_t createShader(const char *shader_source, ShaderType type, std::string &error_msg)
{
	uint32_t shader_type = type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
    uint32_t shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);
	int compile_status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status)
	{
		int log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			error_msg.resize(log_length);
			int written;
			glGetShaderInfoLog(shader, log_length, &written, &error_msg[0]);
			error_msg.resize(written);
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

bool linkProgram(uint32_t program, std::string &error_msg)
{
	glLinkProgram(program);
	int link_status;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if (!link_status)
	{
		int log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0)
		{
			error_msg.resize(log_length);
			int written;
			glGetProgramInfoLog(program, log_length, &written, &error_msg[0]);
			error_msg.resize(written);
		}
		return false;
	}
	return true;
}