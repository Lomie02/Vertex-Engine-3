#pragma once
#include "Component.h"
#include "Model.h"
namespace VertexEngine {

	class SkeletalMeshRenderer : public VertexEngine::Component
	{
	public:
		virtual ComponentFlags GetFlags() const override {
			return ComponentFlags::Skinned;
		}

		void SetModel(const std::shared_ptr<Model>& _model) 
		{ m_Model = _model; }

		std::shared_ptr<Model> GetModel() const { return m_Model; }

	private:
		std::shared_ptr<Model> m_Model;

	};
}

