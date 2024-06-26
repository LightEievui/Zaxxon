#include "GUI.h"


/// <summary>
/// Initializes the GUI (graphical user interface)
/// </summary>
/// <param name="spritesheet"></param>
GUI::GUI(sf::Texture* spritesheet)
{
	this->spritesheet = spritesheet;
	heightMeterBg.setFillColor(sf::Color::Black);
	heightMeterBg.setPosition(0, 73);
	heightMeterBg.setSize(sf::Vector2f(8, 89));
	hudElements.push_back(&heightMeterBg);

	heightH = ZaxxonText::get(spritesheet, 'H');
	heightH.setPosition(0, 73 - 8);
	heightH.setColor(sf::Color(222, 0, 0)); // red right of chars
	hudElements.push_back(&heightH);

	heightL = ZaxxonText::get(spritesheet, 'L');
	heightL.setPosition(0, 162);
	heightL.setColor(sf::Color(0, 222, 0)); // green right of chars
	hudElements.push_back(&heightL);

	for (byte i = 0; i < 10; i++)
	{
		HMSection& section = heightMeterSections[i];
		section.setTexture(*spritesheet);

		if (i == 0)
			section.setType(HMSection::TOP);
		else if (i == 9)
			section.setType(HMSection::BOTTOM);
		else if (i % 2 == 1) // odd number
			section.setType(HMSection::OPEN);
		else
			section.setType(HMSection::LINE);

		section.setPosition(0, heightH.getPosition().y + 16 + 8 * i);
		hudElements.push_back(&section);
	}

	// Copyright text
	ZaxxonText::string(spritesheet, "\x1D\x1ESEGA1982", copyright);

	for (byte i = 0; i < 10; i++)
	{
		copyright[i].setColor(sf::Color(222, 222, 247));

		byte start = 124;
		if (i >= 6)
			start += 16;
		else if (i >= 2)
			start += 8;

		copyright[i].setPosition(start + 8.f * i, 248.f);
	}

	// Top score text
	ZaxxonText::string(spritesheet, "TOP000000", topScore);

	for (byte i = 0; i < 9; i++)
	{
		topScore[i].setColor(sf::Color(0, 222, 247));

		byte start = 0;
		if (i > 2)
			start += 8;

		topScore[i].setPosition(start + 8.f * i, 0.f);
	}

	// 1up score text
	ZaxxonText::string(spritesheet, "1UP000000", curScore);

	for (byte i = 0; i < 9; i++)
	{
		curScore[i].setColor(sf::Color(222, 222, 247));

		byte start = 0;
		if (i > 2)
			start += 8;

		curScore[i].setPosition(start + 8.f * i, 16.f);
	}

	// 2up score text
	ZaxxonText::string(spritesheet, "2UP000000", curScore2);

	for (byte i = 0; i < 9; i++)
	{
		curScore2[i].setColor(sf::Color(222, 222, 247));

		byte start = 0;
		if (i > 2)
			start += 8;

		curScore2[i].setPosition(start + 8.f * i, 32.f);
	}

	// Fuel text
	ZaxxonText::string(spritesheet, "FUELEF", fuelText);

	for (byte i = 0; i < 6; i++)
	{
		if (i < 4)
		{
			fuelText[i].setColor(sf::Color(0, 222, 0));
			fuelText[i].setPosition(12.f + i * 8.f, 240.f);
		}
		else
		{
			fuelText[i].setColor(sf::Color(222, 0, 0));
			fuelText[i].setPosition(52.f + (i - 4.f) * 136.f, 240.f);
		}
	}

	// Enemy plane text
	ZaxxonText::string(spritesheet, "ENEMYPLANE", enemyText);

	for (byte i = 0; i < 10; i++)
	{
		enemyText[i].setColor(sf::Color(222, 222, 247));
		// These use integer for the equation, do not 'float' it
		// edit: explicitedly casted it.
		enemyText[i].setPosition(sf::Vector2f(
			static_cast<float>(176 + i % 5 * 8),
			static_cast<float>(200 + i / 5 * 8)));
	}

	// Fuel bar sprites
	for (byte i = 0; i < 16; i++)
	{
		fuelBar[i].setTexture(*spritesheet);
		fuelBar[i].setTextureRect(sf::IntRect(328, 296, 8, 8));
		fuelBar[i].setPosition(60.f + i * 8.f, 240.f);
	}

	// Lives bar sprites
	livesBar.setTexture(*spritesheet);
	livesBar.setTextureRect(sf::IntRect(344, 288, 16, 16));
	livesBar.setPosition(12.f, 224.f);

	// Enemies left
	ZaxxonText::string(spritesheet, "X\u000a20", enemyLeft);
	enemyLeft[0].setTextureRect(sf::IntRect(368, 296, 8, 8));

	for (byte i = 0; i < 4; i++)
		enemyLeft[i].setPosition(176.f + i * 8.f, 216.f);

	// Start screen, push player button
	ZaxxonText::string(spritesheet, "PUSH1OR2PLAYERBUTTON", pushPlayerText);
	for (byte i = 0; i < 20; i++)
	{
		byte start = 8;

		if (i >= 4)
			start += 8;
		if (i >= 5)
			start += 8;
		if (i >= 7)
			start += 8;
		if (i >= 8)
			start += 8;
		if (i >= 14)
			start += 8;

		pushPlayerText[i].setColor(sf::Color(0, 222, 247));
		pushPlayerText[i].setPosition(sf::Vector2f(start + 8.f * i, 92.f));
	}

	// Start screen, high score text
	ZaxxonText::string(spritesheet, "HIGHSCORES", highScoresText);
	for (byte i = 0; i < 10; i++)
	{
		byte start = 72;

		if (i >= 4)
			start += 8;

		highScoresText[i].setColor(sf::Color(222, 222, 0));
		highScoresText[i].setPosition(sf::Vector2f(
			static_cast<float>(start + i * 8), static_cast<float>(120)));
	}

	// Game over text
	ZaxxonText::string(spritesheet, "GAMEOVER", gameOver);
	for (byte i = 0; i < 8; i++)
	{
		byte start = 60;

		if (i >= 4)
			start += 16;

		gameOver[i].setColor(sf::Color(222, 222, 222));
		gameOver[i].setPosition(sf::Vector2f(static_cast<float>(start + i * 8),
		                                     static_cast<float>(115)));
	}

	// High score entry
	ZaxxonText::string(spritesheet, "HIGHSCOREENTRY", scoreEntry);
	for (byte i = 0; i < 14; i++)
	{
		byte start = 32;

		if (i >= 4)
			start += 8;
		if (i >= 9)
			start += 8;

		scoreEntry[i].setColor(sf::Color(222, 0, 0));
		scoreEntry[i].setPosition(static_cast<float>(start + i * 8),
		                          static_cast<float>(75));
	}

	// Initials :
	ZaxxonText::string(spritesheet, "INITIALS\u0004", initialsText);
	for (byte i = 0; i < 9; i++)
	{
		byte start = 32;

		if (i == 8)
			start += 8;

		initialsText[i].setColor(sf::Color(0, 222, 247));
		initialsText[i].setPosition(static_cast<float>(start + i * 8),
		                            static_cast<float>(95));
	}

	// Entry time
	ZaxxonText::string(spritesheet, "ENTRYTIME\u000620\u0007", entryTime);
	for (byte i = 0; i < 13; i++)
	{
		byte start = 32;
		if (i >= 5)
			start += 8;
		if (i >= 9)
			start += 16;

		entryTime[i].setColor(sf::Color(222, 222, 0));
		entryTime[i].setPosition(static_cast<float>(start + i * 8),
		                         static_cast<float>(115));
	}

	// Name entry keyboard
	for (byte i = 0; i < 28; i++)
	{
		if (i < 26)
			keyboard[i] = ZaxxonText::get(spritesheet, 'A' + i);
		else
			keyboard[i] = ZaxxonText::get(spritesheet, 0 + i - 26);

		keyboard[i].setColor(sf::Color(0, 222, 0));
		keyboard[i].setPosition(static_cast<float>(32 + i % 10 * 16),
		                        static_cast<float>(135 + i / 10 * 16));
	}

	// Keyboard RUB & END
	keyboard[28].setTexture(*spritesheet);
	keyboard[28].setTextureRect(sf::IntRect(200, 248, 29, 8));
	keyboard[28].setColor(sf::Color(0, 222, 0));
	keyboard[28].setPosition(keyboard[27].getPosition().x + 13,
	                         keyboard[27].getPosition().y);

	// Cursor selector for the keyboard
	cursor.setTexture(*spritesheet);
	cursor.setTextureRect(sf::IntRect(316, 264, 16, 16));

	// End screen
	ZaxxonText::string(spritesheet,
	                   "CONGRATULATIONS\u000cYOUGETBONUS1000PTS\u0001GOOD\u000c",
	                   endScreen);
	for (byte i = 0; i < 40; i++)
	{
		// Different positions/colors for each 3 lines of text
		if (i < 16)
		{
			short start = 40;

			if (i == 15)
				start += 8;

			endScreen[i].setPosition(static_cast<float>(start + i * 8),
			                         static_cast<float>(80));
			endScreen[i].setColor(sf::Color(0, 222, 247));
		}
		else if (i < 35)
		{
			short start = 16;

			if (i > 18)
				start += 8;
			if (i > 21)
				start += 8;
			if (i > 26)
				start += 8;
			if (i > 30)
				start += 8;

			endScreen[i].setPosition(static_cast<float>(start + (i - 16) * 8),
			                         static_cast<float>(100));
			endScreen[i].setColor(sf::Color(222, 222, 0));
		}
		else
		{
			short start = 88;

			if (i == 39)
				start += 8;

			endScreen[i].setPosition(static_cast<float>(start + (i - 35) * 8),
			                         static_cast<float>(120));
			endScreen[i].setColor(sf::Color(222, 222, 222));
		}
	}

	ZaxxonText::string(spritesheet, "PLAYER\u0006\u0008\u0007", playerScreen1);
	ZaxxonText::string(spritesheet, "PLAYER\u0006\u0009\u0007", playerScreen2);

	for (byte i = 0; i < 10; i++)
	{
		playerScreen1[i].setColor(sf::Color(222, 0, 0));
		playerScreen2[i].setColor(sf::Color(222, 0, 0));
		byte start = 72;
		if (i >= 6)
			start += 8;

		playerScreen1[i].setPosition(start + 8.f * i, 112.f);
		playerScreen2[i].setPosition(start + 8.f * i, 112.f);
	}
}


/// <summary>
/// Draw all the GUI to the screen each frame.
/// Also run logic for certain interactive GUI elements and
/// update them as needed.
/// </summary>
/// <param name="window"></param>
/// <param name="playerY"></param>
/// <param name="score"></param>
/// <param name="fuel"></param>
void GUI::render(sf::RenderWindow& window, float playerY, int score, int player2score,
	int highScore, byte fuel, byte lives, byte player2Options
)
{
	bool player2mode, player2, HMoff;
	player2mode = player2Options & 0b00000001;
	player2 = player2Options & 0b00000010;
	HMoff = player2Options & 0b00000100;

	if (!HMoff)
	{
		window.draw(heightMeterBg);
		/* 69top 135bottom 66 in between, multiply by below number to get 68
		should be 8 sections each for line/open and 2 each for top/bottom so
		yDiff should be 68 max (68 from 8*8 + 4) also round to make sure correct
		*/
		int yDiff = static_cast<int>(round((playerY - 69.f) * 0.95774647887f));
		if (yDiff < 0)
			yDiff = 0;

		// -4 to make 64
		int selection = (yDiff - 2) / 8;
		if (selection < 0)
			selection = 0;

		bool changingBelow = false, changingAbove = false;
		byte last = 7, localYDiff = (yDiff - 2) % 8;
		if (localYDiff == 6)
			localYDiff = 7;

		if (yDiff <= 66 && yDiff >= 2)
		{
			HMSection& activeSection = heightMeterSections[selection];

			activeSection.progress(localYDiff);

			if (localYDiff >= 5)
			{
				heightMeterSections[selection + 1].progress(localYDiff - 7);
				changingBelow = true;
			}
		}
		else if (yDiff < 8)
		{
			// top
			heightMeterSections[0].progress(12 + localYDiff);
		}
		else if (yDiff > 64)
		{
			// bottom
		}

		for (byte i = 0; i < 10; i++)
		{
			HMSection& section = heightMeterSections[i];
			if (i < selection)
			{
				if (i != selection - 1 || !changingAbove)
					section.empty();
			}
			else if (i > selection)
			{
				if (i != selection + 1 || !changingBelow)
					section.fill();
			}

			window.draw(section);
		}
		window.draw(heightH);
		window.draw(heightL);
	}

	for (byte i = 0; i < 10; i++)
	{
		window.draw(copyright[i]);
		window.draw(enemyText[i]);
	}

	// Update score
	for (byte i = 0; i < 6; i++)
	{
		std::string scoreStr = std::to_string(score);
		std::string player2scoreStr = std::to_string(player2score);
		std::string highStr = std::to_string(highScore);

		while (scoreStr.length() < 6)
			scoreStr = '0' + scoreStr;
		while (player2scoreStr.length() < 6)
			player2scoreStr = '0' + player2scoreStr;
		while (highStr.length() < 6)
			highStr = '0' + highStr;

		curScore[8 - i] = ZaxxonText::get(spritesheet, scoreStr.at(5 - i));
		curScore[8 - i].setPosition(72.f - 8.f * i, 16.f);
		curScore[8 - i].setColor(sf::Color(222, 222, 247));

		curScore2[8 - i] = ZaxxonText::get(spritesheet, player2scoreStr.at(5 - i));
		curScore2[8 - i].setPosition(72.f - 8.f * i, 32.f);
		curScore2[8 - i].setColor(sf::Color(222, 222, 247));
		
		topScore[8 - i] = ZaxxonText::get(spritesheet, highStr.at(5 - i));
		topScore[8 - i].setPosition(72.f - 8.f * i, 0.f);
		topScore[8 - i].setColor(sf::Color(0, 222, 247));
	}

	for (byte i = 0; i < 9; i++)
	{
		window.draw(topScore[i]);

		if (i >= 3)
		{
			window.draw(curScore[i]);
			if(player2mode)
				window.draw(curScore2[i]);
		}
	}

	
	// one score will flash
	if (scoreClock.getElapsedTime().asSeconds() > 0.4)
	{
		for (int i = 0; i < 3; i++)
		{
			if (!player2)
				window.draw(curScore[i]);
			else if (player2mode)
				window.draw(curScore2[i]);
		}

		if (scoreClock.getElapsedTime().asSeconds() > 0.8)
			scoreClock.restart();
	}

	// draw the other score
	for (int i = 0; i < 3; i++)
	{
		if (player2)
			window.draw(curScore[i]);
		else if (player2mode)
			window.draw(curScore2[i]);
	}

	for (byte i = 0; i < 6; i++)
		window.draw(fuelText[i]);

	// Update fuel bar
	for (byte i = 0; i < 16; i++)
	{
		sf::IntRect status(328, 296, 8, 8);
		if (fuel - i * 8 < 8)
			if (fuel - i * 8 > 0)
				status.left = 328 - (8 - (fuel - i * 8) % 8) * 16;
			else
				status.left = 176, status.top = 272;

		fuelBar[i].setTextureRect(status);
		window.draw(fuelBar[i]);
	}

	for (byte i = 0; i < lives; i++)
	{
		livesBar.setPosition(12.f + i * 16.f, 224.f);
		window.draw(livesBar);
	}

	for (byte i = 0; i < 4; i++)
		window.draw(enemyLeft[i]);
}


/// <summary>
/// Rendering specifically for the start screen.
/// This will include the leaderboard will the top 6 scores and names.
/// </summary>
/// <param name="window"></param>
/// <param name="highScore"></param>
void GUI::startRender(sf::RenderWindow& window, int highScore)
{
	for (byte i = 0; i < 10; i++)
		window.draw(copyright[i]);

	for (byte i = 0; i < 9; i++)
	{
		window.draw(topScore[i]);
		window.draw(curScore[i]);
	}

	for (byte i = 0; i < 20; i++)
		window.draw(pushPlayerText[i]);

	for (byte i = 0; i < 42; i++)
		window.draw(highScores[i]);

	for (byte i = 0; i < 10; i++)
		window.draw(highScoresText[i]);

	for (byte i = 0; i < 18; i++)
		window.draw(highScoresNames[i]);
}


/// <summary>
/// Prepare the leaderboard scores to be rendered.
/// </summary>
/// <param name="window"></param>
/// <param name="scores"></param>
void GUI::renderScores(sf::RenderWindow& window, int scores[],
                       std::string names[])
{
	// The score values
	for (byte i = 0; i < 42; i++)
	{
		if (i % 7 == 0)
			highScores[i] = ZaxxonText::get(spritesheet, '1' + i / 7);
		else
		{
			std::string scoreStr = std::to_string(scores[i / 7]);

			while (scoreStr.length() < 6)
				scoreStr = '0' + scoreStr;

			highScores[i] = ZaxxonText::get(spritesheet,
			                                scoreStr[0 + (i % 7) - 1]);
		}

		highScores[i].setColor(sf::Color(0, 222, 0));

		int start = 4;
		if (i % 7)
			start += 8;
		if (i >= 21)
			start += 120;

		int layer = 140;
		layer += 12 * (i % 21 / 7);
		highScores[i].setPosition(sf::Vector2f(
			static_cast<float>(start + i % 7 * 8), static_cast<float>(layer)));
	}

	// The names for each score
	for (byte i = 0; i < 18; i++)
	{
		// We use _ for spaces in names
		if (names[i / 3][i % 3] != '_')
			highScoresNames[i] = ZaxxonText::get(
				spritesheet, names[i / 3][i % 3]);

		highScoresNames[i].setColor(sf::Color(0, 222, 0));

		int start = 76;
		if (i >= 9)
			start += 120;

		int layer = 140;
		layer += 12 * (i % 9 / 3);
		highScoresNames[i].setPosition(sf::Vector2f(
			static_cast<float>(start + i % 3 * 8), static_cast<float>(layer)));
	}
}


/// <summary>
/// Specific GUI that renders once you've run out of lives.
/// This includes the game over text and name entry screen.
/// </summary>
/// <param name="window"></param>
/// <param name="state"></param>
void GUI::renderEnd(sf::RenderWindow& window, byte timeLeft, byte selector,
                    const char name[3])
{
	if (timeLeft == 255)
	{
		for (byte i = 0; i < 8; i++)
			window.draw(gameOver[i]);

		return;
	}

	for (byte i = 0; i < 14; i++)
		window.draw(scoreEntry[i]);

	for (byte i = 9; i < 12; i++)
	{
		if (name[i] != '_' && name[i] != ' ')
			initialsText[i] = ZaxxonText::get(spritesheet, name[i - 9]);
		initialsText[i].setColor(sf::Color(0, 222, 247));
		initialsText[i].setPosition(sf::Vector2f(
			static_cast<float>(120 + (i - 9) * 16), static_cast<float>(95)));
	}

	for (byte i = 0; i < 12; i++)
		window.draw(initialsText[i]);

	entryTime[10] = ZaxxonText::get(spritesheet, '0' + timeLeft / 10);
	entryTime[10].setPosition(136, 115);
	entryTime[10].setColor(sf::Color(222, 222, 0));
	entryTime[11] = ZaxxonText::get(spritesheet, '0' + timeLeft % 10);
	entryTime[11].setPosition(144, 115);
	entryTime[11].setColor(sf::Color(222, 222, 0));

	for (byte i = 0; i < 13; i++)
		window.draw(entryTime[i]);

	// Draw cursor before keyboard
	cursor.setPosition(static_cast<float>(28 + selector % 10 * 16),
	                   static_cast<float>(131 + selector / 10 * 16));
	window.draw(cursor);

	for (byte i = 0; i < 30; i++)
		window.draw(keyboard[i]);
}


/// <summary>
/// Render the win screen each lap completed.
/// </summary>
/// <param name="window"></param>
void GUI::renderWin(sf::RenderWindow& window)
{
	for (byte i = 0; i < 40; i++)
		window.draw(endScreen[i]);
}


void GUI::renderPlayerScreen(sf::RenderWindow& window, bool player2)
{
	for (byte i = 0; i < 10; i++)
	{
		if (player2)
			window.draw(playerScreen2[i]);
		else
			window.draw(playerScreen1[i]);
	}
}
