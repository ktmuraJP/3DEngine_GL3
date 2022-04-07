#include "src.h"
#include "camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_JA_range.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

int winw = 800;
int winh = 600;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
// world space positions of our cubes
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
Camera camera(glm::vec3(0.0f,0.0f,0.3f));

unsigned int setupVAOs()
{
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, 
            GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, 
            GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
 
    return VAO;
}

unsigned int SetupTexture(const char* imgpath)
{
    unsigned int texture;
    unsigned char *data;
    int width, height, nrChannels;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(imgpath,
            &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, 
                GL_RGB, width, height, 0, 
                GL_RGB, GL_UNSIGNED_BYTE, data);  
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        printf("texture load error\n");

    stbi_image_free(data);
    return texture;
}

unsigned int SetupTextureAlpha(const char* imgpath)
{
    unsigned int texture;
    unsigned char *data;
    int width, height, nrChannels;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(imgpath,
            &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, 
                GL_RGBA, width, height, 0, 
                GL_RGBA, GL_UNSIGNED_BYTE, data);  
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        printf("texture load error\n");

    stbi_image_free(data);
    return texture;
}

void mouse_dummy(GLFWwindow* window, double xpos, double ypos){}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void setMat4(const int id, const char* name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, &mat[0][0]);
}

float lastX = winw / 2.0f;
float lastY = winh / 2.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float bias = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime,bias);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime,bias);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime,bias);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime,bias);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, mouse_dummy);
    }
}
void mainloop(GLFWwindow* window)
{
    int program;
    int display_w,display_h;
    
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 1.00f, 1.00f);
    

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable(GL_DEPTH_TEST);

    //program = loadshader(vertexShaderSource,fragmentShaderSource);
    program = loadshader(
            filestring((char*)"shader/vertex.glsl"),
            filestring((char*)"shader/fragment.glsl"));
    
    unsigned int VAO;
    unsigned int texture1,texture2;

    VAO = setupVAOs();
    //texture1 = SetupTexture((const char*)"texture/yaju.jpeg");
    texture1 = SetupTexture((const char*)"texture/container.jpg");
    //SetupTextureAlpha((const char*)"texture/awesomeface.png");
    //texture2 = SetupTextureAlpha((const char*)"texture/bemybaby.png");
    //texture1 = SetupTextureAlpha((const char*)"texture/yajusempai.png");
    //texture2 = SetupTextureAlpha((const char*)"texture/ikisugi.gif");

    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(program, "texture2"), 1);

    float rotate,rotateX,rotateY,rotateZ;
    float posX,posY,posZ;

    rotate = 0;
    rotateX = 0; 
    rotateY = 1; 
    rotateZ = 0; 
    posX = 0;
    posY = 0;
    posZ = 0;

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);

            glm::mat4 projection = glm::perspective(
                    glm::radians(camera.Zoom), 
                (float)display_h / (float)display_w, 0.1f, 100.0f);
            setMat4(program,"projection",projection);

            /*
            view = glm::lookAt(
                    glm::vec3(camX, camY, camZ), 
                    glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            */
            glm::mat4 view = camera.GetViewMatrix();
            setMat4(program,"view",view);

            // render boxes
            glBindVertexArray(VAO);
            for (unsigned int i = 0; i < 5; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);

                model = glm::rotate(model, rotate, glm::vec3(rotateX, rotateY, rotateZ));
                setMat4(program,"model",model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            ImGui::Begin("counter");{
                ImGui::PushButtonRepeat(true);
 
                ImGui::Text("rotate = %f", rotate);
                if(ImGui::Button("rotate+=0.01"))rotate+=0.01;
                if(ImGui::Button("rotate-=0.01"))rotate-=0.01;
                ImGui::Text("rotateX = %f", rotateX);
                if(ImGui::Button("rotateX+=0.1"))rotateX+=0.1;
                if(ImGui::Button("rotateX-=0.1"))rotateX-=0.1;
                ImGui::Text("rotateY = %f", rotateY);
                if(ImGui::Button("rotateY+=0.1"))rotateY+=0.1;
                if(ImGui::Button("rotateY-=0.1"))rotateY-=0.1;
                ImGui::Text("rotateZ = %f", rotateZ);
                if(ImGui::Button("rotateZ+=0.1"))rotateZ+=0.1;
                if(ImGui::Button("rotateZ-=0.1"))rotateZ-=0.1;

                ImGui::Text("posX = %f", posX);
                if(ImGui::Button("posX+=1"))posX+=1;
                if(ImGui::Button("posX-=1"))posX-=1;
                ImGui::Text("posY = %f", posY);
                if(ImGui::Button("posY+=1"))posY+=1;
                if(ImGui::Button("posY-=1"))posY-=1; 
                ImGui::Text("posZ = %f", posZ);
                if(ImGui::Button("posZ+=1"))posZ+=1;
                if(ImGui::Button("posZ-=1"))posZ-=1;
                ImGui::PopButtonRepeat();

            }ImGui::End();
            ImGui::ShowDemoWindow(NULL);
        }
        ImGui::Render();
        //glfwGetFramebufferSize(window, &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);
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
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    int window_rx = winw;
    int window_ry = winh;
    int winsize = 1;

    GLFWwindow* window = glfwCreateWindow(window_rx * winsize, 
            window_ry * winsize, "bsc", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1); // Enable vsync

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
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
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
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
