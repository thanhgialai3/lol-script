#pragma once

#ifndef LINE_COLLISION_H
#define LINE_COLLISION_H

#include "Line3D.h"

#include "Circle3D.h"
#include "CircleCollision.h"


namespace Geometry
{
	class LineCollision
		: public Line3D
	{
	public:
		LineCollision(D3DXVECTOR3 src = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 dest = D3DXVECTOR3(0, 0, 0), float width = 1)
			: Line3D(src, dest) {
			_width = width;
		}

		float GetWidth() { return _width; }
		void SetWidth(float width) { _width = width; }

		bool IsCollision(CircleCollision);
		bool IsCollision(Circle3D);

	private:
		float _width;
	};
}

#endif