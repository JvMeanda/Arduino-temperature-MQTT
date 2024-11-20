#include <ESP8266WiFi.h> // Biblioteca para conexão Wi-Fi
#include <PubSubClient.h> // Biblioteca para comunicação MQTT
#include <OneWire.h> // Biblioteca para comunicação com o sensor de temperatura
#include <DallasTemperature.h> // Biblioteca para o sensor de temperatura DS18B20

// Configurações de rede Wi-Fi
const char* ssid = "******"; // Nome da sua rede Wi-Fi
const char* password = "******"; // Senha da sua rede Wi-Fi

// Configurações do MQTT
const char* mqtt_broker = "******"; // Endereço do servidor MQTT. Exemplo: test.mosquitto.org
const int mqtt_port = ****; // Porta do servidor MQTT. Exemplo: 1883
const char* mqtt_topic = "sensor/temperature"; // Tópico MQTT onde a temperatura será publicada
const char* mqtt_client_id = "ESP8266Client"; // Identificador único do cliente MQTT

// Pinos
const int ONE_WIRE_BUS = D2; // Pino positivo do sensor de temperatura conectado à entrada do ESP266
const int BUZZER_PIN = D5;    // Pino positivo do buzzer conectado à entrada do ESP266

// Configuração do sensor de temperatura
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Configuração da conexão Wi-Fi e MQTT
WiFiClient espClient; // Cria um cliente Wi-Fi
PubSubClient client(espClient); // Cria um cliente MQTT usando o cliente Wi-Fi

// Variáveis para medições de tempo
unsigned long sensorStartTime = 0;
unsigned long sensorEndTime = 0;
unsigned long sensorElapsedTime = 0;

unsigned long actuatorStartTime = 0;
unsigned long actuatorEndTime = 0;
unsigned long actuatorElapsedTime = 0;

void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial para debugging
  pinMode(BUZZER_PIN, OUTPUT); // Configura o pino do buzzer como saída

  setup_wifi(); // Conecta ao Wi-Fi
  client.setServer(mqtt_broker, mqtt_port); // Configura o servidor MQTT
  sensors.begin(); // Inicializa o sensor de temperatura

  connect_mqtt(); // Conecta ao broker MQTT
}

// Função para conectar ao Wi-Fi
void setup_wifi() {
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP()); // Imprime o endereço IP do ESP8266
}

// Função para conectar ao broker MQTT
void connect_mqtt() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("Conectado ao MQTT!");
    } else {
      Serial.print("Falha na conexão MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos.");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    connect_mqtt();
  }
  client.loop();

  // Início da medição de tempo para sensor até MQTT
  sensorStartTime = millis();

  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0); // Leitura da temperatura em graus Celsius

  if (temperatureC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperatura: ");
    Serial.println(temperatureC);

    // Conversão para string e publicação via MQTT
    char tempString[16];
    snprintf(tempString, sizeof(tempString), "%.2f", temperatureC);
    client.publish(mqtt_topic, tempString);

    // Fim da medição de tempo para sensor até MQTT
    sensorEndTime = millis();
    sensorElapsedTime = sensorEndTime - sensorStartTime;
    Serial.print("Tempo do sensor até MQTT: ");
    Serial.print(sensorElapsedTime);
    Serial.println(" ms");

    // Acionamento do buzzer se a temperatura exceder 35°C
    if (temperatureC > 35.0) {
      // Início da medição de tempo para o atuador
      actuatorStartTime = millis();

      for (int i = 0; i < 3; i++) {
        tone(BUZZER_PIN, 1000); // Emite um som no buzzer (1000Hz)
        delay(200);
        noTone(BUZZER_PIN);
        delay(500);
      }

      // Fim da medição de tempo para o atuador
      actuatorEndTime = millis();
      actuatorElapsedTime = actuatorEndTime - actuatorStartTime;
      Serial.print("Tempo para acionar o buzzer: ");
      Serial.print(actuatorElapsedTime);
      Serial.println(" ms");
    }
  } else {
    Serial.println("Erro de leitura do sensor de temperatura!");
  }

  delay(2000); // Aguarda 2 segundos antes da próxima leitura
}