#pragma once

#include "../Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "glm/vec3.hpp"
#include "imgui/imgui.h"

namespace test
{
	class TestParticleSystem : public Test
	{
	public:
		TestParticleSystem();
		~TestParticleSystem();

		void OnUpdate(float deltaTime) override;
		void OnRender(const Camera& camera) override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture1, m_Texture2;

		glm::vec3 m_Translation;

		ImGuiIO& m_IO;

		// configuration params
		static constexpr int m_MaxParticleCount = 100;
		static constexpr float m_ParticleSize = 5.f;
		static constexpr glm::vec3 m_ParticleSource = { 0.f, 0.f, 0.f };
		static constexpr float m_ParticleMaxDistanceSq = 30000.f; // from source
		static constexpr glm::vec3 m_ParticleMaxSpeed = { 1.f, 1.f, 1.0f };
		static constexpr glm::vec3 m_EmissionDirection = { 0.f, 0.f, 0.f };
		static constexpr float m_EmissionRate = 3.0f;

		// buffer
		float m_Vertices[m_MaxParticleCount * 4 * 10];
		glm::vec3 m_ParticleVelocities[m_MaxParticleCount];

		// working params
		float m_TimeSinceEmissionStart = 0;
		unsigned int m_FirstParticleIndex = 0;
		unsigned int m_ActiveParticleCount = 0;

	};
}
