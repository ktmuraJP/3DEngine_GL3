#include "objects.h"
//
//
//================================================== 
//
//
PointObject::PointObject()
{
    Setup();
}

PointObject::~PointObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void PointObject::Draw(glm::vec3 point, glm::vec3 color,int size, ShaderClass shader)
{
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 model = glm::mat4(1.0f);            
    model = glm::translate(model,point);

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model",model);
    shader.setVec3("objectColor",color);

    glPointSize(size);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0,3);
}

void PointObject::Setup()
{     
    float vertices[] = {
       0.0f,0.0f,0.0f, 
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
//
//
//================================================== 
//
//
LineObject::LineObject(glm::vec3 position)
{
    Setup();
}

LineObject::~LineObject()
{    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
} 

void LineObject::Draw(glm::vec3 origin, glm::vec2 angles, glm::vec3 color, float size, ShaderClass shader)
{
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 model = glm::mat4(1.0f);            

    model = glm::translate(model,origin);
    model = glm::scale(model,glm::vec3(size));

    model = glm::rotate(model,glm::radians(angles.x),glm::vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model,glm::radians(angles.y),glm::vec3(0.0f,0.0f,1.0f));

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model",model);
    shader.setVec3("objectColor",color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0,3);
}

void LineObject::Setup()
{     
    float vertices[] = {
       0.0f,0.0f,0.0f, 
       1.0f,0.0f,0.0f 
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

//
//
//================================================== 
//
//
VectorObject::VectorObject()
{
    SetupLine();
    SetupArrow();
}

VectorObject::~VectorObject()
{    
    glDeleteVertexArrays(1, &Line_VAO);
    glDeleteBuffers(1, &Line_VBO);
    glDeleteVertexArrays(1, &Arrow_VAO);
    glDeleteBuffers(1, &Arrow_VBO);
} 
/*
void VectorObject::Draw(glm::vec3 origin, glm::vec2 angles, float size, glm::vec3 color, ShaderClass shader)
{
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 model = glm::mat4(1.0f);            

    model = glm::translate(model,origin);
    model = glm::scale(model,glm::vec3(size));

    model = glm::rotate(model,glm::radians(angles.x),glm::vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model,glm::radians(angles.y),glm::vec3(0.0f,0.0f,1.0f));

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model",model);
    shader.setVec3("objectColor",color);

    glBindVertexArray(Line_VAO);
    glDrawArrays(GL_LINES, 0,2);

    glBindVertexArray(Arrow_VAO);
    glDrawArrays(GL_TRIANGLES, 0,54);
}
*/
void VectorObject::Draw(glm::vec3 origin, glm::quat quat, float size, glm::vec3 color, ShaderClass shader)
{
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 model = glm::mat4(1.0f);            

    model = glm::translate(model,origin);
    model *= glm::toMat4(quat);
    model = glm::scale(model,glm::vec3(size));

    shader.setMat4("model",model);

    glBindVertexArray(Line_VAO);
    glDrawArrays(GL_LINES, 0,2);

    glBindVertexArray(Arrow_VAO);
    glDrawArrays(GL_TRIANGLES, 0,54);
}

void VectorObject::SetupArrow()
{
    float vertices[] = {

        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.05f,
        1.0f,  0.05f, 0.0f,
        
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.05f,
        1.0f, -0.05f, 0.0f,
        
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f, -0.05f,
        1.0f, -0.05f, 0.0f,
        
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f, -0.05f,
        1.0f,  0.05f, 0.0f,
        
        1.125f,  0.0f,   0.0f,
        1.0f,  0.05f,  0.0f,
        1.0f,  0.0f,   0.05f,
        
        1.125f,  0.0f,   0.0f,
        1.0f,  0.0f,  0.05f,
        1.0f,  -0.05f,   0.0f,
        
        1.125f,  0.0f,   0.0f,
        1.0f,  -0.05f,   0.0f,
        1.0f,   0.0f,  -0.05f,
        
        1.125f,  0.0f,   0.0f,
        1.0f,   0.0f,  -0.05f,
        1.0f,   0.05f,  0.0f,
         
    };
    glGenVertexArrays(1, &Arrow_VAO);
    glGenBuffers(1, &Arrow_VBO);

    glBindVertexArray(Arrow_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, Arrow_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void VectorObject::SetupLine()
{     
    float vertices[] = {
       0.0f,0.0f,0.0f, 
       1.0f,0.0f,0.0f 
    };

    glGenVertexArrays(1, &Line_VAO);
    glGenBuffers(1, &Line_VBO);

    glBindVertexArray(Line_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, Line_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
//
//
//================================================== 
//
//
QuadObject::QuadObject()
{
    Setup();
}

void QuadObject::Draw(unsigned int texture)
{
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

}

void QuadObject::Setup()
{
        float Vertices[] = {
            -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
             1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
             1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

}
//
//
//================================================== 
//
//

WirePlane::WirePlane()
{
    Setup();
}

void WirePlane::Draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0,24);
    glBindVertexArray(0);
}

void WirePlane::Setup()
{
    float vertices[] = {
        /*
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
        */
         0.5f,  0.0f,  0.5f, 
         0.5f,  0.0f, -0.5f, 
                    
         0.5f,  0.0f, -0.5f, 
        -0.5f,  0.0f, -0.5f, 
                    
        -0.5f,  0.0f, -0.5f, 
        -0.5f,  0.0f,  0.5f, 
                    
         0.5f,  0.0f,  0.5f, 
        -0.5f,  0.0f,  0.5f 
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
//
//
//================================================== 
//
//
BillboardObject::BillboardObject(std::string imgpath)
{
    Setup();
    texture = SetupTexture(imgpath.c_str(),&width,&height);
    ratio_x = (float)width / (float)(width + height);
    ratio_y = (float)height / (float)(width + height);
}

void BillboardObject::Draw()
{
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model,glm::vec3(ratio_x,ratio_y,1.0f));
    model = glm::translate(model,position);

    shader.use();
    shader.setVec3("ParticleColor", glm::vec3(1.0f));
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model",model);

    glBindTexture(GL_TEXTURE_2D,texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void BillboardObject::Resize(float size)
{
    this->size = size;
}

void BillboardObject::Setup()
{     
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  1.0f, -1.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, -1.0f 
    };
    unsigned int indices[] = {
        0, 1, 3, 
        1, 2, 3  
    };
    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
            5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
//
//
//================================================== 
//
//
ParticleObject::ParticleObject(std::string imgpath, glm::vec3 position, int amount, float life)
{
    this->life = life;
    this->amount = amount;
    this->position = position;
    Setup();
    texture = SetupTexture(imgpath.c_str());
}

ParticleObject::~ParticleObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1,&texture);
} 

void ParticleObject::Draw(bool glow)
{
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 view = camera.GetViewMatrix();
    for(Particle particle : particles){
        if(particle.life > 0.0f){

            glm::mat4 model = glm::mat4(1.0f);
            shader.use();
            shader.setVec3("ParticleColor", glm::vec3(1.0f));
            shader.setFloat("life",particle.life);
            shader.setVec3("position", particle.position);
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model",model);
            
            glBindTexture(GL_TEXTURE_2D,texture); 
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }       
}

void ParticleObject::Update(bool repeat)
{
    for(unsigned int i = 0; i < particles.size(); i++){
        float dt = glfwGetTime() - particles[i].time;

        if(particles[i].life > 0.0){
            particles[i].life -= 1.0;
            
            particles[i].position += glm::vec3(dt) * 
                particles[i].vector *  particles[i].velocity; 
            
            particles[i].cameradistance = glm::length2(
                    particles[i].position - camera.Position);
        }
        if(particles[i].life <= 0.0 && repeat == true){
            particles[i].life = life + (float)(rand()%(int)life);
            particles[i].time = glfwGetTime();
            particles[i].position = position;
            particles[i].cameradistance = -1.0f;
        }
    }
    std::sort(particles.begin(),particles.end());
} 

int ParticleObject::Size()
{
    return (int)particles.size();
}

void ParticleObject::Add(int add)
{
    int i;
    for(i = 0; i < add; ++i){
        Particle particle;    
        particle.life = life*0;
        particle.time = glfwGetTime();
        particle.vector = glm::vec3(randf(-1,1),1.0f,randf(-1,1));
        particle.velocity = glm::vec3(1.0f);
        particle.position = position;
        particles.push_back(particle);
    }
}

void ParticleObject::Remove(int remove)
{
    int i;
    if(particles.size() > 0){
        for(i = 0; i < remove; ++i)
            particles.pop_back();
    }
}

void ParticleObject::SetVelocity(glm::vec3 velocity)
{
    for(unsigned int i = 0; i < particles.size(); ++i){
        particles[i].velocity = velocity;
    }
}

void ParticleObject::Setup()
{    
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  1.0f, -1.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, -1.0f 
    };
    unsigned int indices[] = {
        0, 1, 3, 
        1, 2, 3  
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
            5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    for(int i = 0; i < amount; ++i){
        
        particles.push_back(Particle());
        particles[i].life = life*0;
        particles[i].time = glfwGetTime();
        particles[i].vector = glm::vec3(randf(-1,1),1.0f,randf(-1,1));
        particles[i].velocity = glm::vec3(1.0f);
        particles[i].position = position;
    }
};
