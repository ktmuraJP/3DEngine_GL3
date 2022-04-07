#include "entity.h"


//
//
//
//
//

BlockObject::BlockObject(int id, ModelClass model)
{
    this->id = id;
    this->model = model; 
}

void BlockObject::SetMaterial()
{
}

void BlockObject::Draw(ShaderClass shader)
{
}

//
//
//-------------------- 
//
//

VesselObject::VesselObject()
{
}

void VesselObject::AddBlock(BlockObject block, glm::vec3 pos)
{
}

void VesselObject::Draw()
{
}

void VesselObject::Rotate()
{
}

void VesselObject::Move()
{
}
