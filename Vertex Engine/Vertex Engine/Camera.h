#pragma once
#include "Component.h"
#include "ProjectionType.h"

namespace VertexEngine {

	struct Camera : public VertexEngine::Component
	{
	public:

		virtual ComponentFlags GetFlags() const override {
			return ComponentFlags::Camera;
		}

		ProjectionType m_ProjectionMode = ProjectionType::Perspective;

		float m_FieldOfView = 60.0f;
		float m_NearClip = 0.1f;
		float m_FarClip = 100.0f;

		float m_OrthoSize = 10.0f;
	};
}