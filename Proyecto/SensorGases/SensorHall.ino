#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
int s_analogica_mq135=0;
int aire =12;
int CO2=13;
int gas=14;
String Tipodegas = "0";

// Libreria para ayudas(guardar,recuperar o guardar algo).
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Aqui definimos al wifi que nos vamos a conectar */
#define WIFI_SSID "Johan s"
#define WIFI_PASSWORD "87654321"


/* 2. Definimos la API Key (llave de acceso) */
#define API_KEY "AIzaSyDQRNr-U-sYpuycHp2_Dt5gApqkLLIwwNU"

/* 3. URL de fire base para mostrar los datos en la plataforma */
#define DATABASE_URL "detector-de-gases-6fed7-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. correo que tenemos enlazado nuestro fire base*/
#define USER_EMAIL "jordiastaiza@unicomfacauca.edu.co"
#define USER_PASSWORD "1061686873"

// Definimos el objeto de datos 
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

void setup()
{

  Serial.begin(9600);
  pinMode(aire,OUTPUT);
  pinMode(CO2,OUTPUT);
  pinMode(gas,OUTPUT);
 
  digitalWrite(aire,LOW);
  digitalWrite(CO2,LOW);
  digitalWrite(gas,LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Asiganmos api key (requerida) */
  config.api_key = API_KEY;

  /* Asiganmos credenciales de la red */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Asignamos URL  */
  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.

  Firebase.begin(&config, &auth);


  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

  
}
//Bucles con sus respectivas condiciones
void loop()
{
 s_analogica_mq135 = analogRead(36);      
  Serial.println(s_analogica_mq135, DEC);
  Serial.println(" ppm");
  delay(1000);
  //aire
  if(s_analogica_mq135<=200) //Definimos la condicion que queremos
  {
    Tipodegas = "aire normal ";
  Serial.println("aire normal");
  digitalWrite(aire,LOW);
  digitalWrite(CO2,LOW);
  digitalWrite(gas,LOW);
  
  }
//aire con dioxido humano
  if(s_analogica_mq135>200 && s_analogica_mq135<=400)//Definimos la condicion que queremos
  {
    Tipodegas = "aire con un poco de CO2 ";
  Serial.println("aire con un poco de CO2");
  digitalWrite(aire,HIGH);
  digitalWrite(CO2,LOW);
  digitalWrite(gas,LOW);

  }
 //dioxido de carbono
  if(s_analogica_mq135>400 && s_analogica_mq135<=800)//Definimos la condicion que queremos
  {
    Tipodegas = "dioxido de carbono CO2 ";
  Serial.println(" dioxido de carbono CO2");
  digitalWrite(aire,LOW);
  digitalWrite(CO2,HIGH);
  digitalWrite(gas,LOW);
 
  }
  //gas propano y butano
  if(s_analogica_mq135>800)//Definimos la condicion que queremos
  {
    Tipodegas = "propano butanoo ";
  Serial.println("propano butano 22");
  digitalWrite(aire,LOW);
  digitalWrite(CO2,LOW);
  digitalWrite(gas,HIGH);
  
  }
  // Firebase.ready() should be called repeatedly to handle authentication tasks.

 if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    Serial.printf("Set Gases... %s\n", Firebase.setString(fbdo, F("/Gas:/Tipo"), Tipodegas) ? "ok" : fbdo.errorReason().c_str());
   Serial.printf("Set Gases... %s\n", Firebase.setString(fbdo, F("/Gas:/PPM"), s_analogica_mq135) ? "ok" : fbdo.errorReason().c_str());
    
    Serial.println();

    

    
    count++;
    
  }
  delay(1000);
}
