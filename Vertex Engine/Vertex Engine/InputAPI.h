#pragma once
namespace VertexEngine {

	class InputAPI {
	public:
		virtual ~InputAPI() = default;

		virtual bool GetKeyDown(int _key) {};
		virtual bool GetKeyUp(int _key) {};
		virtual bool GetKey(int _key) {};

	};
}