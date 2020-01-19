#include "CircleCollision.h"

namespace Geometry
{
	bool CircleCollision::IsCollision(Circle3D otherCircle)
	{
		D3DXVECTOR3 pos2d = GetPos();
		pos2d.to2D();
		otherCircle.GetPos().to2D();

		float distance = pos2d.DistTo(otherCircle.GetPos());
		return distance <= this->GetRadius() + otherCircle.GetRadius();
	}
}