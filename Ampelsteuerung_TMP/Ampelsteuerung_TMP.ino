int ampel1red1 = 0;
int ampel1yellow1 = 1;
int ampel1green1 = 2;
#define ampel1red2  3;
#define ampel1green2 4
#define ampel2red1 5
#define ampel2yellow1 6
int ampel2green1 = 7;
int ampel2red2 = 8;
int ampel2green2 = 9;
int ampel1Taster = 12;
int ampel2Taster = 13;


void setup(){
	pinMode(ampel1red1,OUTPUT);
	pinMode(ampel1yellow1,OUTPUT);
	pinMode(ampel1green1,OUTPUT);
	pinMode(ampel1red2,OUTPUT);
	pinMode(ampel1green2,OUTPUT);
	pinMode(ampel2red1,OUTPUT);
	pinMode(ampel2yellow1,OUTPUT);
	pinMode(ampel2green1,OUTPUT);
	pinMode(ampel2red2,OUTPUT);
	pinMode(ampel2green2,OUTPUT);
	pinMode(ampel1Taster,INPUT);
	pinMode(ampel2Taster,INPUT);
}

void loop(){
	
}

