
#include "pch.h"
#include "GLFWgraphics.h"
#include "GLRenderer.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/type_ptr.inl>
#include <gtc/matrix_transform.hpp>
#include <gtx/matrix_decompose.hpp>

VertexEngine::GLRenderer::GLRenderer(GlWindow* _win, std::shared_ptr<Shader> _defaultVertex, std::shared_ptr<Shader> _defaultFrag, std::shared_ptr<Shader> _defaultSkinnedVertex)
{
	m_WindowHandle = _win;
	if (!gladLoadGL()) {
		std::cout << "Glad Failed" << std::endl;

	}

	// Cache the default shaders
	m_DefaultFragShader = _defaultFrag;
	m_DefaultVertexShader = _defaultVertex;
	m_DefaultSkinnedVertexShader = _defaultSkinnedVertex;

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

	InitBoneBuffer();
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
	if (!m_DefaultFragShader || !m_DefaultVertexShader || !m_DefaultSkinnedVertexShader) return;

	UseShader(m_DefaultSkinnedVertexShader, m_DefaultFragShader);

	for (auto& obj : m_RenderQueue) {

		if (!obj.m_Models) continue;

		SetBool("IsSkinned", obj.m_IsSkinned);

		// prepare bones 
		if (obj.m_IsSkinned && obj.m_Models->m_Skeleton) {

			std::vector<glm::mat4> boneMats;

			CalculateSkinningMatrices(*obj.m_Models, boneMats);

			if (!boneMats.empty())
				UploadBoneMatrices(boneMats);

			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_BUFFER, m_BoneTexture);

			SetInt("BoneBuffer", 7);
		}

		RenderModelNode(obj.m_Models->m_RootNode, glm::mat4(1.0f), obj);
	}

	m_RenderQueue.clear();
}

void VertexEngine::GLRenderer::EndFrame()
{
	if (m_WindowHandle)
		glfwSwapBuffers(m_WindowHandle->GetWindowHandle());

	ClearStatisticData();
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
	SetPassChange();
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


			// Check if the material has a override shader. if it does use it.
			 
			auto& Shader = material.GetOvverideShader();

			if (PermissionToUseShader(Shader)){
				UseShader(m_DefaultSkinnedVertexShader, Shader);
				m_CustomShaderOverride = Shader;
			}


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
			BatchesChange();
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

	// Bones
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
	glEnableVertexAttribArray(5);

	// Bone Weight

	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));
	glEnableVertexAttribArray(6);

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

	if (shader == 0) {
		std::cout << "VERTEX ERROR: Shader Failed" << std::endl;
	}

	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success != GL_TRUE)
	{
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(
			logLength > 0 ? static_cast<size_t>(logLength) : 1,
			'\0');

		if (logLength > 0)
		{
			glGetShaderInfoLog(
				shader,
				logLength,
				nullptr,
				log.data());
		}

		std::cerr
			<< "Shader compilation failed ("
			<< (type == GL_VERTEX_SHADER ? "Vertex" :
				type == GL_FRAGMENT_SHADER ? "Fragment" : "Unknown")
			<< "):\n"
			<< log << '\n';

		glDeleteShader(shader);
		return 0;
	}

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

void VertexEngine::GLRenderer::BuildBoneNodeTransforms(const ModelNode& _node, const glm::mat4& _parentTransform, const Skeleton& _skeleton, std::vector<glm::mat4>& _boneNodeTransforms, std::vector<bool>& _boneFound)
{
	glm::mat4 nodeGlobal =
		_parentTransform * _node.m_LocalTransform;

	for (size_t i = 0; i < _skeleton.m_bones.size(); ++i) {
		if (_skeleton.m_bones[i].name == _node.m_Name) {
			_boneNodeTransforms[i] = nodeGlobal;
			_boneFound[i] = true;
		}
	}

	if (_node.m_Name == "mixamorig:Spine")
	{
		std::cout << "Spine parent transform:\n";

		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
				std::cout << _parentTransform[col][row] << ' ';

			std::cout << '\n';
		}
	}

	for (const auto& child : _node.m_Children) {
		BuildBoneNodeTransforms(
			child,
			nodeGlobal,
			_skeleton,
			_boneNodeTransforms,
			_boneFound
		);
	}
}

void VertexEngine::GLRenderer::CalculateSkinningMatrices(const Model& _model, std::vector<glm::mat4>& _outMat)
{
	_outMat.clear();

	if (!_model.m_Skeleton)
		return;

	const Skeleton& skeleton = *_model.m_Skeleton;
	const size_t boneCount = skeleton.m_bones.size();

	if (boneCount == 0)
		return;

	if (boneCount > 310)
	{
		std::cerr
			<< "Warning: Skeleton has "
			<< boneCount
			<< " bones; maximum supported is 310.\n";
	}

	std::vector<glm::mat4> boneNodeTransforms(
		boneCount, glm::mat4(1.0f));

	std::vector<bool> boneFound(boneCount, false);

	BuildBoneNodeTransforms(
		_model.m_RootNode,
		glm::mat4(1.0f),
		skeleton,
		boneNodeTransforms,
		boneFound
	);

	_outMat.resize(boneCount, glm::mat4(1.0f));

	for (size_t i = 0; i < boneCount; ++i)
	{
		if (!boneFound[i])
		{
			std::cerr
				<< "Warning: Bone node not found: "
				<< skeleton.m_bones[i].name
				<< '\n';

			continue;
		}

		_outMat[i] =
			boneNodeTransforms[i] *
			skeleton.m_bones[i].offsetMat;

		if (skeleton.m_bones[i].name == "mixamorig:LeftShoulder")
		{
			auto printMatrix = [](const char* label, const glm::mat4& m)
				{
					std::cout << "\n" << label << ":\n";

					for (int row = 0; row < 4; ++row)
					{
						for (int col = 0; col < 4; ++col)
							std::cout << m[col][row] << ' ';

						std::cout << '\n';
					}
				};

			printMatrix("Shoulder global", boneNodeTransforms[i]);
			printMatrix("Shoulder offset", skeleton.m_bones[i].offsetMat);
			printMatrix("Shoulder skin", _outMat[i]);
		}

		float maxError = 0.0f;

		for (int col = 0; col < 4; ++col)
		{
			for (int row = 0; row < 4; ++row)
			{
				float expected = (col == row) ? 1.0f : 0.0f;

				maxError = std::max(
					maxError,
					std::abs(_outMat[i][col][row] - expected)
				);
			}
		}

		std::cout
			<< skeleton.m_bones[i].name
			<< " bind-pose identity error: "
			<< maxError << '\n';
	}
}

void VertexEngine::GLRenderer::UploadBoneMatrices(const std::vector<glm::mat4>& _matrices)
{
	constexpr size_t MAX_BONES = 310;

	if (!_matrices.empty())
	{
		const glm::mat4& m = _matrices[0];

		std::cout << "Bone 0 matrix:\n";

		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				std::cout << m[col][row] << " ";
			}

			std::cout << '\n';
		}
	}

	if (m_BoneBuffer == 0 || _matrices.empty())
		return;

	const size_t count =
		std::min(_matrices.size(), MAX_BONES);

	glBindBuffer(GL_TEXTURE_BUFFER, m_BoneBuffer);

	glBufferSubData(
		GL_TEXTURE_BUFFER,
		0,
		count * sizeof(glm::mat4),
		_matrices.data()
	);

	glBindBuffer(GL_TEXTURE_BUFFER, 0);
}

bool VertexEngine::GLRenderer::SetBoneRotation(Model& _model, const std::string& _bonename, const glm::quat& _rotation)
{
	return SetBoneRotationRecursive(_model.m_RootNode, _bonename, _rotation);
}

bool VertexEngine::GLRenderer::SetBoneRotationRecursive(ModelNode& _model, const std::string& _bonename, const glm::quat& _rotation)
{
	if (_model.m_Name == _bonename)
	{
		glm::vec3 position;
		glm::vec3 scale;
		glm::quat bindRotation;

		glm::vec3 skew;
		glm::vec4 perspective;

		if (!glm::decompose(
			_model.m_BindLocal,
			scale,
			bindRotation,
			position,
			skew,
			perspective))
		{
			return false;
		}

		glm::mat4 rotationMatrix =
			glm::mat4_cast(glm::normalize(_rotation));

		_model.m_LocalTransform =
			glm::translate(glm::mat4(1.0f), position)
			* rotationMatrix
			* glm::scale(glm::mat4(1.0f), scale);

		return true;
	}

	for (auto& child : _model.m_Children)
	{
		if (SetBoneRotationRecursive(child, _bonename, _rotation))
			return true;
	}

	return false;
}

bool VertexEngine::GLRenderer::PermissionToUseShader(std::shared_ptr<Shader> _shader)
{
	if (_shader.get() == nullptr) return false;

	if (m_CustomShaderOverride.get() != nullptr) return true;

	if (_shader.get() != m_CustomShaderOverride.get()) return true;

	return false;
}

void VertexEngine::GLRenderer::InitBoneBuffer()
{
	glGenBuffers(1, &m_BoneBuffer);
	glBindBuffer(GL_TEXTURE_BUFFER, m_BoneBuffer);

	// 310 matrices × 4 vec4 columns per matrix.
	glBufferData(
		GL_TEXTURE_BUFFER,
		310 * 4 * sizeof(glm::vec4),
		nullptr,
		GL_DYNAMIC_DRAW
	);

	glGenTextures(1, &m_BoneTexture);
	glBindTexture(GL_TEXTURE_BUFFER, m_BoneTexture);

	glTexBuffer(
		GL_TEXTURE_BUFFER,
		GL_RGBA32F,
		m_BoneBuffer
	);

	glBindTexture(GL_TEXTURE_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
}
