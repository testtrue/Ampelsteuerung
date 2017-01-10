// Each constant contains the index of PINS
enum MAP{ AMPEL1RED1=0,
          AMPEL1YELLOW1=1,
          AMPEL1GREEN1=2,
          AMPEL1RED2=3,
          AMPEL1GREEN2=4,
          AMPEL2RED1=5,
          AMPEL2YELLOW1=6,
          AMPEL2GREEN1=7,
          AMPEL2RED2=8,
          AMPEL2GREEN2=9,
          AMPEL1TASTER=10,
          AMPEL2TASTER=11
       };

// Array with the used pinnumbers
// Which Pin is what constant is in the enum MAP          
const int PINS[] = {0,1,2,3,4,5,6,7,8,9,12,13};

void setup() {
  pinMode(PINS[AMPEL1RED1], OUTPUT);
  pinMode(PINS[AMPEL1YELLOW1], OUTPUT);
  pinMode(PINS[AMPEL1GREEN1], OUTPUT);
  pinMode(PINS[AMPEL1RED2], OUTPUT);
  pinMode(PINS[AMPEL1GREEN2], OUTPUT);
  pinMode(PINS[AMPEL2RED1], OUTPUT);
  pinMode(PINS[AMPEL2YELLOW1], OUTPUT);
  pinMode(PINS[AMPEL2GREEN1], OUTPUT);
  pinMode(PINS[AMPEL2RED2], OUTPUT);
  pinMode(PINS[AMPEL2GREEN2], OUTPUT);
  pinMode(PINS[AMPEL1TASTER], INPUT);
  pinMode(PINS[AMPEL2TASTER], INPUT);
  Serial.begin(9600);
}

void loop() {
  int max = sizeof(PINS)/sizeof(PINS[0]);
  for(int i = 0; i < max;i++){
    Serial.println(readDPin(i));
  }
  delay(500);
}

// AMPEL1TASTER = 2^0
// AMPEL2TASTER = 2^1
int input() {
  int t1 = int(digitalRead(AMPEL1TASTER) && HIGH);
  int t2 = int(digitalRead(AMPEL2TASTER) && HIGH);
  return int( t1 * pow(2, 0) + t2 * pow(2, 1) );
}

void output(int outVal) {
  for(int i = 0;i < sizeof(PINS);i++){
    writeDPin(PINS[i],(outVal && pow(2, PINS[i]) == HIGH) ? HIGH : LOW);
  }
}

int readDPin(int k){
  return digitalRead(k);
}

void writeDPin(int k, int v){
  digitalWrite(k,v);
}

int readMDPin(MAP k){
  return readDPin(PINS[k]);
}

void writeMDPin(MAP k, int v){
  digitalWrite(PINS[k],v);
}
