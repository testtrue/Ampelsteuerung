// Each constant contains the index of PINS
enum MAP{ AMPEL1RED1=0,
          AMPEL1YELLOW1,
          AMPEL1GREEN1,
          AMPEL1RED2,
          AMPEL1GREEN2,
          AMPEL2RED1,
          AMPEL2YELLOW1,
          AMPEL2GREEN1,
          AMPEL2RED2,
          AMPEL2GREEN2,
          AMPEL1TASTERRESET,
          AMPEL2TASTERRESET
       };

enum AMAP{ AMPEL1TASTER,
           AMPEL2TASTER,
           LICHTWIEDERSTAND};

// Source: https://www.cs.tut.fi/~jkorpela/round.html
#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

// Array with the used pinnumbers
// Which Pin is what constant is in the enum MAP          
const int PINS[] = {2,3,4,5,6,7,8,9,10,11,12,13};
const int APINS[] = {A1,A2,A3};
boolean stp0 = true,stp1 = false,stp2 = false,stp3 = false,stp4 = false,stp5 = false,stp6 = false,stp7 = false;

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
  pinMode(PINS[AMPEL2TASTERRESET],OUTPUT);

  analogReference(DEFAULT);
  Serial.begin(115200);
 
  int outputval = 0;
  outputval += getAddValue(true,AMPEL1GREEN1);
  outputval += getAddValue(true,AMPEL1RED2);
  outputval += getAddValue(true,AMPEL2RED1);
  outputval += getAddValue(true,AMPEL2RED2);
  output(outputval);
}

void loop() {
  int input = getInput();

  boolean nightmode = constrain(analogRead(A3),35,100) <= 35;
  int outputval = 0;
  if(!nightmode){
    outputval = standardmode(input);
  }else{
    outputval = runnightmode(input);
  }
  output(outputval);

  if(mask(outputval,AMPEL1GREEN1) || mask(outputval,AMPEL2GREEN1) ) delay(5000);
  else if(mask(outputval,AMPEL1YELLOW1) || mask(outputval,AMPEL2YELLOW1)) delay(1000);
  else if(mask(outputval,AMPEL1RED1) || mask(outputval,AMPEL2RED1)) delay(2000);
  delay(500);
}

int standardmode(int input){
  boolean taster1 = map(analogRead(APINS[AMPEL1TASTER]),0,500,0,1) == 1;
  boolean taster2 = map(analogRead(APINS[AMPEL2TASTER]),0,500,0,1) == 1;
  
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
//  Serial.println(outputval);
  return outputval;
}

int runnightmode(int input){
  return getAddValue(!mask(input,AMPEL2YELLOW1),AMPEL2YELLOW1);
}


int getInput() {
  int ret = 0;
  int max = sizeof(PINS)/sizeof(PINS[0]);
  for(int i = 0; i < max;i++){
    ret += digitalRead(PINS[i]) * round(pow(2,PINS[i]));
  }
  return ret;
}

void resetFF(MAP m){
  digitalWrite(PINS[m],HIGH);
  digitalWrite(PINS[m],LOW);
}

void output(int outVal) {
  int max = sizeof(PINS)/sizeof(PINS[0]);
  for(int i = 0;i < max;i++){
    boolean b = mask(outVal,PINS[i]);
    int p = PINS[i];
    digitalWrite(p,b ? HIGH : LOW);
  }
}

/*
 * Es wird der Pin des gemappten Wert m in input ausmaskiert und dann geprueft ob der Wert true ist
 */ 
boolean mask(int input, MAP m){
  return mask(input,getMPin(m));
}

boolean mask(int input, int p){
  int v = round(pow(2, p));
  return (input & v) == v;
}

/* 
 *  Gibt den zu addieren Wert zurueck wenn b true ist sonst 0
 */
int getAddValue(boolean b, MAP m){
  if(b){
    return round(pow(2,getMPin(m)));
  }else{
    return 0;
  }
}

// Pinnummer des gemappten Wertes
int getMPin(MAP m){
  return PINS[m];
}
