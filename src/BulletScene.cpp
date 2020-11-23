#include "BulletScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

#define PPM 30

BulletScene::BulletScene()
{
	BulletScene::start();
}

BulletScene::~BulletScene()
= default;

bool BulletScene::m_viewForce = false;
bool BulletScene::m_viewVelocity = false;

void BulletScene::draw()
{
	TextureManager::Instance()->draw("background", -300, 0, 1500, 600, 0, 255, false);
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(),0,0,0,0);
	
}

void BulletScene::update()
{

	updateDisplayList();

}

void BulletScene::clean()
{
	removeAllChildren();
}

void BulletScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
			
			}
			else
			{
			
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pShip->moveLeft();
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pShip->moveRight();
		}
		else
		{
			m_pShip->stopMovingX();
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			m_pShip->moveUp();
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			m_pShip->moveDown();
		}
		else
		{
			m_pShip->stopMovingY();
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void BulletScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "background");
	// Set GUI Title
	m_guiTitle = "Bullet Scene";
	
	// Pixels Per Meter
	m_PPM = PPM;

	m_pShip = new Ship();
	addChild(m_pShip);

	// Back Button
	m_pBackButton = new Button("../Assets/textures/restartButton.png", "Reset", RESTART_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 500.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/startButton.png", "activate", START_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 500.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		StartSim();
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);
	addChild(m_pInstructionsLabel);

	m_pTempLabel = new Label("X from Bot Ramp: ", "Consolas");
	m_pTempLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 20.0f);
	addChild(m_pTempLabel);

	m_maxVelocity = 0;
}

void BulletScene::GUI_Function() const
{
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Collision simulation", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float CoefficientFriction = 0.42f;
	static float mass = 12.8f;

	if (ImGui::Button("Reset To Default"))
	{
		// Reset to Default values
		CoefficientFriction = 0.42f;
		mass = 12.8f;

		
	}

	ImGui::Separator();


	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

bool BulletScene::StartSim()
{
	return false;
}


