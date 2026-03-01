
#include "pch.h"
#include "GLFWgraphics.h"
#include "GLRenderer.h"
#include <iostream>


VertexEngine::GLRenderer::GLRenderer(GlWindow* _win, std::shared_ptr<Shader> _defaultVertex, std::shared_ptr<Shader> _defaultFrag)
{
	m_WindowHandle = _win;
	if (!gladLoadGL()) {
		std::cout << "Glad Failed" << std::endl;

	}

	m_DefaultFragShader = _defaultFrag;
	m_DefaultVertexShader = _defaultVertex;

	if (m_DefaultFragShader == nullptr ){
		
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

	// Triangle for testing only

	float vertices[] = {
		0.0f,  0.5f, 0.0f,  // top
	   -0.5f, -0.5f, 0.0f,  // bottom left
		0.5f, -0.5f, 0.0f   // bottom right
	};

	glGenVertexArrays(1, &m_TriangleVAO);
	glGenBuffers(1, &m_TriangleVBO);

	glBindVertexArray(m_TriangleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);



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
	glEnable(GL_CULL_FACE);
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
}

void VertexEngine::GLRenderer::Render()
{
	//use shader here
	if (m_DefaultFragShader && m_DefaultVertexShader)
		UseShader(m_DefaultVertexShader, m_DefaultFragShader);

	// Draw the triangle
	glBindVertexArray(m_TriangleVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	// Draw mesh

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
	unsigned int program = CompileProgram(_vertex, _frag);
	glUseProgram(program);
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
