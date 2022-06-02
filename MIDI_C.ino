/* Controlador MIDI
Este código funciona para configurar cualquier número de botones y potenciómetros 
los botones se conectan a partir del pin 2 digital, (2,3,4,5...)
los potenciómetros se conectan a partir de A0,A1....
Para usar el código sólo hay que modificar nBotones, notas[] y nPots  */

const int nBotones = 12; //este es el número de botones
const int calibracion=10; //este valor funciona como una especie de delay
boolean notaioff[nBotones];
int contador[nBotones];
//el array notas[] debe tener un mismo número de elementos
//que el número de botones, pues cada nota corresponde a un botón
// BORRADOR byte notas[] = {34,36,39,43}; //estás son las notas que se dispararán los botones
byte notas[] = {36,37,38,39,40,41,42,43,44,45,46,47}; //estás son las notas que se dispararán los botones


const int nPots=4; //número de potenciómetros
int lecturas[nPots]; 
int lecturasAnteriores[nPots];
void setup() {

 Serial.begin(115200);
  for (int i=0;i<nBotones;i++)
  {
   pinMode(i+2, INPUT_PULLUP);
  }
}

void loop() {
  for (int i=0; i<nBotones; i++) //la variable i recorre los números del 2 al 6
  {
      if (digitalRead(i+2) == LOW) //botón presionado
      {
        if (contador[i]==0)//cuenta regresiva terminada ?
        {
          if (notaioff[i]== 1) //¿la nota esta apagada?
          {
            contador[i]=calibracion; //valor de cuenta regresiva 
            midi(144,notas[i],100); //se envía la nota
            notaioff[i] = 0; //la nota no esta apagada (esta encendida)
          }
        }
        
      }
      else //botón sin presionar (posible envio de Note Off)
      {
        if (contador[i]==0) //cuenta regresiva terminada ?
        {
          if (notaioff[i] ==0) //¿La nota esta esta activada?
          {
            contador[i]=calibracion; //valor de cuenta regresiva 
            midi(128,notas[i],0); //envio de note off
            notaioff[i] = 1;  //la nota ya no está encendida
          }
        }
      }
  }
  for (int i=0; i<nBotones;i++)
  //cuentas regresivas
  {
  if (contador[i]>0) contador[i]--;
  }

  for (int k=0; k<nPots; k++) //se hace un recorrido desde k=0 hasta que k sea menor a nPots
    {
      lecturas[k] = map(analogRead(k),0,1023,0,255); //mapeamos al rango al doble del CC
    }

  for (int k=0; k<nPots;k++)
  {    //vemos si hubo un cambio en el valor de los potenciometros
      if (lecturas[k] > (lecturasAnteriores[k]+1) || lecturas[k] < (lecturasAnteriores[k]-1) )
        {                   
            midi(176,k+30,lecturas[k]/2); //mándamos a partir del CC 30
            lecturasAnteriores[k] = lecturas[k];
        } 
  }
  
}

void midi(unsigned char command, unsigned char note,unsigned char vel){
  Serial.write(command);
  Serial.write(note);
  Serial.write(vel);
}
