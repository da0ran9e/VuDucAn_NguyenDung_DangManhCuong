#include "GSPlay.h"
#include "GameObject/TextureManager.h"
#include "GameObject/Sprite2D.h"
#include "GameObject/MouseButton.h"
#include "GameObject/SpriteAnimation.h"
#include "GameObject/Camera.h"
#include "KeyState.h"

GSPlay::GSPlay()
{
}


GSPlay::~GSPlay()
{
}


void GSPlay::Init()
{
	//auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play1.tga");

	// background
	
	m_background = std::make_shared<Sprite2D>( texture, SDL_FLIP_NONE);
	m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_background->Set2DPosition(0, 0);

	// button close
	texture = ResourceManagers::GetInstance()->GetTexture("btn_close.tga");
	button = std::make_shared<MouseButton>( texture, SDL_FLIP_NONE);
	button->SetSize(50, 50);
	button->Set2DPosition(SCREEN_WIDTH - 50, 10);
	button->SetOnClick([this]() {
		GameStateMachine::GetInstance()->PopState();
		});
	m_listButton.push_back(button);

   // Animation 
	texture = ResourceManagers::GetInstance()->GetTexture("Actor1_2.tga");
	obj = std::make_shared<SpriteAnimation>( texture, 2, 9, 6, 0.2f);
	obj->SetFlip(SDL_FLIP_HORIZONTAL);
	obj->SetSize(40, 50);
	obj->Set2DPosition(240, 400);
	//Camera::GetInstance()->SetTarget(obj);
	m_listAnimation.push_back(obj);

	m_KeyPress = 0;
	
}

void GSPlay::Exit()
{
}


void GSPlay::Pause()
{

}
void GSPlay::Resume()
{
	// button close
	//auto texture = ResourceManagers::GetInstance()->GetTexture("btn_restart.tga");
	//button->SetTexture(texture);

}


void GSPlay::HandleEvents()
{
}

bool keyW = false;
bool keyA = false;
bool keyS = false;
bool keyD = false;
bool keyShift = false;

void GSPlay::HandleKeyEvents(SDL_Event& e)
{
	////Key Down
	
	switch (e.type) {
	case SDL_KEYDOWN:
		
		switch (e.key.keysym.sym) {
		case SDLK_w:
			keyW = true;
			break;
		case SDLK_a:
			keyA = true;
			break;
		case SDLK_s:
			keyS = true;
			break;
		case SDLK_d:
			keyD = true;
			break;
		case SDLK_LSHIFT:
			keyShift = true;
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym) {
		case SDLK_w:
			keyW = false;
			break;
		case SDLK_a:
			keyA = false;
			break;
		case SDLK_s:
			keyS = false;
			break;
		case SDLK_d:
			keyD = false;
			break;
		case SDLK_LSHIFT:
			keyShift = false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	if (keyW) {
		if (keyShift) {
			printf("RUN UP\n");
		}
		else {
			printf("MOVE UP\n");
		}
	}
	if (keyA) {
		if (keyShift) {
			printf("RUN LEFT\n");
		}
		else {
			printf("MOVE LEFT\n");
		}
	}
	if (keyS) {
		if (keyShift) {
			printf("RUN DOWN\n");
		}
		else {
			printf("MOVE DOWN\n");
		}
	}
	if (keyD) {
		if (keyShift) {
			printf("RUN RIGHT\n");
		}
		else {
			printf("MOVE RIGHT\n");
		}
	}

}

void GSPlay::HandleTouchEvents(SDL_Event& e)
{
	for (auto button : m_listButton)
	{
		if (button->HandleTouchEvent(&e))
		{
			break;
		}
	}
}

void GSPlay::HandleMouseMoveEvents(int x, int y)
{
}

void GSPlay::Update(float deltaTime)
{
	switch (m_KeyPress)//Handle Key event
	{
	default:
		break;
	}
	// Key State event

	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
	for (auto it : m_listAnimation)
	{
		if (m_KeyPress == 1)
		{
			
		//	it->MoveLeft(deltaTime);
		}
		it->Update(deltaTime);
	}

	//Update position of camera
	//Camera::GetInstance()->Update(deltaTime);
	//obj->Update(deltaTime);
}

void GSPlay::Draw(SDL_Renderer* renderer)
{
	m_background->Draw(renderer);
	//m_score->Draw();
	for (auto it : m_listButton)
	{
		it->Draw(renderer);
	}
//	obj->Draw(renderer);
	for (auto it : m_listAnimation)
	{
		it->Draw(renderer);
	}
}