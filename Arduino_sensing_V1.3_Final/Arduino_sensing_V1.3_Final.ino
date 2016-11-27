//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND



#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+



#define N 160  //How many frequencies


float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;
int analogPin = 0;
int amp = 0;

float val = 0;
int touche = 0;
int led1 = 2;
int led2 = 4;
int led3 = 7;
int maxresult = 0;
int maxd = 0;
  

void setup()
{

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

 
  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin

  Serial.begin(115200);

  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+


}

void loop()
{
  unsigned int d;

  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator

    results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results
    freq[d] = d;
   /* Serial.print("d=");
    Serial.println(d);
    Serial.print("results[d]=");
    Serial.println(results[d]);
    Serial.print("freq[d]=");
    Serial.println(freq[d]);
    Serial.print("maxresult=");*/
    Serial.println(maxresult);

if(d>80)
{
  if(results[d]>maxresult)
  {
  maxresult = results[d];
  }
}
else if(d<80)
{
  maxresult =0;
}
  }
  
if(320<maxresult && maxresult<350)
  touche = 1;
  else if(350<maxresult && maxresult<365)
  touche = 2;
  else if(365<maxresult && maxresult<600)
  touche = 3;
  else if(maxresult<320)
  touche = 0;

  switch(touche){
    case 0:
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    break;
    case 1:
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    //maxresult =0;
    break;
    case 2:
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    //maxresult =0;
    break;
    case 3:
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    //maxresult =0;
    break;    
  }
    

 //   plot(v,0);              //-Display
 //   plot(results[d],1);
 //   delayMicroseconds(1);



PlottArray(1,freq,results); 
 

  TOG(PORTB,0);           //-Toggle pin 8 after each sweep (good for scope)

   }

  
