
# Projeto Arduíno de sensor de temperatura com minicontrolador ESP8266 e protocolo MQTT

Este projeto é um sistema de monitoramento de temperatura usando o microcontrolador **ESP8266**, o sensor de temperatura **DS18B20**, um **buzzer ativo** de 5V e um **resistor** de 4.7kΩ, conectados a uma **breadboard**. O sistema monitora a temperatura e envia os dados para um broker MQTT. Caso a temperatura ultrapasse **35°C**, o buzzer é ativado e emite um som como alerta.

# Funcionalidades

- **Monitoramento de Temperatura**: Leitura de dados de temperatura usando o sensor DS18B20.
- **Comunicação MQTT**: Envia dados de temperatura para um broker MQTT
- **Sistema de Alerta**: Ativa um buzzer quando a temperatura ultrapassa 35°C.

# Componentes:

- ESP8266
- Sensor de Temperatura DS18B20
- Resistor de 4.7kΩ
- Buzzer ativo de 5V
- Protoboard e fios jumper

# Passo a Passo Arduíno IDE:

- Para que o **ESP8266** seja reconhecido pelo computador, é necessário instalar o driver **CP210x**. Faça o download clicando no link abaixo:

- [Driver CP210x para Windows, Mac e Linux](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)

- Pesquise no **Gerenciador de arquivos** do Windos se a porta **COM** foi instalada corretamente

- No menu do Arduíno IDE (lado superior esquerdo), vá em **File->Preferences** e cole o seguinte comando no final da página: https://arduino.esp8266.com/stable/package_esp8266com_index.json

- No menu vá em **Tools->Board->Boards Manager.** Ao final da página instale o arquivo **esp8266 by ESP8266 Community**

- Por fim, vá em **Tools->Board:"Arduino Yun"** e selecione **NodeMCU 1.0** ou **NodeMCU 0.9**

# Bibliotecas Necessárias do Arduino IDE
- ESP8266WiFi 
- PubSubClient 
- OneWire 
- DallasTemperature 

# Configuração do código

Altere as seguintes variáveis do código com os dados da sua rede wifi, broker e port escolhidos:

```
const char* ssid = ""; // Nome da sua rede WIFI
const char* password = ""; // Senha da sua rede WIFI
const char* mqtt_broker = ""; // Endereço do broker MQTT escolhido
const int mqtt_port = 1883; // Porta do broker MQTT escolhido
```
# Comunicação com o MQTT

Utilizei o aplicativo MyMQTT no celular e conectei o **Host** com o endereço do broker (mqtt_broker), a **Port** foi a porta do broker (mqtt_port) 

- As opções de **SSL** vai ficar desativada e a versão do **MQTT** é o **V3**
- As opções de **Credentials** vão estar em branco, ou caso o seu broker seja privado, coloque as informações necessárias, como **UserName** e **Password** correspondente ao seu broker.

- Após conectado, vá no menu **Subscribe** e em **Topic** coloque o valor do **const char mqtt_topic** presente no código (sensor/temperature).

- Volte para o menu de **Dashboard** e será mostrado os valores da temperatura.

- A primeira leitura demora em torno de **10 a 5 minutos**, dependendo do broker utilizado.
