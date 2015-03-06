#pragma once

class Color
{
public:
	float r;
	float g;
	float b;
public:
	Color()
	{
		r = g = b = 0;
	}

	Color(float _r,float _g,float _b)
	{
		r = _r;
		g = _g;
		b = _b;
	}
	bool operator==(Color color){
		if (this->b == color.b)
			if (this->r == color.r)
				if (this->g == color.g)
					return true;
				return false;
	}
	bool operator!=(Color * color){
		return !(this == color);
	}

	~Color(){}
};