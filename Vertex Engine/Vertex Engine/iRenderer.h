#pragma once

class iRenderer {
public:
	virtual ~iRenderer() = default;

	virtual void ClearRenderFlags() = 0;
	virtual void RenderMesh() = 0;
	virtual void RenderSprite() = 0;

};