#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string.h>
#include <cstdlib>

using namespace std;

struct Obstaculo
{
    int obstaculoX;
    int obstaculoY;
    int tipo;
    int direcao;
} obstaculos[8];


struct Bala
{
    int balaX = -100;
    int balaY = -100;
    int direcao;
} balas[3];


int spaceshipX = 250, spaceshipY = 250;
int anguloRotacao = 0.0f;
int balaAtualIndex = 0;
bool gameOver = false, gameWin = false;
int tempo = 0, pontuacao = 0, velocidadeAsteroides = 1;
bool velocidadesJaAdicionadas[5] = {true, true, true, true, true};
char mensagemBuffer[10];
int spaceShitHitBox[5][2];

void reiniciaObstaculo(int index)
{

    if (obstaculos[index].direcao == 0)
    {
        obstaculos[index].obstaculoX = rand() % 750 + 450;
        obstaculos[index].obstaculoY = rand() % 450 + 100;
    }
    else if (obstaculos[index].direcao == 1)
    {
        obstaculos[index].obstaculoX = -(rand() % 1 + 100);
        obstaculos[index].obstaculoY = rand() % 450 + 100;
    }
    else if (obstaculos[index].direcao == 2)
    {
        obstaculos[index].obstaculoY = rand() % 500 + 450;
        obstaculos[index].obstaculoX = rand() % 450 + 100;
    }
    else
    {
        obstaculos[index].obstaculoY = -(rand() % 1 + 100);
        obstaculos[index].obstaculoX = rand() % 450 + 100;
    }
}

void inicializaObstaculos()
{
    obstaculos[0].direcao = 0;
    obstaculos[1].direcao = 1;
    obstaculos[2].direcao = 1;
    obstaculos[3].direcao = 0;
    obstaculos[4].direcao = 2;
    obstaculos[5].direcao = 3;
    obstaculos[6].direcao = 2;
    obstaculos[7].direcao = 3;

    for (int i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
        {
            obstaculos[i].tipo = 1;
        }
        else
        {
            obstaculos[i].tipo = 0;
        }
        reiniciaObstaculo(i);
    }
}

void reset()
{
    inicializaObstaculos();

    spaceshipX = 250;
    spaceshipY = 250;
    anguloRotacao = 0;
    balaAtualIndex = 0;
    tempo = 0;
    pontuacao = 0;

    gameOver = false;
    gameWin = false;

    for(int i = 0;i<5;i++)
    {
        velocidadesJaAdicionadas[i] = false;
    }

    for(int i = 0;i<3;i++)
    {
        balas[i].balaX = -100;
        balas[i].balaY = -100;
    }
}

void rotacionarNavePeloAngulo()
{
    glColor3f(0.0, 0.0, 1.0);
    glPointSize(5.0);

    if (anguloRotacao == 0)
    {
        glBegin(GL_POLYGON);
        glVertex2f(spaceshipX, spaceshipY);
        glVertex2f(spaceshipX, spaceshipY - 25);
        glVertex2f(spaceshipX + 15, spaceshipY - 25);
        glVertex2f(spaceshipX + 25, spaceshipY - 15);
        glVertex2f(spaceshipX + 15, spaceshipY);
    }
    else if (anguloRotacao == 90)
    {
        glBegin(GL_POLYGON);
        glVertex2f(spaceshipX, spaceshipY - 25);
        glVertex2f(spaceshipX + 25, spaceshipY - 25);
        glVertex2f(spaceshipX + 25, spaceshipY - 15);
        glVertex2f(spaceshipX + 15, spaceshipY);
        glVertex2f(spaceshipX, spaceshipY - 15);
    }
    else if (anguloRotacao == 180)
    {
        glBegin(GL_POLYGON);
        glVertex2f(spaceshipX + 25, spaceshipY);
        glVertex2f(spaceshipX + 25, spaceshipY - 25);

        glVertex2f(spaceshipX + 15, spaceshipY - 25);
        glVertex2f(spaceshipX, spaceshipY - 15);

        glVertex2f(spaceshipX + 15, spaceshipY);
    }
    else
    {
        glBegin(GL_POLYGON);
        glVertex2f(spaceshipX, spaceshipY);
        glVertex2f(spaceshipX + 25, spaceshipY);
        glVertex2f(spaceshipX + 25, spaceshipY - 15);
        glVertex2f(spaceshipX + 15, spaceshipY - 25);
        glVertex2f(spaceshipX, spaceshipY - 15);
    }

    glEnd();
}

void desenhaHitBoxRotacionada()
{
    if (anguloRotacao == 0)
    {

        spaceShitHitBox[0][0] = spaceshipX + 3;
        spaceShitHitBox[0][1] = spaceshipY - 3;

        spaceShitHitBox[1][0] = spaceshipX + 12;
        spaceShitHitBox[1][1] = spaceshipY - 3;

        spaceShitHitBox[2][0] = spaceshipX + 12;
        spaceShitHitBox[2][1] = spaceshipY - 22;

        spaceShitHitBox[3][0] = spaceshipX + 3;
        spaceShitHitBox[3][1] = spaceshipY - 22;

        spaceShitHitBox[4][0] = spaceshipX + 22;
        spaceShitHitBox[4][1] = spaceshipY - 12;
    }
    else if (anguloRotacao == 90)
    {
        spaceShitHitBox[0][0] = spaceshipX + 3;
        spaceShitHitBox[0][1] = spaceshipY - 12;

        spaceShitHitBox[1][0] = spaceshipX + 22;
        spaceShitHitBox[1][1] = spaceshipY - 12;

        spaceShitHitBox[2][0] = spaceshipX + 22;
        spaceShitHitBox[2][1] = spaceshipY - 22;

        spaceShitHitBox[3][0] = spaceshipX + 3;
        spaceShitHitBox[3][1] = spaceshipY - 22;

        spaceShitHitBox[4][0] = spaceshipX + 12;
        spaceShitHitBox[4][1] = spaceshipY - 3;
    }
    else if (anguloRotacao == 180)
    {
        spaceShitHitBox[0][0] = spaceshipX + 12;
        spaceShitHitBox[0][1] = spaceshipY - 3;

        spaceShitHitBox[1][0] = spaceshipX + 22;
        spaceShitHitBox[1][1] = spaceshipY - 3;

        spaceShitHitBox[2][0] = spaceshipX + 22;
        spaceShitHitBox[2][1] = spaceshipY - 22;

        spaceShitHitBox[3][0] = spaceshipX + 12;
        spaceShitHitBox[3][1] = spaceshipY - 22;

        spaceShitHitBox[4][0] = spaceshipX + 3;
        spaceShitHitBox[4][1] = spaceshipY - 12;
    }
    else
    {
        spaceShitHitBox[0][0] = spaceshipX + 3;
        spaceShitHitBox[0][1] = spaceshipY - 3;

        spaceShitHitBox[1][0] = spaceshipX + 22;
        spaceShitHitBox[1][1] = spaceshipY - 3;

        spaceShitHitBox[2][0] = spaceshipX + 22;
        spaceShitHitBox[2][1] = spaceshipY - 12;

        spaceShitHitBox[3][0] = spaceshipX + 3;
        spaceShitHitBox[3][1] = spaceshipY - 12;

        spaceShitHitBox[4][0] = spaceshipX + 12;
        spaceShitHitBox[4][1] = spaceshipY - 22;
    }

}
void drawSpaceship()
{
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();

    rotacionarNavePeloAngulo();
    desenhaHitBoxRotacionada();

    glPopMatrix();
}

void drawBullet(Bala bala)
{
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex2f(bala.balaX, bala.balaY);
    glEnd();
}

void drawObstacles()
{

    for (int i = 0; i < 8; ++i)
    {

        if (obstaculos[i].tipo)
        {
            glColor3f(1.0, 1.0, 0.5);
            glBegin(GL_QUADS);
            glVertex2f(obstaculos[i].obstaculoX, obstaculos[i].obstaculoY);
            glVertex2f(obstaculos[i].obstaculoX + 25, obstaculos[i].obstaculoY + 5);
            glVertex2f(obstaculos[i].obstaculoX + 25, obstaculos[i].obstaculoY - 25);
            glVertex2f(obstaculos[i].obstaculoX, obstaculos[i].obstaculoY - 25);
        }
        else
        {
            glColor3f(0, 0, 0);
            float numEscala = 25.0;

            glColor3f(0.5, 0.3, 0.0);
            glBegin(GL_POLYGON);

            const int numVertices = 100;

            for (int j = 0; j < numVertices; ++j)
            {
                float theta = 2.0f * 3.1415926f * float(rand()) / float(numVertices);
                float x = obstaculos[i].obstaculoX + 12 + numEscala * 0.5 * cos(theta);
                float y = obstaculos[i].obstaculoY - 12 + numEscala * 0.5 * sin(theta);
                glVertex2f(x, y);
            }
        }

        glEnd();
    }
}

void drawText(char ch[], int xpos, int ypos)
{
    int numofchar = strlen(ch);
    glLoadIdentity();
    glRasterPos2f(xpos, ypos);
    for (int i = 0; i < numofchar; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch[i]);
    }
}

bool verificarColissaoEntrePontoEAsteroide(
    float bulletX, float bulletY,
    float asteroideX, float asteroideY)
{

    // Verifica se há colisão nos eixos X e Y
    bool collisionX = (bulletX >= asteroideX && bulletX <= asteroideX + 25);

    bool collisionY = (bulletY >= asteroideY - 25 && bulletY <= asteroideY);

    // Retorna verdadeiro se houver colisão em ambos os eixos
    return collisionX && collisionY;
}

void desenhaCena()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawSpaceship();

    // Desenha todas as balas
    for (int b = 0; b < 3; b++)
    {
        drawBullet(balas[b]);
    }

    if (balaAtualIndex == 3)
    {
        balaAtualIndex = 0;
    }
    drawObstacles();

    // Display the pontuacao
    glColor3f(1, 1, 1);
    sprintf(mensagemBuffer, "Pontuacao: %d", pontuacao);
    drawText(mensagemBuffer, 10, 470);

    // Display the time
    sprintf(mensagemBuffer, "Tempo: %d", tempo / 60);
    drawText(mensagemBuffer, 130, 470);

    glutSwapBuffers();
}

void moveBalas()
{

    // Move as balas
    for (int b = 0; b < 3; b++)
    {

        if (balas[b].balaX > 0)
        {
            if (balas[b].direcao == 90)
                balas[b].balaY += 7;

            else if (balas[b].direcao == 270)
                balas[b].balaY -= 7;

            else if (balas[b].direcao == 180)
                balas[b].balaX -= 7;
            else
                balas[b].balaX += 7;

            // Verifica se alguma bala colidiu com algum obstaculo
            for (int i = 0; i < 8; ++i)
            {

                if (verificarColissaoEntrePontoEAsteroide(
                        balas[b].balaX, balas[b].balaY,
                        obstaculos[i].obstaculoX, obstaculos[i].obstaculoY))
                {
                    reiniciaObstaculo(i);
                    balas[b].balaX = -100;
                    balas[b].balaY = -100; // remove a bala da tela.
                    pontuacao += 1;
                }
            }
        }
        if (balas[b].balaX < -100 ||
            balas[b].balaX > 600 ||
            balas[b].balaY > 600 ||
            balas[b].balaY < 0)
        {
            balas[b].balaX = -100;
            balas[b].balaY = -100;
        }
    }
}

void atualizaVelocidadeObstaculos()
{
    // Pega o tempo atual considerando que estamos atualizando a pagina 60x por segundo
    int tempoAtual = (tempo / 60);

    // Verificamos aqui se é o tempo ideal para atualizar a velocidade dos obstaculos
    if (
        (tempoAtual == 30 || tempoAtual == 60 || tempoAtual == 90 || tempoAtual == 120 || tempoAtual == 150))
    {
        // Aqui pegamos qual valor atual do tempo (30, 60, 90) dividindo o tempo por 30:
        int currentTemp = (tempoAtual / 30); // Assim conseguimos selecionar no array a posição atual

        // Verificamos no array de controle de velocidade se o tempo atual ainda não foi considerado
        if (velocidadesJaAdicionadas[currentTemp])
        {
            // Caso ainda não tenha sido, adicionamos na variavel de velocidade o tempo atual
            // Dessa forma, a cada 30s adicionamos +1 de velocidade
            velocidadeAsteroides += 1;
            velocidadesJaAdicionadas[currentTemp] = false;
        }
    }
}

void moveObstaculos()
{
    // Move os obstaculos
    for (int i = 0; i < 8; ++i)
    {
        if (obstaculos[i].direcao == 1)
            obstaculos[i].obstaculoX += velocidadeAsteroides;
        else if (obstaculos[i].direcao == 0)
            obstaculos[i].obstaculoX -= velocidadeAsteroides;
        else if (obstaculos[i].direcao == 2)
            obstaculos[i].obstaculoY -= velocidadeAsteroides;
        else if (obstaculos[i].direcao == 3)
            obstaculos[i].obstaculoY += velocidadeAsteroides;

        // Verifica se algum dos obstaculos saiu da tela
        if (obstaculos[i].obstaculoX < -100 ||
            obstaculos[i].obstaculoX > 600 ||
            obstaculos[i].obstaculoY > 600 ||
            obstaculos[i].obstaculoY < 0)
        {
            reiniciaObstaculo(i);
        }

        for (int count = 0; count < 5; count++)
        {
            if (verificarColissaoEntrePontoEAsteroide(
                    spaceShitHitBox[count][0], spaceShitHitBox[count][1],
                    obstaculos[i].obstaculoX, obstaculos[i].obstaculoY))
            {
                gameOver = true;
            }
        }
    }
}

void atualiza(int value)
{

    if ((tempo / 60) > 180)
    {
        gameWin = true;
        glColor3f(0, 1, 0);
        drawText("Você ganhou! Click 'ESPACO' para reiniciar", 50, 250);
        glutSwapBuffers();
    }
    else if (!gameOver)
    {

        tempo++;
        moveBalas();
        atualizaVelocidadeObstaculos();
        moveObstaculos();
        glutPostRedisplay();
        glutTimerFunc(16, atualiza, 0);
    }
    else
    {
        glColor3f(1, 0, 0);
        drawText("Game Over, click 'ESPACO' para reinicar", 75, 250);
        glutSwapBuffers();
    }
}

void teclado(unsigned char key, int x, int y)
{
    if (gameOver || gameWin)
    {
        if (key == ' ')
        {
            gameOver = false;
            reset();
            glutTimerFunc(16, atualiza, 0);
        }

        return;
    }
    switch (key)
    {
    case ' ':

        if (anguloRotacao == 90)
        {
            balas[balaAtualIndex].balaX = spaceshipX + 20;
            balas[balaAtualIndex].balaY = spaceshipY;
        }
        else if (anguloRotacao == 270)
        {
            balas[balaAtualIndex].balaX = spaceshipX + 20;
            balas[balaAtualIndex].balaY = spaceshipY - 25;
        }
        else if (anguloRotacao == 180)
        {
            balas[balaAtualIndex].balaX = spaceshipX;
            balas[balaAtualIndex].balaY = spaceshipY - 20;
        }
        else
        {
            balas[balaAtualIndex].balaX = spaceshipX + 25;
            balas[balaAtualIndex].balaY = spaceshipY - 20;
        }

        balas[balaAtualIndex].direcao = anguloRotacao;
        balaAtualIndex++;
        break;
    case '1':
        if (anguloRotacao == 270)
            anguloRotacao = 0;
        else
            anguloRotacao += 90;
        break;
    case '2':
        anguloRotacao = 0;
        break;

    case 27: // ESC key
        exit(0);
        break;
    }
}

void teclasEspeciais(int key, int x, int y)
{
    if (gameOver || gameWin)
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

    inicializaObstaculos();
    glutDisplayFunc(desenhaCena);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciais);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, atualiza, 0);

    glutMainLoop();
    return 0;
}