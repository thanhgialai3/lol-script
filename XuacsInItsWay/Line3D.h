#pragma once

#ifndef LINE3D_H
#define LINE3D_H

#include "d3dx9.h"

class Line3D
{
public:
	Line3D(D3DXVECTOR3 start = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 end = D3DXVECTOR3(0, 0, 0))
	{
		_src = start;
		_dest = end;
	}

	D3DXVECTOR3 GetSourcePosition() { return _src; }
	D3DXVECTOR3 GetDestPosition() { return _dest; }

private:
	D3DXVECTOR3 _src, _dest;
};

#endif