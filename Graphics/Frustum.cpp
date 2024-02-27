#include "Frustum.h"

#include "Camera.h"

// http://davidlively.com/programming/graphics/frustum-calculation-and-culling-hopefully-demystified/
Frustum Frustum::CreateFrustumFromCamera(const Camera& cam, float nearZ, float farZ)
{
	Frustum newFrustum;

	Vector3 farCenter = farZ * cam.transform.forward();
	Vector3 nearCenter = nearZ * cam.transform.forward();

	newFrustum.nearPlane = { cam.transform.forward(), cam.transform.position + nearCenter };
	newFrustum.farPlane = { cam.transform.back(), cam.transform.position + farCenter };

	XMMATRIX invProj = XMMatrixInverse(nullptr, cam.GetProjectionMatrix());

	XMVECTOR XMTL = XMVector4Transform(XMVectorSet(-1.f, 1.f, 0.f, 1.f), invProj);
	XMTL /= XMVectorGetW(XMTL);
	Vector3 TL = XMTL;

	XMVECTOR XMBL = XMVector4Transform(XMVectorSet(-1.f, -1.f, 0.f, 1.f), invProj);
	XMBL /= XMVectorGetW(XMBL);
	Vector3 BL = XMBL;

	newFrustum.left = { cam.transform.basis(Vector3::cross(TL, BL).normalized()), cam.transform.position};

	XMVECTOR XMTR = XMVector4Transform(XMVectorSet(1.f, 1.f, 0.f, 1.f), invProj);
	XMTR /= XMVectorGetW(XMTR);
	Vector3 TR = XMTR;

	XMVECTOR XMBR = XMVector4Transform(XMVectorSet(1.f, -1.f, 0.f, 1.f), invProj);
	XMBR /= XMVectorGetW(XMBR);
	Vector3 BR = XMBR;

	newFrustum.right = { cam.transform.basis(Vector3::cross(BR, TR).normalized()), cam.transform.position };

	newFrustum.top = { cam.transform.basis(Vector3::cross(TL, TR).normalized()), cam.transform.position };
	newFrustum.bottom = { cam.transform.basis(Vector3::cross(BR, BL).normalized()), cam.transform.position };


	return newFrustum;
}
