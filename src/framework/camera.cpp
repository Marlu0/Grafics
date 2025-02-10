#include "camera.h"

#include "main/includes.h"
#include <iostream>

Camera::Camera()
{
	view_matrix.SetIdentity();
    //LookAt(Vector3(0, 0, 5), Vector3(0, 0, -1), Vector3(0, 1, 0));
    //SetOrthographic(-1,1,1,-1,-1, 1);
    
   
    
}

Vector3 Camera::GetLocalVector(const Vector3& v)
{
	Matrix44 iV = view_matrix;
	if (iV.Inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.RotateVector(v);
	return result;
}

Vector3 Camera::ProjectVector(Vector3 pos, bool& negZ)
{
	Vector4 pos4 = Vector4(pos.x, pos.y, pos.z, 1.0);
	Vector4 result = viewprojection_matrix * pos4;
	negZ = result.z < 0;
	if (type == ORTHOGRAPHIC)
		return result.GetVector3();
	else
		return result.GetVector3() / result.w;
}

void Camera::Rotate(float angle, const Vector3& axis)
{
	Matrix44 R;
	R.SetRotation(angle, axis);
	Vector3 new_front = R * (center - eye);
	center = eye + new_front;
	UpdateViewMatrix();
}

void Camera::Move(Vector3 delta)
{
	Vector3 localDelta = GetLocalVector(delta);
	eye = eye - localDelta;
	center = center - localDelta;
	UpdateViewMatrix();
}

void Camera::SetOrthographic(float left, float right, float top, float bottom, float near_plane, float far_plane)
{
	type = ORTHOGRAPHIC;

	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::SetPerspective(float fov, float aspect, float near_plane, float far_plane)
{
	type = PERSPECTIVE;

	this->fov = fov;
	this->aspect = aspect;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	this->eye = eye;
	this->center = center;
	this->up = up;

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
       Vector3 zc = (eye - center).Normalize();
       Vector3 xc = (up.Cross(zc)).Normalize();
       Vector3 yc = (zc.Cross(xc)).Normalize();

       view_matrix.Set(
           xc.x, xc.y, xc.z, -xc.Dot(eye),
           yc.x, yc.y, yc.z, -yc.Dot(eye),
           zc.x, zc.y, zc.z, -zc.Dot(eye),
           0,    0,    0,     1
       );

       UpdateViewProjectionMatrix();
}

// Create a projection matrix
void Camera::UpdateProjectionMatrix()
{
	// Reset Matrix (Identity)
	projection_matrix.SetIdentity();

	// Comment this line to create your own projection matrix!
	//SetExampleProjectionMatrix();

	// Remember how to fill a Matrix4x4 (check framework slides)
	
	if (type == PERSPECTIVE) {
        
        float f = 1/(tan(fov/2));
        
        projection_matrix.Set
           (f/aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, (near_plane+far_plane)/(near_plane-far_plane), (2*far_plane*near_plane)/(near_plane-far_plane),
            0, 0, -1, 0);
	}
	else if (type == ORTHOGRAPHIC) {
        projection_matrix.Set
           (2/(right-left), 0, 0, -(right+left)/(right-left),
            0, (2)/(top-bottom), 0, -(bottom+top)/(bottom-top),
            0, 0, 2/(near_plane-far_plane), -(near_plane+far_plane)/(near_plane-far_plane),
            0, 0, 0, 1);
	}

	UpdateViewProjectionMatrix();
}

void Camera::UpdateViewProjectionMatrix()
{
	viewprojection_matrix = projection_matrix * view_matrix;
}

Matrix44 Camera::GetViewProjectionMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	return viewprojection_matrix;
}

// The following methods have been created for testing.
// Do not modify them.

void Camera::SetExampleViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, view_matrix.m );
}

void Camera::SetExampleProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (type == PERSPECTIVE)
		gluPerspective(fov, aspect, near_plane, far_plane);
	else
		glOrtho(left,right,bottom,top,near_plane,far_plane);

	glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix.m );
	glMatrixMode(GL_MODELVIEW);
}

void Camera::OrbitAroundAxis(const Vector3& axis, float radius, float speed, float seconds_elapsed)
{
    if (axis.Length() == 0) {
        std::cout << "Error: Rotation axis cannot be zero." << std::endl;
        return;
    }

    Vector3 normalizedAxis = axis;
    normalizedAxis.Normalize();

    // Keep the angle within [0, 2π] to avoid large values
    float angle = fmod(speed * seconds_elapsed, 2.0f * 3.14159265f);

    Matrix44 rotationMatrix;
    rotationMatrix.SetRotation(angle, normalizedAxis);

    Vector3 initialOffset(radius, 0, 0);  // Starting point of the orbit
    Vector3 offset = rotationMatrix * initialOffset;

    eye = center + offset;
    UpdateViewMatrix();
}
