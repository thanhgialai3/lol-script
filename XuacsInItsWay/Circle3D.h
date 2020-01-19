#pragma once

#ifndef CIRCLE_3D_H
#define CIRCLE_3D_H

#include "d3dx9.h"

namespace Geometry
{
	class Circle3D
	{
	public:
		Circle3D(D3DXVECTOR3 centerPoint = D3DXVECTOR3(0, 0, 0), float radius = 0)
		{
			_centerPoint = centerPoint;
			_radius = radius;
		}

		float GetRadius() { return _radius; }

		virtual D3DXVECTOR3 GetPos() { return _centerPoint; }

	private:
		D3DXVECTOR3 _centerPoint;
		float _radius;
	};
}


#endif