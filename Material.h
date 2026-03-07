#pragma once
#include "config.h"

class Material
{
public:
	Material(const char* texturePath);
	~Material();
	void use(int unit);
private:
	unsigned int textureID;
};
