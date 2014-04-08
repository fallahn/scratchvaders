///class for capturing audio input///
/////////////////////////////////////
/*
This class inherits SFML ABC sf::SoundRecorder
and impliments the pure virtual function
onProcessSamples. In this case it converts
the values stored in the buffer into dB/amp
and sets the bool member Signal to true if
the value is above m_threshold. Signal uses
C++11 atomic because the audio capture is
run in its own thread. (See this discussion:
http://stackoverflow.com/questions/16320838/when-do-i-really-need-to-use-atomicbool-instead-of-bool
)
The principle is to test Signal and fire
some sort of event when true (ie the audio
input breaches the threshold)
NOTE: this class requires a small change to
the SFML source to provide good response.
sleep(milliseconds(100)); (approx line 155)
in SoundRecorder.cpp needs to be changed to
sleep(milliseconds(1));
*/
/////////////////////////////////////
#ifndef AUDIO_IN_H_
#define AUDIO_IN_H_

#include <SFML/Audio/SoundBufferRecorder.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <atomic>

class AudioIn final 
	: public sf::SoundRecorder,
	public sf::Drawable,
	private sf::NonCopyable
{
public:
	AudioIn();
	std::atomic<bool> Signal;

	//functions for updating and drawing meter
	void Update(float dt);
	void SetCentre(const sf::Vector2f& viewSize);
	void SetThreshold(float threshold);

private:
	bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override;
	float m_threshold; //point at which level sets Signal to true in dB
	const float m_reference;
	const float m_minDb;
	const float m_maxDb;
	const float m_barWidth;
	float m_dB;
	sf::Mutex m_mutex;
	sf::Clock m_calibrationClock; //used to drop the calibration level after
	const float m_calibrationTime; //x seconds

	//these members are just used to draw a fancy meter and set the threshold
	mutable sf::RenderTexture m_renderTexture;
	sf::Sprite m_sprite;
	sf::RectangleShape m_levelShape, m_settingShape;
	sf::Color m_clearColour; //clear the rendertexture with this, set to red when over thresh, else black
	sf::Font m_font;
	sf::Text m_dBText;

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif //AUDIO_IN_H_