//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

char str[4];

// radius of ball in pixels
#define RADIUS 10

// Brick Dims
#define Brick_W 36
#define Brick_H 8

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // Paddle Y axis
    double y = .75*HEIGHT;
    
    double dx = drand48()*3 - 1.5;
    double dy = drand48()*-2;
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // check for mouse event
        GEvent mousy = getNextEvent(MOUSE_EVENT);
        
        // Mouse Movement
        if (mousy != NULL)
        {
            // if the event was movement
            if (getEventType(mousy) == MOUSE_MOVED)
            {
                // ensure circle follows top cursor
                double x = getX(mousy)-.5*Brick_W;
                setLocation(paddle, x, y);
            }
        }
        
        // Ball Movement
        move(ball, dx, dy);             
        
        // Rebounds
        // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            dx = -dx;
            dx -= .01;
        }

        // bounce off left edge of window
        else if (getX(ball) <= 0)
        {
            dx = -dx;
            dx += .01;
        }
        
        // bounce off top of window
        if (getY(ball) <= 0)
        {
            dy = -dy;
            dy += .01;
        }
        
        // lose when contact with gutter
        if (getY(ball) + getWidth(ball) >= getHeight(window))
        {
            lives -= 1;
            removeGWindow(window, ball);
            ball = initBall(window);
            dx = drand48()*3 - 1.5;
            dy = drand48()*-2;
            pause(500);
            
        }
        // Collision Detection
        GObject obj = detectCollision(window,ball);
        
        if (obj != NULL)
        {
            if (obj == paddle)
            {
                dy = -dy;
                dy -= .05;
            }
            
            else if (strcmp(getType(obj), "GRect") == 0)
            {
                removeGWindow(window, obj);
                dy = -dy;
                dy += .2;
                points +=1;
                bricks -= 1;
            }
         }
    sprintf(str,"%i",points);
    setLabel(label,str);
    add(window,label);     
    pause(10);    
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    for(int i = 0;i < COLS; i++)
    {
        for(int j = 0; j < ROWS; j++)
        {
            GRect brick = newGRect(i*Brick_W+4*i,.2*HEIGHT+(j*(Brick_H+4)),Brick_W, Brick_H);
            setFilled(brick, true);
            if(j==0)
                setColor(brick, "RED");
            else if(j==1)
                setColor(brick, "ORANGE");
            else if(j==2)
                setColor(brick, "YELLOW");  
            else if(j==3)
                setColor(brick, "GREEN");
            else
                setColor(brick, "BLUE");       
            add(window,brick);
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(WIDTH/2-.5*RADIUS,.75*HEIGHT-RADIUS,RADIUS, RADIUS);
    setFilled(ball, true);
    setColor(ball, "BLUE");
    add(window,ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(WIDTH/2-.5*(Brick_W*1.5),.75*HEIGHT,Brick_W*1.5, Brick_H);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
    add(window,paddle);
    return paddle;
    
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    setFont(label,"SansSerif-18");
    setLocation(label,WIDTH/2,.65*HEIGHT);
    return label;
    add(window,label);
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + RADIUS, y + RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
