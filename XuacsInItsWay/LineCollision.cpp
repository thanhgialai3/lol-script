#include "LineCollision.h"

namespace Geometry
{
	bool LineCollision::IsCollision(CircleCollision circle)
	{
		return IsCollision(Circle3D(circle.GetPos(), circle.GetRadius()));
	}

	bool LineCollision::IsCollision(Circle3D circle)
	{
		D3DXVECTOR3 circleCenter = circle.GetPos();
		D3DXVECTOR3 vecEndStart = GetDestPosition() - GetSourcePosition();
		D3DXVECTOR3 vecCircleStart = circleCenter - GetSourcePosition();
		vecEndStart.y = 0;
		vecCircleStart.y = 0;

		D3DXVECTOR3 vecProjection = vecCircleStart.ProjectionOn(vecEndStart);
		// Transform projection to same direction as EndStart
		vecProjection = vecProjection.sameDirectionAs(vecEndStart);

		if (vecProjection.sqrMagnitude() > vecEndStart.sqrMagnitude())
		{
			vecProjection = vecEndStart; // == vecProjection = vecProjection.Normalized() * vecEndStart.Length();
			float angle = D3DXVECTOR3::angleBetween(circleCenter, GetDestPosition());
			return ((GetDestPosition()-circleCenter).sqrMagnitude() < circle.GetRadius());
		}
		else
		{
			D3DXVECTOR3 vecCircleToProjection = (vecCircleStart - vecProjection);
			float distFromLineToCircle = vecCircleToProjection.sqrMagnitude();

			return GetWidth() + circle.GetRadius() >= distFromLineToCircle;
		}
	}
}