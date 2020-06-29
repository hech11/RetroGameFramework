#include "RGFpch.h"
#include "RigidBody.h"

#include "Physics.h"



namespace RGF {



	RigidBody::RigidBody(const RigidBodyDef& def)
		: m_Def(def)
	{
		b2World* world = (b2World*)Physics::World();

		b2BodyDef d;
		d.position.Set(def.StartPosition.x, def.StartPosition.y);
		d.angle = m_Def.Angle;
		d.type = (b2BodyType)def.Type;
		d.bullet = (def.DetectionMode == CollisionDetectionMode::Continuous ? true : false);
		d.allowSleep = (def.State == SleepingState::NeverSleep ? true : false);
		d.fixedRotation = !def.CanRotate;
		d.gravityScale = def.GravityScale;
		m_BodyData = world->CreateBody(&d);

	}

	RigidBody::~RigidBody()
	{
		b2World* world = (b2World*)Physics::World();
		for (auto& Fixture : m_Fixtures) {
			m_BodyData->DestroyFixture(Fixture);
		}
		m_Fixtures.clear();
		world->DestroyBody(m_BodyData);
	}

	void RigidBody::AddCollider(PhysicsShapeColliders* collider) {
		b2FixtureDef fDef;

		fDef.shape = (b2Shape*)collider->GetShapeData();
		fDef.density = collider->Density;
		fDef.friction = collider->Friction;
		fDef.isSensor = collider->IsTrigger;

		m_Fixtures.push_back(m_BodyData->CreateFixture(&fDef));
	}

	void RigidBody::RemoveCollider(PhysicsShapeColliders* collider) {
		int iterator = 0;
		for (auto& Fixture : m_Fixtures) {
			if (Fixture->GetShape() == collider->GetShapeData()) {
				m_BodyData->DestroyFixture(Fixture);
				m_Fixtures.erase(m_Fixtures.begin() + iterator);
				break;
			}

			iterator++;
		}
	}


	void RigidBody::SetLinearVelocity(const glm::vec2& velocity)
	{
		const auto& bod = (b2Body*)m_BodyData;
		bod->SetLinearVelocity({ velocity.x, velocity.y });
	}

	glm::vec3 RigidBody::GetPosition() const {
		const auto& bod = (b2Body*)m_BodyData;
		return { bod->GetPosition().x, bod->GetPosition().y, 0.0f };
	}

}