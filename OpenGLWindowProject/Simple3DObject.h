#pragma once
#include <vector>
#include <string>
#include "Material.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"

class IDrawable
{
public:
	virtual void draw(const ShaderProgram& program) const = 0;
	virtual ~IDrawable() = 0;
};

inline IDrawable::~IDrawable() {}

class ITransformational
{
protected:
	float		m_scale;
	glm::vec3   m_translate;
	glm::mat4x4 m_rotate;
	glm::mat4x4 m_model;

public:
	ITransformational(float			scale     = 1.0f,
					  glm::vec3     translate = glm::vec3(0.0f),
					  glm::mat4x4	rotate    = glm::mat4(1.0f),
					  glm::mat4x4	model     = glm::mat4(1.0f))
		: m_scale(scale),
		  m_translate(translate),
		  m_rotate(rotate),
		  m_model(model)
	{}
	virtual void scale(float) = 0;
	virtual void translate(const glm::vec3 &) = 0;
	virtual void rotate(float , const glm::vec3 &) = 0;
	virtual glm::mat4 getModel() const = 0;
	virtual ~ITransformational() = 0;
};

inline ITransformational::~ITransformational() {}

class SimpleObject3D : public IDrawable, public ITransformational
{};