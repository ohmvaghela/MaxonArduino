bool Aold = false;
bool Bold = false;
bool Anew = false;
bool Bnew = false;

long pulse = 0;
float angle = 0;

const byte ChA = 21;
const byte ChB = 20;

void setup() {
  Serial.begin(115200);

  pinMode(ChA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ChA), changeA, CHANGE);
  pinMode(ChB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ChB), changeB, CHANGE);

}

void loop() {

  Serial.print("Angle : ");
  Serial.println(angle);
  updateAngle();
}

void updateAngle()
{
  if(pulse<0)pulse = (16384);//4*4096 from data sheet
  pulse = pulse%(16384);
  angle = (float)pulse*360/(16384);
}

void changeA()
{
  Anew = !Anew;
  updatePulse();
}
void changeB()
{
  Bnew = !Bnew;
  updatePulse();
}


void updatePulse(){
/* Pulse ++
 * A : HIGH-LOW  | B : HIGH-HIGH
 * A : LOW -LOW  | B : HIGH-LOW 
 * A : LOW -HIGH | B : LOW -LOW 
 * A : HIGH-HIGH | B : LOW -HIGH
 */
/* Pulse--
 * A : LOW  - HIGH  | B : HIGH - HIGH
 * A : LOW  - LOW   | B : LOW  - HIGH
 * A : HIGH - LOW   | B : LOW  - LOW 
 * A : HIGH - HIGH  | B : HIGH - LOW 
 */

       if(  Aold && !Anew &&  Bold  &&  Bnew ) pulse++;
  else if( !Aold && !Anew &&  Bold  && !Bnew ) pulse++;
  else if( !Aold &&  Anew && !Bold  && !Bnew ) pulse++;
  else if(  Aold &&  Anew && !Bold  &&  Bnew ) pulse++;
 
  else if(  Anew && !Aold &&  Bnew  &&  Bold ) pulse--;
  else if( !Anew && !Aold &&  Bnew  && !Bold ) pulse--;
  else if( !Anew &&  Aold && !Bnew  && !Bold ) pulse--;
  else if(  Anew &&  Aold && !Bnew  &&  Bold ) pulse--;

  Aold = Anew;
  Bold = Bnew;
}
