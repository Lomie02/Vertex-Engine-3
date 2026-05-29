
#include "pch.h"
#include "GLFWgraphics.h"
#include "GLRenderer.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/type_ptr.inl>
#include <gtc/matrix_transform.hpp>

VertexEngine::GLRenderer::GLRenderer(GlWindow* _win, std::shared_ptr<Shader> _defaultVertex, std::shared_ptr<Shader> _defaultFrag)
{
	m_WindowHandle = _win;
	if (!gladLoadGL()) {
		std::cout << "Glad Failed" << std::endl;

	}

	m_DefaultFragShader = _defaultFrag;
	m_DefaultVertexShader = _defaultVertex;

	if (m_DefaultFragShader == nullptr) {

		std::cout << "Shaders Failed!: Fragment shader not loaded." << std::endl;
	}
	else {
		std::cout << "Shaders Success!: Fragment shader loaded." << std::endl;
	}

	if (m_DefaultVertexShader == nullptr) {

		std::cout << "Shaders Failed!: Vertex Shader Not Loaded." << std::endl;
	}
	else
	{
		std::cout << "Shaders Success!: Vertex shader loaded." << std::endl;
	}


}

void VertexEngine::GLRenderer::BeginFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int width;
	int height;

	glfwGetFramebufferSize(m_WindowHandle->GetWindowHandle(), &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

}

void VertexEngine::GLRenderer::ClearFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3, 0.3, 0.3, 1.0);
}

void VertexEngine::GLRenderer::Submit(const Renderable& _mesh)
{
	m_RenderQueue.push_back(_mesh);

	for (auto& sub : _mesh.m_Models->meshes) {
		if (!sub->m_IsUploaded)
			UploadMesh(sub);
	}

}

void VertexEngine::GLRenderer::Render()
{
	if (!m_DefaultFragShader || !m_DefaultVertexShader) return;

	UseShader(m_DefaultVertexShader, m_DefaultFragShader);

	for (auto& obj : m_RenderQueue) {

		for (auto& sub : obj.m_Models->meshes) {

			if (!sub->m_IsUploaded) continue;

			GPUMesh& gpuData = m_MeshCacheList[sub->m_gpuId];

			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, glm::vec3(0, -1, -0.5));
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::rotate(model, glm::radians(-90.0f),
				glm::vec3(1, 0, 0));

			SetMatrix4("Model", model);
			SetVector4f("Colour", glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
			glBindVertexArray(gpuData.VAO);
			glDrawElements(GL_TRIANGLES, sub->indices.size(), GL_UNSIGNED_INT, nullptr);
		}
	}

	m_RenderQueue.clear();
}

void VertexEngine::GLRenderer::EndFrame()
{
	if (m_WindowHandle)
		glfwSwapBuffers(m_WindowHandle->GetWindowHandle());

	glFlush();
	glFinish();
}

void VertexEngine::GLRenderer::BindTexture(std::shared_ptr<Texture> _texture)
{
	if (m_gpuHandle.find(_texture) == m_gpuHandle.end()) {
		m_gpuHandle[_texture] = UploadTexture(_texture);
	}

	glBindTexture(GL_TEXTURE_2D, m_gpuHandle[_texture]);
}

unsigned int VertexEngine::GLRenderer::CompileProgram(std::shared_ptr<Shader> _vertex, std::shared_ptr<Shader> _frag)
{
	if (m_Programs.find({ _vertex, _frag }) != m_Programs.end())
		return m_Programs[{_vertex, _frag}];

	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, _vertex->GetShaderSource());
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, _frag->GetShaderSource());

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	int success;
	char infoLog[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);



	m_Programs[{_vertex, _frag}] = program;
	return program;
}

void VertexEngine::GLRenderer::UseShader(std::shared_ptr<Shader> _vertex, std::shared_ptr<Shader> _frag)
{
	m_ActiveProgram = CompileProgram(_vertex, _frag);
	glUseProgram(m_ActiveProgram);
}

uint32_t VertexEngine::GLRenderer::UploadMesh(std::shared_ptr<VertexEngine::MeshData> _mesh)
{
	uint32_t id = GenerateUniqueMeshId();

	GPUMesh gpu{};

	glGenVertexArrays(1, &gpu.VAO);
	glGenBuffers(1, &gpu.VBO);
	glGenBuffers(1, &gpu.EBO);

	glBindVertexArray(gpu.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, gpu.VBO);
	glBufferData(GL_ARRAY_BUFFER, _mesh->vertices.size() * sizeof(Vertex), _mesh->vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indices.size() * sizeof(uint32_t), _mesh->indices.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCord));
	glEnableVertexAttribArray(2);


	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);

	m_MeshCacheList[id] = gpu;
	_mesh->m_IsUploaded = true;
	_mesh->m_gpuId = id;

	return id;
}

void VertexEngine::GLRenderer::SetMatrix4(std::string _name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ActiveProgram, _name.c_str()), 1, false, glm::value_ptr(matrix));
}

void VertexEngine::GLRenderer::SetVector4f(std::string _name, const glm::vec4& _vec)
{
	glUniform4f(glGetUniformLocation(m_ActiveProgram, _name.c_str()), _vec.x, _vec.y, _vec.z, _vec.w);
}

unsigned int VertexEngine::GLRenderer::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	return shader;
}

unsigned int VertexEngine::GLRenderer::UploadTexture(std::shared_ptr<Texture> _texture)
{
	unsigned int handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	GLenum format = (_texture->GetChannelds() == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, _texture->GetWidth(), _texture->GetHeight(), 0, format, GL_UNSIGNED_BYTE, _texture->GetPixels().data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return handle;
}
