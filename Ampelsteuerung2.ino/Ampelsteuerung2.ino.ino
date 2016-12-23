#define ampel1red1  0
#define ampel1yellow1 1
#define ampel1green1 2
#define ampel1red2  3
#define ampel1green2 4
#define ampel2red1 5
#define ampel2yellow1 6
#define ampel2green1 7
#define ampel2red2 8
#define ampel2green2 9
#define ampel1Taster 12
#define ampel2Taster 13


void setup() {
  pinMode(ampel1red1, OUTPUT);
  pinMode(ampel1yellow1, OUTPUT);
  pinMode(ampel1green1, OUTPUT);
  pinMode(ampel1red2, OUTPUT);
  pinMode(ampel1green2, OUTPUT);
  pinMode(ampel2red1, OUTPUT);
  pinMode(ampel2yellow1, OUTPUT);
  pinMode(ampel2green1, OUTPUT);
  pinMode(ampel2red2, OUTPUT);
  pinMode(ampel2green2, OUTPUT);
  pinMode(ampel1Taster, INPUT);
  pinMode(ampel2Taster, INPUT);
  Serial.begin(9600);
}

// ampel1Taster = 2^0
// ampel2Taster = 2^1
int input(){
  int t1 = int(digitalRead(ampel1Taster) && HIGH);
  int t2 = int(digitalRead(ampel2Taster) && HIGH);
  return int( t1*pow(2,0) + t2*pow(2,1) );
}

void output(int outVal){
  
}
void loop() {
  Serial.println(input());
}



