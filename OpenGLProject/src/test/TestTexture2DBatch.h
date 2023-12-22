#pragma once

#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "glm/vec3.hpp"
#include "imgui/imgui.h"

namespace test
{
	class TestTexture2DBatch : public Test
	{
	public:
		TestTexture2DBatch();
		~TestTexture2DBatch();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture1, m_Texture2;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;

		ImGuiIO& m_IO;

	};
}
