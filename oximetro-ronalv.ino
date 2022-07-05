// CABECERAS PARA OLED
#include <Arduino.h>
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// CABECERAS PARA TECLADO 4X3
#include <SimpleKeypad.h>
const byte nb_rows = 4;                         // four rows
const byte nb_cols = 3;                         // four columns
char key_chars[nb_rows][nb_cols] = {            // The symbols of the keys
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'S', '0', 'B'}
};
byte rowPins[nb_rows] = {15, 2, 0, 4};           // The pins where the rows are connected
byte colPins[nb_cols] = {5, 18, 19};           // The pins where the columns are connected
SimpleKeypad kp1((char*)key_chars, rowPins, colPins, nb_rows, nb_cols);   // New keypad called kp1

// CABECERAS PARA SENSOR MAX30102
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;
#define MAX_BRIGHTNESS 255
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

// CABECERAS PARA RF95 LORAWAN CHIP
#include "ttn.h"
#include <nvs_flash.h>
const char *appEui = "0000000000000000";// AppEUI (sometimes called JoinEUI)
const char *devEui = "70B3D57ED0052BB5";// DevEUI
const char *appKey = "48C61C5805678E732EFC9C6E68441E86";// AppKey
#define TTN_SPI_HOST HSPI_HOST
#define TTN_SPI_DMA_CHAN 1
#define TTN_PIN_SPI_SCLK 14
#define TTN_PIN_SPI_MOSI 32
#define TTN_PIN_SPI_MISO 25
#define TTN_PIN_NSS 33
#define TTN_PIN_RXTX TTN_NOT_CONNECTED
#define TTN_PIN_RST 12
#define TTN_PIN_DIO0 34
#define TTN_PIN_DIO1 35
bool joined = false;

// CABECERAS PARA TENSORFLOW LITE
#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "model.h"
namespace
{
tflite::ErrorReporter *error_reporter = nullptr;
const tflite::Model *model = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input = nullptr;
TfLiteTensor *output = nullptr;
constexpr int kTensorArenaSize = 2000;
uint8_t tensor_arena[kTensorArenaSize];
}

// DATOS
char dato1[8] = {0};
int dato2 = 0;
int dato3 = 0;
int dato4 = 0;
int dato5 = 0;
int dato6 = 0;
int dato7 = 0;
int dato8 = 0;
int dato9 = 0;
int dato10 = 0;
int dato11 = 0;
float dato12 = 0;
char datos[200] = {0};

// FUNCIONES
void setup() {
  Serial.begin(115200);

  // setup para oled
  u8x8.begin();
  u8x8.setPowerSave(0);

  // setup para max30102
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

  // setup para RF95 LORAWAN CHIP
  lora_init();

  // setup para tensorflow lite
  // Set up logging.
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure.
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION)
  {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    while (1);
  }
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk)
  {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);
}

void loop() {
  /*
    instruccion1();
    instruccion2();
    instruccion3();
    instruccion4();
    instruccion5();
    instruccion6();
    instruccion7();
    instruccion8();
    instruccion9();
    instruccion10();*/
  instruccion11();
  instruccion12();
  instruccion13();
  printDatos();

  // tx lorawan
  instruccion14();
  delay(5000);

}

void messageReceived(const uint8_t *message, size_t length, ttn_port_t port)
{
  printf("Message of %d bytes received on port %d:", length, port);
  for (int i = 0; i < length; i++)
    printf(" %02x", message[i]);
  printf("\n");
}

extern "C" void lora_init()
{
  esp_err_t err;

  // Initialize the GPIO ISR handler service
  err = gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
  ESP_ERROR_CHECK(err);

  // Initialize the NVS (non-volatile storage) for saving and restoring the keys
  err = nvs_flash_init();
  ESP_ERROR_CHECK(err);

  // Initialize SPI bus
  spi_bus_config_t spi_bus_config = { -1};
  spi_bus_config.miso_io_num = TTN_PIN_SPI_MISO;
  spi_bus_config.mosi_io_num = TTN_PIN_SPI_MOSI;
  spi_bus_config.sclk_io_num = TTN_PIN_SPI_SCLK;
  spi_bus_config.quadwp_io_num = -1;
  spi_bus_config.quadhd_io_num = -1;
  spi_bus_config.max_transfer_sz = 0;
  spi_bus_config.intr_flags = 0;
  spi_bus_config.flags = 0;

  err = spi_bus_initialize(TTN_SPI_HOST, &spi_bus_config, TTN_SPI_DMA_CHAN);
  ESP_ERROR_CHECK(err);

  // Initialize TTN
  ttn_init();

  // Configure the SX127x pins
  ttn_configure_pins(TTN_SPI_HOST, TTN_PIN_NSS, TTN_PIN_RXTX, TTN_PIN_RST, TTN_PIN_DIO0, TTN_PIN_DIO1);

  // The below line can be commented after the first run as the data is saved in NVS
  ttn_provision(devEui, appEui, appKey);

  // Register callback for received messages
  ttn_on_message(messageReceived);

}
bool lora_join()
{
  if (joined)
    return true;

  ttn_reset();
  printf("Joining.......\n");
  if ( ttn_join_provisioned())
  {
    joined = true;
    printf("Joined.\n");
    printf("    appEui:%s\n", appEui);
    printf("    devEui:%s\n", devEui);
    printf("    appKey:%s\n", appKey);
    return true;
  }

  printf("Join failed.\n");
  return false;
}

bool lora_transmit(uint8_t *data, int size)
{
  printf("Sending LoRa message:%s\n", (char*)data);
  ttn_response_code_t res = ttn_transmit_message(data, size, 1, false);
  printf(res == TTN_SUCCESSFUL_TRANSMISSION ? "Message sent.\n" : "Transmission failed.\n");
  bool result = (res == TTN_SUCCESSFUL_TRANSMISSION) ? true : false;
  if (result == false)
    joined = false;
  return result;
}

void printString(char* str, int len) {
  for (int i = 0; i < len; i++) {
    Serial.print(str[i]);
  }
  Serial.println("");
}

void printDatos() {
  Serial.println("- - DATOS ACUMULADOS - -");
  Serial.print("Dato1 (DNI): ");
  printString(dato1, strlen(dato1));
  Serial.print("Dato2 (sexo): ");
  Serial.println(dato2);
  Serial.print("Dato3 (edad): ");
  Serial.println(dato3);
  Serial.print("Dato4 (pescado): ");
  Serial.println(dato4);
  Serial.print("Dato5 (carne): ");
  Serial.println(dato5);
  Serial.print("Dato6 (viseras): ");
  Serial.println(dato6);
  Serial.print("Dato7 (vegetales): ");
  Serial.println(dato7);
  Serial.print("Dato8 (inapetencia): ");
  Serial.println(dato8);
  Serial.print("Dato9 (sueno): ");
  Serial.println(dato9);
  Serial.print("Dato10 (dolor): ");
  Serial.println(dato10);
  Serial.print("Dato11 (SPO2): ");
  Serial.println(dato11);
  Serial.print("Dato12 (hemoglobina): ");
  Serial.println(dato12);
}
int leer_spo2_de_max30102()
{
  while (1) {
    memset(irBuffer, 0, 100);
    memset(redBuffer, 0, 100);
    /*
      while (particleSensor.available() == false) //do we have new data?
      {
      Serial.println("Coloque dedo en el sensor");
      delay(1000);
      }
    */
    bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

    //read the first 100 samples, and determine the signal range
    for (int i = 0 ; i < bufferLength ; i++)
    {

      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data

      uint32_t redval = particleSensor.getRed();
      uint32_t irval = particleSensor.getIR();

      if ( redval < 10000 || irval < 10000)
      {
        i--;
        continue;
      }

      redBuffer[i] = redval;
      irBuffer[i] = irval;
      particleSensor.nextSample(); //We're finished with this sample so move to next sample

      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      Serial.print(F(", ir="));
      Serial.println(irBuffer[i], DEC);
    }

    //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second

    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data

      uint32_t redval = particleSensor.getRed();
      uint32_t irval = particleSensor.getIR();

      if ( redval < 10000 || irval < 10000)
      {
        i--;
        continue;
      }

      redBuffer[i] = redval;
      irBuffer[i] = irval;
      particleSensor.nextSample(); //We're finished with this sample so move to next sample
    }

    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    Serial.print(F(", HRvalid="));
    Serial.print(validHeartRate, DEC);

    Serial.print(F(", SPO2="));
    Serial.print(spo2, DEC);

    Serial.print(F(", SPO2Valid="));
    Serial.println(validSPO2, DEC);


    if (validHeartRate == 1 && validSPO2 == 1) {
      return spo2;
    }
    Serial.println("SPO2 invalido. Repetir proceso.");
  }
}
void instruccion1() {
  memset(dato1, 0, 8);
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "  Ingrese DNI");
    u8x8.drawString(0, 1, " y presione SI");

    // ACUMULAR DIGITOS DE DNI HASTA RECIBIR TECLA "S"
    char dni[8] = {0};
    char keypressed;
    int index = 0;
    int len = 0;
    bool stop = false;
    while (1) {
      char keypressed = kp1.getKey();
      if (!keypressed)                                     // If getKey returned any key
        continue;

      if (keypressed == 'S')
      {
        stop = true;
        goto printdni;
      }

      if (keypressed == 'B' )
      {
        index--;
        len--;
        if (index < 0)
          index = 0;
        if (len < 0)
          len = 0;
        if (len == 6)
          index = 6;
        dni[index] = 0;
        goto printdni;
      }

      dni[index] = keypressed;
      index++;
      len++;
      if (index > 6)
        index = 6;
      if (len > 7)
        len = 7;

printdni:
      char stringdni[30] = {0};
      sprintf(stringdni, " % 7s", dni);
      u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
      u8x8.drawString(0, 4, stringdni);
      printString(dni, len);
      if (stop == true)
        break;
    }

    // VERIFICAR DNI CON 7 DIGITOS
    if ( len != 7 )
      continue;

    // GUARDAR DNI EN DATO 1
    memcpy(dato1, dni, 8);
    Serial.print("Dato 1 (DNI): ");
    printString(dato1, strlen(dato1));
    break;
  }
}
void instruccion2() {
  dato2 = 0;
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "Ingrese el Sexo");
    u8x8.drawString(0, 1, " del Paciente");
    u8x8.drawString(0, 3, " 1 = Femenino");
    u8x8.drawString(0, 4, " 2 = Masculino");

    // ESPERAR RESPUESTA DEL PACIENTE
    int sexo = 0;

    while (1) {
      char keypressed = kp1.getKey();
      if (!keypressed)                                     // If getKey returned any key
        continue;
      sexo = keypressed - '0';
      break;
    }

    // VERIFICAR RESPUESTA (1 o 2)
    if ( sexo != 1 && sexo != 2 )
      continue;

    // GUARDAR DNI EN DATO 1
    dato2 = sexo;
    Serial.print("Dato 2 (sexo): ");
    Serial.println(dato2);
    break;
  }

}
void instruccion3() {
  dato3 = 0;
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "Ingrese su edad");
    u8x8.drawString(0, 1, "y presione Si");

    // ACUMULAR DIGITOS DE EDAD HASTA RECIBIR TECLA "S"
    char edad[4] = {0};
    char keypressed;
    int index = 0;
    int len = 0;
    bool stop = false;
    while (1) {
      char keypressed = kp1.getKey();
      if (!keypressed)                                     // If getKey returned any key
        continue;

      if (keypressed == 'S')
      {
        stop = true;
        goto printedad;
      }

      if (keypressed == 'B' )
      {
        index--;
        len--;
        if (index < 0)
          index = 0;
        if (len < 0)
          len = 0;
        if (len == 2)
          index = 2;
        edad[index] = 0;
        goto printedad;
      }

      edad[index] = keypressed;
      index++;
      len++;
      if (index > 2)
        index = 2;
      if (len > 3)
        len = 3;

printedad:
      char stringedad[30] = {0};
      sprintf(stringedad, " % 3s", edad);
      u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
      u8x8.drawString(4, 4, stringedad);
      printString(edad, len);
      if (stop == true)
        break;
    }

    // VERIFICAR EDAD
    bool edad_solonumeros = true;
    for (int i = 0; i < len; i++) {
      if ( !isdigit(edad[i]) )
      {
        edad_solonumeros = false;
        break;
      }
    }
    if ( !edad_solonumeros )
      continue;
    if ( atoi(edad) > 120 )
      continue;

    // GUARDAR EDAD EN DATO 3
    dato3 = atoi(edad);
    Serial.print("Dato 3 (edad): ");
    Serial.println(dato3);
    break;
  }
}
void instruccion4() {
  dato4 = 0;
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "En una escala de");
    u8x8.drawString(0, 1, "0 a 3, Cuantas");
    u8x8.drawString(0, 2, "veces por semana");
    u8x8.drawString(0, 3, "consume carne de");
    u8x8.drawString(0, 4, "pescado ? ");

    // RECIBIR RANGO DE CONSUMO DE PESCADO HASTA RECIBIR TECLA "S"
    char pescado[2] = {0};
    char keypressed;
    int index = 0;
    int len = 0;
    bool stop = false;
    while (1) {
      char keypressed = kp1.getKey();
      if (!keypressed)                                     // If getKey returned any key
        continue;

      if (keypressed == 'S')
      {
        stop = true;
        goto printPescado;
      }

      if (keypressed == 'B' )
      {
        index--;
        len--;
        if (index < 0)
          index = 0;
        if (len < 0)
          len = 0;
        if (len == 0)
          index = 0;
        pescado[index] = 0;
        goto printPescado;
      }

      pescado[index] = keypressed;
      index++;
      len++;
      if (index > 0)
        index = 0;
      if (len > 1)
        len = 1;

printPescado:
      char stringPescado[30] = {0};
      sprintf(stringPescado, " % 1s", pescado);
      u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
      u8x8.drawString(5, 6, stringPescado);
      printString(pescado, len);
      if (stop == true)
        break;
    }

    // VERIFICAR RANGO DE CONSUMO DE PESCADO
    bool pescado_solonumeros = true;
    for (int i = 0; i < len; i++) {
      if ( !isdigit(pescado[i]) )
      {
        pescado_solonumeros = false;
        break;
      }
    }
    if ( !pescado_solonumeros )
      continue;
    int pescado_num = atoi(pescado);
    if ( pescado_num != 0 && pescado_num != 1
         && pescado_num != 2 && pescado_num != 3 )
      continue;

    // GUARDAR CONSUMO DE PESSCADO EN DATO 4
    dato4 = atoi(pescado);
    Serial.print("Dato 4 (pescado) : ");
    Serial.println(dato4);
    break;
  }
}
void instruccion5() {
  dato5 = 0;
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "En una escala de");
    u8x8.drawString(0, 1, "0 a 3, Cuantas");
    u8x8.drawString(0, 2, "veces por semana");
    u8x8.drawString(0, 3, "consume carne de");
    u8x8.drawString(0, 4, "res, pollo, cerdo");
    u8x8.drawString(0, 5, "o pavo ? ");

    // RECIBIR RANGO DE CONSUMO DE PESCADO HASTA RECIBIR TECLA "S"
    char carne[2] = {0};
    char keypressed;
    int index = 0;
    int len = 0;
    bool stop = false;
    while (1) {
      char keypressed = kp1.getKey();
      if (!keypressed)                                     // If getKey returned any key
        continue;

      if (keypressed == 'S')
      {
        stop = true;
        goto printCarne;
      }

      if (keypressed == 'B' )
      {
        index--;
        len--;
        if (index < 0)
          index = 0;
        if (len < 0)
          len = 0;
        if (len == 0)
          index = 0;
        carne[index] = 0;
        goto printCarne;
      }

      carne[index] = keypressed;
      index++;
      len++;
      if (index > 0)
        index = 0;
      if (len > 1)
        len = 1;

printCarne:
      char stringCarne[30] = {0};
      sprintf(stringCarne, " % 1s", carne);
      u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
      u8x8.drawString(6, 6, stringCarne);
      printString(carne, len);
      if (stop == true)
        break;
    }

    // VERIFICAR RANGO DE CONSUMO DE CARNE
    bool carne_solonumeros = true;
    for (int i = 0; i < len; i++) {
      if ( !isdigit(carne[i]) )
      {
        carne_solonumeros = false;
        break;
      }
    }
    if ( !carne_solonumeros )
      continue;
    int carne_num = atoi(carne);
    if ( carne_num != 0 && carne_num != 1
         && carne_num != 2 && carne_num != 3 )
      continue;

    // GUARDAR CONSUMO DE CARNE EN DATO 5
    dato5 = atoi(carne);
    Serial.print("Dato 5 (carne) : ");
    Serial.println(dato5);
    break;
  }
}
void instruccion6() {
  dato6 = 0;
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "En una escala de");
    u8x8.drawString(0, 1, "0 a 3, Cuantas");
    u8x8.drawString(0, 2, "veces por semana");
    u8x8.drawString(0, 3, "consume viseras");
    u8x8.drawString(0, 4, "rojas(Bazo, higa");
    u8x8.drawString(0, 5, "do, bofe, corazon");
    u8x8.drawString(0, 6, "rinon, sangrecita");

    // RECIBIR RANGO DE CONSUMO DE VISERAS HASTA RECIBIR TECLA "S"
    char viseras[2] = {0};
    char keypressed;
    int index = 0;
    int len = 0;
    bool stop = false;
    while (1) {
      char keypressed = kp1.getKey();
      if (!keypressed)                               // If getKey returned any key
        continue;

      if (keypressed == 'S')
      {
        stop = true;
        goto printViseras;
      }

      if (keypressed == 'B' )
      {
        index--;
        len--;
        if (index < 0)
          index = 0;
        if (len < 0)
          len = 0;
        if (len == 0)
          index = 0;
        viseras[index] = 0;
        goto printViseras;
      }

      viseras[index] = keypressed;
      index++;
      len++;
      if (index > 0)
        index = 0;
      if (len > 1)
        len = 1;

printViseras:
      char stringViseras[30] = {0};
      sprintf(stringViseras, " % 1s", viseras);
      u8x8.setFont(u8x8_font_chroma48medium8_r);
      u8x8.drawString(6, 7, stringViseras);
      printString(viseras, len);
      if (stop == true)
        break;
    }

    // VERIFICAR RANGO DE CONSUMO DE VISERAS
    bool viseras_solonumeros = true;
    for (int i = 0; i < len; i++) {
      if ( !isdigit(viseras[i]) )
      {
        viseras_solonumeros = false;
        break;
      }
    }
    if ( !viseras_solonumeros )
      continue;
    int viseras_num = atoi(viseras);
    if ( viseras_num != 0 && viseras_num != 1
         && viseras_num != 2 && viseras_num != 3 )
      continue;

    // GUARDAR CONSUMO DE VISERAS EN DATO 6
    dato6 = atoi(viseras);
    Serial.print("Dato 6 (viseras): ");
    Serial.println(dato6);
    break;
  }
}
void instruccion7() {
  dato7 = 0;
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "En una escala de");
    u8x8.drawString(0, 1, "0 a 3, Cuantas");
    u8x8.drawString(0, 2, "veces por semana");
    u8x8.drawString(0, 3, "consume vegeta");
    u8x8.drawString(0, 4, "les y menestras");

    // RECIBIR RANGO DE CONSUMO DE VEGETALES HASTA RECIBIR TECLA "S"
    char vegetales[2] = {0};
    char keypressed;
    int index = 0;
    int len = 0;
    bool stop = false;
    while (1) {
      char keypressed = kp1.getKey();
      if (!keypressed)                                     // If getKey returned any key
        continue;

      if (keypressed == 'S')
      {
        stop = true;
        goto printVegetales;
      }

      if (keypressed == 'B' )
      {
        index--;
        len--;
        if (index < 0)
          index = 0;
        if (len < 0)
          len = 0;
        if (len == 0)
          index = 0;
        vegetales[index] = 0;
        goto printVegetales;
      }

      vegetales[index] = keypressed;
      index++;
      len++;
      if (index > 0)
        index = 0;
      if (len > 1)
        len = 1;

printVegetales:
      char stringVegetales[30] = {0};
      sprintf(stringVegetales, " % 1s", vegetales);
      u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
      u8x8.drawString(6, 6, stringVegetales);
      printString(vegetales, len);
      if (stop == true)
        break;
    }

    // VERIFICAR RANGO DE CONSUMO DE VEGETALES
    bool vegetales_solonumeros = true;
    for (int i = 0; i < len; i++) {
      if ( !isdigit(vegetales[i]) )
      {
        vegetales_solonumeros = false;
        break;
      }
    }
    if ( !vegetales_solonumeros )
      continue;
    int vegetales_num = atoi(vegetales);
    if ( vegetales_num != 0 && vegetales_num != 1
         && vegetales_num != 2 && vegetales_num != 3 )
      continue;

    // GUARDAR CONSUMO DE VEGETALES EN DATO 67
    dato7 = atoi(vegetales);
    Serial.print("Dato 7 (vegetales): ");
    Serial.println(dato7);
    break;
  }
}
void instruccion8() {
  dato8 = 0;
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "Presione 1 si");
    u8x8.drawString(0, 1, "ultimamente");
    u8x8.drawString(0, 2, "presenta inape");
    u8x8.drawString(0, 3, "tencia, sino");
    u8x8.drawString(0, 4, "presione 0");

    // RECIBIR RANGO DE INAPETENCIA HASTA RECIBIR TECLA "S"
    char inapetencia[2] = {0};
    char keypressed;
    int index = 0;
    int len = 0;
    bool stop = false;
    while (1) {
      char keypressed = kp1.getKey();
      if (!keypressed)                                     // If getKey returned any key
        continue;

      if (keypressed == 'S')
      {
        stop = true;
        goto printInapetencia;
      }

      if (keypressed == 'B' )
      {
        index--;
        len--;
        if (index < 0)
          index = 0;
        if (len < 0)
          len = 0;
        if (len == 0)
          index = 0;
        inapetencia[index] = 0;
        goto printInapetencia;
      }

      inapetencia[index] = keypressed;
      index++;
      len++;
      if (index > 0)
        index = 0;
      if (len > 1)
        len = 1;

printInapetencia:
      char stringInapetencia[30] = {0};
      sprintf(stringInapetencia, " % 1s", inapetencia);
      u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
      u8x8.drawString(6, 6, stringInapetencia);
      printString(inapetencia, len);
      if (stop == true)
        break;
    }

    // VERIFICAR RANGO DE INAPETENCIA
    bool inapetencia_solonumeros = true;
    for (int i = 0; i < len; i++) {
      if ( !isdigit(inapetencia[i]) )
      {
        inapetencia_solonumeros = false;
        break;
      }
    }
    if ( !inapetencia_solonumeros )
      continue;
    int inapetencia_num = atoi(inapetencia);
    if ( inapetencia_num != 0 && inapetencia_num != 1)
      continue;

    // GUARDAR RANGO DE INAPETENCIA EN DATO 8
    dato8 = atoi(inapetencia);
    Serial.print("Dato 8 (inapetencia): ");
    Serial.println(dato8);
    break;
  }
}
void instruccion9() {
  dato9 = 0;
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "Presione 1 si");
    u8x8.drawString(0, 1, "ultimamente");
    u8x8.drawString(0, 2, "presenta can");
    u8x8.drawString(0, 3, "sancio o sueno");
    u8x8.drawString(0, 4, "incrementando, ");
    u8x8.drawString(0, 5, "sino presione 0");

    // RECIBIR RANGO DE sueno HASTA RECIBIR TECLA "S"
    char sueno[2] = {0};
    char keypressed;
    int index = 0;
    int len = 0;
    bool stop = false;
    while (1) {
      char keypressed = kp1.getKey();
      if (!keypressed)                                     // If getKey returned any key
        continue;

      if (keypressed == 'S')
      {
        stop = true;
        goto printSueno;
      }

      if (keypressed == 'B' )
      {
        index--;
        len--;
        if (index < 0)
          index = 0;
        if (len < 0)
          len = 0;
        if (len == 0)
          index = 0;
        sueno[index] = 0;
        goto printSueno;
      }

      sueno[index] = keypressed;
      index++;
      len++;
      if (index > 0)
        index = 0;
      if (len > 1)
        len = 1;

printSueno:
      char stringSueno[30] = {0};
      sprintf(stringSueno, " % 1s", sueno);
      u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
      u8x8.drawString(6, 6, stringSueno);
      printString(sueno, len);
      if (stop == true)
        break;
    }

    // VERIFICAR RANGO DE sueno
    bool sueno_solonumeros = true;
    for (int i = 0; i < len; i++) {
      if ( !isdigit(sueno[i]) )
      {
        sueno_solonumeros = false;
        break;
      }
    }
    if ( !sueno_solonumeros )
      continue;
    int sueno_num = atoi(sueno);
    if ( sueno_num != 0 && sueno_num != 1)
      continue;

    // GUARDAR RANGO DE sueno EN DATO 8
    dato9 = atoi(sueno);
    Serial.print("Dato 9 (sueno): ");
    Serial.println(dato9);
    break;
  }
}
void instruccion10() {
  dato10 = 0;
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "Presione 1 si");
    u8x8.drawString(0, 1, "ultimamente pre");
    u8x8.drawString(0, 2, "senta dolor de");
    u8x8.drawString(0, 3, "cabeza o mareos");
    u8x8.drawString(0, 4, "constantemente, ");
    u8x8.drawString(0, 5, "sino presione 0");

    // RECIBIR RANGO DE dolor HASTA RECIBIR TECLA "S"
    char dolor[2] = {0};
    char keypressed;
    int index = 0;
    int len = 0;
    bool stop = false;
    while (1) {
      char keypressed = kp1.getKey();
      if (!keypressed)                                     // If getKey returned any key
        continue;

      if (keypressed == 'S')
      {
        stop = true;
        goto printDolor;
      }

      if (keypressed == 'B' )
      {
        index--;
        len--;
        if (index < 0)
          index = 0;
        if (len < 0)
          len = 0;
        if (len == 0)
          index = 0;
        dolor[index] = 0;
        goto printDolor;
      }

      dolor[index] = keypressed;
      index++;
      len++;
      if (index > 0)
        index = 0;
      if (len > 1)
        len = 1;

printDolor:
      char stringDolor[30] = {0};
      sprintf(stringDolor, " % 1s", dolor);
      u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
      u8x8.drawString(6, 6, stringDolor);
      printString(dolor, len);
      if (stop == true)
        break;
    }

    // VERIFICAR RANGO DE dolor
    bool dolor_solonumeros = true;
    for (int i = 0; i < len; i++) {
      if ( !isdigit(dolor[i]) )
      {
        dolor_solonumeros = false;
        break;
      }
    }
    if ( !dolor_solonumeros )
      continue;
    int dolor_num = atoi(dolor);
    if ( dolor_num != 0 && dolor_num != 1)
      continue;

    // GUARDAR RANGO DE dolor EN DATO 8
    dato10 = atoi(dolor);
    Serial.print("Dato 10 (dolor): ");
    Serial.println(dato10);
    break;
  }
}
void instruccion11() {
  dato11 = 0;
  while (1) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();

    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "Porfavor coloque");
    u8x8.drawString(0, 1, "su indice dere");
    u8x8.drawString(0, 2, "cho en el SENSOR");
    u8x8.drawString(0, 3, "durante 10 segun");
    u8x8.drawString(0, 4, "dos");

    // LEER SPO2 DEL SENSOR MAX30102
    int spo2 = leer_spo2_de_max30102();

    char stringSpo2[30] = {0};
    sprintf(stringSpo2, "%i", spo2);
    u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
    u8x8.drawString(5, 6, stringSpo2);
    Serial.println(spo2);

    // GUARDAR VALOR DE SPO2 EN DATO 11
    dato11 = spo2;
    Serial.print("Dato 11 (SPO2): ");
    Serial.println(dato11);

    delay(5000);
    break;
  }
}
void instruccion12() {
  dato12 = 0.0;

  // Establecer valor de entrada (SPO2)
  input->data.f[0] = (float)dato11;

  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk)
  {
    printf("Inferencia error\n");
    return;
  }

  // Extraer resultado
  float hemoglobina = output->data.f[0];

  // LIMPIAR PANTALLA
  u8x8.clearDisplay();

  // IMPRIMIR TITULO
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(2, 1, "Hemoglobina");

  // IMPRIMIR VALOR DE HEMOGLOBINA
  char stringHg[30] = {0};
  sprintf(stringHg, "%.2f", hemoglobina);
  u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
  u8x8.drawString(3, 5, stringHg);
  Serial.println(hemoglobina);

  // Guardar resultado
  dato12 = hemoglobina;

  delay(5000);
}
void instruccion13() {
  // LIMPIAR PANTALLA
  u8x8.clearDisplay();

  // IMPRIMIR TITULO
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 2, "Test de hemo");
  u8x8.drawString(0, 3, "globina satisfac");
  u8x8.drawString(0, 4, "torio, muchas ");
  u8x8.drawString(0, 5, "gracias");
  delay(5000);
}
void instruccion14() {
  // EMPAQUETAR DATOS
  memset(datos, 0, 200);
  snprintf(datos, 200, "%i;%i;%i;%i;%i;%i;"
           "%i;%i;%i;%i;%i;%f\n",
           dato1, dato2, dato3, dato4,
           dato5, dato6, dato7, dato8,
           dato9, dato10, dato11, dato12);

  // CONECTAR CON GATEWAY
join:

  // LIMPIAR PANTALLA
  u8x8.clearDisplay();
  // IMPRIMIR TITULO
  u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
  u8x8.drawString(0, 3, "JOINING");

  if ( !lora_join() ) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();
    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
    u8x8.drawString(0, 2, "ERROR:");
    u8x8.drawString(0, 4, "ESPERE 5");
    u8x8.drawString(0, 6, "SEGUNDOS");

    printf("Wait 5 seconds ...\n");
    delay(5000);
    goto join;
  }

  // LIMPIAR PANTALLA
  u8x8.clearDisplay();
  // IMPRIMIR TITULO
  u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
  u8x8.drawString(3, 3, "JOINED");

  delay(2000);

  // LIMPIAR PANTALLA
  u8x8.clearDisplay();
  // IMPRIMIR TITULO
  u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
  u8x8.drawString(0, 3, "ENVIANDO");

  // TRASMITIR DATOS
  bool tx_success = lora_transmit((uint8_t*)datos, strlen(datos));
  if (!tx_success) {
    // LIMPIAR PANTALLA
    u8x8.clearDisplay();
    // IMPRIMIR TITULO
    u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
    u8x8.drawString(0, 2, "ERROR:");
    u8x8.drawString(0, 4, "ESPERE 5");
    u8x8.drawString(0, 6, "SEGUNDOS");

    printf("Wait 5 seconds ...\n");
    delay(5000);
    goto join;
  }

  // LIMPIAR PANTALLA
  u8x8.clearDisplay();
  // IMPRIMIR TITULO
  u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
  u8x8.drawString(3, 3, "ENVIADO");

  delay(2000);
}
