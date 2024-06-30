#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"    

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Camera.h"

#include "test/TestClearColor.h"
#include "test/TestTexture2D.h"
#include "test/TestTexture2DBatch.h"
#include "test/TestDynamicGeometry.h"
#include "test/particles/TestParticleSystem.h"

float LastMousePosX = 0.f, LastMousePosY = 0.f;
bool MouseStartedMoving = false;

// Function to handle mouse movement
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    LastMousePosX = xpos;
    LastMousePosY = ypos;
}

int main(void)
{
    GLFWwindow* window;
    int win_width = 960, win_height = 540;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(win_width, win_height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // vsync / sync with monitor refresh rate

    if (glewInit() != GLEW_OK)
        return -1;

    std::cout << glGetString(GL_VERSION) << std::endl;

    { // scope for stack alloc data (incl buffers) to be destructed before glfwTerminate and losing opengl context

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        Camera camera(win_width, win_height);

        glfwSetCursorPosCallback(window, mouseCallback);

        // ImGUI stuff
        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

        ImGui::CreateContext();
        
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        // ImGui state
        //bool show_demo_window = true;
        //bool show_another_window = false;
        //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
        testMenu->RegisterTest<test::TestTexture2DBatch>("Batched 2D Texture");
        testMenu->RegisterTest<test::TestDynamicGeometry>("Dynamic Geometry");
        testMenu->RegisterTest<test::TestParticleSystem>("Particle System");

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
            /* Render here */
            renderer.Clear();

            // ImGui new frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            camera.OnUpdate(window, LastMousePosX, LastMousePosY); // check camera movement
            camera.OnImGuiRender(); // show camera parameters

            if (currentTest)
            {
                currentTest->OnUpdate(0.01667f); // TODO: Get actual deltaTime value
                currentTest->OnRender(camera);

                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-")) // add back button
                {
	                // if button clicked
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            // ImGui Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;

    }

    // ImGui shutdown
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwTerminate();
    return 0;
}
