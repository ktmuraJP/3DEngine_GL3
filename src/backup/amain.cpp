#include "src.h"
#include "shader.h"
#include "model.h"
#include "objects.h"
//#include "camera.h"
#include "input.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_JA_range.h" // original header

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"
#include <iostream>

int window_w = 800;
int window_h = 600;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 cursor;
Camera camera(glm::vec3(2.0f,1.0f,1.0f));

class SceneClass
{
    public:
        GLFWwindow *window;
        unsigned int width,height;
        unsigned int framebuffer,depthbuffer,texture,drawbuffer;

        BlockPartsObject Acannon{"46cm naval gun","46cm",
            "data/460mm/460mm.obj"};
        BlockPartsObject Bcannon{"12.7cm naval gun","12.7cm",
            "data/127mmBdouble/127mmB.obj"};
        BlockPartsObject block{"cube","cube!",
           //"data/460mm/460mm.obj"};
            //"data/127mmBdouble/127mmB.obj"};
            "data/cube/cube.obj"};
        BlockPartsObject cannon{"cannon","cannon",
           //"data/460mm/460mm.obj"};
            "data/127mmBdouble/127mmB.obj"};
            //"data/cube/cube.obj"};

        VesselObject vessel{"test vessel"};
        glm::vec3 lightPos;

        SceneClass(unsigned int width, unsigned int height)
        {
            this->width = width;
            this->height = height;
            this->lightPos = glm::vec3(5.0f,5.0f,0.0f);
        }
        void render()
        {
            glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 projection = glm::perspective(
                    glm::radians(camera.Zoom),
                (float)this->width / (float)this->height, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model = glm::mat4(1.0f);
            
            {
                model = glm::translate(model,glm::vec3(2.0f,0.5f,0.0f)); 
                cannon.DrawMainModel(projection,view,model,1);
                model = glm::mat4(1.0f);
                model = glm::translate(model,glm::vec3(2.0f,0.5f,0.5f)); 
                cannon.DrawMainModel(projection,view,model,0);
                if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
                    cannon.DrawDimensionCube(projection,view,model);
            }
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model,glm::vec3(2.0f,0.5f,0.0f)); 
                block.DrawMainModel(projection,view,model,1);
                if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
                    block.DrawDimensionCube(projection,view,model);
            }
            {
                //UI
                DrawOcean();
                DrawCursor();
                DrawBackground();
            }
        }
    private:
        ModelClass cursor{"data/cursor/cursor.obj"};
        ShaderClass cursorshader{"shader/interface/interfacevs.glsl",
            "shader/interface/interfacefs.glsl"}; 
        
        void DrawCursor()
        {
            glm::mat4 model = glm::mat4(1.0f);
            float size,rx,ry;
            size = 10.0f;
            rx = size / (float)this->width;
            ry = size / (float)this->height;

            model = glm::scale(model, 
                    glm::vec3(rx,ry,0));
            
            this->cursorshader.use();
            this->cursorshader.setVec3("ObjectColor",1,1,1);

            this->cursorshader.setMat4("model", model);
            this->cursor.Draw(this->cursorshader);
        }
        std::vector<std::string> paths{
                "texture/cubemap/skybox/right.jpg",
                "texture/cubemap/skybox/left.jpg",
                "texture/cubemap/skybox/top.jpg",
                "texture/cubemap/skybox/bottom.jpg",
                "texture/cubemap/skybox/front.jpg",
                "texture/cubemap/skybox/back.jpg"
            };
        CubemapBackground cubemap{paths};
        
        void DrawBackground()
        {
            glm::mat4 projection = glm::perspective(
                    glm::radians(camera.Zoom),
                (float)this->width / (float)this->height, 0.1f, 100.0f);
            glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
            cubemap.Draw(projection,view); 
        }

        ModelClass ocean{"data/plane/plane.obj"};
        ShaderClass oceanshader{"shader/plain/plainvs.glsl",
            "shader/plain/plainfs.glsl"};

        void DrawOcean()
        {
            glm::mat4 projection = glm::perspective(
                    glm::radians(camera.Zoom),
                (float)this->width / (float)this->height, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();

            oceanshader.use();
            oceanshader.setVec3("ObjectColor",1.0f,1.0f,1.0f);
            oceanshader.setMat4("projection",projection);
            oceanshader.setMat4("view",view);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, 
                    glm::vec3(camera.Position.x,-1.0f,camera.Position.z)); 
            model = glm::scale(model, glm::vec3(10000));
            oceanshader.setMat4("model", model);
            ocean.Draw(oceanshader);
        }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mainloop(GLFWwindow* window)
{
    int display_w,display_h;
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable(GL_DEPTH_TEST);

    SceneClass mainscene(window_w,window_h);
    mainscene.window = window;
    ModelClass cube{"data/cube/cube.obj"};
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        KeyProcessInput(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        {
            mainscene.width = display_w;
            mainscene.height = display_h;
            mainscene.render();
            
            ImGui::Begin("fps");{
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }ImGui::End(); 

            glm::mat4 view = camera.GetViewMatrix();
            ImGui::Begin("camera");{
                ImGui::Text("camera x %f",camera.Position.x);
                ImGui::Text("camera y %f",camera.Position.y);
                ImGui::Text("camera z %f",camera.Position.z);
                ImGui::PushButtonRepeat(true);
                for(int i=0;i<4;i++){
                    ImGui::Text("camera %f,%f,%f,%f",
                            view[i].x,view[i].y,view[i].z,view[i].w);
                }
                ImGui::PopButtonRepeat();
            }ImGui::End();
            ImGui::ShowDemoWindow(NULL);
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int WindowContext()
{

    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // 3.0+ only

    GLFWwindow* window = glfwCreateWindow(window_w, 
            window_h, "bsc", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1); // Enable vsync

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize OpenGL loader

    bool err = gl3wInit();

    if (err)
    {
        fprintf(stderr, "gl3w initialize failure\n");
        return 1;
    }
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    //// Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      
    //// Enable Gamepad Controls
    
    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    //ImGuiStyle &style = ImGui::GetStyle();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("lib/imgui/misc/fonts/Roboto-Medium.ttf",10.0f);
    //io.Fonts->AddFontFromFileTTF("lib/fonts/SoukouMincho.ttf",15.0f,NULL,JAglyphRanges);
    
    //standard font 
    io.Fonts->AddFontFromFileTTF("lib/fonts/x14y24pxHeadUpDaisy.ttf",17.0f,NULL,JAglyphRanges);

    //ImFont* font = 
    //io.Fonts->AddFontFromFileTTF
    //("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Main loop
    mainloop(window);
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

int main()
{
    // Setup window
    if(WindowContext() == 1)
        return 1;
    // Setup Dear ImGui context
    return 0;
}
