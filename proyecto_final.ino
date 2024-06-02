#include <Key.h>
#include <Keypad.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 5     // Pin al que está conectado el sensor DHT11
#define DHTTYPE DHT11   // Define el tipo de sensor DHT

DHT dht(DHTPIN, DHTTYPE);

int buzzer = 4;

LiquidCrystal_I2C lcd(0x27, 16, 2);
//Linea de comunicacion ideal para pasar datos por el i2c

const int FILAS = 4;
const int COLUMNAS = 4;
char keys[FILAS][COLUMNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinesFilas[FILAS] = {13,12,11,10};
byte pinesColumnas[COLUMNAS] = {9,8,7,6};
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

String entradaUsuario = "";




enum Estado {BIENVENIDA, CONFIGURACION, OPCIONES,ACEPTAR_RECHAZAR}; // Definición de estados
Estado estadoActual = BIENVENIDA; // Estado inicial
int menuActual = 1;   // Menú actual
int cultivoActual =1; //Cultivo actual
int riegoActual=1;
int cultivoAux=1;


int temperatura;

int motoBomba = 3;
int humedad =0;
int humedad2 =0;
float temperatura1 = 0;
float humedad1 = 0;


unsigned long tiempo;
unsigned long tiempo1;
unsigned long tiempo2;
bool riegoActivo = false;
bool cultivoMostrado = false;

//Menu de Bienvenida
void mostrarBienvenida() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   BIENVENIDO  ");
  lcd.setCursor(0, 1);
  lcd.print("      GEFE");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    RIEGO DE  ");
  lcd.setCursor(0, 1);
  lcd.print("     PRUEBA");
  delay(1000); 
}

//Configuracion de los eventos cuando se va a cambiar el cultivo
void cambiarCultivo(int cultivo) {
  
  riegoActivo = true;
  switch (cultivo) {
   case 1:
    lcd.clear();
      lcd.print("1)     TOMATE ");
      lcd.setCursor(0, 1);
      lcd.print("RIEGO 2H T 24");
      tiempo1=7200000;
      break;
    case 2:
      lcd.clear();
      lcd.print("2) FRIJOL ROJO ");
      lcd.setCursor(0, 1);
      lcd.print("RIEGO 2.5H T 25");
      tiempo1=9000000;
      
    
      break;
    case 3:
    lcd.clear();
      lcd.print("3) FRIJOL NEGRO ");
      lcd.setCursor(0, 1);
      lcd.print("RIEGO 2.5H T 25");
      tiempo1=9000000;
      
    
      break;
    case 4:
    lcd.clear();
      lcd.print("4)   GARBANZOS  ");
      lcd.setCursor(0, 1);
      lcd.print("RIEGO 2.5H T 20");
      tiempo1=9000000; 
      
      break;
    case 5:
    lcd.clear();
      lcd.print("5)  ARBEJAS ");
      lcd.setCursor(0, 1);
      lcd.print("RIEGO 1.5H T 20");
      tiempo1=5400000;

      break;
    case 6:
    lcd.clear();
      lcd.print("6)    HABAS");
      lcd.setCursor(0, 1);
      lcd.print("RIEGO 2H T 20 ");
      tiempo1=7200000;

      break;
    case 7:
    lcd.clear();
      lcd.print("7)  LECHUGA ");
      lcd.setCursor(0, 1);
      lcd.print("RIEGO 2H T 20 ");
      tiempo1=7200000;
 
      break;
    case 8:
    lcd.clear();
      lcd.print("8)   PAPAYA ");
      lcd.setCursor(0, 1);
      lcd.print("RIEGO 2H T 25 ");
      tiempo1=7200000;
      
      break;
    case 9:
    lcd.clear();
      lcd.print("9)     MENTA ");
      lcd.setCursor(0, 1);
      lcd.print("RIEGO 2H T 24");
   	  tiempo1=7200000;
      break;
    case 10:
    lcd.clear();
      lcd.print(" SELECCIONAR EL ");
      lcd.setCursor(0, 1);
      lcd.print("    CULTIVO     ");
    break;
   
  }
}

//Menu para mostrar los diferentes menus de configuracion
void mostrarMenu(int menu) {
  lcd.clear();

  switch (menu) {
    case 1:
      lcd.print("CONFIG: 1)TOMATE ");
      lcd.setCursor(0, 1);
      lcd.print("2)FRIJOL 3)POROTO");
      break;
    case 2:
      lcd.print("4)GARBANZOS");
      lcd.setCursor(0, 1);
      lcd.print("5)ARBEJA 6)HABAS");
      break;
    case 3:
      lcd.print("7)LECHUGA");
      lcd.setCursor(0, 1);
      lcd.print("8)PAPAYA 9)MENTA");
      break;
    case 4:
      lcd.print("0)REINICIAR");
      lcd.setCursor(0, 1);
      lcd.print("*)SALIR #CONFIG");
      break;
  }
}
//Menu para mostrar el Cultivo actual
void mostrarCultivo(int temperatura, int humedad_entera, int cultivoActual) {
  lcd.clear();
  switch (cultivoActual) {
    case 1:
      lcd.print("  TOMATE ");
      break;
    case 2:
      lcd.print("FRIJOL NEGRO ");
      break;
    case 3:
      lcd.print("FRIJOL ROJO ");
      break;
    case 4:
      lcd.print("  GARBANZOS ");
      break;
    case 5:
    lcd.setCursor(0,0);
      lcd.print("  ARBEJA ");
      break;
    case 6:
      lcd.print("    HABAS ");
      break;
    case 7:
      lcd.print("  LECHUGA ");
      break;
    case 8:
      lcd.print("   PAPAYA ");
      break;
    case 9:
      lcd.print("    MENTA ");
      break;
  }
  lcd.setCursor(0, 1);
  lcd.print("T: ");
  lcd.print(temperatura);
  lcd.print("C H: ");
  humedad=humedad-1023;
  humedad*=-1;
  lcd.print(humedad/10.23);
  lcd.print("%");
}

void setup() {
  //Inicializacion de las variables de entorno
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  lcd.begin(16, 2);
  mostrarBienvenida();
  pinMode(motoBomba, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(buzzer, OUTPUT);
  dht.begin(); // Inicializar el sensor DHT11
 
}

void loop(){
char TECLA = teclado.getKey();//Entrada de caracteres por teclado
  
  
  tiempo =millis();
  float temperatura = dht.readTemperature(); // Leer la temperatura en grados Celsius

  // Verificar si hubo un error al leer el sensor
  if (isnan(temperatura)) {
    Serial.println("Error al leer del sensor DHT11!");
    return;
  }
  //Humedad
  humedad2=analogRead(A1);
  humedad = analogRead(A2);
  
  int humedad_entera = map(humedad, 0, 1023, 0, 100); // Mapear el valor de humedad de 0 a 1023 a un rango de 0 a 100
  
  int temperatura_entera = int(temperatura);

humedad2=(humedad2-1023)*-1;  
Serial.println(humedad2);

if (humedad2 <= 400) {
    digitalWrite(buzzer, HIGH);
  } else {
    if 
    digitalWrite(buzzer, LOW); // Apagar el buzzer si la temperatura es mayor a 20 
  }
 if (tiempo -tiempo2>=10000)
 { 
  tiempo2=tiempo;
  if (tiempo > 3000) {
    digitalWrite(motoBomba, HIGH);
    if (!cultivoMostrado) {  // Verificar si el cultivo ya se ha mostrado
      mostrarCultivo(temperatura, humedad, cultivoActual);
      cultivoMostrado = true;  // Marcar que el cultivo se ha mostrado
    } else if (temperatura1 != temperatura || humedad1 != humedad) {
      temperatura1 = temperatura;
      humedad1 = humedad;
      cultivoMostrado = false;
     
    }


  } else {
    digitalWrite(motoBomba, HIGH);  // Encender la bomba si no ha pasado el tiempo
    cultivoMostrado = false;        // Reiniciar la variable para la próxima vez que se alcance el tiempo
  }
  
 } 
  if (TECLA) {
    Serial.print(TECLA);
    entradaUsuario += TECLA;
    delay(200);
    lcd.clear();

    switch (estadoActual) {

      case BIENVENIDA:
        if (TECLA == '#') {
          estadoActual = CONFIGURACION;
          mostrarMenu(menuActual);
        }
      
        else {
          
          mostrarCultivo(temperatura,humedad,cultivoActual);
          
        }
        break;
      
      case CONFIGURACION:
        // muestra los diferentes menus que hay

        if (TECLA == 'A') {
          menuActual++;
          if (menuActual > 4) {
            menuActual = 1;
          }
          mostrarMenu(menuActual);
        }
        else if (TECLA == 'B') {
          menuActual--;
          if (menuActual < 1) {
            menuActual = 4;
          }
          mostrarMenu (menuActual);
        }
    	else if (TECLA == 'C') { // Si se presiona 'C', cambia al estado de OPCIONES
    	
       lcd.clear();
       lcd.print(" CAMBIAR CULTIVO");
       lcd.setCursor(0, 1);
       lcd.print("                ");
          estadoActual = OPCIONES;  
        }
      else if (TECLA=='*'){
      estadoActual=BIENVENIDA;
        mostrarMenu(menuActual);
      }
       
  
        break;
      case OPCIONES:
      
      
    
      if (TECLA == 'A') {
          riegoActual++;
          if (riegoActual > 10) {
            riegoActual = 1;
          }
          cambiarCultivo(riegoActual);
        }
        else if (TECLA == 'B') {
          riegoActual--;
          if (riegoActual < 1) {
            riegoActual = 10;
          }
		  cambiarCultivo(riegoActual);
        }
      	else if (TECLA == '*') { 
    	  estadoActual = CONFIGURACION;
    	  mostrarMenu(menuActual);  	
        }
        else if (TECLA >= '1' && TECLA <= '9') { 
   		 cultivoActual = TECLA - '0'; 
          
           estadoActual = ACEPTAR_RECHAZAR;
               
      lcd.clear();
       lcd.print("    ACEPTAR  C  ");
       lcd.setCursor(0, 1);
       lcd.print("    RECHAZAR  D  ");
    	  
    	 
        }
          else if (TECLA=='#'){
            
          estadoActual = CONFIGURACION;
    	  mostrarMenu(menuActual);
            
          }
      else{
        lcd.print("  ERROR   DE    ");
        lcd.setCursor(0, 1);
        lcd.print("     VALIDACION   ");
        
        estadoActual = BIENVENIDA;
    	  mostrarMenu(menuActual);
         
      }
      break;
      
 case ACEPTAR_RECHAZAR:
      
      if (TECLA=='C'){
         mostrarCultivo(temperatura,humedad,cultivoActual);
        cultivoAux=cultivoActual;
      }
      else if (TECLA=='D'){
        
        mostrarCultivo(temperatura,humedad,cultivoAux);
        cultivoActual=cultivoAux;
      }
      else if (TECLA=='#'){
        estadoActual = CONFIGURACION;
          mostrarMenu(menuActual);
      }
       
      
      break;
    }//cierra el swhich
  }
}