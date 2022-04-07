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

//#define STB_IMAGE_IMPLEMENTATION 
//#include "stb_image.h"
#include <iostream>

int window_w = 800;
int window_h = 600;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 cursor;
Camera camera(glm::vec3(0.0f,0.0f,0.0f));

enum BSCenum{
    BATTLEMODE,
    EDITMODE
};

class SceneClass
{

public:
    GLFWwindow *window;
    unsigned int width,height;
    unsigned int framebuffer,depthbuffer,texture,drawbuffer;
    enum BSCenum mode = BATTLEMODE;
     
    BlockPartsObject block{"127","127B",
        //"data/cube/cube.obj"};
        "data/models/127mmBdouble/127mmB.obj"};
    BlockPartsObject block2{"460","460mm",
        //"data/cube/cube.obj"};
        "data/models/460mm/460mm.obj"};
    BlockPartsObject block3{"cube","cubic",
        "data/models/cube/cube.obj"};

    
    ParticleObject particle{"texture/laugh.png",glm::vec3(5.0f,0.0f,5.0f),100,10.0f};
    BillboardObject board{"texture/yukkuri.png"};
    ProjectileObject projectile{block};
    VectorObject vector{glm::vec3(0.0f)};
    PointObject point{}; 
    glm::vec3 lightPos; 

    SceneClass(unsigned int width, unsigned int height)
    {
        this->width = width;
        this->height = height;
        camera.Position = glm::vec3(0.0f,7.0f,20.0f);
        mode = EDITMODE;
        camera.isOrtho = true;
        camera.SpeedRate =100.0f;
    }

    float view_distance = 1000;
    float zoomrate = 1.0f;
    /*
        if(glfwGetKey(window,GLFW_KEY_O)==GLFW_PRESS);

        projection = glm::ortho(
                -aspectRatio,aspectRatio,
                -1.0f,1.0f,
                dx,dy); 
     */
    float ax = 1.0f;
    float ay = 1.0f;
    float az = 1.0f;
    
    float scalx = 0.0f;
    float scaly = 0.0f;
    float scalz = 0.0f;
    void render()
    { 
        //float aspectRatio = (float)this->width / (float)this->height;
        float time = glfwGetTime();
        
        if(glfwGetKey(window,GLFW_KEY_B)==GLFW_PRESS){
            mode = BATTLEMODE;
            camera.isOrtho = false; 
        }if(glfwGetKey(window,GLFW_KEY_E)==GLFW_PRESS){
            mode = EDITMODE;
            camera.isOrtho = true; 
        }

            //basic camera settings per frame 

            camera.width = width;
            camera.height = height;
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 projection = camera.GetProjectionMatrix();
            
            if(glfwGetKey(window,GLFW_KEY_R)==GLFW_PRESS)
                camera.Position = glm::vec3(0.0f);

            {
                if(mode == BATTLEMODE){
                    //UI
                    DrawOcean();
                    //DrawCursor();
                    //DrawBackground();
                }
            }
        
        {
            block.position = glm::vec3(-5.0f,0.0f,0.0f);
            block.DrawMainModel();
            block.position = glm::vec3(-5.0f,0.0f,10.0f);
            block.DrawMainModel(false);
        }
        {
            block2.position = glm::vec3(-5.0f,0.0f,20.0f);
            block2.DrawMainModel();
            block2.position = glm::vec3(-5.0f,0.0f,40.0f);
            block2.DrawMainModel(false);
        }
        {
            float k = 30;
            block3.position = glm::vec3(k*cos(time),0,k*sin(time));
            block3.rotation = time*100;
            block3.DrawMainModel(); 
        }
        {
            if(glfwGetKey(window,GLFW_KEY_F)==GLFW_PRESS)
                projectile.ResetTime();
            //projectile.angles = glm::vec2(M_PI/2,M_PI/4);
            projectile.Update();
            //projectile.position = glm::vec3(glfwGetTime());
            projectile.Draw();
        }
        {
            vector.Resize(5.0f);
            vector.Draw(glm::vec3(0.0f),
                    glm::vec2(cos(time)*180, 75.0f),
                    glm::vec3(1.0f));
            
            point.Draw(glm::vec3(scalx,scaly,scalz),glm::vec3(1.0f),10);
            
            ImGui::Begin("vector");{

                ImGui::InputFloat("scalar x",&scalx, 0.1f, 32.0f, "%.1f");
                ImGui::InputFloat("scalar y",&scaly, 0.1f, 32.0f, "%.1f");
                ImGui::InputFloat("scalar z",&scalz, 0.1f, 32.0f, "%.1f");

            }ImGui::End();
        }
        {
            //パーティカルの描画は最後に！
            board.position = glm::vec3(3*cos(time),0.0f,5*sin(time));
            board.Resize(0.1);
            //board.Draw();
            particle.position = glm::vec3(4*cos(time),0.0f,2*sin(time));
            particle.Draw();
            particle.Update();
            particle.SetVelocity(glm::vec3(ax,ay,az));
            
            if(glfwGetKey(window,GLFW_KEY_I)==GLFW_PRESS){
                particle.Add(1);
            } 
            if(glfwGetKey(window,GLFW_KEY_U)==GLFW_PRESS){
                particle.Remove(1);
            } 
            ImGui::Begin("particles");{
                ImGui::Text("x : y =  %f : %f",board.ratio_x, board.ratio_y);
                ImGui::Text("%d particles",particle.Size());
                ImGui::InputFloat("life",&particle.life, 0.1f, 32.0f, "%.1f");
                ImGui::InputFloat("ax",&ax, 0.01f, 32.0f, "%.1f");
                ImGui::InputFloat("ay",&ay, 0.01f, 32.0f, "%.1f");
                ImGui::InputFloat("az",&az, 0.01f, 32.0f, "%.1f");

            }ImGui::End();
        }
        {
            Draw3DAxis(particle.position,glm::vec3(ax,ay,az));
        }
    }
private:

    std::vector<std::string> paths{
            "texture/cubemap/skybox/right.jpg",
            "texture/cubemap/skybox/left.jpg",
            "texture/cubemap/skybox/top.jpg",
            "texture/cubemap/skybox/bottom.jpg",
            "texture/cubemap/skybox/front.jpg",
            "texture/cubemap/skybox/back.jpg"
        };
    //CubemapBackground cubemap{paths};
    
    ModelClass ocean{"data/models/plane/plane.obj",true};
    ShaderClass oceanshader{"shader/plain/plainvs.glsl",
        "shader/plain/plainfs.glsl"};

    void DrawBackground()
    {
        /*
        glm::mat4 projection = glm::perspective(
                glm::radians(camera.Zoom),
            (float)this->width / (float)this->height, 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        *///cubemap.Draw(projection,view); 
    }
    VectorObject axisvector{};
    void Draw3DAxis(glm::vec3 position, glm::vec3 size)
    {
        vector.Resize(size.x);
        vector.Draw(position,glm::vec2(0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f));
        vector.Resize(size.y);
        vector.Draw(position,glm::vec2(0.0f,90.0f),glm::vec3(0.0f,1.0f,0.0f));
        vector.Resize(size.z);
        vector.Draw(position,glm::vec2(270.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f));
    }
    void DrawOcean()

    {
        float zoom = zoomrate * camera.Zoom;
        glm::mat4 projection = glm::perspective(
                glm::radians(zoom),
            (float)this->width / (float)this->height, 0.1f, view_distance);

        glm::mat4 view = camera.GetViewMatrix();

        oceanshader.use();
        oceanshader.setVec3("multiplyColor",1.0f,1.0f,1.0f);
        oceanshader.setVec3("addColor",0.0f,0.0f,0.0f);
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
    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.3, 0.3, 0.3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
            camera.width = display_w;
            camera.height = display_h;
            mainscene.render();
            
            ImGui::Begin("data");{
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::Text("window width %d", display_w);
                ImGui::Text("window heigth %d", display_h);
            }ImGui::End(); 

            glm::mat4 view = camera.GetViewMatrix();
            ImGui::Begin("camera");{
                ImGui::Text("Zoom %f",camera.Zoom);
                ImGui::Text("camera x %f",camera.Position.x);
                ImGui::Text("camera y %f",camera.Position.y);
                ImGui::Text("camera z %f",camera.Position.z);
                
                ImGui::Text("cursor x %f",camera.mouse.x);
                ImGui::Text("cursor y %f",camera.mouse.y);
                
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
