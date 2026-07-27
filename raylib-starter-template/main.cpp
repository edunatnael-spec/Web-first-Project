#include <raylib.h>
#include <cstdlib>
using namespace std;

enum Block {BarBlock, BoxBlock, TBlock, LBlock, JBlock, ZBlock, SBlock};
enum Orientation {Up, Right, Down, Left};

const int BOARD_WIDTH = 300;
const int BOARD_HEIGHT = 600;
const int ROWS = 20;
const int COLS = 10;
const int CELL_WIDTH = BOARD_WIDTH / COLS;
const int CELL_HEIGHT = BOARD_HEIGHT / ROWS;
const int INFO_AREA_WIDTH = 250;
const Color WINDOW_BG_COLOR = WHITE;
const Color GRID_LINE_COLOR = LIGHTGRAY;
const Color LOCKED_CELL_COLOR = DARKGRAY;

struct ActiveBlock {
    Block block;
    Orientation orientation;
    int x;
    int y;
    Color color;
};

int cellInfo[ROWS][COLS];
int score = 0;
ActiveBlock nextBlock;
bool gameOver = false;

void init();
void initCells();
void prepareNextBlock();
void spawnBlock(ActiveBlock &activeBlock);
int findMiddle(Block block, Orientation orientation);
Block chooseRandomBlock();
Orientation chooseRandomOrientation();
void drawGrid();
void playGame(ActiveBlock &activeBlock);
void drawActiveBlock(ActiveBlock activeBlock);
void drawBarBlock(ActiveBlock activeBlock);
void drawBoxBlock(ActiveBlock activeBlock);
void drawTBlock(ActiveBlock activeBlock);
void drawLBlock(ActiveBlock activeBlock);
void drawJBlock(ActiveBlock activeBlock);
void drawZBlock(ActiveBlock activeBlock);
void drawSBlock(ActiveBlock activeBlock);
bool canBlockGoDown(ActiveBlock activeBlock);
bool canBarGoDown(ActiveBlock activeBlock);
bool canBoxGoDown(ActiveBlock activeBlock);
bool canTGoDown(ActiveBlock activeBlock);
bool canLGoDown(ActiveBlock activeBlock);
bool canJGoDown(ActiveBlock activeBlock);
bool canZGoDown(ActiveBlock activeBlock);
bool canSGoDown(ActiveBlock activeBlock);
void lockActiveBlock(ActiveBlock activeBlock);
void lockBarBlock(ActiveBlock activeBlock);
void lockBoxBlock(ActiveBlock activeBlock);
void lockTBlock(ActiveBlock activeBlock);
void lockLBlock(ActiveBlock activeBlock);
void lockJBlock(ActiveBlock activeBlock);
void lockZBlock(ActiveBlock activeBlock);
void lockSBlock(ActiveBlock activeBlock);
void drawLockedCells();
bool canBlockGoLeft(ActiveBlock activeBlock);
bool canBarGoLeft(ActiveBlock activeBlock);
bool canBoxGoLeft(ActiveBlock activeBlock);
bool canTGoLeft(ActiveBlock activeBlock);
bool canLGoLeft(ActiveBlock activeBlock);
bool canJGoLeft(ActiveBlock activeBlock);
bool canZGoLeft(ActiveBlock activeBlock);
bool canSGoLeft(ActiveBlock activeBlock);
bool canBlockGoRight(ActiveBlock activeBlock);
bool canBarGoRight(ActiveBlock activeBlock);
bool canBoxGoRight(ActiveBlock activeBlock);
bool canTGoRight(ActiveBlock activeBlock);
bool canLGoRight(ActiveBlock activeBlock);
bool canJGoRight(ActiveBlock activeBlock);
bool canZGoRight(ActiveBlock activeBlock);
bool canSGoRight(ActiveBlock activeBlock);
bool canBlockRotate(ActiveBlock activeBlock);
bool canBarRotate(ActiveBlock activeBlock);
bool canBoxRotate(ActiveBlock activeBlock);
bool canTRotate(ActiveBlock activeBlock);
bool canLRotate(ActiveBlock activeBlock);
bool canJRotate(ActiveBlock activeBlock);
bool canZRotate(ActiveBlock activeBlock);
bool canSRotate(ActiveBlock activeBlock);
void rotateBlock(ActiveBlock &activeBlock);
void clearFullLines();
void drawInfoPanel();
bool isGameOver();
void drawGameOverScreen();

int main(){
    init();
    initCells();
    prepareNextBlock();
    ActiveBlock activeBlock;
    spawnBlock(activeBlock);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WINDOW_BG_COLOR);
        if(gameOver){
            drawLockedCells();
            drawGrid();
            drawInfoPanel();
            drawGameOverScreen();
            if(IsKeyPressed(KEY_R)){
                initCells();
                score = 0;
                gameOver = false;
                prepareNextBlock();
                spawnBlock(activeBlock);
            }
        } else {
            drawGrid();
            playGame(activeBlock);
            drawInfoPanel();
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void init(){
    InitWindow(BOARD_WIDTH + INFO_AREA_WIDTH, BOARD_HEIGHT, "TETRIS");
}

void initCells(){
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            cellInfo[row][col] = 0;
        }
    }
}

void prepareNextBlock(){
    nextBlock.block = chooseRandomBlock();
    nextBlock.orientation = chooseRandomOrientation();
    nextBlock.x = 0;
    nextBlock.y = 0;
    nextBlock.color = GetRandomValue(0,1) == 0 ? RED : MAGENTA;
}

void spawnBlock(ActiveBlock &activeBlock){
    activeBlock.block = nextBlock.block;
    activeBlock.orientation = nextBlock.orientation;
    activeBlock.x = findMiddle(activeBlock.block, activeBlock.orientation);
    activeBlock.y = 0;
    activeBlock.color = nextBlock.color;
    prepareNextBlock();
}

int findMiddle(Block block, Orientation orientation){
    switch (block){
        case BarBlock:
            if(orientation == Up || orientation == Down){
                return COLS/2;
            } else{
                return (COLS/2) - 2;
            }
            break;
        case BoxBlock:
        case TBlock:
        case LBlock:
        case JBlock:
        case ZBlock:
        case SBlock:
            return (COLS/2)-1;
        default:
            return COLS/2;
        }
}

Block chooseRandomBlock(){
    return Block(GetRandomValue(0,6));
}

Orientation chooseRandomOrientation(){
    return Up;
}

void drawGrid(){
    for(int row = 0; row <= ROWS; row++){
        DrawLine(0, row * CELL_HEIGHT, BOARD_WIDTH, row * CELL_HEIGHT, GRID_LINE_COLOR);
    }
    for(int col = 0; col <= COLS; col++){
        DrawLine(col * CELL_WIDTH, 0, col * CELL_WIDTH, BOARD_HEIGHT, GRID_LINE_COLOR);
    }
}

// Line Clearing 

void clearFullLines(){
    int linesCleared = 0;
    for(int row = ROWS - 1; row >= 0; row--){
        bool full = true;
        for(int col = 0; col < COLS; col++){
            if(cellInfo[row][col] == 0){ full = false; break; }
        }
        if(full){
            // Shift everything above down
            for(int r = row; r > 0; r--){
                for(int col = 0; col < COLS; col++){
                    cellInfo[r][col] = cellInfo[r-1][col];
                }
            }
            // Clear top row
            for(int col = 0; col < COLS; col++) cellInfo[0][col] = 0;
            linesCleared++;
            row++; // Re-check same row index after shift
        }
    }
    // Classic Tetris scoring
    if(linesCleared == 1) score += 100;
    else if(linesCleared == 2) score += 300;
    else if(linesCleared == 3) score += 500;
    else if(linesCleared == 4) score += 800;
}

//Info Panel 

void drawInfoPanel(){
    int panelX = BOARD_WIDTH + 10;

    // SCORE
    DrawText("SCORE", panelX, 20, 22, DARKGRAY);
    DrawRectangle(panelX, 48, INFO_AREA_WIDTH - 20, 40, LIGHTGRAY);
    DrawRectangleLines(panelX, 48, INFO_AREA_WIDTH - 20, 40, DARKGRAY);
    DrawText(TextFormat("%d", score), panelX + 8, 58, 20, BLACK);

    //  NEXT BLOCK
    DrawText("NEXT", panelX, 105, 22, DARKGRAY);

    ActiveBlock preview = nextBlock;
    preview.x = 13;   // centers in the info panel horizontally
    preview.y = 6;    // sits nicely below the "NEXT" label
    drawActiveBlock(preview);
}

void playGame(ActiveBlock &activeBlock){
    drawActiveBlock(activeBlock);
    drawLockedCells();
    float gameSpeed = 0.2;
    static float fallDelay = 0;
    fallDelay += GetFrameTime();
    if(fallDelay > gameSpeed){
        if(canBlockGoDown(activeBlock)){
            activeBlock.y++;
            fallDelay = 0;
        } else {
            lockActiveBlock(activeBlock);
            clearFullLines();
            spawnBlock(activeBlock);
            if(isGameOver()) gameOver = true;
        }
    }
    if(IsKeyPressed(KEY_LEFT)){
        if(canBlockGoLeft(activeBlock)) activeBlock.x--;
    }
    if(IsKeyPressed(KEY_RIGHT)){
        if(canBlockGoRight(activeBlock)) activeBlock.x++;
    }
    if(IsKeyPressed(KEY_UP)){
        if(canBlockRotate(activeBlock)){
            rotateBlock(activeBlock);
        }
    }
    if(IsKeyPressed(KEY_SPACE)){
        while(canBlockGoDown(activeBlock)){
            activeBlock.y++;
        }
        lockActiveBlock(activeBlock);
        clearFullLines();
        spawnBlock(activeBlock);
        fallDelay = 0;
        if(isGameOver()) gameOver = true;
    }
}

void drawActiveBlock(ActiveBlock activeBlock){
    switch(activeBlock.block){
        case BarBlock: drawBarBlock(activeBlock); break;
        case BoxBlock: drawBoxBlock(activeBlock); break;
        case TBlock:   drawTBlock(activeBlock);   break;
        case LBlock:   drawLBlock(activeBlock);   break;
        case JBlock:   drawJBlock(activeBlock);   break;
        case ZBlock:   drawZBlock(activeBlock);   break;
        case SBlock:   drawSBlock(activeBlock);   break;
    }
}

void drawBarBlock(ActiveBlock activeBlock){
    int barCells = 4;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        for(int y = 0; y < barCells; y++){
            DrawRectangle((activeBlock.x) * CELL_WIDTH, (activeBlock.y + y) * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, activeBlock.color);
        }
    } else {
        for(int x = 0; x < barCells; x++){
            DrawRectangle((activeBlock.x + x) * CELL_WIDTH, (activeBlock.y) * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, activeBlock.color);
        }
    }
}

void drawBoxBlock(ActiveBlock activeBlock){
    for(int y = 0; y < 2; y++){
        for(int x = 0; x < 2; x++){
            DrawRectangle((activeBlock.x + x) * CELL_WIDTH,((activeBlock.y + y) * CELL_HEIGHT),CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        }
    }
}

void drawTBlock(ActiveBlock activeBlock){
    switch(activeBlock.orientation){
        case Up:
            DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            for(int i=0;i<3;i++)
                DrawRectangle((activeBlock.x+i)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;
        case Right:
            for(int i=0;i<3;i++)
                DrawRectangle((activeBlock.x)*CELL_WIDTH,(activeBlock.y+i)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;
        case Down:
            for(int i=0;i<3;i++)
                DrawRectangle((activeBlock.x+i)*CELL_WIDTH,(activeBlock.y)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;
        case Left:
            DrawRectangle((activeBlock.x)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            for(int i=0;i<3;i++)
                DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y+i)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;
    }
}

void drawLBlock(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){

        case Up:
            for(int i=0;i<3;i++)
                DrawRectangle(x*CELL_WIDTH,(y+i)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            DrawRectangle((x+1)*CELL_WIDTH,(y+2)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;

        case Right:
            for(int i=0;i<3;i++)
                DrawRectangle((x+i)*CELL_WIDTH,y*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            DrawRectangle(x*CELL_WIDTH,(y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;

        case Down:
            for(int i=0;i<3;i++)
                DrawRectangle((x+1)*CELL_WIDTH,(y+i)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            DrawRectangle(x*CELL_WIDTH,y*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;

        case Left:
            for(int i=0;i<3;i++)
                DrawRectangle((x+i)*CELL_WIDTH,(y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            DrawRectangle((x+2)*CELL_WIDTH,y*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;
    }
}

void drawJBlock(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){

        case Up:
            for(int i=0;i<3;i++)
                DrawRectangle((x+1)*CELL_WIDTH,(y+i)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            DrawRectangle(x*CELL_WIDTH,(y+2)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;

        case Right:
            for(int i=0;i<3;i++)
                DrawRectangle((x+i)*CELL_WIDTH,(y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            DrawRectangle(x*CELL_WIDTH,y*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;

        case Down:
            for(int i=0;i<3;i++)
                DrawRectangle(x*CELL_WIDTH,(y+i)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            DrawRectangle((x+1)*CELL_WIDTH,y*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;

        case Left:
            for(int i=0;i<3;i++)
                DrawRectangle((x+i)*CELL_WIDTH,y*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            DrawRectangle((x+2)*CELL_WIDTH,(y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
            break;
    }
}

void drawZBlock(ActiveBlock activeBlock){
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        DrawRectangle((activeBlock.x)*CELL_WIDTH,(activeBlock.y)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x+2)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
    } else {
        DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x)*CELL_WIDTH,(activeBlock.y+2)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
    }
}

void drawSBlock(ActiveBlock activeBlock){
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x+2)*CELL_WIDTH,(activeBlock.y)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
    } else {
        DrawRectangle((activeBlock.x)*CELL_WIDTH,(activeBlock.y)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y+1)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        DrawRectangle((activeBlock.x+1)*CELL_WIDTH,(activeBlock.y+2)*CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
    }
}

//canBlockGoDown

bool canBlockGoDown(ActiveBlock activeBlock){
    switch (activeBlock.block){
        case BarBlock: return canBarGoDown(activeBlock);
        case BoxBlock: return canBoxGoDown(activeBlock);
        case TBlock:   return canTGoDown(activeBlock);
        case LBlock:   return canLGoDown(activeBlock);
        case JBlock:   return canJGoDown(activeBlock);
        case ZBlock:   return canZGoDown(activeBlock);
        case SBlock:   return canSGoDown(activeBlock);
        default:       return false;
    }
}

bool canBarGoDown(ActiveBlock activeBlock){
    int barCells = 4;
    if (activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.y + barCells >= ROWS) return false;
        if(cellInfo[activeBlock.y + barCells][activeBlock.x] == 1) return false;
    } else {
        if(activeBlock.y + 1 >= ROWS) return false;
        for(int x = 0; x < barCells; x++){
            if(cellInfo[activeBlock.y + 1][activeBlock.x + x] == 1) return false;
        }
    }
    return true;
}

bool canBoxGoDown(ActiveBlock activeBlock){
    if(activeBlock.y+2>=ROWS) return false;
    for(int x=0;x<2;x++){
        if(cellInfo[activeBlock.y+2][activeBlock.x+x]==1) return false;
    }
    return true;
}

bool canTGoDown(ActiveBlock activeBlock){
    switch(activeBlock.orientation){
        case Up:
            if(activeBlock.y+2>=ROWS) return false;
            for(int i=0;i<3;i++)
                if(cellInfo[activeBlock.y+2][activeBlock.x+i]==1) return false;
            break;
        case Right:
            if(activeBlock.y+3>=ROWS) return false;
            if(cellInfo[activeBlock.y+3][activeBlock.x]==1) return false;
            if(cellInfo[activeBlock.y+2][activeBlock.x+1]==1) return false;
            break;
        case Down:
            if(activeBlock.y+2>=ROWS) return false;
            if(cellInfo[activeBlock.y+1][activeBlock.x]==1) return false;
            if(cellInfo[activeBlock.y+2][activeBlock.x+1]==1) return false;
            if(cellInfo[activeBlock.y+1][activeBlock.x+2]==1) return false;
            break;
        case Left:
            if(activeBlock.y+3>=ROWS) return false;
            if(cellInfo[activeBlock.y+2][activeBlock.x]==1) return false;
            if(cellInfo[activeBlock.y+3][activeBlock.x+1]==1) return false;
            break;
    }
    return true;
}

bool canLGoDown(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){

        case Up:
            if(y + 3 >= ROWS) return false;
            if(cellInfo[y+3][x] == 1) return false;
            if(cellInfo[y+3][x+1] == 1) return false;
            break;

        case Right:
            if(y + 2 >= ROWS) return false;
            if(cellInfo[y+2][x] == 1) return false;
            for(int i=0;i<2;i++)
                if(cellInfo[y+1][x+i] == 1) return false;
            break;

        case Down:
            if(y + 3 >= ROWS) return false;
            if(cellInfo[y+1][x] == 1) return false;
            if(cellInfo[y+3][x+1] == 1) return false;
            break;

        case Left:
            if(y + 2 >= ROWS) return false;
            for(int i=0;i<3;i++)
                if(cellInfo[y+2][x+i] == 1) return false;
            break;
    }

    return true;
}

bool canJGoDown(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){

        case Up:
            if(y + 3 >= ROWS) return false;
            if(cellInfo[y+3][x] == 1) return false;
            if(cellInfo[y+3][x+1] == 1) return false;
            break;

        case Right:
            if(y + 2 >= ROWS) return false;
            for(int i=0;i<3;i++)
                if(cellInfo[y+2][x+i] == 1) return false;
            break;

        case Down:
            if(y + 3 >= ROWS) return false;
            if(cellInfo[y+3][x] == 1) return false;
            if(cellInfo[y+1][x+1] == 1) return false;
            break;

        case Left:
            if(y + 2 >= ROWS) return false;
            if(cellInfo[y+2][x+2] == 1) return false;
            for(int i=0;i<2;i++)
                if(cellInfo[y+1][x+i] == 1) return false;
            break;
    }

    return true;
}
bool canZGoDown(ActiveBlock activeBlock){
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.y + 2 >= ROWS) return false;
        if(cellInfo[activeBlock.y+1][activeBlock.x]==1)return false;
        if(cellInfo[activeBlock.y+2][activeBlock.x+1]==1)return false;
        if(cellInfo[activeBlock.y+2][activeBlock.x+2]==1)return false;
    }
    else{
        if(activeBlock.y + 3 >= ROWS) return false;
        if(cellInfo[activeBlock.y+3][activeBlock.x]==1)return false;
        if(cellInfo[activeBlock.y+2][activeBlock.x+1]==1)return false;
    }
    return true;
}

bool canSGoDown(ActiveBlock activeBlock){
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.y + 2 >= ROWS) return false;
        if(cellInfo[activeBlock.y + 2][activeBlock.x] == 1) return false;
        if(cellInfo[activeBlock.y + 2][activeBlock.x + 1] == 1) return false;
        if(cellInfo[activeBlock.y+1][activeBlock.x+2]==1)return false;
    }
    else{
        if(activeBlock.y + 3 >= ROWS) return false;
        if(cellInfo[activeBlock.y +2][activeBlock.x ] == 1) return false;
        if(cellInfo[activeBlock.y + 3][activeBlock.x + 1] == 1) return false;
    }
    return true;
}

//lockActiveBlock

void lockActiveBlock(ActiveBlock activeBlock){
    switch (activeBlock.block){
        case BarBlock: lockBarBlock(activeBlock); break;
        case BoxBlock: lockBoxBlock(activeBlock); break;
        case TBlock:   lockTBlock(activeBlock);   break;
        case LBlock:   lockLBlock(activeBlock);   break;
        case JBlock:   lockJBlock(activeBlock);   break;
        case ZBlock:   lockZBlock(activeBlock);   break;
        case SBlock:   lockSBlock(activeBlock);   break;
    }
}

void lockBarBlock(ActiveBlock activeBlock){
    int barCells = 4;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        for(int y = 0; y < barCells; y++){
            cellInfo[activeBlock.y + y][activeBlock.x] = 1;
        }
    } else {
        for(int x = 0; x < barCells; x++){
            cellInfo[activeBlock.y][activeBlock.x + x] = 1;
        }
    }
}

void lockBoxBlock(ActiveBlock activeBlock){
    for(int x = 0; x < 2; x++){
        for(int y = 0; y < 2; y++){
            cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
        }
    }
}

void lockTBlock(ActiveBlock activeBlock){
    switch(activeBlock.orientation){
        case Up:
            cellInfo[activeBlock.y][activeBlock.x+1]=1;
            for(int i=0;i<3;i++) cellInfo[activeBlock.y+1][activeBlock.x+i]=1;
            break;
        case Right:
            for(int i=0;i<3;i++) cellInfo[activeBlock.y+i][activeBlock.x]=1;
            cellInfo[activeBlock.y+1][activeBlock.x+1]=1;
            break;
        case Down:
            for(int i=0;i<3;i++) cellInfo[activeBlock.y][activeBlock.x+i]=1;
            cellInfo[activeBlock.y+1][activeBlock.x+1]=1;
            break;
        case Left:
            cellInfo[activeBlock.y+1][activeBlock.x]=1;
            for(int i=0;i<3;i++) cellInfo[activeBlock.y+i][activeBlock.x+1]=1;
            break;
    }
}

void lockLBlock(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){
        case Up:
            for(int i=0;i<3;i++) cellInfo[y+i][x]=1;       // vertical line
            cellInfo[y+2][x+1]=1;                          // bottom-right
            break;
        case Right:
            for(int i=0;i<3;i++) cellInfo[y][x+i]=1;       // top horizontal
            cellInfo[y+1][x]=1;                            // left vertical
            break;
        case Down:
            for(int i=0;i<3;i++) cellInfo[y+i][x+1]=1;     // vertical line
            cellInfo[y][x]=1;                              // top-left
            break;
        case Left:
            for(int i=0;i<3;i++) cellInfo[y+1][x+i]=1;     // bottom horizontal
            cellInfo[y][x+2]=1;                            // top-right
            break;
    }
}

void lockJBlock(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){
        case Up:
            for(int i=0;i<3;i++) cellInfo[y+i][x+1]=1;     // vertical line
            cellInfo[y+2][x]=1;                            // bottom-left
            break;
        case Right:
            for(int i=0;i<3;i++) cellInfo[y+1][x+i]=1;     // bottom horizontal
            cellInfo[y][x]=1;                              // top-left
            break;
        case Down:
            for(int i=0;i<3;i++) cellInfo[y+i][x]=1;       // vertical line
            cellInfo[y][x+1]=1;                            // top-middle
            break;
        case Left:
            for(int i=0;i<3;i++) cellInfo[y][x+i]=1;       // top horizontal
            cellInfo[y+1][x+2]=1;                          // bottom-right
            break;
    }
}

void lockZBlock(ActiveBlock activeBlock){
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        cellInfo[activeBlock.y][activeBlock.x] = 1;
        cellInfo[activeBlock.y][activeBlock.x+1] = 1;
        cellInfo[activeBlock.y+1][activeBlock.x+1] = 1;
        cellInfo[activeBlock.y+1][activeBlock.x+2] = 1;
    }
    else{
        cellInfo[activeBlock.y][activeBlock.x+1] = 1;
        cellInfo[activeBlock.y+1][activeBlock.x] = 1;
        cellInfo[activeBlock.y+1][activeBlock.x+1] = 1;
        cellInfo[activeBlock.y+2][activeBlock.x] = 1;
    }
}

void lockSBlock(ActiveBlock activeBlock){
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        cellInfo[activeBlock.y][activeBlock.x+1] = 1;
        cellInfo[activeBlock.y][activeBlock.x+2] = 1;
        cellInfo[activeBlock.y+1][activeBlock.x] = 1;
        cellInfo[activeBlock.y+1][activeBlock.x+1] = 1;
    }
    else{
        cellInfo[activeBlock.y][activeBlock.x] = 1;
        cellInfo[activeBlock.y+1][activeBlock.x] = 1;
        cellInfo[activeBlock.y+1][activeBlock.x+1] = 1;
        cellInfo[activeBlock.y+2][activeBlock.x+1] = 1;
    }
}

void drawLockedCells(){
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            if(cellInfo[row][col] == 1){
                DrawRectangle(col * CELL_WIDTH, row * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, LOCKED_CELL_COLOR);
            }
        }
    }
}

// canBlockGoLeft

bool canBlockGoLeft(ActiveBlock activeBlock){
    switch(activeBlock.block){
        case BarBlock: return canBarGoLeft(activeBlock);
        case BoxBlock: return canBoxGoLeft(activeBlock);
        case TBlock:   return canTGoLeft(activeBlock);
        case LBlock:   return canLGoLeft(activeBlock);
        case JBlock:   return canJGoLeft(activeBlock);
        case ZBlock:   return canZGoLeft(activeBlock);
        case SBlock:   return canSGoLeft(activeBlock);
        default:       return false;
    }
}

bool canBarGoLeft(ActiveBlock activeBlock){
    int barCells = 4;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.x - 1 < 0) return false;
        for(int y = 0; y < barCells; y++){
            if(cellInfo[activeBlock.y + y][activeBlock.x - 1] == 1) return false;
        }
    } else {
        if(activeBlock.x - 1 < 0) return false;
        if(cellInfo[activeBlock.y][activeBlock.x - 1] == 1) return false;
    }
    return true;
}

bool canBoxGoLeft(ActiveBlock activeBlock){
    if(activeBlock.x - 1 < 0) return false;
    for(int y = 0; y < 2; y++){
        if(cellInfo[activeBlock.y + y][activeBlock.x - 1] == 1) return false;
    }
    return true;
}

bool canTGoLeft(ActiveBlock activeBlock){
    switch(activeBlock.orientation){
        case Up:
            if(activeBlock.x - 1 < 0) return false;
            if(cellInfo[activeBlock.y][activeBlock.x]==1) return false;
            if(cellInfo[activeBlock.y+1][activeBlock.x-1]==1) return false;
            break;
        case Right:
            if(activeBlock.x - 1 < 0) return false;
            for(int i=0;i<3;i++)
                if(cellInfo[activeBlock.y+i][activeBlock.x-1]==1) return false;
            break;
        case Down:
            if(activeBlock.x - 1 < 0) return false;
            if(cellInfo[activeBlock.y][activeBlock.x-1]==1) return false;
            if(cellInfo[activeBlock.y+1][activeBlock.x]==1) return false;
            break;
        case Left:
            if(activeBlock.x - 1 < 0) return false;
            if(cellInfo[activeBlock.y+1][activeBlock.x-1]==1) return false;
            if(cellInfo[activeBlock.y][activeBlock.x+1]==1) return false;
            if(cellInfo[activeBlock.y+2][activeBlock.x+1]==1) return false;
            break;
    }
    return true;
}

bool canLGoLeft(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){

        case Up:
            if(x - 1 < 0) return false;
            for(int i=0;i<3;i++)
                if(cellInfo[y+i][x-1] == 1) return false;
            break;

        case Right:
            if(x - 1 < 0) return false;
            if(cellInfo[y][x-1] == 1) return false;
            if(cellInfo[y+1][x-1] == 1) return false;
            break;

        case Down:
            if(x - 1 < 0) return false;
            if(cellInfo[y][x-1] == 1) return false;
            break;

        case Left:
            if(x - 1 < 0) return false;
            if(cellInfo[y+1][x-1] == 1) return false;
            break;
    }

    return true;
}

bool canJGoLeft(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){

        case Up:
            if(x - 1 < 0) return false;
            if(cellInfo[y+2][x-1] == 1) return false;
            break;

        case Right:
            if(x - 1 < 0) return false;
            for(int i=0;i<2;i++)
                if(cellInfo[y+i][x-1] == 1) return false;
            break;

        case Down:
            if(x - 1 < 0) return false;
            for(int i=0;i<3;i++)
                if(cellInfo[y+i][x-1] == 1) return false;
            break;

        case Left:
            if(x - 1 < 0) return false;
            if(cellInfo[y][x-1] == 1) return false;
            break;
    }

    return true;
}
bool canZGoLeft(ActiveBlock activeBlock){
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.x - 1 < 0) return false;
        if(cellInfo[activeBlock.y][activeBlock.x-1]==1) return false;
        if(cellInfo[activeBlock.y+1][activeBlock.x]==1) return false;
    } else {
        if(activeBlock.x - 1 < 0) return false;
        if(cellInfo[activeBlock.y][activeBlock.x]==1) return false;
        if(cellInfo[activeBlock.y+1][activeBlock.x-1]==1) return false;
        if(cellInfo[activeBlock.y+2][activeBlock.x-1]==1) return false;
    }
    return true;
}

bool canSGoLeft(ActiveBlock activeBlock){
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.x - 1 < 0) return false;
        if(cellInfo[activeBlock.y][activeBlock.x]==1) return false;
        if(cellInfo[activeBlock.y+1][activeBlock.x-1]==1) return false;
    } else {
        if(activeBlock.x - 1 < 0) return false;
        if(cellInfo[activeBlock.y][activeBlock.x-1]==1) return false;
        if(cellInfo[activeBlock.y+1][activeBlock.x-1]==1) return false;
        if(cellInfo[activeBlock.y+2][activeBlock.x]==1) return false;
    }
    return true;
}

//canBlockGoRight

bool canBlockGoRight(ActiveBlock activeBlock){
    switch(activeBlock.block){
        case BarBlock: return canBarGoRight(activeBlock);
        case BoxBlock: return canBoxGoRight(activeBlock);
        case TBlock:   return canTGoRight(activeBlock);
        case LBlock:   return canLGoRight(activeBlock);
        case JBlock:   return canJGoRight(activeBlock);
        case ZBlock:   return canZGoRight(activeBlock);
        case SBlock:   return canSGoRight(activeBlock);
        default:       return false;
    }
}

bool canBarGoRight(ActiveBlock activeBlock){
    int barCells = 4;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.x + 1 >= COLS) return false;
        for(int y = 0; y < barCells; y++){
            if(cellInfo[activeBlock.y + y][activeBlock.x + 1] == 1) return false;
        }
    } else {
        if(activeBlock.x + barCells >= COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + barCells] == 1) return false;
    }
    return true;
}

bool canBoxGoRight(ActiveBlock activeBlock){
    int boxSize = 2;
    if(activeBlock.x + boxSize >= COLS) return false;
    for(int y = 0; y < boxSize; y++){
        if(cellInfo[activeBlock.y + y][activeBlock.x + boxSize] == 1) return false;
    }
    return true;
}

bool canTGoRight(ActiveBlock activeBlock){
    switch(activeBlock.orientation){
        case Up:
            if(activeBlock.x + 4 > COLS) return false;
            if(cellInfo[activeBlock.y][activeBlock.x+2]==1) return false;
            if(cellInfo[activeBlock.y+1][activeBlock.x+3]==1) return false;
            break;
        case Right:
            if(activeBlock.x + 3 > COLS) return false;
            if(cellInfo[activeBlock.y][activeBlock.x+1]==1) return false;
            if(cellInfo[activeBlock.y+1][activeBlock.x+2]==1) return false;
            if(cellInfo[activeBlock.y+2][activeBlock.x+1]==1) return false;
            break;
        case Down:
            if(activeBlock.x + 4 > COLS) return false;
            if(cellInfo[activeBlock.y][activeBlock.x+3]==1) return false;
            if(cellInfo[activeBlock.y+1][activeBlock.x+2]==1) return false;
            break;
        case Left:
            if(activeBlock.x + 3 > COLS) return false;
            for(int i=0;i<3;i++)
                if(cellInfo[activeBlock.y+i][activeBlock.x+2]==1) return false;
            break;
    }
    return true;
}

bool canLGoRight(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){

        case Up:
            if(x + 2 >= COLS) return false;
            if(cellInfo[y+2][x+2] == 1) return false;
            break;

        case Right:
            if(x + 3 >= COLS) return false;
            for(int i=0;i<2;i++)
                if(cellInfo[y+i][x+3] == 1) return false;
            break;

        case Down:
            if(x + 2 >= COLS) return false;
            for(int i=0;i<3;i++)
                if(cellInfo[y+i][x+2] == 1) return false;
            break;

        case Left:
            if(x + 3 >= COLS) return false;
            if(cellInfo[y][x+3] == 1) return false;
            if(cellInfo[y+1][x+3] == 1) return false;
            break;
    }

    return true;
}

bool canJGoRight(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){

        case Up:
            if(x + 2 >= COLS) return false;
            for(int i=0;i<3;i++)
                if(cellInfo[y+i][x+2] == 1) return false;
            break;

        case Right:
            if(x + 3 >= COLS) return false;
            if(cellInfo[y+1][x+3] == 1) return false;
            break;

        case Down:
            if(x + 2 >= COLS) return false;
            if(cellInfo[y][x+2] == 1) return false;
            break;

        case Left:
            if(x + 3 >= COLS) return false;
            if(cellInfo[y][x+3] == 1) return false;
            if(cellInfo[y+1][x+3] == 1) return false;
            break;
    }

    return true;
}

bool canZGoRight(ActiveBlock activeBlock){
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.x + 4 > COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x+2]==1) return false;
        if(cellInfo[activeBlock.y+1][activeBlock.x+3]==1) return false;
    } else {
        if(activeBlock.x + 3 > COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x+2]==1) return false;
        if(cellInfo[activeBlock.y+1][activeBlock.x+2]==1) return false;
        if(cellInfo[activeBlock.y+2][activeBlock.x+1]==1) return false;
    }
    return true;
}

bool canSGoRight(ActiveBlock activeBlock){
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.x + 4 > COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x+3]==1) return false;
        if(cellInfo[activeBlock.y+1][activeBlock.x+2]==1) return false;
    } else {
        if(activeBlock.x + 3 > COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x+1]==1) return false;
        if(cellInfo[activeBlock.y+1][activeBlock.x+2]==1) return false;
        if(cellInfo[activeBlock.y+2][activeBlock.x+2]==1) return false;
    }
    return true;
}

//canBlockRotate

bool canBlockRotate(ActiveBlock activeBlock){
    switch(activeBlock.block){
        case BarBlock: return canBarRotate(activeBlock);
        case BoxBlock: return canBoxRotate(activeBlock);
        case TBlock:   return canTRotate(activeBlock);
        case LBlock:   return canLRotate(activeBlock);
        case JBlock:   return canJRotate(activeBlock);
        case ZBlock:   return canZRotate(activeBlock);
        case SBlock:   return canSRotate(activeBlock);
        default:       return false;
    }
}

bool canBarRotate(ActiveBlock activeBlock){
    int barCells = 4;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if((activeBlock.x -1) < 0) return false;
        if((activeBlock.x-1) + barCells >= COLS) return false;
        for(int x = -1; x < barCells -1; x++){
            if(cellInfo[activeBlock.y][activeBlock.x + x] == 1) return false;
        }
    } else {
        if(activeBlock.y + barCells >= ROWS) return false;
        for(int y = 0; y < barCells; y++){
            if(cellInfo[activeBlock.y + y][activeBlock.x + 1] == 1) return false;
        }
    }
    return true;
}

bool canBoxRotate(ActiveBlock activeBlock){ return true; }

bool canTRotate(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){

        case Down: // your Down → Right
            if(x < 0 || x+1 >= COLS) return false;
            if(y+2 >= ROWS) return false;

            if(cellInfo[y+1][x] == 1) return false;
            break;

        case Right: // Right → Up
            if(x+2 >= COLS) return false;
            if(y+1 >= ROWS) return false;

            if(cellInfo[y][x+1] == 1) return false;
            break;

        case Up: // Up → Left
            if(x < 0 || x+1 >= COLS) return false;
            if(y+2 >= ROWS) return false;

            if(cellInfo[y+1][x+1] == 1) return false;
            break;

        case Left: // Left → Down
            if(x+2 >= COLS) return false;
            if(y+1 >= ROWS) return false;

            if(cellInfo[y+1][x+1] == 1) return false;
            break;
    }

    return true;
}

bool canLRotate(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){
        case Up:     // rotating Up → Right
            if(x + 2 >= COLS) return false;   // check right boundary
            if(y + 1 >= ROWS) return false;   // check bottom
            if(cellInfo[y][x+1]==1) return false;   // top row collision
            if(cellInfo[y+1][x+2]==1) return false; // bottom-right collision
            break;

        case Right:   // Right → Down
            if(x + 1 >= COLS) return false;
            if(y + 2 >= ROWS) return false;
            if(cellInfo[y+2][x+1]==1) return false;
            if(cellInfo[y][x+1]==1) return false;
            break;

        case Down:    // Down → Left
            if(x + 2 >= COLS) return false;
            if(y + 2 >= ROWS) return false;
            if(cellInfo[y+2][x+1]==1) return false;
            if(cellInfo[y+2][x]==1) return false;
            break;

        case Left:    // Left → Up
            if(x + 2 >= COLS) return false;
            if(y + 1 >= ROWS) return false;
            if(cellInfo[y+1][x]==1) return false;
            if(cellInfo[y][x+2]==1) return false;
            break;
    }

    return true;
}

bool canJRotate(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    switch(activeBlock.orientation){
        case Up:     // Up → Right
            if(x + 2 >= COLS) return false;
            if(y + 1 >= ROWS) return false;
            if(cellInfo[y+2][x]==1) return false;    // bottom-left
            if(cellInfo[y+2][x+1]==1) return false;  // bottom-middle
            break;

        case Right:   // Right → Down
            if(x + 1 >= COLS) return false;
            if(y + 2 >= ROWS) return false;
            if(cellInfo[y+2][x]==1) return false;    // bottom-left
            if(cellInfo[y][x+1]==1) return false;    // top-right
            break;

        case Down:    // Down → Left
            if(x + 2 >= COLS) return false;
            if(y + 2 >= ROWS) return false;
            if(cellInfo[y][x+1]==1) return false;    // top-middle
            if(cellInfo[y+2][x+1]==1) return false;  // bottom-middle
            break;

        case Left:    // Left → Up
            if(x + 2 >= COLS) return false;
            if(y + 1 >= ROWS) return false;
            if(cellInfo[y][x+2]==1) return false;    // top-right
            if(cellInfo[y+1][x+2]==1) return false;  // bottom-right
            break;
    }

    return true;
}

bool canZRotate(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        // Horizontal → Vertical
        if(x + 1 >= COLS) return false;
        if(y + 2 >= ROWS) return false;

        if(cellInfo[y+1][x] == 1) return false;
        if(cellInfo[y+2][x] == 1) return false;

    } else {
        // Vertical → Horizontal
        if(x + 2 >= COLS) return false;
        if(y + 1 >= ROWS) return false;

        if(cellInfo[y][x+2] == 1) return false;
        if(cellInfo[y+1][x+2] == 1) return false;
    }

    return true;
}

bool canSRotate(ActiveBlock activeBlock){
    int x = activeBlock.x;
    int y = activeBlock.y;

    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        // Horizontal → Vertical
        if(x + 1 >= COLS) return false;
        if(y + 2 >= ROWS) return false;

        if(cellInfo[y][x] == 1) return false;
        if(cellInfo[y+2][x+1] == 1) return false;

    } else {
        // Vertical → Horizontal
        if(x + 2 >= COLS) return false;
        if(y + 1 >= ROWS) return false;

        if(cellInfo[y+1][x+2] == 1) return false;
        if(cellInfo[y][x+1] == 1) return false;
    }

    return true;
}

// rotateBlock 

void rotateBlock(ActiveBlock &activeBlock){
    switch(activeBlock.block){
        case BarBlock:
            switch(activeBlock.orientation){
                case Up:
                case Down:
                    activeBlock.x--;
                    activeBlock.orientation = Right;
                    break;
                case Right:
                case Left:
                    activeBlock.x++;
                    activeBlock.y--;
                    activeBlock.orientation = Up;
                    break;
            }
            break;
        case BoxBlock:
            break;
        case TBlock:
            switch(activeBlock.orientation){
                case Up:    activeBlock.orientation = Right; break;
                case Right: activeBlock.orientation = Down;  break;
                case Down:  activeBlock.orientation = Left;  break;
                case Left:  activeBlock.orientation = Up;    break;
            }
            break;
        case LBlock:
            switch(activeBlock.orientation){
                case Up:    activeBlock.orientation = Right; break;
                case Right: activeBlock.orientation = Down;  break;
                case Down:  activeBlock.orientation = Left;  break;
                case Left:  activeBlock.orientation = Up;    break;
            }
            break;
        case JBlock:
            switch(activeBlock.orientation){
                case Up:    activeBlock.orientation = Right; break;
                case Right: activeBlock.orientation = Down;  break;
                case Down:  activeBlock.orientation = Left;  break;
                case Left:  activeBlock.orientation = Up;    break;
            }
            break;
        case ZBlock:
            switch(activeBlock.orientation){
                case Up:
                case Down:  activeBlock.orientation = Right; break;
                case Right:
                case Left:  activeBlock.orientation = Up;    break;
            }
            break;
        case SBlock:
            switch(activeBlock.orientation){
                case Up:
                case Down:  activeBlock.orientation = Right; break;
                case Right:
                case Left:  activeBlock.orientation = Up;    break;
            }
            break;
    }
}

// Game Over 

bool isGameOver(){
    // If any cell in the top row is occupied, game over
    for(int col = 0; col < COLS; col++){
        if(cellInfo[0][col] == 1) return true;
    }
    return false;
}

void drawGameOverScreen(){
    // Semi-transparent dark overlay over the board
    DrawRectangle(0, 0, BOARD_WIDTH, BOARD_HEIGHT, (Color){0, 0, 0, 160});

    // "GAME OVER" title
    const char* title = "GAME OVER";
    int titleSize = 36;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, (BOARD_WIDTH - titleW) / 2, BOARD_HEIGHT / 2 - 60, titleSize, RED);

    // Final score
    const char* scoreLine = TextFormat("SCORE: %d", score);
    int scoreSize = 22;
    int scoreW = MeasureText(scoreLine, scoreSize);
    DrawText(scoreLine, (BOARD_WIDTH - scoreW) / 2, BOARD_HEIGHT / 2, scoreSize, WHITE);

    // Restart prompt
    const char* restart = "Press R to restart";
    int restartSize = 18;
    int restartW = MeasureText(restart, restartSize);
    DrawText(restart, (BOARD_WIDTH - restartW) / 2, BOARD_HEIGHT / 2 + 40, restartSize, LIGHTGRAY);
}
