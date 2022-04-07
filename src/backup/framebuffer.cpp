#include "src.h"
#include "objects.h"
#include "gameobj.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_JA_range.h" // original header

//#define STB_IMAGE_IMPLEMENTATION 
//#include "stb_image.h"

int window_w = 800;
int window_h = 600;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 cursor;
Camera camera(glm::vec3(0.0f,0.0f,0.0f));
std::vector<glm::vec3> lightPositions;
//std::vector<ShaderClass> shaders = {};


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void inputprocess(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_B)==GLFW_PRESS)
        camera.isOrtho = false; 
    if(glfwGetKey(window,GLFW_KEY_E)==GLFW_PRESS)
        camera.isOrtho = true; 

    if(glfwGetKey(window,GLFW_KEY_R)==GLFW_PRESS)
        camera.Position = glm::vec3(0.0f);
    if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS)
        camera.SpeedRate = 100.0f; 
}

class SceneClass
{

public:
    GLFWwindow *window;
    unsigned int width,height;
    unsigned int FBO, IFBO, depthmap, screentexture;
     
    BlockPartsObject block{"127","127B",
        "data/models/127mmBdouble/127mmB.obj"};
    BlockPartsObject block2{"460","460mm",
        "data/models/460mm/460mm.obj"};
    BlockPartsObject block3{"cube","cubic",
        "data/models/cube/cube.obj"};

    //ShaderClass model{"shader/model/modelvs.glsl",
    //    "shader/model/modelfs.glsl", "shader/model/modelgs.glsl"};
    ShaderClass model{"shader/model/normal/modelvs.glsl",
        "shader/model/normal/modelfs.glsl"};
    ShaderClass depth{"shader/depth/depthvs.glsl",
        "shader/depth/depthfs.glsl"};
    ShaderClass solid{"shader/solid/solidvs.glsl",
        "shader/solid/solidfs.glsl"};
    ShaderClass texsolid{"shader/solid/texture/solidvs.glsl",
        "shader/solid/texture/solidfs.glsl"};
    ShaderClass gui{"shader/gui/guivs.glsl",
        "shader/gui/guifs.glsl"};

    glm::mat4 lightSpaceMatrix; 
    ParticleObject particle{"texture/laugh.png",
        glm::vec3(5.0f,0.0f,5.0f),100,10.0f};
    BillboardObject board{"texture/yukkuri.png"};
    PointObject point{}; 
    LineObject line{};
    VectorObject vector{};
    QuadObject quad{};
    glm::vec3 lightPos; 

    float view_distance = 10000;
    float zoomrate = 1.0f;

    SceneClass(unsigned int width, unsigned int height);
    void renderscene();
    void render();

private:

    void indicaters();
    void Draw3DAxis(glm::vec3 position, glm::vec3 size);
    void DrawWireGround();
};


SceneClass::SceneClass(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    camera.Position = glm::vec3(0.0f,7.0f,20.0f);
    camera.isOrtho = false;
    camera.SpeedRate =10.0f;

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int textureColorBufferMultiSampled;
    glGenTextures(1, &textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, window_w, window_h, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, 
            GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, window_w, window_h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int intermediateFBO;
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    unsigned int screenTexture;
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_w, window_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->screentexture = screenTexture;
    this->FBO = framebuffer;
    this->IFBO = intermediateFBO;
    this->depthmap = depthMap;
}

void SceneClass::indicaters()
{
    Draw3DAxis(camera.GetViewDirection(),glm::vec3(0.01f));
    Draw3DAxis(glm::vec3(0.0f),glm::vec3(1.0f));
    DrawWireGround();
}

void SceneClass::renderscene()
{
    float time = glfwGetTime();
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D,screentexture);
    block2.position = glm::vec3(0.0f);
    //block2.DrawShadow(texsolid);
    block2.Draw(model);
    
    glm::mat4 projection = camera.GetProjectionMatrix(); 
    glm::mat4 view = camera.GetViewMatrix(); 
    glm::mat4 model = glm::mat4(1.0);
    //model = glm::rotate(model,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
    model = glm::translate(model,glm::vec3(20.0f,0.0f,10.0f));
    model = glm::scale(model,glm::vec3(10.0f));
    texsolid.use();
    texsolid.setVec3("objectColor",glm::vec3(1.0f)); 
    texsolid.setMat4("projection",projection);
    texsolid.setMat4("view",view);
    texsolid.setMat4("model",model);
    quad.Draw(screentexture);
}
void SceneClass::render()
{
        glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
        glEnable(GL_DEPTH_TEST);
        glViewport(0,0,width, height);
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        indicaters();
        renderscene();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->IFBO);
        glBlitFramebuffer(0, 0, width, height, 
                0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        gui.use();
        gui.setFloat("time",glfwGetTime());
        glActiveTexture(GL_TEXTURE0);
        quad.Draw(screentexture);
}

void SceneClass::Draw3DAxis(glm::vec3 position, glm::vec3 size)
{
    vector.Draw(position,glm::vec2(0.0f,0.0f),size.x,glm::vec3(1.0f,0.0f,0.0f),solid);
    vector.Draw(position,glm::vec2(0.0f,90.0f),size.y,glm::vec3(0.0f,1.0f,0.0f),solid);
    vector.Draw(position,glm::vec2(270.0f,0.0f),size.z,glm::vec3(0.0f,0.0f,1.0f),solid);
}

void SceneClass::DrawWireGround()
{   
    int i,j;
    int length,width;
    int scale;
    scale = 5;
    length = 50*scale;
    width = 50*scale;
    
    float cx,cz; 
    cx = length/2;
    cz = width/2;

    for(i = 0; i < length; i += scale){
        line.Draw(glm::vec3(-cx,0.0f,-i+cz),
                glm::vec2(0.0f),
                glm::vec3(1.0f),
                length-scale, solid);
        for(j = 0; j < length; j += scale){
            line.Draw(glm::vec3(j-cx,0.0f,cz),
                    glm::vec2(90.0f,0.0f),
                    glm::vec3(1.0f),width-scale, solid);
        }
    }
}

void imGui_process(GLFWwindow *window)
{
    int display_w,display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
 
    ImGui::Begin("data");{
        ImGui::Text("average %.3f ms/frame (%.1f FPS)", 
                1000.0f / ImGui::GetIO().Framerate, 
                ImGui::GetIO().Framerate);
        ImGui::Text("window width %d", display_w);
        ImGui::Text("window heigth %d", display_h);
    }ImGui::End(); 
    ImGui::Begin("camera");{
        ImGui::Text("Zoom %f",camera.Zoom);
        ImGui::Text("camera x %f",camera.Position.x);
        ImGui::Text("camera y %f",camera.Position.y);
        ImGui::Text("camera z %f",camera.Position.z);
        
        ImGui::Text("cursor x %f",camera.mouse.x);
        ImGui::Text("cursor y %f",camera.mouse.y);            
    }ImGui::End();
    ImGui::ShowDemoWindow(NULL);
}

void mainloop(GLFWwindow* window)
{
    int display_w,display_h;
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    SceneClass mainscene(window_w,window_h);
    mainscene.window = window;

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        KeyProcessInput(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
            
        mainscene.width = display_w;
        mainscene.height = display_h;
        camera.width = display_w;
        camera.height = display_h;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame(); 

        inputprocess(window);
        imGui_process(window);
        
        { 
            mainscene.render(); 
        
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
    glfwWindowHint(GLFW_SAMPLES, 4);

        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // 3.0+ only

    GLFWwindow* window = glfwCreateWindow(window_w, 
            window_h, "bsc", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(0); // Enable vsync

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
