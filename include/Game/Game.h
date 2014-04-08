///creates the actual game///

#ifndef GAME_H_
#define GAME_H_

#include <Game/AudioIn.h>
#include <Game/Player.h>
#include <Game/AmmoManager.h>
#include <Game/InvaderManager.h>
#include <Game/DefenceManager.h>
#include <Game/StarField.h>

class Game final : private sf::NonCopyable
{
public:
	Game();
	void Run();
private:

	enum class Mode
	{
		Menu,
		Playing,
		GameOver,
		Calibrate
	} m_currentMode;

	sf::RenderWindow m_renderWindow;
	sf::Vector2f m_viewSize; //resolution of the world to render scaled to window
	sf::Clock m_dtClock;
	const float m_maxTime; //maximum time a single update is allowed to take
	AudioIn m_audioIn;
	sf::Uint8 m_prevInput; //the previous frame's control input
	const sf::Uint8 FIRE_BUTTON; //bit mask representing 'fire'
	const sf::Uint8 CONTINUE_BUTTON;
	const sf::Uint8 CALIBRATE_BUTTON;

	//for displaying menu messages
	sf::Font m_font;
	sf::Text m_countText, m_messageText;
	sf::Text m_scoreText, m_continueText;
	sf::Uint8 m_countIn;

	//private functions
	void m_Update(float dt);
	void m_Render();
	//parse input looks at any possible inputs
	//(currently keyboard or sound) and sets
	//corresponding bit flags in a single byte
	//which is returned as sf::Uint8. The update
	//function then compares this result with the
	//previous frame's result and performs any
	//actions such as firing the player weapon
	sf::Uint8 m_ParseInput();
	//used to set messages and modes in menu navigation
	void m_SetMode(Mode mode);

	//game entities
	Player m_player;
	AmmoManager m_ammoManager;
	InvaderManager m_invaderManager;
	DefenceManager m_defenceManager;
	StarField m_starField;

	//sound
	sf::Music m_gameMusic;
};

#endif //GAME_H_