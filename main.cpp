#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <math.h>
#include <string.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <math.h>

const int visibleFrameWidth = 560; // Slightly smaller than the window
const int visibleFrameHeight = 560; // Slightly smaller than the window
const int frameWidth = 600;
const int frameHeight = 600;
float progressBarX = 10.0f; // X position of the progress bar
float progressBarY = frameHeight - 30.0f; // Y position of the progress bar
float progressBarWidth = 200.0f; // Width of the progress bar
float progressBarHeight = 20.0f; // Height of the progress bar

std::time_t lastCollisionTime = std::time(nullptr);
const int collisionCooldown = 1;  // Define a cooldown time (in seconds)
const int powerUp1EffectDuration = 5; // Duration of the power-up effect in seconds
std::time_t powerUp1ActivationTime = 0;
int maxLife = 5; // Define the maximum life
int life = maxLife; // Initialize the life variable
const int numCircleSegments = 100;
const GLfloat circleRadius = 20.0f;
GLfloat x = 300.0f; // Initial X position within the frame
GLfloat y = 300.0f; // Initial Y position within the frame
GLfloat moveSpeed = 10.0f; // Define the movement speed
GLfloat rotationAngle = 0.0f; // Initialize the rotation angle
int healthLife = 5;
bool isPowerUpVisible = true;    // Indicates whether power-ups are currently visible
std::time_t powerUpTimer;
const float goalWidth = 60.0f;
const float goalHeight = 30.0f;
const float goalX = (frameWidth - visibleFrameWidth) / 2 + 10;
const float goalY = (frameHeight - visibleFrameHeight) / 2 + 10;
long long int gameTimeInSeconds = 3600; // 1 hour in seconds
std::time_t gameStartTime;
bool gameOver = false;
int gameTime = 60; // Initial game time in seconds
time_t startTime;
//progressbar
void drawProgressBar(float x, float y, float width, float height, float value, float maxValue) {
    float barWidth = (width * value) / maxValue;
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + barWidth, y);
    glVertex2f(x + barWidth, y + height);
    glVertex2f(x, y + height);
    glEnd();
}


// Structure for the first power-up
struct PowerUp1 {
    float x;
    float y;
    bool taken;
};

// Structure for the second power-up
struct PowerUp2 {
    float x;
    float y;
    bool taken;
};

PowerUp1 powerUp1;
PowerUp2 powerUp2;


//collectibles
struct Collectible {
    float x;
    float y;
    bool taken;
};

// background circles
GLfloat backgroundRotationAngle = 0.0f;
GLfloat backgroundRotationSpeed = 0.1f; // You can adjust the speed
GLfloat backgroundCircleRadius1 = 280.0f;
GLfloat backgroundCircleRadius2 = 240.0f;



// Frame dimensions

// Define obstacle structures
struct Obstacle {
    float x;
    float y;
    float width;
    float height;
    float rotationAngle;
};

Obstacle obstacles[5]; // Array to store 5 obstacles
Collectible collectibles[5]; //array of collectibles


void generateStaticObstacles() {
    // Set the positions, widths, heights, and rotation angles for the obstacles
    obstacles[0].x = progressBarX + progressBarWidth + 50.0f;
    obstacles[0].y = progressBarY-70;
    obstacles[0].width = 40.0f;
    obstacles[0].height = 40.0f;
    obstacles[0].rotationAngle = 0.0f;
    
    
    obstacles[1].x = 160.0f;
    obstacles[1].y = 160.0f;
    obstacles[1].width = 50.0f;
    obstacles[1].height = 45.0f;
    obstacles[1].rotationAngle = 0.0f;

    obstacles[2].x = 230.0f;
    obstacles[2].y =230.0f;
    obstacles[2].width = 50.0f;
    obstacles[2].height = 45.0f;
    obstacles[2].rotationAngle = 0.0f;

    obstacles[3].x = 400.0f;
    obstacles[3].y = 400.0f;
    obstacles[3].width = 50.0f;
    obstacles[3].height = 45.0f;
    obstacles[3].rotationAngle = 0.0f;

    obstacles[4].x =500.0f;
    obstacles[4].y = 500.0f;
    obstacles[4].width = 50.0f;
    obstacles[4].height = 45.0f;
    obstacles[4].rotationAngle = 0.0f;

    srand(time(NULL));

    for (int i = 0; i < 5; i++) {
        collectibles[i].x = static_cast<float>(rand() % (visibleFrameWidth - 40) + 20);
        collectibles[i].y = static_cast<float>(rand() % (visibleFrameHeight - 40) + 20);
        collectibles[i].taken = false;
    }

    //PowerUps
    powerUp1.x = static_cast<float>(rand() % (visibleFrameWidth - 40) + 20);
    powerUp1.y = static_cast<float>(rand() % (visibleFrameHeight - 40) + 20);
    powerUp1.taken = false;

    powerUp2.x = static_cast<float>(rand() % (visibleFrameWidth - 40) + 20);
    powerUp2.y = static_cast<float>(rand() % (visibleFrameHeight - 40) + 20);
    powerUp2.taken = false;


}

bool checkCollision() {
    std::time_t currentTime = std::time(nullptr);

    // Check if enough time has passed since the last collision
    if (currentTime - lastCollisionTime < collisionCooldown) {
        return false;  // Collision cooldown is still active
    }

    for (int i = 0; i < 5; i++) {
        if (x >= obstacles[i].x && x <= obstacles[i].x + obstacles[i].width &&
            y >= obstacles[i].y && y <= obstacles[i].y + obstacles[i].height) {
            life--; // Decrease life when a collision is detected
            lastCollisionTime = currentTime;  // Update the last collision time
            std::cout << "Collision Detected! Life: " << life << std::endl;

            if (life <= 0) {
                gameOver = true; // Set the game over flag
            }

            return true; // Collision detected
        }
    }

    return false; // No collision
}




// Function to draw a rectangle
void drawRectangle(float x, float y, float width, float height) {

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}
//draw goal
void drawGoal() {
     // Blue color for the goal
    float centerX = (frameWidth - visibleFrameWidth) / 2 + 10;
    float centerY = (frameHeight - visibleFrameHeight) / 2 + 10;

    for (int i = 0; i <= 6; i++) {
        glColor3f(0.0f, 0.0f, 1.0f);
        // Adjust the size and position based on the current iteration
        float width = 60 - i * 10;
        float height = 30 - i * 4;
        float x = centerX - i * 5;
        float y = centerY - i * 2;
        

        drawRectangle(x+15, y, width, height);
    }
    
}


// Function to draw a line
void drawLine(float x0, float y0, float x1, float y1) {
    glBegin(GL_LINES);
    glVertex2f(x0, y0);
    glVertex2f(x1, y1);
    glEnd();
}

void drawCircle(float cx, float cy, float r, int numSegments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSegments; ++i) {
        float theta = 2.0f * 3.14159f * float(i) / float(numSegments);
        float x = r * cos(theta);
        float y = r * sin(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

// Function to draw static obstacles
void drawStaticObstacles() {
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glTranslatef(obstacles[i].x, obstacles[i].y, 0);
        
        glColor3f(0.0f, 0.0f, 0.0f); // Black color
        drawRectangle(-obstacles[i].width / 2, -obstacles[i].height / 2, obstacles[i].width, obstacles[i].height);

        glColor3f(1.0f, 0.0f, 0.0f); // Red color
        drawLine(-obstacles[i].width / 2, -obstacles[i].height / 2, obstacles[i].width / 2, obstacles[i].height / 2);
        drawLine(obstacles[i].width / 2, -obstacles[i].height / 2, -obstacles[i].width / 2, obstacles[i].height / 2);

        glPopMatrix();
    }

    glutSwapBuffers();
    glutPostRedisplay();
}


void print(int x, int y, char* string) {
    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

void drawPerson() {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(rotationAngle, 0, 0, 1);

    glColor3f(1.4f, 0.0f, 0.0f);
    drawCircle(0, 40.0f, circleRadius, numCircleSegments);

    //Body
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-20.0f, -20.0f);
    glVertex2f(20.0f, -20.0f);
    glVertex2f(0.0f, 20.0f);
    glEnd();
    //legs
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-10.0f, -20.0f);
    glVertex2f(-10.0f, -40.0f);

    glVertex2f(10.0f, -20.0f);
    glVertex2f(10.0f, -40.0f);
    glEnd();
    //armssss
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-5.0f, 5.0f);
    glVertex2f(-30.0f, 0.0f);

    glVertex2f(5.0f, 5.0f);
    glVertex2f(30.0f, 0.0f);
    glEnd();
    //faceee
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f);

    glVertex2f(-8.0f, 45.0f); // Left eye
    glVertex2f(8.0f, 45.0f);  // Right eye
    glVertex2f(0, 38.0f);     // Nose

    glEnd();
    glPointSize(1.0f);

    glPopMatrix();
}


void drawFrame() {
    // Set the frame color
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glLineWidth(2.0f); // Set line width for the frame

    // Draw the original frame
    glBegin(GL_LINE_LOOP);
    glVertex2f((frameWidth - visibleFrameWidth) / 2, (frameHeight - visibleFrameHeight) / 2);
    glVertex2f(frameWidth - (frameWidth - visibleFrameWidth) / 2, (frameHeight - visibleFrameHeight) / 2);
    glVertex2f(frameWidth - (frameWidth - visibleFrameWidth) / 2, frameHeight - (frameHeight - visibleFrameHeight) / 2);
    glVertex2f((frameWidth - visibleFrameWidth) / 2, frameHeight - (frameHeight - visibleFrameHeight) / 2);
    glEnd();

    // Draw the outer frame
    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 10);
    glVertex2f(frameWidth - 10, 10);
    glVertex2f(frameWidth - 10, frameHeight - 10);
    glVertex2f(10, frameHeight - 10);
    glEnd();

    glLineWidth(1.0f); // Reset line width to default
}

int collectibleCounter = 0; // Initialize the counter

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    std::time_t currentTime = std::time(nullptr);
    float rotationSpeed = 45.0f;  // Adjust as needed
 

    //life variable
    char lifeText[20];
    // sprintf(lifeText, "Life: %d", life);
    print(10, frameHeight - 30, lifeText);
    // Draw the rotating background figure
    glColor3f(0.04f, 0.5f, 0.06f);
    glPushMatrix();
    glTranslatef(frameWidth / 2, frameHeight / 2, 0);
    glRotatef(backgroundRotationAngle, 0, 0, 1);

    // Draw the first circle
    drawCircle(0, 0, backgroundCircleRadius1, numCircleSegments);

    // Draw the second circle
    drawCircle(0, 0, backgroundCircleRadius2, numCircleSegments);

    // Connect the circles with straight lines
    glBegin(GL_LINES);
    glVertex2f(0, backgroundCircleRadius1);
    glVertex2f(0, backgroundCircleRadius2);
    glVertex2f(0, -backgroundCircleRadius1);
    glVertex2f(0, -backgroundCircleRadius2);
    glVertex2f(backgroundCircleRadius1, 0);
    glVertex2f(backgroundCircleRadius2, 0);
    glVertex2f(-backgroundCircleRadius1, 0);
    glVertex2f(-backgroundCircleRadius2, 0);
    glEnd();

    glPopMatrix();


    // Draw the progress bar for life
    float progressBarX = 10.0f; // X position of the progress bar
    float progressBarY = frameHeight - 30.0f; // Y position of the progress bar
    float progressBarWidth = 200.0f; // Width of the progress bar
    float progressBarHeight = 20.0f; // Height of the progress bar

    drawProgressBar(progressBarX, progressBarY, progressBarWidth, progressBarHeight, life, maxLife);

    // Draw Power-Up 1
    if ( !powerUp1.taken) {
   
        float centerX1 = powerUp1.x;
        float centerY1 = powerUp1.y;
        float radius1 = 15.0f; // Adjust the size as needed

        for (int i = 0; i < 4; i++) {
            float angle1 = 2 * 3.14 * i / 4;
            float angle2 = 2 * 3.14 * (i + 1) / 4;

            float x0 = centerX1 + radius1 * cos(angle1);
            float y0 = centerY1 + radius1 * sin(angle1);

            float x1 = centerX1 + radius1 * cos(angle2);
            float y1 = centerY1 + radius1 * sin(angle2);

            float x2 = centerX1;
            float y2 = centerY1;

            // Define colors or customize as needed
            glColor3f(0.6f, 0.4f, 0.2f);  // brown color
            glBegin(GL_TRIANGLES);
            glVertex2f(x0, y0);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glEnd();

            // Adjust the radius for the next primitive
            radius1 -= 3.0f;  // Customize the difference in radius
        }
        glPopMatrix();
    }

    if (!powerUp2.taken) {
         if (isPowerUpVisible && currentTime - powerUpTimer > 10) {
             isPowerUpVisible = false;
             powerUpTimer = currentTime;
         } else if (!isPowerUpVisible && currentTime - powerUpTimer > 30) {
             isPowerUpVisible = true;
             powerUpTimer = currentTime;
         }
        float centerX2 = powerUp2.x;
        float centerY2 = powerUp2.y;
        float width = 25.0f;  // Width of the rectangles
        float height = 10.0f; // Height of the rectangles

        for (int i = 0; i < 4; i++) {
            float angle = 3.14 / 4 * i;

            float x0 = centerX2 + width * cos(angle);
            float y0 = centerY2 + height * sin(angle);

            float x1 = centerX2 + width * cos(angle + 3.14 / 4);
            float y1 = centerY2 + height * sin(angle + 3.14 / 4);

            // Define colors or customize as needed
            glColor3f(1.0f, 0.0f, 1.0f);  // purple color
            glBegin(GL_QUADS);
            glVertex2f(x0, y0);
            glVertex2f(x1, y0);
            glVertex2f(x1, y1);
            glVertex2f(x0, y1);
            glEnd();
        }
        glPopMatrix();
    }


    // Function to draw static obstacles


    // Draw the collectibles (static, not rotating)
    for (int i = 0; i < 5; i++) {
        if (!collectibles[i].taken) {
            // Calculate the individual rotation angle for each collectible
            float individualRotationAngle = (static_cast<float>(currentTime % 360) * rotationSpeed * i / 5);

            glPushMatrix();
            glTranslatef(collectibles[i].x, collectibles[i].y, 0);
            glRotatef(individualRotationAngle, 0, 0, 1);

            GLfloat cx = 0.0f;  // Center X, as it's already accounted for in the translation
            GLfloat cy = 0.0f;  // Center Y, as it's already accounted for in the translation
            float radius = 20.0f;  // Radius of the outermost triangle

            for (int j = 0; j < 3; j++) {
                float angle1 = 2 * 3.14 * j / 3;
                float angle2 = 2 * 3.14 * (j + 1) / 3;

                // Calculate vertices of the triangles
                float x0 = cx + radius * cos(angle1);
                float y0 = cy + radius * sin(angle1);

                float x1 = cx + radius * cos(angle2);
                float y1 = cy + radius * sin(angle2);

                float x2 = cx;
                float y2 = cy;

                // Define colors (you can customize the colors)
                GLfloat colors[3][3] = {
                    {0.0f, 0.0f, 1.0f},  // Blue
                    {1.0f, 0.0f, 0.0f},  // Red
                    {0.0f, 1.0f, 0.0f}   // Green
                };

                // Draw the triangles
                glColor3fv(colors[j]);
                glBegin(GL_TRIANGLES);
                glVertex2f(x0, y0);
                glVertex2f(x1, y1);
                glVertex2f(x2, y2);
                glEnd();

                // Adjust the radius for the next inner triangle
                radius -= 3.0f;  // Customize the difference in radius
            }
            glPopMatrix();
        }
    }


    // Continue with the rest of your drawing code

    drawFrame(); // Draw the frame
    drawPerson();
    drawStaticObstacles();
    drawGoal(); // Call the drawGoal function to draw the goal

    char p0s[20];
    // sprintf(p0s, "Score: %d", 50);
    print(500, frameHeight - 30, p0s);
    char p1s[20];
    glColor3f(0.0f, 0.f, 0.0f);
    sprintf(p1s, "Time: %d mins", 60);
    print(300, frameHeight - 30, p1s);

    //if (checkCollision()) {
  //      healthLife--;
  //      std::cout << "Collision Detected! Game Over." << std::endl;
  //  }
    
    // Check for collectible collisions
    for (int i = 0; i < 5; i++) {
        if (!collectibles[i].taken &&
            x > collectibles[i].x - 35 && x < collectibles[i].x + 35 &&
            y > collectibles[i].y - 35 && y < collectibles[i].y + 35) {
            collectibles[i].taken = true;
            collectibleCounter++; // Increment the counter
        }
    }
    if (!powerUp1.taken &&
        x > powerUp1.x - 35 && x < powerUp1.x + 35 &&
        y > powerUp1.y - 35 && y < powerUp1.y + 35) {
        powerUp1.taken = true;

    }
    if (!powerUp2.taken &&
        x > powerUp2.x - 35 && x < powerUp2.x + 35 &&
        y > powerUp2.y - 35 && y < powerUp2.y + 35) {
        powerUp2.taken = true;

    }

    float scoreX = progressBarX + progressBarWidth + 10.0f;
    float scoreY = progressBarY;
    char collectibleCountText[50];
    glColor3f(0.0f, 0.f, 0.0f);
    sprintf(collectibleCountText, "Score: %d", collectibleCounter);
    print(scoreX, scoreY, collectibleCountText);



    if (gameOver) {
        char gameOverText[] = "Game Over!";
        print(frameWidth / 2 - 50, frameHeight / 2, gameOverText);

        // Close the game when it's over
        exit(0); // Terminate the program
    }

    glFlush();
    glutPostRedisplay();

    // Update the rotation angle for the background figure
    backgroundRotationAngle += backgroundRotationSpeed;
}


void SpecialKeys(int key, int xx, int yy) {
    GLfloat newX = x;
    GLfloat newY = y;
    if (key == GLUT_KEY_UP) {
        if (newY < frameHeight - moveSpeed) {
            
            newY += moveSpeed;  // Move the person up
        }
        rotationAngle = 0; // Reset rotation to face up
    }
    if (key == GLUT_KEY_DOWN) {
        if (newY > (frameHeight - visibleFrameHeight) / 2 - moveSpeed) {
            
            newY -= moveSpeed;  // Move the person down
        }
        rotationAngle = 180; // Reset rotation to face down
    }
    if (key == GLUT_KEY_RIGHT) {
        if (newX < (frameWidth - moveSpeed)) {
            
            newX += moveSpeed;  // Move the person right
        }
        rotationAngle = 270; // Reset rotation to face right
    }
    if (key == GLUT_KEY_LEFT) {
        if (newX > (frameWidth - visibleFrameWidth) / 2 - moveSpeed) {
            
            newX -= moveSpeed;  // Move the person left
        }
        rotationAngle = 90; // Reset rotation to face left
    }
    
    // Check for collision with obstacles
    bool collision = false;
    for (int i = 0; i < 5; i++) {
        if (newX + 15 > obstacles[i].x && newX - 15 < obstacles[i].x + obstacles[i].width &&
            newY + 15 > obstacles[i].y && newY - 15 < obstacles[i].y + obstacles[i].height) {
            collision = true;
            break; // Exit the loop when a collision is detected
        }
    }

    if (!collision) {
        
        x = newX;
        y = newY;
        collision = false;
    }else{
        std::time_t currentTime = std::time(nullptr);

        // Check if enough time has passed since the last collision
        if (currentTime - lastCollisionTime < collisionCooldown) {
              // Collision cooldown is still active
        }
        else {
            life--;
            std::cout << "Collision Detected! Game Over." << std::endl;
            lastCollisionTime = currentTime;  // Update the last collision time
            collision = false;
        }
        }
    if (life <= 0) {
        gameOver = true; // Set the game over flag
    }
    int currentWindow = 0; // Initialize the current window ID

    if (newX + 15 <= 100 && newY <= 50) {
           std::cout << "GOALLLL!!!" << std::endl;
           
           glutSetWindow(currentWindow); // Set the current window back to the initial window
           glutDestroyWindow(currentWindow); // Close the initial window

           glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
           glutInitWindowSize(300, 300);
           glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 300) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - 300) / 2);
           currentWindow = glutCreateWindow("You Won"); // Save the ID of the new window

           // Set up the orthographic projection for the new window
           glMatrixMode(GL_PROJECTION);
           glLoadIdentity();
           gluOrtho2D(0, 300, 0, 300);

           glMatrixMode(GL_MODELVIEW);
           glLoadIdentity();

           // Clear the window
           glClearColor(1.0, 1.0, 1.0, 0);
           glClear(GL_COLOR_BUFFER_BIT);

           // Display the "You Won" message, score, and time
           glColor3f(0.0f, 0.0f, 0.0f);

           char message[100];
           sprintf(message, "You Won!");

           glRasterPos2f(50, 150);  // Adjust the position as needed
           int len = (int)strlen(message);
           for (int i = 0; i < len; i++) {
               glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
           }

           glutPostRedisplay(); // Trigger a display callback
           glutMainLoop(); // Start the GLUT main loop for the new window
       }

        
    
    
    
    
    std::cout << x << std::endl;
    std::cout << y << std::endl;

    if (x < (frameWidth - visibleFrameWidth) / 2 || x > frameWidth - (frameWidth - visibleFrameWidth) / 2 ||
        y < (frameHeight - visibleFrameHeight) / 2 || y > frameHeight - (frameHeight - visibleFrameHeight) / 2) {
        std::time_t currentTime = std::time(nullptr);

        // Check if enough time has passed since the last boundary collision
        if (currentTime - lastCollisionTime < collisionCooldown) {
            return; // Collision cooldown is still active
        }

        life--; // Decrease life when hitting the frame boundaries
        lastCollisionTime = currentTime;  // Update the last collision time
        std::cout << "Person reached the frame boundary! Life: " << life << std::endl;

        if (life <= 0) {
            gameOver = true; // Set the game over flag
        }

        glutPostRedisplay();
    }
}




        




int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800); // Set the window size to 800x800
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 800) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - 800) / 2); // Center the window
    glutCreateWindow("Game");
    glutDisplayFunc(display);
    glutSpecialFunc(SpecialKeys);
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, frameWidth, 0, frameHeight); // Set the orthographic projection to match the frame dimensions

    generateStaticObstacles(); // Generate static obstacles
    gameStartTime = std::time(nullptr);

    glutMainLoop();
    return 0;
}
