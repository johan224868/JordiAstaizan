#include <WiFi.h>
#include <FirebaseESP32.h>

int t_inicial = 500; 
int contador = 0;
int totalRevoluciones = 0; 
unsigned long tiempoAnterior = 0;
unsigned long tiempoUltimaRevolucion = 0;
bool estadoAnterior = HIGH; 
const int debounceDelay = 50;
unsigned long ultimoDebounceTiempo = 0;

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "Johan s"
#define WIFI_PASSWORD "987654321"
#define API_KEY "AIzaSyDQRNr-U-sYpuycHp2_Dt5gApqkLLIwwNU"
#define DATABASE_URL "https://console.firebase.google.com/project/sensor-7945f/database/sensor-7945f-default-rtdb/data/~2F?hl=es-419"
#define USER_EMAIL "sebastianmunoz@unicomfacauca.edu.co"
#define USER_PASSWORD "1002967211"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

void setup() {
  pinMode(27, INPUT_PULLUP);
  Serial.begin(9600);

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
  unsigned long tiempoActual = millis(); 

  if (tiempoActual - tiempoAnterior >= t_inicial) {
    Serial.print("Revoluciones en el periodo: ");
    Serial.println(contador);
    Serial.print("Total de revoluciones: ");
    Serial.println(totalRevoluciones);

    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
      sendDataPrevMillis = millis();
      String path = "/Revoluciones/";
      Firebase.setInt(fbdo, path + "contador", contador);
      Firebase.setInt(fbdo, path + "totalRevoluciones", totalRevoluciones);
    }

    tiempoAnterior = tiempoActual;
    contador = 0;
  }

  bool estadoActual = digitalRead(27);

  if (estadoAnterior == HIGH && estadoActual == LOW) {
    if (tiempoActual - ultimoDebounceTiempo > debounceDelay) {
      unsigned long tiempoEntreRevoluciones = tiempoActual - tiempoUltimaRevolucion;
      tiempoUltimaRevolucion = tiempoActual;
      Serial.print("Tiempo entre revoluciones: ");
      Serial.println(tiempoEntreRevoluciones);
      contador++;
      totalRevoluciones++;
      ultimoDebounceTiempo = tiempoActual;
    }
  }

  estadoAnterior = estadoActual;
}
