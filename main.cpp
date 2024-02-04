#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize  = 30;
int cellCount = 25;

double lastUpdateTime = 0;

bool elementInDeque(Vector2 element, deque<Vector2> deque){
    for(int i = 0; i < deque.size(); i++){
        if(Vector2Equals(deque[i], element)){
            return true;
        }
    }

}

bool eventTriggered(double interval){
    double currTime = GetTime();
    if(currTime - lastUpdateTime >= interval){
        lastUpdateTime = currTime;
        return true;
    }
    return false;

}

class Snake{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1,0};

    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){
            int x = body[i].x;
            int y = body[i].y;
            Rectangle segment = Rectangle{(float)x*cellSize, (float)y*cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, darkGreen);

        }
    }

    void Update(){
        body.pop_back();
        body.push_front(Vector2Add(body[0], direction));
    }
};

class Food{

public:
    Vector2 position;
    Texture2D texture ;

    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = generateRandomPos(snakeBody);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, darkGreen);
    }

    Vector2 generateRandomCell(){
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x,y};
    }

    Vector2 generateRandomPos(deque<Vector2> snakeBody){
        Vector2 position = generateRandomCell();
        while(elementInDeque(position, snakeBody)){
            position = generateRandomCell();
        }
        return position;
    }
};

class Game{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);

    void Draw(){
        snake.Draw();
        food.Draw();
    }

    void Update(){
        snake.Update();
        checkCollisionWithFood();
    }

    void checkCollisionWithFood(){
        if(Vector2Equals(snake.body[0], food.position)){
            food.position = food.generateRandomPos(snake.body);
        }
    }
};

int main () {

    cout << "Strating the game" << endl;
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);
    Game game = Game();

    while(WindowShouldClose() == false){
        BeginDrawing();

        if(eventTriggered(0.2)){
            game.Update();
        }
        //arrow key movement
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0,-1};
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0,1};
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1,0};
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1,0};
        }

        
        ClearBackground(green);
        game.Draw();

        EndDrawing();
    }

    
    CloseWindow();
    return 0;
}