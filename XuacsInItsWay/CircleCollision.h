#pragma once

#ifndef CIRCLE_COLLISION_H
#define CIRCLE_COLLISION_H

#include "Circle3D.h"

#include "Globals.h"

namespace Geometry
{
	class LineCollision;

	class CircleCollision
		: public Circle3D
	{
	public:
		CircleCollision(CObject object, float radius = 1) : Circle3D(object.position, radius) {
			_object = object;
		}

		CObject GetAIObject() { return _object; }
		void SetAIObject(CObject object) { _object = object; }

		virtual D3DXVECTOR3 GetPos() { return GetAIObject().position/* + GetAIObject()->GetAIManager_Client()->pNavigation->vVelocity * 5*/; }

		bool IsCollision(Circle3D);

	private:
		CObject _object;
	};
}


#endif