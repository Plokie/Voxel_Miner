#include "Frustum.h"

#include "Camera.h"

// http://davidlively.com/programming/graphics/frustum-calculation-and-culling-hopefully-demystified/
Frustum Frustum::CreateFrustumFromCamera(const Camera& cam, float aspect, float FOV, float nearZ, float farZ)
{
	Frustum newFrustum;

	/*const float halfVside = farZ * tanf(FOV * 0.5f);
	const float halfHside = halfVside * aspect;
	const Vector3 frontMultFar = farZ * cam.transform.forward();*/
	const Vector3 frontMultFar = farZ * cam.transform.forward();

	const float hh = tanf(FOV * 0.5f) * nearZ;
	const float hw = hh * aspect;

	const Vector3 nw = Vector3(-hw, hh, 1.f);
	const Vector3 ne = Vector3(hw, hh, 1.f);
	const Vector3 se = Vector3(hw, -hh, 1.f);
	const Vector3 sw = Vector3(-hw, -hh, 1.0f);

	newFrustum.nearPlane = { cam.transform.forward(), cam.transform.position + nearZ * cam.transform.forward() };
	newFrustum.farPlane = { -cam.transform.forward(), cam.transform.position + frontMultFar };

	newFrustum.left = { Vector3::cross(sw, nw).normalized(), cam.transform.position };

	//newFrustum.left = { cam.transform.right(), cam.transform.position };

	//newFrustum.left = {
	//	//Vector3::cross(frontMultFar - cam.transform.right() * halfHside, cam.transform.up()).normalized(),
	//	Vector3::cross()
	//	cam.transform.position
	//};

	//newFrustum.left = { Vector3::cross(frontMultFar - cam.transform.right() * halfHside, cam.transform.up()), cam.transform.position};
	//newFrustum.left = { Vector3::cross(cam.transform.up(), frontMultFar + cam.transform.right() * halfHside), cam.transform.position };
	//newFrustum.top = { Vector3::cross(cam.transform.right(), frontMultFar - cam.transform.up() * halfVside), cam.transform.position };
	//newFrustum.bottom = { Vector3::cross(frontMultFar + cam.transform.up() * halfVside, cam.transform.right()), cam.transform.position };

	//newFrustum.left 


	return newFrustum;
}
