#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize  = 20;
int cellCount = 25;
int offset = 75;

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
    bool addSegment = false;

    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){
            int x = body[i].x;
            int y = body[i].y;
            Rectangle segment = Rectangle{(float)offset + x * cellSize, (float)offset + y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, darkGreen);

        }
    }

    void Update(){
        
        body.push_front(Vector2Add(body[0], direction));
        if(addSegment){
            addSegment = false;

        }else{
            body.pop_back(); 
        }
    }

    void Reset(){
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1,0};
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
        DrawRectangle(offset + position.x * cellSize, offset + position.y * cellSize, cellSize, cellSize, darkGreen);
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
    bool running = true;
    int score = 0;

    void Draw(){
        snake.Draw();
        food.Draw();
    }

    void Update(){
        if(running){
            snake.Update();
            checkCollisionWithFood();
            checkCollisionWithEdge();
            checkCollisionWithTail();
        }
        
    }

    void checkCollisionWithFood(){
        if(Vector2Equals(snake.body[0], food.position)){
            food.position = food.generateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
        }
    }

    void checkCollisionWithEdge(){
        if(snake.body[0].x == cellCount || snake.body[0].x == -1){
            GameOver();
        }

        if(snake.body[0].y == cellCount || snake.body[0].y == -1){
            GameOver();
        }
    }

    void GameOver(){
        snake.Reset();
        food.position = food.generateRandomPos(snake.body);
        running = false;
        score = 0;
    }

    void checkCollisionWithTail(){
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if(elementInDeque(snake.body[0], headlessBody)){
            GameOver();
        }
    }
};

int main () {

    cout << "Strating the game" << endl;
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Retro Snake");
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
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0,1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1,0};
            game.running = true;
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1,0};
            game.running = true;
        }

        
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, darkGreen);
        DrawText("retro snake", offset - 20, 20, 40, darkGreen);
        DrawText(TextFormat("%i", game.score), offset -5, offset + cellSize*cellCount+10, 40, darkGreen);
        game.Draw();

        EndDrawing();
    }

    
    CloseWindow();
    return 0;
}