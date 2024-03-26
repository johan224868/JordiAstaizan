#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include <SevenSeg.h>

String numeros;
const int numOfDigits = 4;

SevenSeg disp(5, 7, 4, 11, 10, 3, 6);

int digitPins[numOfDigits] = {13, 12, 9, 8};

#define WIFI_SSID "Johan s"
#define WIFI_PASSWORD "987654321"

#define API_KEY "AIzaSyDQRNr-U-sYpuycHp2_Dt5gApqkLLIwwNU"
#define DATABASE_URL "detector-de-gases-6fed7-default-rtdb.firebaseio.com/"
#define USER_EMAIL "sebastianmunoz@unicomfacauca.edu.co"
#define USER_PASSWORD "1002967211"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;
double periodo = 60000;
int contador = 0;
unsigned long tiempoAhora = 0;
int valorFijado = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  disp.setDigitPins(numOfDigits, digitPins);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
}

void loop() {
  if (millis() > (periodo + tiempoAhora)) {
    valorFijado = contador;
    Serial.println(contador);
    sendDataToFirebase(); // Envía el valor actual de contador a Firebase
    tiempoAhora = millis();
    contador = 0;
  }

  if (digitalRead(2) == 0) {
    delay(300);
    contador++;
  }

  disp.write(valorFijado);
}

void sendDataToFirebase() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    Serial.printf("Set Contador... %s\n", Firebase.setInt(fbdo, F("/Contador"), contador) ? "ok" : fbdo.errorReason().c_str());
    Serial.println();
    count++;
  }
}


    

    
    count++;
    
  }
  delay(1000);
}
