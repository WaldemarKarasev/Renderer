#include <Nutckracker.h>



class ExampleLayer : public NK::Layer
{
private:
	struct MovementStates
	{
		bool MoveUp = false;
		bool MoveDown = false;
		bool MoveLeft = false;
		bool MoveRight = false;
		bool MoveForward = false;
		bool MoveBackward = false;

		bool RotateUp = false;
		bool RotateDown = false;
		bool RotateRight = false;
		bool RotateLeft = false;
	};

private:
	MovementStates m_MovementStates_{};

public:
	ExampleLayer()
		: Layer("Editor Layer")
	{

	}

	void OnUpdate() override
	{
		glm::vec3 rotate{0};
		glm::vec3 direction{0};

		if (m_MovementStates_.MoveForward) 	{ direction.x += 1; }
		if (m_MovementStates_.MoveBackward) { direction.x -= 1; }
		if (m_MovementStates_.MoveRight) 	{ direction.y += 1; }
		if (m_MovementStates_.MoveLeft) 	{ direction.y -= 1; }
		if (m_MovementStates_.MoveUp) 		{ direction.z += 1; }
		if (m_MovementStates_.MoveDown) 	{ direction.z -= 1; }
		
		if (m_MovementStates_.RotateUp)		{ rotate.x += 1; }
		if (m_MovementStates_.RotateDown)	{ rotate.x -= 1; }
		if (m_MovementStates_.RotateRight)	{ rotate.y += 1; }
		if (m_MovementStates_.RotateLeft)	{ rotate.y -= 1; }

		NK::Application& app = NK::Application::Get();

		NK::Camera* camera = app.GetCamera();

		camera->MoveCameraInPlaneXZ(rotate, direction);
		
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(NK::Event& event) override
	{
		NK::Application& app = NK::Application::Get();
		glm::vec3 MovementDelta = {0.0, 0.0, 0.0};
		glm::vec3 RotationDelta = {0.0, 0.0, 0.0};
		if (event.GetEventType() == NK::EventType::KeyPressed)
		{
			NK::KeyPressedEvent& e = (NK::KeyPressedEvent&)event;
			
			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_W))
			{
				m_MovementStates_.MoveForward = true;
				MovementDelta.x += 0.1f;
			}
			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_S))
			{
				m_MovementStates_.MoveBackward = true;
				MovementDelta.x -= 0.1f;
			}
			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_A))
			{
				m_MovementStates_.MoveLeft = true;
				MovementDelta.y -= 0.1f;
			}
			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_D))
			{
				m_MovementStates_.MoveRight = true;
				MovementDelta.y += 0.1f;
			}
			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_E))
			{
				m_MovementStates_.MoveUp = true;
				MovementDelta.z += 0.1f;
			}
			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_Q))
			{
				m_MovementStates_.MoveDown = true;
				MovementDelta.z -= 0.1f;
			}

			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_UP))
			{
				m_MovementStates_.RotateUp = true;
				RotationDelta.y -= 1.f;
			}
			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_DOWN))
			{
				m_MovementStates_.RotateDown = true;
				RotationDelta.y += 1.f;
			}
			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_RIGHT))
			{
				m_MovementStates_.RotateRight = true;
				RotationDelta.z -= 1.f;
			}
			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_LEFT))
			{
				m_MovementStates_.RotateLeft = true;
				RotationDelta.z += 1.f;
			}
		}

		if (event.GetEventType() == NK::EventType::KeyReleased)
		{
			NK::KeyPressedEvent& e = (NK::KeyPressedEvent&)event;
			
			if (NK::Input::IsKeyPressed(NK::KeyCodes::NK_KEY_W))
			{
				m_MovementStates_.MoveForward = false;
				MovementDelta.x += 0.1f;
			}

			if (NK::Input::IsKeyReleased(NK::KeyCodes::NK_KEY_W))
			{
				m_MovementStates_.MoveForward = false;
				MovementDelta.x += 0.1f;
			}
			if (NK::Input::IsKeyReleased(NK::KeyCodes::NK_KEY_S))
			{
				m_MovementStates_.MoveBackward = false;
				MovementDelta.x -= 0.1f;
			}
			if (NK::Input::IsKeyReleased(NK::KeyCodes::NK_KEY_A))
			{
				m_MovementStates_.MoveLeft = false;
				MovementDelta.y -= 0.1f;
			}
			if (NK::Input::IsKeyReleased(NK::KeyCodes::NK_KEY_D))
			{
				m_MovementStates_.MoveRight = false;
				MovementDelta.y += 0.1f;
			}
			if (NK::Input::IsKeyReleased(NK::KeyCodes::NK_KEY_E))
			{
				m_MovementStates_.MoveUp = false;
				MovementDelta.z += 0.1f;
			}
			if (NK::Input::IsKeyReleased(NK::KeyCodes::NK_KEY_Q))
			{
				m_MovementStates_.MoveDown = false;
				MovementDelta.z -= 0.1f;
			}

			if (NK::Input::IsKeyReleased(NK::KeyCodes::NK_KEY_UP))
			{
				m_MovementStates_.RotateUp = false;
				RotationDelta.y -= 1.f;
			}
			if (NK::Input::IsKeyReleased(NK::KeyCodes::NK_KEY_DOWN))
			{
				m_MovementStates_.RotateDown = false;
				RotationDelta.y += 1.f;
			}
			if (NK::Input::IsKeyReleased(NK::KeyCodes::NK_KEY_RIGHT))
			{
				m_MovementStates_.RotateRight = false;
				RotationDelta.z -= 1.f;
			}
			if (NK::Input::IsKeyReleased(NK::KeyCodes::NK_KEY_LEFT))
			{
				m_MovementStates_.RotateLeft = false;
				RotationDelta.z += 1.f;
			}
		}

		//NK::Camera* camera = app.GetCamera();
		//camera->MoveCameraInPlaneXZ(RotationDelta, MovementDelta);
	}
	
};


class Sandbox : public NK::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

NK::Application* NK::CreateApplication()
{
	return new Sandbox();
}
