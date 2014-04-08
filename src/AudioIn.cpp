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

///source for audio capture///
#include <Game/AudioIn.h>
#include <sstream>

AudioIn::AudioIn()
	: m_threshold		(-1.f),
	Signal				(false),
	m_reference			(32768.f),
	m_minDb				(/*20.f * log10(1.f / m_reference)*/-40.f), //change reference so result is -40dB
	m_maxDb				(0.f),
	m_barWidth			(400.f),
	m_calibrationTime	(3.f)
{
	//set up sprite for input bar
	m_renderTexture.create(static_cast<unsigned>(m_barWidth), 80u);
	m_sprite.setTexture(m_renderTexture.getTexture());

	m_levelShape.setFillColor(sf::Color::Green); //TODO set height to smaller than render texture
	m_levelShape.setPosition(0, 10.f); //magic number
	m_settingShape.setFillColor(sf::Color::Blue);
	m_settingShape.setSize(sf::Vector2f(10.f, 80.f));
	m_settingShape.setOrigin(5.f, 0.f);

	if(m_font.loadFromFile("assets/fonts/VeraMono.ttf"))
	{
		m_dBText.setFont(m_font);
		m_dBText.move(4.f, 6.f);
	}
}

bool AudioIn::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
	//sum and average the value of the current set of samples
	sf::Int64 sum = 0;
	for(std::size_t i = 0; i < sampleCount; i++)
	{
		sum += std::abs(samples[i]);
	}

	float avg = static_cast<float>(sum / sampleCount);
     

    //10 * log() == power, 20 * log() == amplitude
	//See: http://www.sengpielaudio.com/calculator-db.htm
	sf::Lock lock(m_mutex);
	m_dB = (avg > 0) ? 20.f * log10(avg / m_reference) : -1000000.f; //in theory could go to -inf so we cut off somewhere
	Signal = (m_dB > m_threshold);

	return true;
}

void AudioIn::Update(float dt)
{
	//update size of level shape
	const float barWidth = (m_dB - m_minDb) * m_barWidth / (m_maxDb - m_minDb);
	m_levelShape.setSize(sf::Vector2f(barWidth, 60.f));

	//update threshold from keyboard input
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
		&& m_threshold < 0.f) m_threshold += 10.f * dt;
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
		&& m_threshold > m_minDb) m_threshold -= 10.f * dt;

	//auto peak detection
	float newThresh = m_dB - 10.f;
	if(newThresh < m_minDb) newThresh = m_minDb;
	if(newThresh > m_threshold) m_threshold = newThresh;
	//else if(m_calibrationClock.getElapsedTime().asSeconds() > m_calibrationTime
	//	&& m_threshold > m_dB + 5.f)
	//{
	//	m_calibrationClock.restart();
	//	m_threshold = newThresh;
	//}

	//update bar from threshold
	const float barPos = (m_threshold - m_minDb) * m_barWidth / (m_maxDb - m_minDb);
	m_settingShape.setPosition(barPos, 0.f);

	//update clear colour from signal status
	m_clearColour = (Signal) ? sf::Color::Red : sf::Color::Black;

	//update text
	std::stringstream ss;
	ss << m_threshold;
	m_dBText.setString(ss.str());

}

void AudioIn::SetCentre(const sf::Vector2f& viewSize)
{
	sf::Vector2f size(static_cast<float>(m_renderTexture.getSize().x / 2u), static_cast<float>(m_renderTexture.getSize().y / 2u));
	m_sprite.setPosition((viewSize / 2.f) - size);
}

void AudioIn::SetThreshold(float threshold)
{
	if(threshold > 0.f) threshold = 0.f;
	else if (threshold < m_minDb) threshold = m_minDb;

	m_threshold = threshold;
}

//private
void AudioIn::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
	//draw to render texture
	m_renderTexture.clear(m_clearColour);
	m_renderTexture.draw(m_levelShape);
	m_renderTexture.draw(m_settingShape);
	m_renderTexture.draw(m_dBText);
	m_renderTexture.display();

	rt.draw(m_sprite);
}