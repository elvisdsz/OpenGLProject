#include "TestDynamicGeometry.h"

#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    TestDynamicGeometry::TestDynamicGeometry()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_IO(ImGui::GetIO())
	{
        float positions[] = { // pos_x, pos_y, pos_z, color_r, color_g, color_b, color_a, tex_x, tex_y, tex_id
            -50.0f, -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,
             50.0f, -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
             50.0f,  50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
            -50.0f,  50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,

            50.0f,  50.0f,   0.0f, 1.0f, 0.5f, 0.4f, 1.0f, 0.0f, 0.0f, 1.0f,
        	150.0f, 50.0f,   0.0f, 1.0f, 0.5f, 0.4f, 1.0f, 1.0f, 0.0f, 1.0f,
        	150.0f, 150.0f,  0.0f, 1.0f, 0.5f, 0.4f, 1.0f, 1.0f, 1.0f, 1.0f,
            50.0f,  150.0f,  0.0f, 1.0f, 0.5f, 0.4f, 1.0f, 0.0f, 1.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 10 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3); // 3D pos
        layout.Push<float>(4); // Color RGBA
        layout.Push<float>(2); // Texture coordinates
        layout.Push<float>(1); // Texture ID
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_Shader = std::make_unique<Shader>("res/shaders/Advanced.shader");
        m_Shader->Bind();

        // Texture stuff
        m_Texture1 = std::make_unique<Texture>("res/textures/pix-platformer.png");
        m_Texture2 = std::make_unique<Texture>("res/textures/scenery.png");
        //m_Shader->SetUniform1i("u_Texture", 0);
        int samplers[2] = { 0, 1 };
        m_Shader->SetUniform1iv("u_Textures", 2, samplers);

		(void)m_IO;

	}

    TestDynamicGeometry::~TestDynamicGeometry() = default;

	void TestDynamicGeometry::OnUpdate(float deltaTime)
	{
	}

	void TestDynamicGeometry::OnRender(const Camera& camera)
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer; // low memory footprint. Not a big issue creating every frame.

        m_Texture1->Bind(0);
        m_Texture2->Bind(1);

        {
            m_Shader->Bind(); // Bind the shader
            // translate model
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model; // OpenGL is column major, hence P V M 
            // Draw
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        /*{
            m_Shader->Bind(); // Bind the shader
            // translate model
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model; // OpenGL is column major, hence P V M 
            // Draw again
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }*/
	}

	void TestDynamicGeometry::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO.Framerate, m_IO.Framerate);
	}

}
