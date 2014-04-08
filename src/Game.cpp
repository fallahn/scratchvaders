/*********************************************************************
Zlib license.

Matt Marchant 2013 - 2014
http://trederia.blogspot.com

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

///source code for game class///
#include <Game/Game.h>
#include <sstream>
#include <iostream>

//ctor
Game::Game()
	: m_renderWindow	(sf::VideoMode(1024u, 768u), "Scratch Invaders"),
	m_viewSize			(1440.f, 1080.f), //assumes 4:3 window
	m_maxTime			(1.f / 60.f), //60 updates per second
	FIRE_BUTTON			(0x01),
	CONTINUE_BUTTON		(0x02),
	CALIBRATE_BUTTON	(0x04),
	m_currentMode		(Mode::Menu),
	m_countIn			(5u),
	m_ammoManager		(m_viewSize),
	m_player			(sf::Vector2f(10.f, 1000.f), m_viewSize, m_ammoManager),
	m_invaderManager	(m_viewSize, m_ammoManager, m_player),
	m_defenceManager	(m_viewSize, m_ammoManager)
{
	//set the view so that we always scale 1080p to window size (assuming 4:3 ratio)
	sf::View view(m_viewSize / 2.f, m_viewSize);
	m_renderWindow.setView(view);
	//m_renderWindow.setVerticalSyncEnabled(true);
	m_renderWindow.setFramerateLimit(120u);

	m_font.loadFromFile("assets/fonts/VeraMono.ttf");
	m_countText.setCharacterSize(560u);
	m_countText.setFont(m_font);

	m_messageText.setCharacterSize(45u);
	m_messageText.setFont(m_font);

	m_continueText.setCharacterSize(50u);
	m_continueText.setFont(m_font);
	
	m_scoreText.setCharacterSize(40u);
	m_scoreText.setFont(m_font);
	m_scoreText.setPosition(30.f, m_viewSize.y - 120.f);

	m_SetMode(Mode::Menu);

	m_audioIn.SetCentre(m_viewSize);
	m_gameMusic.openFromFile("assets/audio/music/music.ogg");
	m_gameMusic.setLoop(true);
}

//public
void Game::Run()
{
	//attempt to open audio device for input and warn if not available
	if(m_audioIn.isAvailable())
		m_audioIn.start(22050u);
	else
		std::cout << "Could not open Audio Device for input" << std::endl;
	
	//enter main loop
	float dt = 0;
	while(m_renderWindow.isOpen())
	{
		//poll input
		sf::Event event;
		while(m_renderWindow.pollEvent(event))
		{
			if(event.type == sf::Event::Closed ||
				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				m_renderWindow.close();
        }
		//calc time since last frame and update logic
		dt += m_dtClock.restart().asSeconds();
		while(dt > 0.f)
		{
			dt -= m_maxTime;
			m_Update(m_maxTime);
		}

		//draw
		m_Render();
	}
	//stop the audio capture when quitting
	m_audioIn.stop();
}

//private
void Game::m_Update(float dt)
{
	//update background
	m_starField.Update(dt, sf::FloatRect(sf::Vector2f(), m_viewSize));
	
	//get input state
	sf::Uint8 input = m_ParseInput();

	//perform updates based on current mode of fsm
	switch(m_currentMode)
	{
	case Mode::Calibrate:
		m_audioIn.Update(dt);
		if((input & CONTINUE_BUTTON))
		{
			m_SetMode(Mode::Menu);
		}
		break;
	case Mode::Menu:
	default:
		//count in if input and switch mode @ 0
		if(m_countIn)
		{
			if((input & FIRE_BUTTON) &&
				!(m_prevInput & FIRE_BUTTON))
			{
				m_countIn--;
			}
			else if((input & CALIBRATE_BUTTON))
			{
				m_SetMode(Mode::Calibrate);
			}
		}
		else
		{
			m_SetMode(Mode::Playing);
		}
		{ //scope
			std::stringstream ss;
			ss << (int)m_countIn;			
			m_countText.setString(ss.str());

			sf::Vector2f position;
			position.x = (m_viewSize.x - m_countText.getLocalBounds().width) / 2.f; 
			position.y = (m_viewSize.y - m_countText.getLocalBounds().height) / 2.f;
			m_countText.setPosition(position);

		}//end scope
		break;
	//--------------------------------------------//
	case Mode::Playing:
		//fire player weapon if input
		if((input & FIRE_BUTTON) &&
			!(m_prevInput & FIRE_BUTTON))
			m_player.Fire();

		//update player
		m_player.Update(dt);

		//update managers
		m_invaderManager.Update(dt);
		m_ammoManager.Update(dt);
		m_defenceManager.Update(dt);

		//check if we beat a wave and update score
		if(m_invaderManager.GetInvaderCount() == 0u)
		{
			//start a new wave
			m_invaderManager.Reset();
			m_ammoManager.Reset();
			m_player.AddRound();
			m_player.AddScore(100u);
		}

		//check for game end condition
		if(m_player.GetState() == BaseEntity::State::Dead)
		{
			m_SetMode(Mode::GameOver);
		}

		//update screen score output
		{
			std::stringstream ss;
			ss << "Round: " << (int)m_player.GetRound() << std::endl << "Score: " << m_player.GetScore();
			m_scoreText.setString(ss.str());
		}

		break;
	//--------------------------------------------//
	case Mode::GameOver:
		//return to main menu when button pressed
		if((input & CONTINUE_BUTTON) &&
			!(m_prevInput & CONTINUE_BUTTON))
		{
			//switch to main menu
			m_SetMode(Mode::Menu);
		}
		break;
	}
	
	//save input for next frame
	m_prevInput = input;

	//fade music if it's playing and we aren't in game
	if(m_gameMusic.getStatus() == sf::Music::Playing
		&& m_currentMode != Mode::Playing)
	{
		float vol = m_gameMusic.getVolume();
		if(vol > 1.f)
		{
			vol -= 50.f * dt;
			m_gameMusic.setVolume(vol);
		}
		else m_gameMusic.stop();
	}
}

void Game::m_Render()
{
	m_renderWindow.clear(sf::Color::Transparent);
	m_renderWindow.draw(m_starField);
	//draw content based on current mode
	switch(m_currentMode)
	{
	case Mode::Calibrate:
		m_renderWindow.draw(m_messageText);
		m_renderWindow.draw(m_continueText);
		m_renderWindow.draw(m_audioIn);
		break;
	case Mode::Menu:
	default:
		m_renderWindow.draw(m_messageText);
		m_renderWindow.draw(m_continueText);
		m_renderWindow.draw(m_countText);
		break;
	//---------------------------------------//
	case Mode::Playing:
		m_renderWindow.draw(m_ammoManager);
		m_renderWindow.draw(m_invaderManager);
		m_renderWindow.draw(m_defenceManager);
		m_renderWindow.draw(m_player.GetShape());
		m_renderWindow.draw(m_scoreText);
		break;
	//---------------------------------------//
	case Mode::GameOver:
		m_renderWindow.draw(m_ammoManager);
		m_renderWindow.draw(m_invaderManager);
		m_renderWindow.draw(m_defenceManager);
		m_renderWindow.draw(m_player.GetShape());
		m_renderWindow.draw(m_scoreText);

		m_renderWindow.draw(m_continueText);
		m_renderWindow.draw(m_messageText);
		break;
	}
	m_renderWindow.display();
}

sf::Uint8 Game::m_ParseInput()
{
	//parses inputs and sets corresponding bit flags
	//currently only uses two buttons, but in current
	//state up to 8 input types can be set from varying sources
	
	sf::Uint8 retVal = 0u;
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
		|| m_audioIn.Signal)
	{
		retVal |= FIRE_BUTTON;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		retVal |= CONTINUE_BUTTON;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		retVal |= CALIBRATE_BUTTON;

	return retVal;
}

void Game::m_SetMode(Mode mode)
{
	switch(mode)
	{
	case Mode::Menu:
		m_messageText.setString("Press Fire to start");	
		m_messageText.setPosition((m_viewSize.x - m_messageText.getLocalBounds().width) / 2.f, 340.f);
		m_continueText.setString("Press C to calibrate audio input");
		m_continueText.setPosition((m_viewSize.x - m_continueText.getLocalBounds().width) / 2.f, 40.f);
		m_currentMode = Mode::Menu;		
		break;
	case Mode::Calibrate:
		m_audioIn.SetThreshold(-45.f);
		m_messageText.setString("Use left and right to calibrate threshold");	
		m_messageText.setPosition((m_viewSize.x - m_messageText.getLocalBounds().width) / 2.f, 340.f);				
		m_continueText.setString("Press Return to continue");
		m_continueText.setPosition((m_viewSize.x - m_continueText.getLocalBounds().width) / 2.f, 40.f);
		m_currentMode = Mode::Calibrate;		
		break;
	case Mode::Playing:
		//reset game state
		m_ammoManager.Reset();
		m_invaderManager.Reset();
		m_defenceManager.Reset();
		m_player.Reset();

		m_countIn = 5u;
		m_gameMusic.setVolume(100.f);
		m_gameMusic.play();
		m_currentMode = Mode::Playing;
		break;
	case Mode::GameOver:
		m_messageText.setString("Game Over");
		m_messageText.setPosition((m_viewSize.x - m_messageText.getLocalBounds().width) / 2.f, 440.f);
		m_continueText.setString("Press Return to continue");
		m_continueText.setPosition((m_viewSize.x - m_continueText.getLocalBounds().width) / 2.f, 520.f);	
		m_currentMode = Mode::GameOver;
		break;
	default: break;
	}
}
