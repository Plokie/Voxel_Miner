#include "Frustum.h"

#include "Camera.h"

Frustum Frustum::CreateFrustumFromCamera(const Camera& cam, float aspect, float FOV, float nearZ, float farZ)
{
	Frustum newFrustum;

	const float halfVside = farZ * tanf(FOV * 0.5f);
	const float halfHside = halfVside * aspect;
	const Vector3 frontMultFar = farZ * cam.transform.forward();

	newFrustum.nearPlane = { cam.transform.forward(), cam.transform.position + nearZ * cam.transform.forward() };
	newFrustum.farPlane = { -cam.transform.forward(), cam.transform.position + frontMultFar };

	newFrustum.left = { cam.transform.right(), cam.transform.position };

	newFrustum.right = { Vector3::cross(frontMultFar - cam.transform.right() * halfHside, cam.transform.up()), cam.transform.position};
	//newFrustum.left = { Vector3::cross(cam.transform.up(), frontMultFar + cam.transform.right() * halfHside), cam.transform.position };
	newFrustum.top = { Vector3::cross(cam.transform.right(), frontMultFar - cam.transform.up() * halfVside), cam.transform.position };
	newFrustum.bottom = { Vector3::cross(frontMultFar + cam.transform.up() * halfVside, cam.transform.right()), cam.transform.position };

	return newFrustum;
}
