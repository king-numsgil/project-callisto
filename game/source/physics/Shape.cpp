#include <umbriel/physics/Shape.hpp>

namespace umbriel::physics
{
	Shape::Shape(cpShape* handle) : ChipmunkHandle<cpShape>{handle, cpShapeFree}
	{}

	BoundingBox Shape::update(f64dcomp transform)
	{
		cpBB ret = cpShapeUpdate(handle(), cpTransformMult(
				cpTransformTranslate({transform.translation().x(), transform.translation().y()}),
				cpTransformRotate((f64) transform.rotation().angle())
		));
		return {ret.l, ret.b, ret.r, ret.t};
	}

	f64 Shape::mass() const
	{
		return cpShapeGetMass(handle());
	}

	void Shape::mass(f64 value)
	{
		cpShapeSetMass(handle(), value);
	}

	f64 Shape::density() const
	{
		return cpShapeGetDensity(handle());
	}

	void Shape::density(f64 value)
	{
		cpShapeSetDensity(handle(), value);
	}

	f64 Shape::moment() const
	{
		return cpShapeGetMoment(handle());
	}

	f64 Shape::area() const
	{
		return cpShapeGetArea(handle());
	}

	f64vec2 Shape::center_of_gravity() const
	{
		cpVect ret = cpShapeGetCenterOfGravity(handle());
		return {ret.x, ret.y};
	}

	BoundingBox Shape::bounding_box() const
	{
		cpBB ret = cpShapeGetBB(handle());
		return {ret.l, ret.b, ret.r, ret.t};
	}

	bool Shape::sensor() const
	{
		return cpShapeGetSensor(handle()) == cpTrue;
	}

	void Shape::sensor(bool value)
	{
		cpShapeSetSensor(handle(), value ? cpTrue : cpFalse);
	}

	f64 Shape::elasticity() const
	{
		return cpShapeGetElasticity(handle());
	}

	void Shape::elasticity(f64 value)
	{
		cpShapeSetElasticity(handle(), value);
	}

	f64 Shape::friction() const
	{
		return cpShapeGetFriction(handle());
	}

	void Shape::friction(f64 value)
	{
		cpShapeSetFriction(handle(), value);
	}

	f64vec2 Shape::surface_velocity() const
	{
		cpVect ret = cpShapeGetSurfaceVelocity(handle());
		return {ret.x, ret.y};
	}

	void Shape::surface_velocity(f64vec2 value)
	{
		cpShapeSetSurfaceVelocity(handle(), {value.x(), value.y()});
	}

	u64 Shape::collision_type() const
	{
		return cpShapeGetCollisionType(handle());
	}

	void Shape::collision_type(u64 value)
	{
		cpShapeSetCollisionType(handle(), value);
	}
}
