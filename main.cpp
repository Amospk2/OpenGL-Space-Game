#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

int spaceshipX = 250, spaceshipY = 250;
int bullets[3][2];
int currentBulletIndex = 0;
bool gameOver = false;
static int rotationAngle = 0.0f;

int obstacleX[6] = {500, 600, 500, 750};
int obstacleY[6] = {100, 250, 400, 100};

void drawSpaceship()
{
    glColor3f(1.0, 1.0, 1.0); 
    glPushMatrix();         

    glTranslatef(spaceshipX, spaceshipY, 0.0);
    glRotatef((GLfloat)rotationAngle, 0.0, 0.0, 1.0);
    glTranslatef(-spaceshipX, -spaceshipY, 0.0);

	glBegin(GL_QUADS);
	glVertex2f(spaceshipX + 12, spaceshipY - 12);
	glVertex2f(spaceshipX - 12, spaceshipY - 12);
	glVertex2f(spaceshipX - 12, spaceshipY + 12);
	glVertex2f(spaceshipX + 12, spaceshipY + 12);
    glEnd();

	glColor3f(1.0, 0.0, 0.0); 
	glBegin(GL_TRIANGLES);
    glVertex2f(spaceshipX + 10, spaceshipY);
    glVertex2f(spaceshipX - 25, spaceshipY + 15);
    glVertex2f(spaceshipX - 25, spaceshipY - 15);
    glEnd();
    
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
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < 4; ++i)
    {
        glBegin(GL_QUADS);
        glVertex2f(obstacleX[i] + 10, obstacleY[i] - 20);
        glVertex2f(obstacleX[i] - 10, obstacleY[i] - 20);
        glVertex2f(obstacleX[i] - 10, obstacleY[i] + 20);
        glVertex2f(obstacleX[i] + 10, obstacleY[i] + 20);
        glEnd();
    }
}


bool checkCollision(float pointX, float pointY, float obstacleX, float obstacleY) {
    // Assuming obstacleX and obstacleY represent the center of the rectangle
    float leftBound = obstacleX - 10;
    float rightBound = obstacleX + 10;
    float upperBound = obstacleY + 20;
    float lowerBound = obstacleY - 20;

    // Check if the point is within the rectangle
    if (pointX >= leftBound && pointX <= rightBound && pointY >= lowerBound && pointY <= upperBound) {
        return true; // Collision detected
    }

    return false; // No collision
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
	
	if(!gameOver)
    {
		
		// Move bullets
    for (int b = 0; b < 3; b++)
    {

        if (bullets[b][0] > 0)
        {
            bullets[b][0] += 5;

            // Check for bullet collision with obstacles
            for (int i = 0; i < 4; ++i)
            {

                if (checkCollision(bullets[b][0], bullets[b][1], obstacleX[i], obstacleY[i]))
                {
                    // Collision detected
                    bullets[b][0] = -100;
                    bullets[b][1] = -100; // Move bullet out of screen
                    obstacleX[i] = 700;
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
        obstacleX[i] -= 3;
        if (obstacleX[i] < 0)
        {
           obstacleX[i] = 700;
           obstacleY[i] = rand() % 450;
        }


        if (checkCollision( spaceshipX, spaceshipY, obstacleX[i], obstacleY[i]))
        {
            gameOver = true;
        }
    }

	}
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	if(gameOver) return;
    switch (key)
    {
    case ' ':
        bullets[currentBulletIndex][0] = spaceshipX + 10;
        bullets[currentBulletIndex][1] = spaceshipY;
        currentBulletIndex++;
        break;
    case '1':
        rotationAngle = (rotationAngle - 10) % 360;
        break;
    case '2':
        rotationAngle = (rotationAngle + 10) % 360;
        break;
    case '3':
        rotationAngle = 0;
        break;
    case 27: // ESC key
        exit(0);
        break;
    }
}

void specialKeys(int key, int x, int y)
{
	if(gameOver) return;
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

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glutMainLoop();
    return 0;
}