#include "Camera.h"

Camera::Camera()
{
	projectionMatrix = XMMatrixIdentity();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	this->fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->aspectRatio = aspectRatio;
	this->nearZ = nearZ;
	this->farZ = farZ;

	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);

	this->viewFrustum = Frustum::CreateFrustumFromCamera(*this, nearZ, farZ);
}

void Camera::SetProjectionOthographic(float size, float nearZ, float farZ)
{
	this->orthoSize = size;
	this->nearZ = nearZ;
	this->farZ = farZ;

	this->projectionMatrix = XMMatrixOrthographicLH(size, size, nearZ, farZ);
	this->viewFrustum = Frustum::CreateFrustumFromCamera(*this, nearZ, farZ);
}

//https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
bool Camera::IsAABBInFrustum(const AABB& aabb)
{
	//return aabb.IntersectsPlane(viewFrustum.nearPlane);
	return aabb.IsOnOrForwardPlane(viewFrustum.nearPlane) && aabb.IsOnOrForwardPlane(viewFrustum.left) && aabb.IsOnOrForwardPlane(viewFrustum.right);
	//return aabb.IsOnOrForwardPlane(viewFrustum.nearPlane) && aabb.IsOnOrForwardPlane(viewFrustum.left);
	//return aabb.IsOnOrForwardPlane(viewFrustum.left);
	//return aabb.IsOnOrForwardPlane(viewFrustum.left);
	//return aabb.IntersectsPlane(viewFrustum.right) || aabb.IntersectsPlane(viewFrustum.left);
	//return aabb.IsOnOrForwardPlane(viewFrustum.left) &&
	//	aabb.IsOnOrForwardPlane(viewFrustum.right) &&
	//	aabb.IsOnOrForwardPlane(viewFrustum.top) &&
	//	aabb.IsOnOrForwardPlane(viewFrustum.bottom) &&
	//	aabb.IsOnOrForwardPlane(viewFrustum.nearPlane) &&
	//	aabb.IsOnOrForwardPlane(viewFrustum.farPlane);
}

void Camera::UpdateViewFrustum()
{
	this->viewFrustum = Frustum::CreateFrustumFromCamera(*this, nearZ, farZ);
}

const float Camera::GetOrthographicSize() const
{
	return this->orthoSize;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

void Camera::SetProjectionMatrix(const XMMATRIX& mx)
{
	projectionMatrix = mx;
}
