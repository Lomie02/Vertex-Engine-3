
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
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

		if (!obj.m_Models) continue;

		RenderModelNode(obj.m_Models->m_RootNode, glm::mat4(1.0f), obj);
	}

	m_RenderQueue.clear();
}

void VertexEngine::GLRenderer::EndFrame()
{
	if (m_WindowHandle)
		glfwSwapBuffers(m_WindowHandle->GetWindowHandle());
}

void VertexEngine::GLRenderer::SubmitCamera(const CameraRenderable& _camera)
{
	m_ActiveCamera = _camera;
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

void VertexEngine::GLRenderer::RenderModelNode(const VertexEngine::ModelNode& node, const glm::mat4& parentTrans, const Renderable& obj)
{
	glm::mat4 nodeWorld = parentTrans * node.m_LocalTransform;

	glm::mat4 finalWorld = obj.ModelMatrix * nodeWorld;

	for (uint32_t meshIndex : node.m_MeshIndices) {

		if (meshIndex >= obj.m_Models->meshes.size())
			continue;

		auto& mesh = obj.m_Models->meshes[meshIndex];

		if (!mesh->m_IsUploaded)
			continue;

		GPUMesh& gpuData = m_MeshCacheList[mesh->m_gpuId];

		SetMatrix4("Model", finalWorld);
		SetMatrix4("View", m_ActiveCamera.m_ViewMatrix);
		SetMatrix4("Projection", m_ActiveCamera.m_ProjectionMatrix);

		glBindVertexArray(gpuData.VAO);

		for (auto& sub : mesh->subMeshes) {

			if (sub.materialIndex >= obj.m_Models->materials.size())
				continue;

			auto& material =
				obj.m_Models->materials[sub.materialIndex];

			// Set albedo colour
			SetVector4f("Colour", material.GetAlbedoColour());

			auto albedo = material.GetAlbedoMap();

			if (albedo) {

				glActiveTexture(GL_TEXTURE0);
				BindTexture(albedo);

				SetInt("AlbedoMap", 0);
				SetBool("HasAlbedo", true);
			}
			else {
				SetBool("HasAlbedo", false);
			}

			glDrawElements(GL_TRIANGLES, sub.indexCount, GL_UNSIGNED_INT, (void*)(sub.indexOffset * sizeof(uint32_t)));
		}
	}

	for (const auto& child : node.m_Children) {
		RenderModelNode(child, nodeWorld, obj);
	}
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

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCord));
	glEnableVertexAttribArray(2);

	// Tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);

	// Bi Tangent
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

void VertexEngine::GLRenderer::SetInt(std::string _name, int _value)
{
	glUniform1i(glGetUniformLocation(m_ActiveProgram, _name.c_str()), _value);
}

void VertexEngine::GLRenderer::SetBool(std::string _name, bool _state)
{
	glUniform1i(glGetUniformLocation(m_ActiveProgram, _name.c_str()), _state ? 1 : 0);
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
	std::cout
		<< "Uploading Texture: "
		<< _texture->GetWidth()
		<< "x"
		<< _texture->GetHeight()
		<< " Channels: "
		<< _texture->GetChannelds()
		<< "\n";

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
