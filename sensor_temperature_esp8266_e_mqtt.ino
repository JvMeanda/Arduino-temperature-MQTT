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
const int ONE_WIRE_BUS = D2; //Pino positivo do sensor de temperatura conectado a entrada do ESP266
const int BUZZER_PIN = D5; //Pino positivo do buzzer conectado a entrada do ESP266

// Configuração do sensor de temperatura
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Configuração da conexão Wi-Fi e MQTT
WiFiClient espClient; // Cria um cliente Wi-Fi
PubSubClient client(espClient); // Cria um cliente MQTT usando o cliente Wi-Fi

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

  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0); // temperatura em graus Celsius

  if (temperatureC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperatura: ");
    Serial.println(temperatureC);

    char tempString[16]; // Temperatura em formato string
    snprintf(tempString, sizeof(tempString), "%.2f", temperatureC); // Temperatura com duas casas decimais
    client.publish(mqtt_topic, tempString); // Publica a temperatura no tópico MQTT

    if (temperatureC > 35.0) { // Aciona o buzzer se a temperatura for maior que 35°C
    for (int i = 0; i < 3; i++) {
      tone(BUZZER_PIN, 1000); // Emite um som no buzzer (1000Hz)
      delay(200);
      noTone(BUZZER_PIN);
      delay(500);
    }
      Serial.println("Buzzer ativado!");
    }
  } else {
    Serial.println("Erro de leitura do sensor de temperatura!");
  }

  delay(2000); // Aguarda 2 segundos antes da próxima leitura
}