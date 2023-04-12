import processing.serial.*;
import controlP5.*;

Serial arduino;

ControlP5 cp5;
PFont font;


final int   COLUMNS=7;
final int ROWS=4;
final int BALL_RADIUS=8;
final int BALL_DIAMETER=BALL_RADIUS*2;
final   int MAX_VELOCITY=8;
final int MARGIN=10;
final int PADDLE_WIDTH=60;
final   int PADDLE_HEIGHT=15;
final int BRICK_WIDTH=40;
final int BRICK_HEIGHT=20;
final   int HEIGHT=300;
final int LINE_FEED=10;

boolean gameStarted = false;

int px, py;
int vx, vy;
float   xpos;
int[][] bricks= new int[COLUMNS][ROWS];

boolean spacePressed=false;
boolean   paused=true;
boolean done=true;

void setup(){
  size(300, 300);
   noCursor();
  font = createFont("arial", 40);
  initGame();
  cp5 = new ControlP5(this);
  arduino = new Serial(this, "COM7", 9600);
  
  xpos = width/2;

}


void initGame(){   
  initBricks();
  initBall();
}

void initBricks(){
  for(int x=0; x<COLUMNS; x++)
   for(int y=0; y<ROWS; y++)
  bricks[x][y]=1;
}

void initBall(){
   px=width/2;
  py=height/2;
  vx=int(random(-MAX_VELOCITY, MAX_VELOCITY));
   vy=-2;
}







void draw() {
  background(0);
  stroke(255);
  strokeWeight(3);
  
  if (!gameStarted) {
  textAlign(CENTER, CENTER);
  textSize(50);
  text("BREAKOUT", width/2, height * 7/20);
  textSize(20);
  text("PRESS SPACE TO BEGIN", width/2, height/2);
} else {
    // Game is started, update the game state
    done=drawBricks();
    if(done){
      paused=true;
      printWinMessage();
    }
    if(paused)
      printPauseMessage();
    else
      updateGame();
      
    drawBall();
    drawPaddle();
  }
  
  // Toggle the paused variable if the Space key is pressed
  if (keyPressed && key == ' ') {
    if (!gameStarted) {
      gameStarted = true;
    }
    paused = !paused;
    if (done) {
      done = false;
      initGame();
    }
  }
}




boolean drawBricks(){
  boolean   allEmpty=true;
  for(int x=0; x<COLUMNS; x++){
    for(int y=0; y<ROWS; y++){
       if(bricks[x][y]>0){
        allEmpty=false;
        fill(0, 0, 100+y*8);
         rect(
          MARGIN+x*BRICK_WIDTH,
          MARGIN+y*BRICK_HEIGHT,
           BRICK_WIDTH,
          BRICK_HEIGHT
        );
      }
    }
   }
  return allEmpty;
}



void drawBall(){
  strokeWeight(1);
   fill(128, 0, 0);
  ellipse(px, py, BALL_DIAMETER,  BALL_DIAMETER);
}




void printWinMessage(){
  fill(225);
   textSize(36);
  textAlign(CENTER);
  text("YOU WIN", width/2, height*2/3);
}


  
void   printPauseMessage(){
  fill(128);
  textSize(16);
  textAlign(CENTER);
   text("GAME PAUSED. PRESS SPACE TO RESUME!", width/2, height * 2/3);
}



void updateGame(){
   if(ballDropped()){
    initBall();
    paused=true;
  } else{
    checkBrickCollision();
     checkWallCollision();
    checkPaddleCollision();
    px+=vx;
    py+=vy;
   }
}



boolean ballDropped(){
  return py+vy> height - BALL_RADIUS;
}

boolean   inXRange(final int row, final int v){
  return px + v > row*BRICK_WIDTH&&
          px+v < (row+1)*BRICK_WIDTH+BALL_DIAMETER;
}

boolean inYRange(final   int col, final int v){
  return py+v> col*BRICK_HEIGHT&&
         py+v< (col+1)*BRICK_HEIGHT+BALL_DIAMETER;
}



void   checkBrickCollision(){
  for(int x=0; x<COLUMNS; x++){
    for(int y=0; y<ROWS;   y++){
      if(bricks[x][y]>0){
        if(inXRange(x, vx)&& inYRange(y, vy)){
           bricks[x][y]=0;
          if(inXRange(x, 0))
          vy=-vy;
           if(inYRange(y, 0))
          vx=-vx;
        }
      }
    }
   }
}


void checkWallCollision(){
  if(px+vx < BALL_RADIUS || px+vx   > width - BALL_RADIUS)
  vx=-vx;
  
  if(py+vy < BALL_RADIUS || py+vy >   height - BALL_RADIUS)
  vy=-vy;
}

void checkPaddleCollision(){
   final float cx = xpos;
  if(py+vy >=height - (PADDLE_HEIGHT + MARGIN + 6)&&
     px >= cx - PADDLE_WIDTH/2 &&
    px <= cx + PADDLE_WIDTH/2)
    {
       vy=-vy;
      vx=int(
        map(
          px - cx,
          -(PADDLE_WIDTH/2),   PADDLE_WIDTH/2,
          -MAX_VELOCITY,
          MAX_VELOCITY
        )
       );
    }
}


void   drawPaddle(){
  float x = constrain(xpos - PADDLE_WIDTH/2, 0, width - PADDLE_WIDTH);
  int y = height - 25;
  strokeWeight(1);
   fill(128);
  rect(x, y, 60, 15);
}

void serialEvent(Serial port){
  String arduinoData = port.readStringUntil('\n');
   if(arduinoData!=null){
   println(arduinoData);
   
   float data = Float.parseFloat(arduinoData);
   
    if(!paused)
      xpos = data * 350;
      
      println(data);    
   }  
}
  
  
