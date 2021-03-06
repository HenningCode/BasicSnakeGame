

#include <stdio.h>
#include <stdlib.h>  
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

typedef struct{
    int x;
    int y;
}food_struct;

typedef struct{
    int x;
    int y;
    int tail_length;
    int xtail[100];
    int ytail[100];
    enum {RIGHT,LEFT,UP,DOWN} direction;
}snake_struct;


//global game variables
int cols;
int rows;
int score;
bool game_over;
snake_struct snake;
food_struct food;



void setup(){
    // Init Curses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    clear();

    //Init Gamevariables
    cols = 40;
    rows = 20;
    game_over = false;

    //Init Snake
    snake.x = cols/2;
    snake.y = rows/2;
    snake.tail_length = 0;
    snake.direction = RIGHT;

    //First Food position
    srand(time(0));
    food.x = rand() % (cols-1)+1;
    food.y = rand() % (rows-1)+1;
}

void input(){
    //Slow down the game
    usleep(180000);
    int c = getch();
    switch(c){
        case KEY_RIGHT:
            if(snake.direction != LEFT)
                snake.direction = RIGHT;
            break;
        case KEY_LEFT:
            if(snake.direction != RIGHT)
                snake.direction = LEFT;
            break;
        case KEY_UP:
            if(snake.direction != DOWN)
                snake.direction = UP;
            break;
        case KEY_DOWN:
            if(snake.direction != UP)
                snake.direction = DOWN;
            break;
    }
}

void logic(){

    //Shift Tail
    for(int i = snake.tail_length-1; i >= 0; i--){
        snake.xtail[i+1] = snake.xtail[i];
        snake.ytail[i+1] = snake.ytail[i];
    }

    //Old Snakeposition is the tail 
    snake.xtail[0] = snake.x;
    snake.ytail[0] = snake.y;

    //Calculate new Snake Position
    switch(snake.direction){
        case RIGHT:
            snake.x++;
            break;
        case LEFT:
            snake.x--;
            break;
        case UP:
            snake.y--;
            break;
        case DOWN:
            snake.y++;
            break;
        
    }

    //Hit a border
    if(snake.x < 1 || snake.x > cols-2 || snake.y < 1 || snake.y > rows-2){
        game_over = true;
    }

    //Check if tail is hit
    for(int i = 0; i < snake.tail_length; i++){
        if(snake.x == snake.xtail[i] && snake.y == snake.ytail[i])
            game_over = true;
    }
    

    // Food Eaten
    if (snake.x == food.x && snake.y == food.y){
        //Update Score and Tail_length
        score += 10;
        snake.tail_length++;

        //New Food
        food.x = rand() % (cols-1)+1;
        food.y = rand() % (rows-1)+1;        
    }
    
   
    
}

void draw(){

    //Draw Snake
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(i == 0 || i == rows-1 || j == 0 || j == cols-1)
                mvaddch(i,j,'#');
            else if(i == snake.y && j == snake.x){
                mvaddch(i,j,'X');
            }else if(i == food.y && j == food.x){
                mvaddch(i,j,'F');
            }else 
                mvaddch(i,j,' ');
        }
    }

    //Draw Tail
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            for(int k = 0; k < snake.tail_length; k++){
                if(snake.xtail[k] == j && snake.ytail[k] == i)
                    mvaddch(i,j,'o');
            }
        }
    }
    //Output score
    mvprintw(rows,0,"Score %d",score);
    refresh();
}

int main(){

    setup();

    while(!game_over){
        input();
        logic();
        draw();
    }

    return 0;
}
