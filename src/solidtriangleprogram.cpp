#include "solidtriangleprogram.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "program.h"

static const char* vertex_shader_text = R"shader(
uniform mat4 proj;
attribute vec2 vUv;
attribute vec3 vPos;
varying vec2 uv;
void main()
{
    gl_Position = proj * vec4(vPos, 1.0);
    uv = vUv;
};
)shader";
static const char* fragment_shader_text = R"shader(
varying vec2 uv;
void main()
{
	float factor = uv.x * uv.x - uv.y;
	if (factor > 0.0)
		discard;
	gl_FragColor = vec4(uv, 1.0, 1.0);
};
)shader";

SolidTriangleProgram::SolidTriangleProgram()
{
	std::string error;
	uint32_t vertex_shader = createShader(vertex_shader_text, ShaderType::Vertex, error);
	if (!vertex_shader)
	{
		fprintf(stderr, "Failed to compile vertex shader for SolidTriangleProgram:\n%s\n", error.c_str());
		return;
	}
	uint32_t fragment_shader = createShader(fragment_shader_text, ShaderType::Fragment, error);
	if (!fragment_shader)
	{
		fprintf(stderr, "Failed to compile fragment shader for SolidTriangleProgram:\n%s\n", error.c_str());
		return;
	}

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	if (!linkProgram(program, error)) {
		fprintf(stderr, "Failed to link SolidTriangleProgram:\n%s\n", error.c_str());
		return;
	}
	glUseProgram(program);
    proj_loc = glGetUniformLocation(program, "proj");
    vpos_loc = glGetAttribLocation(program, "vPos");
    vcol_loc = glGetAttribLocation(program, "vUv");
}

void SolidTriangleProgram::setViewport(float width, float height)
{
	glUseProgram(program);
	glm::mat4 proj = glm::ortho(0.f, width, 0.f, height, -20000.f, 20000.f);
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));
}

void SolidTriangleProgram::setVertexLayoutForCurrentVaoAndArrayBuffer()
{
	glUseProgram(program);
    glEnableVertexAttribArray(vpos_loc);
    glVertexAttribPointer(vpos_loc, 3, GL_FLOAT, GL_FALSE,
                          sizeof(SolidTriangleVertexData), (void*) 0);
    glEnableVertexAttribArray(vcol_loc);
    glVertexAttribPointer(vcol_loc, 2, GL_FLOAT, GL_FALSE,
                          sizeof(SolidTriangleVertexData), (void*) (offsetof(SolidTriangleVertexData, u)));
}

void SolidTriangleProgram::uploadIndicesToCurrentBuffer(const std::vector<uint16_t> indices)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void SolidTriangleProgram::modifyIndicesToCurrentBuffer(size_t offset, const std::vector<uint16_t> indices)
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeof(indices[0]) * indices.size(), indices.data());
}

void SolidTriangleProgram::uploadVertexDataToCurrentBuffer(const std::vector<SolidTriangleVertexData> data)
{
	glBufferData(GL_ARRAY_BUFFER, sizeof(data[0]) * data.size(), data.data(), GL_STATIC_DRAW);
}

void SolidTriangleProgram::modifyVertexDataToCurrentBuffer(size_t offset, const std::vector<SolidTriangleVertexData> data)
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, data.size() * sizeof(data[0]), data.data());
}

void SolidTriangleProgram::render(uint32_t iboSize)
{
	if (!iboSize)
		return;
	glUseProgram(program);
	glDrawElements(GL_TRIANGLES, iboSize, GL_UNSIGNED_SHORT, 0);
}

