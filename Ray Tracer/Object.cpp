#include "Object.h"

ColorObject::ColorObject(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 intersection_point, bool valid) :
mAmbientColor( ambient ),
mDiffuseColor( diffuse ),
mIsValid( valid ),
mIntersectionPoint( intersection_point)
{}

glm::vec3 ColorObject::GetIntersectionPoint()
{
	return mIntersectionPoint;
}

glm::vec3 ColorObject::GetAmbientColor()
{
	return mAmbientColor;
}

glm::vec3 ColorObject::GetDiffuseColor()
{
	return mDiffuseColor;
}

bool ColorObject::IsValid()
{
	return mIsValid;
}