#ifndef OBJECTS_H
#define OBJECTS_H
#include <jansson.h>
#define REALMODE 0
#define EDITMODE 1
class BlockPartsObject
{
public:
    std::string BlockName;
    std::string BlockDescription;

    glm::vec3 positions;
    glm::vec3 dimension;
    glm::vec3 axis;
    glm::vec3 lightPos;
    //color vectors
    glm::vec3 color;
    //glm::vec3 ambient;
    float ambient;
    glm::vec3 diffuse;
    float luster;
    int attached;
    int ID;
    
    ModelClass cube{"data/cube/wirecube.obj"};
    ModelClass model{"data/error/error.obj"};

    ShaderClass modelshader{"shader/model/modelvs.glsl",
        "shader/model/modelfs.glsl"};
    ShaderClass editmodeshader{"shader/solid/solidvs.glsl",
        "shader/solid/solidfs.glsl"};
    ShaderClass frameshader{"shader/frame/framevs.glsl",
        "shader/frame/framefs.glsl"};
    
    BlockPartsObject(std::string BlockName = "name", 
            std::string BlockDescription = "description",
            std::string ModelPath = "data/cube/cube.obj")
    {
        this->BlockName = BlockName;
        this->BlockDescription = BlockDescription;
        this->model = ModelClass(ModelPath);
        
        this->color =glm::vec3(0.227f,0.270f,0.296f);
        this->ambient = 1.0f;//glm::vec3(0.3f,0.3f,0.3f);
        
        this->diffuse = glm::vec3(1.0f,1.0f,1.0f);
        this-> luster = 20.0f;
        
        this->dimension = glm::vec3(1.0f,1.0f,1.0f);

    }
    void DrawMainModel(glm::mat4 projection, glm::mat4 view, glm::mat4 model, 
            int mode = REALMODE)
    {
        if(mode == 0){
            this->modelshader.use();
            //this->modelshader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
            this->modelshader.setVec3("lightPos", this->lightPos);
            this->modelshader.setVec3("viewPos", camera.Position);
            
            this->modelshader.setVec3("objectColor",color);
            this->modelshader.setVec3("lightColor",glm::vec3(1.0f));
            this->modelshader.setFloat("ambientStrength",ambient);

            this->modelshader.setMat4("projection",projection);
            this->modelshader.setMat4("view",view);
            model = glm::scale(model, glm::vec3(0.1f));
            this->modelshader.setMat4("model", model);
            this->model.Draw(this->modelshader);
            
        }else if(mode == 1){
            
            this->editmodeshader.use();
            this->editmodeshader.setVec3("ObjectColor",color);
            this->editmodeshader.setMat4("projection",projection);
            this->editmodeshader.setMat4("view",view);
            model = glm::scale(model, glm::vec3(0.1f));
            this->editmodeshader.setMat4("model", model);
            this->model.Draw(this->editmodeshader);
            this->editmodeshader.setVec3("ObjectColor",color*glm::vec3(1.6));
            glLineWidth(3);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            this->model.Draw(this->editmodeshader);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glLineWidth(1);
        }
    }

    void DrawDimensionCube(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
    { 

        this->frameshader.use();
        this->frameshader.setVec3("ObjectColor",1.0f,1.0f,0.0f);
        this->frameshader.setMat4("projection",projection);
        this->frameshader.setMat4("view",view);
        model = glm::scale(model, this->dimension*glm::vec3(1.2f));
        this->frameshader.setMat4("model", model);
        glLineWidth(2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        this->cube.Draw(this->frameshader);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glLineWidth(1);
        //this->cube.Draw(this->frameshader);
    }
};
class  LampObject
{
    public:
        ShaderClass lampshader{"shader/lamp/lampvs.glsl",
            "shader/lamp/lampfs.glsl"};
        ModelClass lamp{"data/error/error.obj"};
        glm::vec3 color;
        LampObject(std::string path)
        {
            this->lamp = ModelClass(path);
            this->lampshader.setVec3("Color",glm::vec3(1.0f,1.0f,1.0f));
        }
        void DrawLamp(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
        {
            this->lampshader.use();
            this->lampshader.setVec3("Color",this->color.x,this->color.y,this->color.z);
            this->lampshader.setMat4("projection",projection);
            this->lampshader.setMat4("view",view);
            this->lampshader.setMat4("model", model);
            this->lamp.Draw(this->lampshader);
        }
};
class VesselObject
{
    public:
        std::vector<BlockPartsObject> Vessel;
        std::string VesselName;
        glm::vec3 axis;
        VesselObject(std::string name)
        {
            VesselName = name;
            axis = glm::vec3(0.0f,0.0f,0.0f);

        }
        void Draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model,int mode = 0)
        {
            if(mode == 0){
                for(BlockPartsObject block : Vessel){
                    block.DrawMainModel(projection,view,model);
                }
            }else if(mode == 1){
            }
        }
        void AddBlock(BlockPartsObject block,glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f))
        { 
            block.axis = axis;
            this->Vessel.push_back(block);
        }
        void RemoveBlock(int ID)
        {
        }
    private:
};
#endif
