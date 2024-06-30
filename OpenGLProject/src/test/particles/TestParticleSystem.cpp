#include "TestParticleSystem.h"

#include "Camera.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/fast_square_root.hpp"
#include "glm/gtx/norm.inl"

namespace test
{

    // static definitions for linker
    constexpr glm::vec3 TestParticleSystem::m_ParticleSource;
    constexpr glm::vec3 TestParticleSystem::m_ParticleMaxSpeed;

    TestParticleSystem::TestParticleSystem()
        : m_Translation(0, 0, 0),
		m_IO(ImGui::GetIO()),
        m_lastCameraPosition(0.f, 0.f, 0.f)
	{
        unsigned int indices[m_MaxParticleCount * 6];

        for (int i=0; i < m_MaxParticleCount; ++i)
        {
            m_ParticlePositions[i] = m_ParticleSource;

            float rectVertices[] = {
                // pos_x, pos_y, pos_z, color_r, color_g, color_b, color_a, tex_x, tex_y, tex_id
                m_ParticleSource.x - m_ParticleSize, m_ParticleSource.y - m_ParticleSize, m_ParticleSource.z+0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,
                m_ParticleSource.x + m_ParticleSize, m_ParticleSource.y - m_ParticleSize, m_ParticleSource.z+0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
                m_ParticleSource.x + m_ParticleSize, m_ParticleSource.y + m_ParticleSize, m_ParticleSource.z+0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
                m_ParticleSource.x - m_ParticleSize, m_ParticleSource.y + m_ParticleSize, m_ParticleSource.z+0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,
            };

            memcpy( &m_Vertices[i*4*10], &rectVertices, sizeof(rectVertices));

            unsigned int rectIndices[] = {
                0 + 4u * i, 1 + 4u * i, 2 + 4u * i, 2 + 4u * i, 3 + 4u * i, 0 + 4u * i
            };

            memcpy(&indices[i * 6], &rectIndices, sizeof(rectIndices));

            m_ParticleVelocities[i] = glm::vec3{ glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f) };
        }

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, m_MaxParticleCount * 6);

        m_VertexBuffer = std::make_unique<VertexBuffer>(m_Vertices, sizeof(m_Vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3); // 3D pos
        layout.Push<float>(4); // Color RGBA
        layout.Push<float>(2); // Texture coordinates
        layout.Push<float>(1); // Texture ID
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_Shader = std::make_unique<Shader>("res/shaders/BasicParticle.shader");
        m_Shader->Bind();

        // Texture stuff
        m_Texture1 = std::make_unique<Texture>("res/textures/pix-platformer.png");
        m_Texture2 = std::make_unique<Texture>("res/textures/scenery.png");
        //m_Shader->SetUniform1i("u_Texture", 0);
        int samplers[2] = { 0, 1 };
        m_Shader->SetUniform1iv("u_Textures", 2, samplers);

		(void)m_IO;

	}

    TestParticleSystem::~TestParticleSystem() = default;

	void TestParticleSystem::OnUpdate(float deltaTime)
	{
        // Emit particles
        m_TimeSinceEmissionStart += deltaTime;
        if(m_TimeSinceEmissionStart*m_EmissionRate > m_ActiveParticleCount)
        {
            unsigned int newPartclesToEmit = (m_TimeSinceEmissionStart * m_EmissionRate) - m_ActiveParticleCount;
            if (newPartclesToEmit > 0 && m_ActiveParticleCount + newPartclesToEmit <= m_MaxParticleCount)
            {
                m_ActiveParticleCount += newPartclesToEmit;
            }
        }

        // Update particles
        for (unsigned int i = 0; i < m_ActiveParticleCount; ++i)
        {
            unsigned int particleIndex = m_FirstParticleIndex + i;

            const glm::vec3 particleVelocity = m_ParticleMaxSpeed * m_ParticleVelocities[particleIndex];
            m_ParticlePositions[i] += particleVelocity;

            // check if distance limit reached. approximated from first vertex - bottom left.
            const glm::vec3 distanceVec = m_ParticlePositions[i] - m_ParticleSource;
            const float sqDistanceFromSource = glm::dot(distanceVec, distanceVec);

            // make shape
            const float rectVertices[] = {
                -m_ParticleSize, -m_ParticleSize, 0.0f,
                +m_ParticleSize, -m_ParticleSize, 0.0f,
                +m_ParticleSize, +m_ParticleSize, 0.0f,
                -m_ParticleSize, +m_ParticleSize, 0.0f
            };

            if (sqDistanceFromSource >= m_ParticleMaxDistanceSq)
            {
                // set particle back to emitter source
                m_ParticlePositions[i] = m_ParticleSource;
            }

            // Rotate particles to face camera
            glm::mat4 billboardMatrix = GetBillboardMatrix(m_ParticlePositions[i], m_lastCameraPosition, Camera::GetCameraUp());

            for (int vi = 0; vi < 4; ++vi)
            {
            	glm::vec3 vertexPosition = glm::vec3(
                    billboardMatrix * glm::vec4(rectVertices[0 + vi * 3], rectVertices[1 + vi * 3], rectVertices[2 + vi * 3], 1.0f)
				);

	            vertexPosition += m_ParticlePositions[i];
	            memcpy(&m_Vertices[particleIndex * 4 * 10 + vi*10], glm::value_ptr(vertexPosition), sizeof(float) * 3);
            }
            
        }

        m_VertexBuffer->UpdateBuffer(0, sizeof(m_Vertices), &m_Vertices);
	}

	void TestParticleSystem::OnRender(const Camera& camera)
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer; // low memory footprint. Not a big issue creating every frame.

        m_Texture1->Bind(0);
        m_Texture2->Bind(1);

        {
            m_Shader->Bind(); // Bind the shader
            // translate model
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);

            glm::mat4 mvp = camera.GetCameraMatrix() * model; // OpenGL is column major, hence P V M 
            // Draw
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        m_lastCameraPosition = camera.GetCameraPosition();
	}

	void TestParticleSystem::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_Translation.x, 0.0f, 960.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO.Framerate, m_IO.Framerate);
	}

    glm::mat4 TestParticleSystem::GetBillboardMatrix(const glm::vec3& particleCenter, const glm::vec3& cameraPosition, const glm::vec3& upVector)
    {
        const glm::vec3 direction = glm::normalize(cameraPosition - particleCenter);
        const glm::vec3 right = glm::cross(upVector, direction);
        const glm::vec3 newUp = glm::cross(direction, right);

        glm::mat4 rotationMatrix(1.0f);
        rotationMatrix[0] = glm::vec4(right, 0.0f);
        rotationMatrix[1] = glm::vec4(newUp, 0.0f);
        rotationMatrix[2] = glm::vec4(direction, 0.0f);

        return rotationMatrix;
    }

}
