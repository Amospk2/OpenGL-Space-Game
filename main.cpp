#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string.h>

using namespace std;

int spaceshipX = 250, spaceshipY = 250;
int bullets[3][3];
int currentBulletIndex = 0;
bool gameOver = false;
static int rotationAngle = 0.0f;
int gameOverI;
int obstacleX[4] = {500, 0, 0, 750};
int obstacleY[4] = {100, 250, 400, 100};

int obstacleTypeAndDirection[4][2] = {
	1, 0,
	1, 1,
	0, 1,
	0, 0};

void reset()
{
	spaceshipX = 250;
	spaceshipY = 250;

	obstacleX[0] = 500;
	obstacleX[1] = 0;
	obstacleX[2] = 0;
	obstacleX[3] = 750;

	obstacleY[0] = 100;
	obstacleY[1] = 250;
	obstacleY[2] = 400;
	obstacleY[3] = 100;
}

void rotacionarNavePeloAngulo()
{
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(5.0);

	if (rotationAngle == 0)
	{
		glBegin(GL_POLYGON);
		glVertex2f(spaceshipX, spaceshipY);
		glVertex2f(spaceshipX, spaceshipY - 35);
		glVertex2f(spaceshipX + 20, spaceshipY - 35);
		glVertex2f(spaceshipX + 35, spaceshipY - 20);
		glVertex2f(spaceshipX + 20, spaceshipY);
	}
	else if (rotationAngle == 90)
	{
		glBegin(GL_POLYGON);
		glVertex2f(spaceshipX, spaceshipY - 35);
		glVertex2f(spaceshipX + 35, spaceshipY - 35);
		glVertex2f(spaceshipX + 35, spaceshipY - 20);
		glVertex2f(spaceshipX + 20, spaceshipY);
		glVertex2f(spaceshipX, spaceshipY - 20);
	}
	else if (rotationAngle == 180)
	{
		glBegin(GL_POLYGON);
		glVertex2f(spaceshipX + 35, spaceshipY);
		glVertex2f(spaceshipX + 35, spaceshipY - 35);

		glVertex2f(spaceshipX + 20, spaceshipY - 35);
		glVertex2f(spaceshipX, spaceshipY - 20);

		glVertex2f(spaceshipX + 20, spaceshipY);
	}
	else
	{
		glBegin(GL_POLYGON);
		glVertex2f(spaceshipX, spaceshipY);
		glVertex2f(spaceshipX + 35, spaceshipY);
		glVertex2f(spaceshipX + 35, spaceshipY - 20);
		glVertex2f(spaceshipX + 20, spaceshipY - 35);
		glVertex2f(spaceshipX, spaceshipY - 20);
	}

	glEnd();
}
void drawSpaceship()
{
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();

	glBegin(GL_QUADS);
	glVertex2f(spaceshipX, spaceshipY);
	glVertex2f(spaceshipX + 35, spaceshipY);
	glVertex2f(spaceshipX + 35, spaceshipY - 35);
	glVertex2f(spaceshipX, spaceshipY - 35);
	glEnd();

	rotacionarNavePeloAngulo();

	glPopMatrix();
}

void drawBullet(int x, int y)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void drawObstacles()
{

	for (int i = 0; i < 4; ++i)
	{

		if (obstacleTypeAndDirection[i][0])
		{
			glColor3f(1.0, 1.0, 0.5);
			glBegin(GL_QUADS);
			glVertex2f(obstacleX[i], obstacleY[i]);
			glVertex2f(obstacleX[i] + 35, obstacleY[i] + 5);
			glVertex2f(obstacleX[i] + 35, obstacleY[i] - 35);
			glVertex2f(obstacleX[i], obstacleY[i] - 35);
		}
		else
		{
			glColor3f(0, 0, 0);
			glBegin(GL_QUADS);
			glVertex2f(obstacleX[i], obstacleY[i]);
			glVertex2f(obstacleX[i] + 35, obstacleY[i]);
			glVertex2f(obstacleX[i] + 35, obstacleY[i] - 35);
			glVertex2f(obstacleX[i], obstacleY[i] - 35);

			glEnd();

			float numEscala = 35.0;

			glColor3f(0.5, 0.3, 0.0);
			glBegin(GL_POLYGON);

			const int numVertices = 100;

			for (int j = 0; j < numVertices; ++j)
			{
				float theta = 2.0f * 3.1415926f * float(rand()) / float(numVertices);
				float x = obstacleX[i] + 18 + numEscala * 0.5 * cos(theta);
				float y = obstacleY[i] - 18 + numEscala * 0.5 * sin(theta);
				glVertex2f(x, y);
			}
		}

		glEnd();
	}
}

void drawText(char ch[], int xpos, int ypos) // draw score/message on screen
{
	int numofchar = strlen(ch);
	glLoadIdentity();
	glRasterPos2f(xpos, ypos);
	for (gameOverI = 0; gameOverI < numofchar; gameOverI++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch[gameOverI]);
	}
}

bool checkColissionBulletAndObstacle(
	float bulletX, float bulletY,
	float obstacleX, float obstacleY)
{
	// Verifica se há colisão nos eixos X e Y
	bool collisionX = (bulletX + 3 >= obstacleX && bulletX <= obstacleX + 35);
	bool collisionY = (bulletY >= obstacleY - 35 && bulletY - 3 <= obstacleY);

	// Retorna verdadeiro se houver colisão em ambos os eixos
	return collisionX && collisionY;
}

bool checkCollisionSpaceAndObstacle(
	float spaceshipX, float spaceshipY,
	float obstacleX, float obstacleY)
{
	// Verifica se há colisão nos eixos X e Y
	bool collisionX = (spaceshipX + 35 >= obstacleX && spaceshipX <= obstacleX + 35);
	bool collisionY = (spaceshipY >= obstacleY - 35 && spaceshipY - 35 <= obstacleY);

	// Retorna verdadeiro se houver colisão em ambos os eixos
	return collisionX && collisionY;
}

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawSpaceship();

	// Draw all bullets
	for (int b = 0; b < 3; b++)
	{
		drawBullet(bullets[b][0], bullets[b][1]);
	}

	if (currentBulletIndex == 3)
	{
		currentBulletIndex = 0;
	}
	drawObstacles();
	glutSwapBuffers();
}

void update(int value)
{

	if (!gameOver)
	{

		// Move bullets
		for (int b = 0; b < 3; b++)
		{

			if (bullets[b][0] > 0)
			{
				if (bullets[b][2] == 90)
					bullets[b][1] += 7;

				else if (bullets[b][2] == 270)
					bullets[b][1] -= 7;

				else if (bullets[b][2] == 180)
					bullets[b][0] -= 7;
				else
					bullets[b][0] += 7;

				// Check for bullet collision with obstacles
				for (int i = 0; i < 4; ++i)
				{

					if (checkColissionBulletAndObstacle(
							bullets[b][0], bullets[b][1],
							obstacleX[i], obstacleY[i]))
					{
						// Collision detected

						if (obstacleTypeAndDirection[i][1])
						{
							obstacleX[i] = -100;
						}
						else
						{
							obstacleX[i] = 600;
						}
						bullets[b][0] = -100;
						bullets[b][1] = -100; // Move bullet out of screen
						obstacleY[i] = rand() % 450;
					}
				}
			}
			if (bullets[b][1] > 500 || bullets[b][0] == -1)
			{
				bullets[b][0] = -100;
				bullets[b][1] = -100;
			}
		}

		// Move obstacles
		for (int i = 0; i < 4; ++i)
		{

			if (obstacleTypeAndDirection[i][1])
				obstacleX[i] += 3;
			else
				obstacleX[i] -= 3;

			if (obstacleX[i] < -100)
			{
				obstacleX[i] = 600;
				obstacleY[i] = rand() % 450;
			}
			else if (obstacleX[i] > 600)
			{
				obstacleX[i] = 0;
				obstacleY[i] = rand() % 450;
			}

			if (checkCollisionSpaceAndObstacle(spaceshipX, spaceshipY, obstacleX[i], obstacleY[i]))
			{
				gameOver = true;
			}
		}
		glutPostRedisplay();
		glutTimerFunc(16, update, 0);
	}
	else
	{
		glColor3f(1, 1, 1);
		drawText("Game Over, para reinicar o jogo aperte 'ESPACO'", 50, 250);
		glutSwapBuffers();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (gameOver)
	{
		if (key == ' ')
		{
			gameOver = false;
			reset();
			glutTimerFunc(16, update, 0);
			return;
		}
		else
		{
			return;
		}
	}
	switch (key)
	{
	case ' ':

		if (rotationAngle == 90)
		{
			bullets[currentBulletIndex][0] = spaceshipX + 20;
			bullets[currentBulletIndex][1] = spaceshipY;
		}
		else if (rotationAngle == 270)
		{
			bullets[currentBulletIndex][0] = spaceshipX + 20;
			bullets[currentBulletIndex][1] = spaceshipY - 35;
		}
		else if (rotationAngle == 180)
		{
			bullets[currentBulletIndex][0] = spaceshipX;
			bullets[currentBulletIndex][1] = spaceshipY - 20;
		}
		else
		{
			bullets[currentBulletIndex][0] = spaceshipX + 35;
			bullets[currentBulletIndex][1] = spaceshipY - 20;
		}

		bullets[currentBulletIndex][2] = rotationAngle;
		currentBulletIndex++;
		break;
	case '1':
		if (rotationAngle == 270)
			rotationAngle = 0;
		else
			rotationAngle += 90;
		break;
	case '2':
		rotationAngle = 0;
		break;

	case 27: // ESC key
		exit(0);
		break;
	}
}

void specialKeys(int key, int x, int y)
{
	if (gameOver)
		return;
	switch (key)
	{
	case GLUT_KEY_UP:
		spaceshipY += 30;
		break;
	case GLUT_KEY_DOWN:
		spaceshipY -= 30;
		break;
	case GLUT_KEY_LEFT:
		spaceshipX -= 30;
		break;
	case GLUT_KEY_RIGHT:
		spaceshipX += 30;
		break;
	}

	// Wrap spaceship movement around the screen
	if (spaceshipX > 500)
	{
		spaceshipX = 500;
	}
	else if (spaceshipX < 20)
	{
		spaceshipX = 20;
	}

	if (spaceshipY > 500)
	{
		spaceshipY = 490;
	}
	else if (spaceshipY < 0)
	{
		spaceshipY = 10;
	}

	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 500, 0, 500);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Space Shot Game");

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutReshapeFunc(reshape);
	glutTimerFunc(16, update, 0);

	glutMainLoop();
	return 0;
}
