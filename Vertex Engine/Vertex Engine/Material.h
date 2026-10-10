#pragma once
#include <string>
#include <memory>
#include "Texture.h"
#include "MaterialData.h"
namespace VertexEngine {

	class Material : public VertexEngine::Asset {

	public:

		Material() = default;

		Material(const MaterialData& _data) {
			m_Name = _data.m_Name;
			m_AlbedoColour = _data.m_AlbedoColour;
			m_Metallic = _data.m_Metallic;
			m_Roughness = _data.m_Roughness;
			m_AlbedoOverride = _data.m_AlbedoOverride;
		}

		void Load(const std::string& _path) override {}

		const std::string& GetName() const { return m_Name; }

		void SetAlbedoColour(const glm::vec4& _colour) { m_AlbedoColour = _colour; }

		void SetMetallic(float _metal) { m_Metallic = _metal; }
		void SetRoughness(float _rough) { m_Roughness = _rough; }

		void SetAlbedoMap(std::shared_ptr<Texture> _tex) { m_AlbedoMap = _tex; }
		void SetMetallicMap(std::shared_ptr<Texture> _tex) { m_MetallicMap = _tex; }
		void SetRoughnessMap(std::shared_ptr<Texture> _tex) { m_RoughnessMap = _tex; }
		void SetNormalMap(std::shared_ptr<Texture> _tex) { m_NormalMap = _tex; }

		// Getter funcs

		const std::shared_ptr<Texture>& GetAlbedoMap() const { return m_AlbedoMap; }
		const std::shared_ptr<Texture>& GetMetallicMap() const { return m_MetallicMap; }
		const std::shared_ptr<Texture>& GetRoughnessMap() const { return m_RoughnessMap; }
		const std::shared_ptr<Texture>& GetNormalMap() const { return m_NormalMap; }

		const std::string& GetAlbedoOverride() const { return m_AlbedoOverride; }

		const glm::vec4& GetAlbedoColour() { return m_AlbedoColour; }


	private:

		std::string m_Name;
		glm::vec4 m_AlbedoColour{ 1.0f };

		float m_Metallic = 0.0f;
		float m_Roughness = 1.0f;

		std::string m_AlbedoOverride;

		std::shared_ptr<Texture> m_AlbedoMap;
		std::shared_ptr<Texture> m_MetallicMap;
		std::shared_ptr<Texture> m_RoughnessMap;
		std::shared_ptr<Texture> m_NormalMap;

		uint32_t m_ID = 0;
	};

}