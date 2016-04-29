volatile char randomize;
volatile char loopNow;
long randNum;
char animationNum;
char frameNum;
char loopsPerFrame;
unsigned char counter = 0, k = 0;

#define ROW 4
#define COL 2

unsigned char frameArray1[1][ROW][COL] = 
{
{{0x00, 0x00},
{0x06, 0x60},
{0x06, 0x60},
{0x00, 0x00}}
};

unsigned char frameArray2[1][ROW][COL] = 
{
{{0x00, 0x33},
{0x00, 0x33},
{0xCC, 0x00},
{0xCC, 0x00}}
};

unsigned char frameArray3[1][ROW][COL] = 
{
{{0x00, 0x01},
{0x00, 0x13},
{0x01, 0x37},
{0x13, 0x7f}}
};

unsigned char frameArray4[1][ROW][COL] = 
{
{{0x90, 0x09},
{0x90, 0x09},
{0x90, 0x09},
{0x90, 0x09}}
};

unsigned char frameArray5[1][ROW][COL] = 
{
{{0x96, 0x69},
{0x96, 0x69},
{0x96, 0x69},
{0x96, 0x69}}
};

unsigned char frameArray6[1][ROW][COL] = 
{
{{0xfe, 0xc9},
{0xec, 0x93},
{0xc9, 0x37},
{0x93, 0x7f}}
};

void setup() {
  // put your setup code here, to run once:

  // Setup Attiny84 pins for I/O
  DDRA |= 0xFF;
  DDRB |= 0x3;

  // Reset pins
  PORTB &= 0x0;
  PORTA &= 0x00;

  // Set up interrupt on INT0
  GIMSK = 1<<INT0;
  MCUCR = 0x03;
  sei();

}

void loop() {
  
  // Test for button push
  if(randomize) {
    diceResult(); // get properly randomized number 1-12
//    rainfall(); // run rolling animation
    loopNow = 1; // set loop flag
     // dynamically create function to call
    randomize = 0;
  }

  // Test for state of display loop
  if(loopNow) {
    counter++;
    if (counter == loopsPerFrame){
       k = (k == frameNum-1)? 0 : k+1;
       counter = 0;
    }
    for (char j=0; j<4; j++) {
      PORTB |= 0x1;
      delay(0);
      for(char i=1; i>=0; i--) {
        
        // Push the first(third) row to shift register
        PORTA &= 0x00;
        PORTB &= 0x0;

//        char b =  (frameArray1[k][j][i] & 0x08) >> 2; 
//        char a0 = (frameArray1[k][j][i] & 0x04) >> 2; 
//        char a5 = (frameArray1[k][j][i] & 0x02) << 4; 
//        char a6 = (frameArray1[k][j][i] & 0x01) << 6;
        
        char b =  (callArray(randNum, i, j, k) & 0x08) >> 2; 
        char a0 = (callArray(randNum, i, j, k) & 0x04) >> 2; 
        char a5 = (callArray(randNum, i, j, k) & 0x02) << 4; 
        char a6 = (callArray(randNum, i, j, k) & 0x01) << 6; 

        PORTB |= b;
        PORTA |= a0;
        PORTA |= a5;
        PORTA |= a6;

        delay(0);
        PORTA |= 0x80; //clock high
        delay(0);

        // Push second (fourth) row to shift register
        PORTA &= 0x00;
        PORTB &= 0x0;

        
//        b =  (frameArray1[k][j][i] & 0x80) >> 6; 
//        a0 = (frameArray1[k][j][i] & 0x40) >> 6; 
//        a5 = frameArray1[k][j][i] & 0x20; 
//        a6 = (frameArray1[k][j][i] & 0x10) << 2; 

        b =  (callArray(randNum, i, j, k) & 0x80) >> 6; 
        a0 = (callArray(randNum, i, j, k) & 0x40) >> 6; 
        a5 = callArray(randNum, i, j, k) & 0x20; 
        a6 = (callArray(randNum, i, j, k) & 0x10) << 2; 
    
        PORTB |= b;
        PORTA |= a0;
        PORTA |= a5;
        PORTA |= a6;

        delay(0);
        PORTA |= 0x80; //clock high
        delay(0);
      }

      // Activate layers
      switch(j){
        case 0:
        PORTA |= 0x10; //0th layer (top)
        break;
        case 1:
        PORTA |= 0x08; //1
        break;
        case 2:
        PORTA |= 0x04; //2
        break;
        case 3:
        PORTA |= 0x02; //3
        break;
      }
      delay(5); // Yes, this is necessary
    }
  }
}

ISR(EXT_INT0_vect) {
  randomize = 1;
  randomSeed(millis());
}

void diceResult() {
  randNum = (random(1,6) % 6) + 1;
}

unsigned char callArray(char N, char i, char j, char k) {

  switch(N){
    case 1: 
      frameNum=sizeof(frameArray1)/(ROW*COL);
      loopsPerFrame=10;
      return frameArray1[k][j][i];
      break;
    case 2: 
      frameNum=sizeof(frameArray2)/(ROW*COL);
      loopsPerFrame=10;
      return frameArray2[k][j][i];
      break;
    case 3: 
      frameNum=sizeof(frameArray3)/(ROW*COL);
      loopsPerFrame=10;
      return frameArray3[k][j][i];
      break;
    case 4: 
      frameNum=sizeof(frameArray4)/(ROW*COL);
      loopsPerFrame=10;
      return frameArray4[k][j][i];
      break;
    case 5: 
      frameNum=sizeof(frameArray5)/(ROW*COL);
      loopsPerFrame=10;
      return frameArray5[k][j][i];
      break;
    case 6: 
      frameNum=sizeof(frameArray6)/(ROW*COL);
      loopsPerFrame=10;
      return frameArray6[k][j][i];
      break;
  }
}

void rainfall() {
  // animation to play at the beginning of each roll
}



