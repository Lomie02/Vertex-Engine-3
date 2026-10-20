#pragma once
#include "GLWindow.h"
#include "Renderer.h"
#include <unordered_map>
#include "Shader.h"
#include <map>
#include "GPUMesh.h"
namespace VertexEngine {
	class GLRenderer : public VertexEngine::Renderer
	{
	public:
		GLRenderer(GlWindow* _win, std::shared_ptr<Shader> _defaultVertex, std::shared_ptr<Shader> _defaultFrag, std::shared_ptr<Shader> _defaultSkinnedVertex); // Takes in the window alongside the default opengl shaders. Provided by backend.

		void BeginFrame() override; // Prepare start of frame.
		void ClearFrame() override; // Clear flags
		void Submit(const Renderable& _mesh) override; // Submit renderables to the render queue
		void Render() override; // Render everything in queue
		void EndFrame() override; // End frame & switch polls

		void SubmitCamera(const CameraRenderable& _camera) override; // Submit Camera
		void BindTexture(std::shared_ptr<Texture> _texture) override; // Bind textures

		// Shader
		unsigned int CompileProgram(std::shared_ptr<Shader> _vertex, std::shared_ptr<Shader> _frag); // Compile the shader program
		void UseShader(std::shared_ptr<Shader> _vertex, std::shared_ptr<Shader> _frag); // Use shader

		void RenderModelNode(const VertexEngine::ModelNode& node, const glm::mat4& parentTrans, const Renderable& obj);
	private:

		CameraRenderable m_ActiveCamera; // Active CameraRenderable

		uint32_t UploadMesh(std::shared_ptr<VertexEngine::MeshData> _mesh);

		uint32_t GenerateUniqueMeshId() { return m_GeneratedIds++; } // Generate a different id for each uploaded mesh for easy cache

		uint32_t m_GeneratedIds = 0;
		void SetMatrix4(std::string _name, const glm::mat4& matrix); // Set the matrix uniform of shader.
		void SetVector4f(std::string _name, const glm::vec4& _vec); // Set a vector 4 of shader.

		void SetInt(std::string _name, int _value);
		void SetBool(std::string _name, bool _state);

		unsigned int CompileShader(unsigned int type, const std::string& source); // Compile the shaders
		unsigned int UploadTexture(std::shared_ptr<Texture> _texture); // Upload texture to gpu

		GlWindow* m_WindowHandle; // Game window

		std::vector<Renderable> m_RenderQueue; // Render queue for renderering
		std::unordered_map<std::shared_ptr<Texture>, unsigned int >m_gpuHandle; //lazy loading for textures
		std::map<std::pair<std::shared_ptr<Shader>, std::shared_ptr<Shader>>, unsigned int > m_Programs; // Lazy loading for shaders

		// Default OpenGL shaders
		std::shared_ptr<Shader> m_DefaultVertexShader; 
		std::shared_ptr<Shader> m_DefaultSkinnedVertexShader; 
		std::shared_ptr<Shader> m_DefaultFragShader;

		// Cache the stored custom shader to avoid loading it twice in a render.
		std::shared_ptr<Shader> m_CustomShaderOverride;

		// GLRenderer.h
		unsigned int m_ActiveProgram = 0;
		unsigned int m_TriangleVAO = 0;
		unsigned int m_TriangleVBO = 0;

		std::unordered_map<uint32_t, VertexEngine::GPUMesh> m_MeshCacheList; // Cache all uploaded Meshes

		// Skinned Meshes
		void BuildBoneNodeTransforms(const ModelNode& _node, const glm::mat4& _parentTransform, const Skeleton& _skeleton, std::vector<glm::mat4>& _boneNodeTransforms, std::vector<bool>& _boneFound);
		void CalculateSkinningMatrices(const Model& _model, std::vector<glm::mat4>& _outMtraices);
		void UploadBoneMatrices(const std::vector<glm::mat4>& _matrices);

		bool SetBoneRotation(Model& _model, const std::string& _bonename, const glm::quat& _rotation);
		bool SetBoneRotationRecursive(ModelNode& _model, const std::string& _bonename, const glm::quat& _rotation);

		// Shader
		bool PermissionToUseShader(std::shared_ptr<Shader> _shader); // Check if renderer has permission to change the current shader.

		// Skeletal Buffers
		void InitBoneBuffer();
		unsigned int m_BoneBuffer = 0;
		unsigned int m_BoneTexture = 0;
	};
}

