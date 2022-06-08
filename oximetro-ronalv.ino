#include <Arduino.h>
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

#include <SimpleKeypad.h>
const byte nb_rows = 4;                         // four rows
const byte nb_cols = 3;                         // four columns
char key_chars[nb_rows][nb_cols] = {            // The symbols of the keys
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'S', '0', 'B'}
};
byte rowPins[nb_rows] = {A0, A1, A2, A3};           // The pins where the rows are connected
byte colPins[nb_cols] = {13, 12, 11};           // The pins where the columns are connected
SimpleKeypad kp1((char*)key_chars, rowPins, colPins, nb_rows, nb_cols);   // New keypad called kp1

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

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  u8x8.begin();
  u8x8.setPowerSave(0);

}

void loop() {

  instruccion1();
  instruccion2();
  instruccion3();
  instruccion4();
  instruccion5();
  instruccion6();
  instruccion7();
  instruccion8();
  instruccion9();
  instruccion10();
  //instruccion11();
  printDatos();
  delay(5000);

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
