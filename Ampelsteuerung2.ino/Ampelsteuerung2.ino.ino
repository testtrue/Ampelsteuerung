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
          AMPEL1TASTERRESET=11,
          AMPEL2TASTER=12,
          AMPEL2TASTERRESET=13
       };

// Array with the used pinnumbers
// Which Pin is what constant is in the enum MAP          
const int PINS[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};

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
  pinMode(PINS[AMPEL1TASTERRESET],OUTPUT);
  pinMode(PINS[AMPEL1TASTER], INPUT);
  pinMode(PINS[AMPEL2TASTERRESET],OUTPUT);
  pinMode(PINS[AMPEL2TASTER], INPUT);

  int outputval = 0;
  outputval += getAddValue(true,AMPEL1GREEN1);
  outputval += getAddValue(true,AMPEL1RED2);
  outputval += getAddValue(true,AMPEL2RED1);
  outputval += getAddValue(true,AMPEL2RED2);
  output(outputval);
  
  Serial.begin(9600);
}

void loop() {
  boolean nightmode = false;
  int input = getInput();
  int ret = 0;
  if(nightmode){
    ret = runnightmode(input);
  }else{
    ret = runstandardmode(input);
  }
  output(ret);
  delay(500);
}

int runstandardmode(int input){
  boolean stp0 = true,stp1 = false,stp2 = false,stp3 = false,stp4 = false,stp5 = false,stp6 = false,stp7 = false;

  boolean taster1 = mask(input,AMPEL1TASTER);
  boolean taster2 = mask(input,AMPEL2TASTER);
  
  if(stp7 && mask(input,AMPEL1YELLOW1) &&
            mask(input,AMPEL1RED1) &&
            mask(input,AMPEL2RED1)){
    stp7 = false;
    stp0 = true;
  }
  
  if(stp0 && mask(input, AMPEL1GREEN1) &&
            mask(input, AMPEL2RED1) ){
    stp0 = false;
    stp1 = true;
  }

  if(stp1 && mask(input,AMPEL1YELLOW1) &&
            mask(input,AMPEL2RED2) ){
    stp1 = false;
    stp2 = true;
  }

  if(stp2 && mask(input,AMPEL1RED1) &&
            mask(input,AMPEL2RED1) ){
    stp2 = false;
    stp3 = true;
  }

  if(stp3 && mask(input,AMPEL1RED1) &&
            mask(input,AMPEL2YELLOW1) &&
            mask(input,AMPEL2RED1) ){
    stp3 = false;
    stp4 = true;
  }

  if(stp4 && mask(input,AMPEL1RED1) &&
            mask(input,AMPEL2GREEN1)){
    stp4 = false;
    stp5 = true;            
  }

  if(stp5 && mask(input,AMPEL1RED1) &&
            mask(input,AMPEL2YELLOW1)){
    stp5 = false;
    stp6 = true;
  }

  if(stp6 && mask(input,AMPEL1RED1)&&
            mask(input,AMPEL2RED1)){
    stp6 = false;
    stp7 = true;
  }

  int outputval = 0;
  outputval += getAddValue(stp0,AMPEL1GREEN1);
  outputval += getAddValue(stp1||stp7,AMPEL1YELLOW1);
  outputval += getAddValue(!stp0&&!stp1,AMPEL1RED1);
  outputval += getAddValue(stp0 && taster1,AMPEL1GREEN2);
  outputval += getAddValue(!(stp0 && taster1),AMPEL1RED2);
  outputval += getAddValue(stp4,AMPEL2GREEN1);
  outputval += getAddValue(stp3||stp5,AMPEL2YELLOW1);
  outputval += getAddValue(!stp4&&!stp5,AMPEL2RED1);
  outputval += getAddValue(stp4&&taster2,AMPEL2GREEN2);
  outputval += getAddValue(!(stp4&&taster2),AMPEL2RED2);

  if(!(stp0 && taster1)) resetFF(AMPEL1TASTERRESET);
  if(!(stp4 && taster2)) resetFF(AMPEL2TASTERRESET);
  //output(outputval);
  return outputval;
}

int runnightmode(int input){
  return getAddValue(!mask(input,AMPEL2YELLOW1),AMPEL2YELLOW1);
}

int getInput() {
  int ret = 0;
  for(int i = 0; i < arrayLength(PINS);i++){
    ret += readDPin(PINS[i]) * pow(2,PINS[i]);
  }
  return ret;
}

void resetFF(MAP m){
  writeMDPin(m,HIGH);
  writeMDPin(m,LOW);
}

void output(int outVal) {
  int max = arrayLength(PINS);
  for(int i = 0;i < max;i++){
    writeDPin(PINS[i],mask(outVal,PINS[i]) ? HIGH : LOW);
  }
}

/*
 * Berechne arraylänge
 */
int arrayLength(int a[]){
  return sizeof(a)/sizeof(a[0]);
}

/*
 * Es wird der Pin des gemappten Wert m in input ausmaskiert und dann geprüft ob der Wert true ist
 */ 
boolean mask(int input, MAP m){
  return mask(input,getMPin(m));
}

boolean mask(int input, int p){
  return (input & int(pow(2, p))) == true;
}

/* 
 *  Gibt den zu addieren Wert zurück wenn b true ist sonst 0
 */
int getAddValue(boolean b, MAP m){
  if(b){
    return pow(2,getMPin(m));
  }else{
    return 0;
  }
}

// Pinnummer des gemappten Wertes
int getMPin(MAP m){
  return PINS[m];
}

// k = Pinnummer
int readDPin(int k){
  return digitalRead(k);
}

/*  k = Pinnummer
 *  v = 1 oder 0
 */ 
void writeDPin(int k, int v){
  digitalWrite(k,v);
}

// k = Wert aus MAP
int readMDPin(MAP k){
  return readDPin(PINS[k]);
}
/*  k = Wert aus MAP
 *  v = 1 oder 0
 */
void writeMDPin(MAP k, int v){
  digitalWrite(PINS[k],v);
}
