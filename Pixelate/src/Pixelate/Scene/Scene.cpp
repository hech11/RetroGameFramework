#include "PXpch.h"
#include "Scene.h"

#include "Pixelate/Rendering/Renderer2D.h"
#include "Components.h"
#include "Entity.h"
#include "Pixelate/Rendering/RenderCommand.h"
#include "glm/ext/quaternion_float.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "glm/gtx/quaternion.hpp"


#include "Pixelate/Core/Application.h"


namespace Pixelate {



	Scene::Scene(const std::string& name /*= "Scene"*/) : m_Name(name)
	{
		Init();
	}


	Scene::~Scene() {
		m_Reg.clear();
	}

	void Scene::Init()
	{
		// TODO: Set stuff in here once reading the scene from a file.

		// temp for now..?
		m_SceneEntity = m_Reg.create();
		m_Reg.emplace<PhysicsWorldComponent>(m_SceneEntity);

		auto entity = CreateEntity("Camera");
		entity.AddComponent<CameraComponent>();
	}

	


	void Scene::OnUpdate(float ts, const Ref<EditorCamera>& camera)
	{


		// Rendering the scene editor viewport
		RenderCommand::Clear();
		Renderer2D::ResetStatistics();

		Renderer2D::BeginScene(camera.get());
		Renderer2D::DrawSceneGrid(camera->GetOrthographicSize());

		auto renderGroup = m_Reg.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : renderGroup) {
			auto[transformComp, spriteComp] = renderGroup.get<TransformComponent, SpriteRendererComponent>(entity);

			// Does not support rotation yet
			if (spriteComp.SpriteRect) {
				Renderer2D::DrawSprite(transformComp.Transform, spriteComp.SpriteRect, spriteComp.TintColor);
			} else {
				Renderer2D::DrawSprite(transformComp.Transform, spriteComp.TintColor);
			}

		}


		Renderer2D::EndScene();

	}


	void Scene::OnGameViewportRender()
	{
		// find the primary camera to use for rendering.
		CameraComponent* renderCam = nullptr;
		auto allCameras = m_Reg.view<CameraComponent>();
		bool continueloop = true;
		for (auto entity : allCameras) {
			if (continueloop) {
				Entity e{ entity, this };
				auto& transformComp = e.GetComponent<TransformComponent>();
				auto& camComp = e.GetComponent<CameraComponent>();
				camComp.Camera.SetViewMatrix(transformComp.Transform);
				const auto& orthoSize = camComp.Camera.GetOrthographicSize();
				camComp.Camera.SetProjection({ -m_AspectRatio * orthoSize, m_AspectRatio * orthoSize, -orthoSize, orthoSize });

				if (camComp.PrimaryCamera) {
					renderCam = &camComp;
					continueloop = false;
				}

			}

		}

		// if there was no camera found in the scene
		if (renderCam != nullptr) {
			RenderCommand::SetClearColor(renderCam->ClearColor.r, renderCam->ClearColor.g, renderCam->ClearColor.b, renderCam->ClearColor.a);
			RenderCommand::Clear();
			Renderer2D::ResetStatistics();

			Renderer2D::BeginScene(&renderCam->Camera);

			auto renderGroup = m_Reg.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : renderGroup) {
				auto [transformComp, spriteComp] = renderGroup.get<TransformComponent, SpriteRendererComponent>(entity);

				// Does not support rotation yet
				if (spriteComp.SpriteRect) {
					Renderer2D::DrawSprite(transformComp.Transform, spriteComp.SpriteRect, spriteComp.TintColor);
				}
				else {
					Renderer2D::DrawSprite(transformComp.Transform, spriteComp.TintColor);
				}

			}
			Renderer2D::EndScene();
		} else {
			RenderCommand::Clear();
			PX_CORE_WARN("Currently no primary camera in the scene!\n");
		}
	}

	void Scene::OnRuntimeStart() {
		auto scene = m_Reg.view<PhysicsWorldComponent>();
		auto& physicsWorld = m_Reg.get<PhysicsWorldComponent>(scene.front());


		auto rbView = m_Reg.view<RigidBodyComponent>();

		for (auto entity : rbView) {
			Entity e{ entity, this };
			auto& transformComp = e.GetComponent<TransformComponent>();
			auto& ridBodComp = e.GetComponent<RigidBodyComponent>();
			
			auto [pos, rotationQ, scale] = transformComp.DecomposeTransform();
			float rotation = glm::degrees(glm::eulerAngles(rotationQ)).z;
			
			ridBodComp.Definition.Position = pos;
			ridBodComp.Definition.Angle = rotation;

			ridBodComp.RigidBody.Init(this, ridBodComp.Definition);
			


			if (e.HasComponent<BoxColliderComponent>()) {

				auto& cc = e.GetComponent<BoxColliderComponent>();

				cc.ColliderData = new b2PolygonShape;
				cc.ColliderData->SetAsBox(cc.Size.x, cc.Size.y, {cc.Center.x, cc.Center.y}, rotation);

				ridBodComp.RigidBody.AddCollider(cc.ColliderData, 1.0f, 1.0f, cc.IsTrigger);
				
			}

		}
	}

	void Scene::OnRuntimeUpdate(float ts) {



		// Physics
		auto scene = m_Reg.view<PhysicsWorldComponent>();
		auto& physicsWorld = m_Reg.get<PhysicsWorldComponent>(scene.front());

		auto& time = Application::GetApp().GetTime();

		physicsWorld.World->Step(physicsWorld.FixedTimeStep, physicsWorld.VelocityIterations, physicsWorld.PositionIterations);
// 		while (physicsWorld.CurrentSimulationTime < time.GetElapsedSeconds()) {
// 			physicsWorld.CurrentSimulationTime += physicsWorld.FixedTimeStep;
// 		}

		auto rbView = m_Reg.view<RigidBodyComponent>();

		for (auto entity : rbView) {

			Entity e{ entity, this };

			auto& transformComp = e.GetComponent<TransformComponent>();
			const auto& ridBodComp = e.GetComponent<RigidBodyComponent>();


			const auto& position = ridBodComp.RigidBody.GetPosition();
			const auto& angle = ridBodComp.RigidBody.GetAngle();
			auto [pos, rotationQ, scale] = transformComp.DecomposeTransform();

			auto rotation = glm::eulerAngles(rotationQ);

			transformComp.Transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) *
				glm::toMat4(glm::quat({ rotation.x, rotation.y, angle })) *
				glm::scale(glm::mat4(1.0f), scale);
			
		}



	}

	void Scene::OnRuntimeStop() {
		auto scene = m_Reg.view<PhysicsWorldComponent>();
		auto& physicsWorld = m_Reg.get<PhysicsWorldComponent>(scene.front());

		auto rbView = m_Reg.view<RigidBodyComponent>();

		for (auto entity : rbView) {
			Entity e{ entity, this };
			auto& transformComp = e.GetComponent<TransformComponent>();
			auto& ridBodComp = e.GetComponent<RigidBodyComponent>();

			ridBodComp.RigidBody.Destroy();

		}
		
	}


	void Scene::SetGameViewport(unsigned int width, unsigned int height) {
		m_AspectRatio = (float)width / (float)height;
	}

	Entity Scene::CreateEntity(const std::string& name) {
		auto entity = Entity(m_Reg.create(), this);
		entity.AddComponent<NameComponent>(name);
		glm::mat4 transform = glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f,1.0f });
		entity.AddComponent<TransformComponent>(transform);
		return entity;
	}

	void Scene::DeleteEntity(Entity entity) {
		m_Reg.destroy(entity.GetHandle());
	}

}