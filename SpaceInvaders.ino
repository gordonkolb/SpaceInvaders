#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();

class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
    }
    // sets values for private date members x and y
    Invader(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }

    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_strength() const {
      return strength;
    }

    void set_strength(int strength_arg){
      strength = strength_arg;
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    void move() {
      y++;
    }

    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
      if (strength == 1) {
        draw_with_rgb(RED, BLUE);
      }
      if (strength == 2) {
        draw_with_rgb(ORANGE, BLUE);
      }
      if (strength == 3) {
        draw_with_rgb(YELLOW, BLUE);
      }
      if (strength == 4) {
        draw_with_rgb(GREEN, BLUE);
      }
      if (strength == 5) {
        draw_with_rgb(BLUE, BLUE);
      }
      if (strength == 6) {
        draw_with_rgb(PURPLE, BLUE);
      }
      if (strength == 7) {
        draw_with_rgb(WHITE, BLUE);
      }
    }

    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK, BLACK);
      //call draw_with_rgb and make body color balck and eyes
    }

    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      strength--;
      if (strength == 0) {
        draw_with_rgb(BLACK, BLACK);
      }
    }

  private:
    int x;
    int y;
    int strength;

    // draws the Invader
    void draw_with_rgb(Color body_color, Color eye_color) {
      matrix.drawPixel(x, y + 1, body_color.to_333());
      matrix.drawPixel(x, y + 2, body_color.to_333());
      matrix.drawPixel(x, y + 3, body_color.to_333());
      matrix.drawPixel(x + 1, y, body_color.to_333());
      matrix.drawPixel(x + 1, y + 1, eye_color.to_333());
      matrix.drawPixel(x + 1, y + 2, body_color.to_333());
      matrix.drawPixel(x + 2, y, body_color.to_333());
      matrix.drawPixel(x + 2, y + 1, eye_color.to_333());
      matrix.drawPixel(x + 2, y + 2, body_color.to_333());
      matrix.drawPixel(x + 3, y + 1, body_color.to_333());
      matrix.drawPixel(x + 3, y + 2, body_color.to_333());
      matrix.drawPixel(x + 3, y + 3, body_color.to_333());
    }
};

class Cannonball {
  public:
    Cannonball() {
      x = 0;
      y = 13;
      fired = false;
    }

    // resets private data members to initial values
    void reset() {
      x = 0;
      y = 13;
      fired = false;
    }

    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    bool has_been_fired() const {
      return fired;
    }

    // sets private data members
    void fire(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      fired = true;
    }

    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      y--;

      if (y < -1) {
        x = 0;
        y = 13;
        fired = false;
      }
    }

    // resets private data members to initial values
    void hit() {
      x = 0;
      y = 13;
      fired = false;
    }

    // draws the Cannonball, if it is fired
    void draw() {
      if (fired == true) {
        matrix.drawPixel(x, y, ORANGE.to_333());
        matrix.drawPixel(x, y - 1, ORANGE.to_333());
      }
    }

    // draws black where the Cannonball used to be
    void erase() {
      matrix.drawPixel(x, y, BLACK.to_333());
      matrix.drawPixel(x, y + 1, BLACK.to_333());
    }

  private:
    int x;
    int y;
    bool fired;
};

class Player {
  public:
    Player() {
      x = 0;
      y = 14;
      lives = 3;
    }

    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_lives() const {
      return lives;
    }

    // setter
    void set_x(int x_arg) {
      x = x_arg;
    }

    // Modifies: lives
    void die() {
      lives--;
    }

    // draws the Player
    // calls: draw_with_rgb
    void draw() {
      draw_with_rgb(AQUA);
    }

    // resets player after game over
    void reset() {
      lives = 3;
    }

    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK);
    }

  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }

    // draws the player
    void draw_with_rgb(Color color) {
      matrix.drawPixel(x, y, color.to_333());
      matrix.drawPixel(x - 1, y + 1, color.to_333());
      matrix.drawPixel(x, y + 1, color.to_333());
      matrix.drawPixel(x + 1, y + 1, color.to_333());
    }
};

class Game {
  public:
    Game() {
      level = 1;
      time = 0;
      lastValue = 0;
      lastMoveInvader = 0;
      lastChangeButton = 0;
    }

    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    void setupGame() {


      //print level
      matrix.fillScreen(BLACK.to_333());
      print_level(level);
      delay(2000);

      //print number of lives
      matrix.fillScreen(BLACK.to_333());
      print_lives(player.get_lives());
      delay(2000);
      matrix.fillScreen(BLACK.to_333());



      //creating invaders
      if (level == 1) {
        Invader test_invader1(0, 0, 1);
        for (int i = 0; i < 8; i++) {
          enemies[i] = test_invader1;
          test_invader1.initialize((test_invader1.get_x()) + 4, 0, 1);
        }
      }
      if (level == 2) {
        Invader test_invader3a(0, 0, 1);
        enemies[0] = test_invader3a;
        Invader test_invader3b(4, 0, 2);
        enemies[1] = test_invader3b;
        Invader test_invader3c(8, 0, 1);
        enemies[2] = test_invader3c;
        Invader test_invader3d(12, 0, 2);
        enemies[3] = test_invader3d;
        Invader test_invader3e(16, 0, 1);
        enemies[4] = test_invader3e;
        Invader test_invader3f(20, 0, 2);
        enemies[5] = test_invader3f;
        Invader test_invader3g(24, 0, 1);
        enemies[6] = test_invader3g;
        Invader test_invader3h(28, 0, 2);
        enemies[7] = test_invader3h;
        Invader test_invader3i(0, 4, 2);
        enemies[8] = test_invader3i;
        Invader test_invader3j(4, 4, 1);
        enemies[9] = test_invader3j;
        Invader test_invader3k(8, 4, 2);
        enemies[10] = test_invader3k;
        Invader test_invader3l(12, 4, 1);
        enemies[11] = test_invader3l;
        Invader test_invader3m(16, 4, 2);
        enemies[12] = test_invader3m;
        Invader test_invader3n(20, 4, 1);
        enemies[13] = test_invader3n;
        Invader test_invader3o(24, 4, 2);
        enemies[14] = test_invader3o;
        Invader test_invader3p(28, 4, 1);
        enemies[15] = test_invader3p;
      }
      if (level == 3) {
        Invader test_invader3a(0, 0, 1);
        enemies[0] = test_invader3a;
        Invader test_invader3b(4, 0, 2);
        enemies[1] = test_invader3b;
        Invader test_invader3c(8, 0, 3);
        enemies[2] = test_invader3c;
        Invader test_invader3d(12, 0, 4);
        enemies[3] = test_invader3d;
        Invader test_invader3e(16, 0, 5);
        enemies[4] = test_invader3e;
        Invader test_invader3f(20, 0, 1);
        enemies[5] = test_invader3f;
        Invader test_invader3g(24, 0, 2);
        enemies[6] = test_invader3g;
        Invader test_invader3h(28, 0, 3);
        enemies[7] = test_invader3h;
        Invader test_invader3i(0, 4, 4);
        enemies[8] = test_invader3i;
        Invader test_invader3j(4, 4, 5);
        enemies[9] = test_invader3j;
        Invader test_invader3k(8, 4, 1);
        enemies[10] = test_invader3k;
        Invader test_invader3l(12, 4, 2);
        enemies[11] = test_invader3l;
        Invader test_invader3m(16, 4, 3);
        enemies[12] = test_invader3m;
        Invader test_invader3n(20, 4, 4);
        enemies[13] = test_invader3n;
        Invader test_invader3o(24, 4, 5);
        enemies[14] = test_invader3o;
        Invader test_invader3p(28, 4, 1);
        enemies[15] = test_invader3p;
      }
      if (level == 4) {
        Invader test_invader4a(0, 0, 5);
        enemies[0] = test_invader4a;
        Invader test_invader4b(4, 0, 4);
        enemies[1] = test_invader4b;
        Invader test_invader4c(8, 0, 5);
        enemies[2] = test_invader4c;
        Invader test_invader4d(12, 0, 4);
        enemies[3] = test_invader4d;
        Invader test_invader4e(16, 0, 5);
        enemies[4] = test_invader4e;
        Invader test_invader4f(20, 0, 4);
        enemies[5] = test_invader4f;
        Invader test_invader4g(24, 0, 5);
        enemies[6] = test_invader4g;
        Invader test_invader4h(28, 0, 4);
        enemies[7] = test_invader4h;
        Invader test_invader4i(0, 4, 2);
        enemies[8] = test_invader4i;
        Invader test_invader4j(4, 4, 3);
        enemies[9] = test_invader4j;
        Invader test_invader4k(8, 4, 2);
        enemies[10] = test_invader4k;
        Invader test_invader4l(12, 4, 3);
        enemies[11] = test_invader4l;
        Invader test_invader4m(16, 4, 2);
        enemies[12] = test_invader4m;
        Invader test_invader4n(20, 4, 3);
        enemies[13] = test_invader4n;
        Invader test_invader4o(24, 4, 2);
        enemies[14] = test_invader4o;
        Invader test_invader4p(28, 4, 3);
        enemies[15] = test_invader4p;
      }
      if (level > 4) {
        Invader test_invader5a(0, 0, random(1, 7));
        enemies[0] = test_invader5a;
        Invader test_invader5b(4, 0, random(1, 7));
        enemies[1] = test_invader5b;
        Invader test_invader5c(8, 0, random(1, 7));
        enemies[2] = test_invader5c;
        Invader test_invader5d(12, 0, random(1, 7));
        enemies[3] = test_invader5d;
        Invader test_invader5e(16, 0, random(1, 7));
        enemies[4] = test_invader5e;
        Invader test_invader5f(20, 0, random(1, 7));
        enemies[5] = test_invader5f;
        Invader test_invader5g(24, 0, random(1, 7));
        enemies[6] = test_invader5g;
        Invader test_invader5h(28, 0, random(1, 7));
        enemies[7] = test_invader5h;
        Invader test_invader5i(0, 4, random(1, 7));
        enemies[8] = test_invader5i;
        Invader test_invader5j(4, 4, random(1, 7));
        enemies[9] = test_invader5j;
        Invader test_invader5k(8, 4, random(1, 7));
        enemies[10] = test_invader5k;
        Invader test_invader5l(12, 4, random(1, 7));
        enemies[11] = test_invader5l;
        Invader test_invader5m(16, 4, random(1, 7));
        enemies[12] = test_invader5m;
        Invader test_invader5n(20, 4, random(1, 7));
        enemies[13] = test_invader5n;
        Invader test_invader5o(24, 4, random(1, 7));
        enemies[14] = test_invader5o;
        Invader test_invader5p(28, 4, random(1, 7));
        enemies[15] = test_invader5p;
      }

      
      time = millis();
      lastMoveInvader = time;

      

    }

    // advances the game simulation one step and renders the graphics
    // see spec for details of game
    // Modifies: global variable matrix
    void update(int potentiometer_value, bool button_pressed) {
      // game dynamics section of the specification

  

      //drawing invaders
      for (int i = 0; i < 16; i++) {
        enemies[i].draw();
      }

     
      
   //moving invaders    
      bool movingTopInvaders = true;

      int count = 0;
      for (int i = 8; i < 16; i++){
        if (enemies[i].get_strength() != 0){
          movingTopInvaders = false;
        }
      }
      
      if ((movingTopInvaders)){
        
        if (time > lastMoveInvader + 2000) {
        for (int i = 0; i < 8; i++) {
          enemies[i].erase();
          enemies[i].move();
          enemies[i].draw();
        }
        lastMoveInvader = time;
        }
      }
      else{
        if (time > lastMoveInvader + 2000) {
          for (int i = 8; i < 16; i++) {
           //if (enemies[0].get_strength());
            enemies[i].erase();
            enemies[i].move();
           enemies[i].draw();
          }
        lastMoveInvader = time;
        }
      }
      

      




      //moving the player
      if (abs(lastValue - (potentiometer_value)) > 10) {
        player.erase();
        player.set_x(potentiometer_value / 32);
        player.draw();
      }


      //fire the bullet if button is pressed and there is no other bullet on the screen
      if (button_pressed && !(ball.has_been_fired()) && !(lastChangeButton)) {
        ball.fire(player.get_x(), 13);
        lastChangeButton = 1;
        ball.draw();
        lastMoveBall = time;
      }

      if (!(button_pressed)){
        lastChangeButton = 0;
      }

      if (ball.has_been_fired()) {
        if (time > lastMoveBall + 100) {
          ball.erase();
          ball.move();
          ball.draw();
          lastMoveBall = time;
          player.draw();
        }
      }

      //checking if cannonball hits enemy
      for (int i = 0; i < 16; i++) {
         if (ball.has_been_fired()){
          if ((ball.get_x() == (enemies[i].get_x()) & (ball.get_y() <= enemies[i].get_y() + 3)) ||
            (ball.get_x() == (enemies[i].get_x() + 1) & (ball.get_y() <= enemies[i].get_y() + 2)) ||
            (ball.get_x() == (enemies[i].get_x() + 2) & (ball.get_y() <= enemies[i].get_y() + 2)) ||
            (ball.get_x() == (enemies[i].get_x() + 3) & (ball.get_y() <= enemies[i].get_y() + 3))) {

            if (enemies[i].get_strength() > 0) {
              enemies[i].hit();
              ball.erase();
              ball.reset();
            }
          }
        }
      }

      //checking if enemy touches player or bottom of screen and if game over or should reset level
      for (int i = 0; i < 16; i++) {
        if ((player.get_x() == (enemies[i].get_x()) & (player.get_y() == enemies[i].get_y() + 3)) ||
           (player.get_x() == (enemies[i].get_x() + 3) & (player.get_y() == enemies[i].get_y() + 3)) ||
           (player.get_x() - 1 == (enemies[i].get_x()) & ((player.get_y() + 1) == enemies[i].get_y() + 3)) ||
           (player.get_x() - 1 == (enemies[i].get_x() + 3) & ((player.get_y() + 1) == enemies[i].get_y() + 3)) ||
           (player.get_x() + 1 == (enemies[i].get_x()) & ((player.get_y() + 1) == enemies[i].get_y() + 3)) ||
           (player.get_x() + 1 == (enemies[i].get_x() + 3) & ((player.get_y() + 1) == enemies[i].get_y() + 3)) ||
           (enemies[i].get_y() + 3 >= 15)){

        if (enemies[i].get_strength() > 0) {
            player.die(); //lives--
            for (int i = 0; i < 16; i++) {
        enemies[i].set_strength(0);
                  }
            reset_level();           
          }
        }
      }

      //checking if level has been cleared
      if(level_cleared()){
        level++;
        reset_level();
      }

      //Keeping track of time
      time = millis();
    }

  private:
    int level;
    unsigned long time;
    unsigned long lastMoveBall;
    unsigned long lastMoveInvader;
    int lastValue;
    Player player;
    Cannonball ball;
    Invader enemies[NUM_ENEMIES];
    bool lastChangeButton;

    // check if Player defeated all Invaders in current level
    bool level_cleared() {
      for (int i = 0; i < 16; i++) {
        if (enemies[i].get_strength() > 0) {
          return false;
        }
      }
      return true;
    }


    // set up a level
    void reset_level() {
      if (player.get_lives() == 0){
        for (int i = 0; i < 16; i++) {
        enemies[i].set_strength(0);
        }
        matrix.fillScreen(BLACK.to_333());
        game_over();
        delay(2000);
        matrix.fillScreen(BLACK.to_333());
        delay(2000);
        level = 1;
        player.reset();

        
        
      }
      
       setupGame();
       player.draw();
      
      
      
      
    }
};

// a global variable that represents the game Space Invaders
Game game;

// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  randomSeed(analogRead(4));
  matrix.begin();
  game.setupGame();
}

// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
  
  game.update(potentiometer_value, button_pressed);
}

// displays Level
void print_level(int level) {
  matrix.setCursor(2,0);
  matrix.print("Level");
  matrix.setCursor(14,8);
  matrix.print(level);
}

// displays number of lives
void print_lives(int lives) {
  matrix.setCursor(1,0);
  matrix.print("Lives");
  matrix.setCursor(13,8);
  matrix.print(lives);
}

// displays "game over"
void game_over() {
  matrix.setCursor(4,0);
  matrix.print("Game Over");
}
