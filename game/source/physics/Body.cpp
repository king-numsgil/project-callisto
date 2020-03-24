#include <umbriel/physics/Body.hpp>

namespace umbriel::physics
{
	Body::Body() : ChipmunkHandle<cpBody>{nullptr, cpBodyFree}
	{}

	Body::Body(Space& space, f64 mass, f64 moment)
			: ChipmunkHandle<cpBody>{cpSpaceAddBody(space.handle(), cpBodyNew(mass, moment)), cpBodyFree}
	{}

	f64dcomp Body::transform() const
	{
		auto pos = cpBodyGetPosition(handle());
		return f64dcomp::translation({pos.x, pos.y}) *
		       f64dcomp::rotation(f64rad{cpBodyGetAngle(handle())});
	}

	void Body::activate()
	{
		cpBodyActivate(handle());
	}

	void Body::sleep()
	{
		cpBodySleep(handle());
	}

	bool Body::sleeping() const
	{
		return cpBodyIsSleeping(handle()) == cpTrue;
	}

	BodyType Body::type() const
	{
		return static_cast<BodyType>(cpBodyGetType(handle()));
	}

	void Body::type(BodyType value)
	{
		cpBodySetType(handle(), static_cast<cpBodyType>(value));
	}

	f64 Body::mass() const
	{
		return cpBodyGetMass(handle());
	}

	void Body::mass(f64 value)
	{
		cpBodySetMass(handle(), value);
	}

	f64 Body::moment() const
	{
		return cpBodyGetMoment(handle());
	}

	void Body::moment(f64 value)
	{
		cpBodySetMoment(handle(), value);
	}

	f64vec2 Body::position() const
	{
		cpVect ret = cpBodyGetPosition(handle());
		return {ret.x, ret.y};
	}

	void Body::position(f64vec2 value)
	{
		cpBodySetPosition(handle(), {value.x(), value.y()});
	}

	f64vec2 Body::center_of_gravity() const
	{
		cpVect ret = cpBodyGetCenterOfGravity(handle());
		return {ret.x, ret.y};
	}

	void Body::center_of_gravity(f64vec2 value)
	{
		cpBodySetCenterOfGravity(handle(), {value.x(), value.y()});
	}

	f64vec2 Body::velocity() const
	{
		cpVect ret = cpBodyGetVelocity(handle());
		return {ret.x, ret.y};
	}

	void Body::velocity(f64vec2 value)
	{
		cpBodySetVelocity(handle(), {value.x(), value.y()});
	}

	f64vec2 Body::force() const
	{
		cpVect ret = cpBodyGetForce(handle());
		return {ret.x, ret.y};
	}

	void Body::force(f64vec2 value)
	{
		cpBodySetForce(handle(), {value.x(), value.y()});
	}

	f64 Body::angular_velocity() const
	{
		return cpBodyGetAngularVelocity(handle());
	}

	void Body::angular_velocity(f64 value)
	{
		cpBodySetAngularVelocity(handle(), value);
	}

	f64 Body::torque() const
	{
		return cpBodyGetTorque(handle());
	}

	void Body::torque(f64 value)
	{
		cpBodySetTorque(handle(), value);
	}

	f64vec2 Body::local_to_world(f64vec2 point) const
	{
		cpVect ret = cpBodyLocalToWorld(handle(), {point.x(), point.y()});
		return {ret.x, ret.y};
	}

	f64vec2 Body::world_to_local(f64vec2 point) const
	{
		cpVect ret = cpBodyWorldToLocal(handle(), {point.x(), point.y()});
		return {ret.x, ret.y};
	}

	void Body::apply_force_at_world_point(f64vec2 force, f64vec2 point)
	{
		cpBodyApplyForceAtWorldPoint(handle(), {force.x(), force.y()}, {point.x(), point.y()});
	}

	void Body::apply_force_at_local_point(f64vec2 force, f64vec2 point)
	{
		cpBodyApplyForceAtLocalPoint(handle(), {force.x(), force.y()}, {point.x(), point.y()});
	}

	void Body::apply_impulse_at_world_point(f64vec2 impulse, f64vec2 point)
	{
		cpBodyApplyImpulseAtWorldPoint(handle(), {impulse.x(), impulse.y()}, {point.x(), point.y()});
	}

	void Body::apply_impulse_at_local_point(f64vec2 impulse, f64vec2 point)
	{
		cpBodyApplyImpulseAtLocalPoint(handle(), {impulse.x(), impulse.y()}, {point.x(), point.y()});
	}

	f64 Body::kinetic_energy() const
	{
		return cpBodyKineticEnergy(handle());
	}
}
