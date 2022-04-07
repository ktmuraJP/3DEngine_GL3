#include "src.h"
#include "scene.h"
#include "system.h"
#include "objects.h"


int window_w = 1024;
int window_h = 768;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 cursor;
Camera camera(glm::vec3(0.0f,10.0f,30.0f),10.0f);


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking


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
    //io.Fonts->AddFontFromFileTTF("lib/fonts/x14y24pxHeadUpDaisy.ttf",17.0f,NULL,JAglyphRanges);
    io.Fonts->AddFontFromFileTTF("lib/fonts/round-mplus/rounded-mplus-1m-regular.ttf",17.0f,NULL,io.Fonts->GetGlyphRangesJapanese());

    //ImFont* font = 
    //io.Fonts->AddFontFromFileTTF
    //("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    SetupDatabase();
    
    IOSystemClass sys(window,1024,768);
    // Main loop
    sys.Loop();


    
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


    if(WindowContext() == 1){
        printf("Window Creation Error\n");
        return 1;
    }
    return 0;
}
