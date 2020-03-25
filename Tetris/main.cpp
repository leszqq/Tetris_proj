#include <SFML/Graphics.hpp>

#include <iostream>
#include <Windows.h>
#include <string>
#include <stdlib.h>

bool clearPlane(int plane[22][10],int *pts);	// zwraca '1' je�li koniec gry

int main()
{
	const int verNum = 22;	// liczba wierszy planszy
	const int colNum = 10;	// liczba kolumn planszy
	const int tileSize{ 32 };	// rozmiar bloku	
	const int blockSize{ 4 };	// liczba blok�w tworz�cych bloki

	int points{ 0 };	
	int *pointsPointer = &points;
	sf::Time fallingTime = sf::milliseconds(420);	// czas pomiedzy automatycznym spadaniem bloku
	sf::Clock clock;
	sf::Vector2i rot{ 0,0 };
	sf::Vector2i planeOffset{ tileSize * 2, 0 };	// przesuni�cie planszy wzgl�dem lewego g�rnego rogu ekranu
	int plane[verNum][colNum]{ 0 };		// tablica reprezentuj�ca plansze. pierwsza cyfra kom�rki: 0 - wolna przestrze�, 1 - spadaj�cy blok, 2 -sta�y blok. Druga cyfra - kolor
	bool blockFalling{ false };		// zmienna ograniczaj�ca generowanie nowych blok�w
	bool keyPressed{ false };		
	bool noCollision{ true };		
	int n{ 1 };
	sf::Vector2i cords[4]{
		{0,0},
		{0,0},
		{0,0},
		{0,0},
	};	//wsp�rz�dne, na kt�rych znajduje si� spadaj�cy blok
	sf::Vector2i tempVec[4]{ cords[0], cords[1], cords[2], cords[3] };

	//ladowanie czcionki
	sf::Font font;
	font.loadFromFile("ariali.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::White);
	text.setPosition(tileSize * 15, tileSize * 4);
	text.setFont(font);
	text.setCharacterSize(32);

	//ladowanie tekstur
	sf::Texture texture;
	sf::Texture textureBack;
	sf::Texture textureMask;
	textureMask.loadFromFile("maska.png");
	textureBack.loadFromFile("background.png");
	texture.loadFromFile("tile.png");
	texture.setRepeated(true);
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sf::Sprite spriteBack;
	spriteBack.setTexture(textureBack);
	sf::Sprite spriteMask;
	spriteMask.setTexture(textureMask);
	spriteMask.setPosition(64, 0);
	
	//renderowanie okna
	sf::RenderWindow window(sf::VideoMode(704, 768), "Tetris", sf::Style::Titlebar | sf::Style::Close);	// stworz i otworz okno
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		if (!blockFalling) {
			blockFalling = true;
			n = std::rand() % 7 + 1;
			if (n == 1) {	// I
				for (int i = 0; i < blockSize; i++)
				{
					cords[i].x = 5;
					cords[i].y = i;
				}
			}
			else if (n == 2) {	// S
				cords[0].x = cords[1].x = 4;
				cords[2].x = cords[3].x = 5;
				cords[0].y = 0;
				cords[1].y = cords[2].y = 1;
				cords[3].y = 2;
			}
			else if (n == 3) {	// Z
				cords[0].x = cords[1].x = 5;
				cords[2].x = cords[3].x = 4;
				cords[0].y = 0;
				cords[1].y = cords[2].y = 1;
				cords[3].y = 2;
			}
			else if (n == 4) {	// T
				cords[0].x = cords[2].x = 5;
				cords[1].x = 4;
				cords[3].x = 6;
				cords[1].y = cords[2].y = cords[3].y = 1;
				cords[0].y = 0;
			}
			else if (n == 5) {	// L
				cords[0].x = cords[1].x = cords[2].x = 4;
				cords[3].x = 5;
				cords[0].y = 0;
				cords[1].y = 2;
				cords[2].y = 1;
				cords[3].y = 2;
			}
			else if (n == 6) { // J
				cords[0].x = cords[1].x = cords[2].x = 5;
				cords[3].x = 4;
				cords[0].y = 0;
				cords[1].y = cords[3].y = 2;
				cords[2].y = 1;
			}
			else if (n == 7) { // O
				cords[0].x = cords[3].x = 4;
				cords[1].x = cords[2].x = 5;
				cords[0].y = cords[1].y = 0;
				cords[2].y = cords[3].y = 1;
			}
		}	  // generuje nowy blok je�li poprzedni ju� spad�

		// spobs�uga wszystkich zdarze� od ostatniej iteracji
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:	// obs�uga klawiatury

				if (event.key.code == sf::Keyboard::Left)	// przesuwa blok w lewo po nacisnieciu strza�ki w lewo
				{
					keyPressed = true;
					for (int i = 0; i < blockSize; i++)
					{
						if (cords[i].x == 0 || plane[cords[i].y][cords[i].x - 1] / 10 == 2)
							noCollision = false;
					}
					if (noCollision)
					{
						for (int i = 0; i < blockSize; i++)
						{
							cords[i].x = cords[i].x - 1;
						}
					}
					noCollision = true;
				}

				else if (event.key.code == sf::Keyboard::Right)
				{
					keyPressed = true;
					for (int i = 0; i < blockSize; i++)
					{
						if (cords[i].x == colNum - 1 || plane[cords[i].y][cords[i].x + 1] / 10 == 2)
							noCollision = false;
					}
					if (noCollision)
					{
						for (int i = 0; i < blockSize; i++)
						{
							cords[i].x = cords[i].x + 1;
						}
					}
					noCollision = true;
				}   // przesuwa blok w prawo

				else if (event.key.code == sf::Keyboard::Down)
				{
					keyPressed = true;
					for (int i = 0; i < blockSize; i++)
					{
						if (cords[i].y == verNum - 1 || plane[cords[i].y + 1][cords[i].x] / 10 == 2)
							noCollision = false;
					}
					if (noCollision)
					{
						for (int i = 0; i < blockSize; i++)
						{
							cords[i].y = cords[i].y + 1;
						}
					}
					else
					{
						noCollision = true;
						for (int i = 0; i < blockSize; i++)
						{
							plane[cords[i].y][cords[i].x] = 20 + n;
							cords[i].x = 0;
							cords[i].y = 0;
						}
						blockFalling = false;
					}
					//noCollision = true;
				}	  // przesuwa blok w d�

				else if (event.key.code == sf::Keyboard::Up)
				{
					keyPressed = true;
					for (int i = 0; i < blockSize; i++)
					{
						tempVec[i].x = cords[i].x;
						tempVec[i].y = cords[i].y;
					}
					rot = cords[2];
					for (int i = 0; i < blockSize; i++)
					{
						cords[i].x = -cords[i].y + rot.y + rot.x;
						cords[i].y = tempVec[i].x - rot.x + rot.y;
					}
					clearPlane(plane, pointsPointer);
					for (int i = 0; i < blockSize; i++) {
						if (cords[i].x < 0 || cords[i].x >= colNum || plane[cords[i].y][cords[i].x] / 10 != 0) {
							noCollision = false;
							break;
						}
					}
					if (!noCollision) {		// je�li po obrocie blok kolidowa� by z innymi, wraca do poprzedniej pozycji
						noCollision = true;
						for (int j = 0; j < blockSize; j++) {
							cords[j].x = tempVec[j].x;
							cords[j].y = tempVec[j].y;
						}
					}
				}	// obr�t bloku
				break;
			default:
				break;
			}
		}

		// opadanie bloku po czasie fallingTime
		if (clock.getElapsedTime() > fallingTime)
		{
			clock.restart();
			for (int i = 0; i < blockSize; i++)
			{
				if (cords[i].y == verNum - 1 || plane[cords[i].y + 1][cords[i].x] / 10 == 2)	// 
					noCollision = false;
			}
			if (noCollision)
			{
				for (int i = 0; i < blockSize; i++)	// je�li blok nie b�dzie kolidowa� z innymi, zmienia jego pozycje
				{
					cords[i].y = cords[i].y + 1;
				}
			}
			else	// je�li pod blokiem znajduje si� kolejny, unieruchom go i odblokuj generowanie kolejnego
			{
				noCollision = true;
				for (int i = 0; i < blockSize; i++)
				{
					plane[cords[i].y][cords[i].x] = 20 + n;
					cords[i].x = 0;
					cords[i].y = 0;
				}
				blockFalling = false;
			}
		}

		// aktualizacja planszy, koniec gry je�li plansza jest przepe�niona 
		if (clearPlane(plane, pointsPointer)) {
			window.clear(sf::Color::Black);
			text.setStyle(sf::Text::Bold);
			text.setPosition(210, 300);
			text.setString("\tKoniec gry\n\tTw�j wynik:\n\t\t "+ std::to_string(points) +"\n   Nacisnij Esc\n aby zakonczyc");
			window.draw(text);
			window.display();
			while(true) {
				window.pollEvent(event);
				switch (event.type)
				{
				case sf::Event::Closed:
					window.close();
					return 0;
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape) {
						window.close();
						return 0;
						break;
					}
					break;
				}
			}
		}
		for (int i = 0; i < 4; i++)
		{
			plane[cords[i].y][cords[i].x] = 10 + n;
		}

		// czyszczenie ekranu
		window.clear(sf::Color::Black);
		// rysowanie do bufora
		window.draw(spriteBack);
		text.setString("WYNIK: \n" + std::to_string(points));
		window.draw(text);
		for (int i = 0; i < verNum; i++) {
			for (int j = 0; j < colNum; j++) {
				if (plane[i][j] / 10 == 1 || plane[i][j] / 10 == 2)
				{
					switch (plane[i][j] % 10) {
					case 1:
						sprite.setColor(sf::Color::Blue);
						break;
					case 2:
						sprite.setColor(sf::Color::Red);
						break;
					case 3:
						sprite.setColor(sf::Color::Yellow);
						break;
					case 4:
						sprite.setColor(sf::Color::Green);
						break;
					case 5:
						sprite.setColor(sf::Color::Magenta);
						break;
					case 6:
						sprite.setColor(sf::Color::Cyan);
						break;
					case 7:
						sprite.setColor(sf::Color::Color(122, 222, 50, 255));
						break;
					}
					sprite.setPosition(j * tileSize + planeOffset.x, i * tileSize + planeOffset.y);
					window.draw(sprite);
				}
			}
		}
		window.draw(spriteMask);
		//wyswietlanie obrazu
		window.display();
	}
}

//funkcja clearPlane aktualizuje plansz�, usuwa wype�nione wiersze oraz sprawdza, czy plansza nie jest przepe�niona(je�li jest - zwraca "true")
bool clearPlane(int plane[22][10], int *pts) {
	bool verseFilled{ true };
	for (int i = 0; i < 22; i++)
	{
		verseFilled = true;
		for (int j = 0; j < 10; j++) {
			if (plane[i][j] / 10 == 2 &&(i == 0 || i ==1)) {
				return true;
			}

			if (plane[i][j] / 10 != 2) {
				plane[i][j] = 0;
				verseFilled = false;
			}
		}
		if (verseFilled) {
			*pts = *pts + rand() % 100 + 100;
			for (int k = i; k > 0; k--) {
				for (int l = 0; l < 10; l++) {
					plane[k][l] = plane[k - 1][l];
				}
			}
		}
	}
	return false;
}	
