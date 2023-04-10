// AUTORES DEL C�DIGO:
// ---JESUS MAXIMILIANO GONZALEZ FRIAS
// ---RICARDO SALVADOR HEREDIA BALLIN
// FECHA DE ENTREGA: 5 DE JULIO DE 2020
// UNIVERSIDAD AUT�NOMA DE AGUASCALIENTES, INGENIER�A EN SISTEMAS COMPUTACIONALES

// DESCRIPCI�N GENERAL DEL C�DIGO
// ESTE C�DIGO REALIZA UNA CAPTURA DE LOS EMPLEADOS DE UNA EMPRESA SOLICITANDO UNOS DATOS DETERMINADOS, ESTO CON EL FIN DE TENER ALMACENADA LA INFORMACI�N DE ESTAS PERSONAS EN UN ARCHIVO DE TEXTO.
// A SU VEZ, ESTE C�DIGO TIENE LA OPCI�N DE GENERAR UNA GR�FICA UTILIZANDO LA INFORMACI�N REGISTRADA EN EL ARCHIVO YA MENCIONADO ANTERIORMENTE.
// LA REALIZACI�N DE ESTA GR�FICA FUE POSIBLE GRACIAS A LA LIBRER�A DE C++ LLAMADA "GRAPHICS.H".

// LIBRERIAS UTILIZADAS
#include <iostream> // LIBRER�A BASE DE C++

#include <stdio.h> // LIBRER�A DE ENTRADA Y SALIDA EST�NDAR

#include <string.h> // LIBRER�A PARA EL USO DE CARACTERES

#include <graphics.h> // LIBRER�A DE GR�FICOS

#include <locale.h> // LIBRER�A PARA PONER ACENTOS EN EL COMPILADOR

//INSTRUCCI�N POR DEFECTO DE C++
using namespace std;

//DECLARACI�N DE UNA CONSTANTE
const int TAM = 12;

//FUNCI�N POR DEFECTO DE LA LIBRER�A GRAPHICS.H
void gotoxy(int x, int y) {
  HANDLE hcon;
  hcon = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD dwPos;
  dwPos.X = x;
  dwPos.Y = y;
  SetConsoleCursorPosition(hcon, dwPos);
}

//ENUMERACI�N UTILIZADA
enum TipoEmp {
  ADMINISTRADOR,
  GERENTE,
  VENDEDOR,
  CONSULTA,
  REGRESAR
};

char menuEmp[][15] = {
  "Administrador",
  "Gerente",
  "Vendedor",
  "CONSULTA",
  "REGRESAR"
};

//ESTRUCTURAS UTILIZADAS
struct Fecha {
  int dd, mm, aa;
};

struct Empleado {
  int codigoEmp;
  char nomEmpleado[99];
  Fecha FechaVta;
  float montoVenta;
};

//PROTOTIPOS DE LAS FUNCIONES UTILIZADAS
void menu();
float * limpiarMonto(float * monto);
void llenado(FILE * arch, float * monto);
void mostrarEmpleados(FILE * arch);
void mostrarMon(float * monto);
void capturaEmpleado(int op, FILE * arch, FILE * arch2);
void consulta(int op, FILE * arch, FILE * arch2);
void graficas(FILE * arch);
int regla3(float * monto, int i);

//FUNCI�N MAIN
int main() {
  setlocale(LC_ALL, "spanish");
  system("color f0");
  menu();
}

//FUNCI�N DEL MEN�
void menu() {
  //DECLARACI�N DE VARIABLES Y ARCHIVOS A UTILIZAR
  FILE * arch;
  FILE * arch2;
  int op = 0;
  do {
    //CREACI�N DEL MEN�
    //LA INSTRUCCI�N \t SIRVE PARA RECORRER EL TEXTO 
    cout << "\n\t\t\t\t\t\t***MEN� EMPRESARIAL***\n ";
    cout << "\n\t\t\t\t\t\t1.- Empleados.";
    cout << "\n\t\t\t\t\t\t2.- Ingresar Datos.";
    cout << "\n\t\t\t\t\t\t3.- Mostrar Gr�ficos.";
    cout << "\n\t\t\t\t\t\t4.- Salir.";
    cout << "\n\t\t\t\t\t\tIngrese una opci�n: ";

    //C�DIGO UTILIZADO PARA VALIDAR SI SE INGRESA UN CARACTER
    if (scanf("%d", &op) != 1) {
      cout << "\n\t\t\t\t\t\tERROR!!! EL VALOR DEBE SER UN NUMERO\n";
      while (getchar() != '\n');
      system("pause>null");
      system("cls");
    } else if (op == 1 || op == 2 || op == 3 || op == 4) {

      switch (op) {
      case 1:
        //INSTRUCCI�N PARA LIMPIAR LA PANTALLA
        system("cls");
        //LLAMADA DE LA FUNCI�N 
        mostrarEmpleados(arch);

        cout << endl;
        //INSTRUCCI�N PARA QUE EL SISTEMA HAGA UNA PAUSA AL SER EJECUTADO
        system("pause");
        system("cls");
        break;

      case 2:
        system("cls");
        //LLAMADA DE LA FUNCI�N 
        capturaEmpleado(op, arch, arch2);

        cout << endl;
        system("pause");
        system("cls");
        break;

      case 3:
        //LLAMADA DE LA FUNCI�N 
        graficas(arch);

        system("cls");
        break;
      }
    } else {
      //C�DIGO UTILIZADO PARA VALIDAR SI SE INGRESA UNA OPCI�N DIFERENTE AL RANGO CONTEMPLADO
      cout << "\n\t\t\t\t\t\tSELECCIONE UNA OPCION DEL 1 AL 4 SOLAMENTE\n";
      break;
    }
    //CONDICI�N DEL CICLO WHILE
  } while (op != 4);
}

//FUNCI�N DIN�MICA PARA LIMPIAR EL VECTOR DIN�MICO UTILIZADO EN EL PROGRAMA
float * limpiarMonto(float * monto) {
  for (int i = 0; i < TAM; i++) {
    monto[i] = 0;
  }
  return monto;
}

//FUNCI�N DIN�MICA PARA LLENAR EL VECTOR DIN�MICO
void llenado(FILE * arch, float * monto) {
  for (int i = 0; i < TAM; i++) {
    cout << "\nEscribe el monto de venta del mes " << i + 1 << ": ";
    cin >> monto[i];
    if (i == TAM - 1) {
      //GRABAR DATOS EN EL VECTOR DIN�MICO AL ARCHIVO
      fprintf(arch, " %.2f", monto[i]);
    } else {
      fprintf(arch, " %.2f ", monto[i]);
    }
  }
}

//FUNCI�N PARA MOSTRAR LOS EMPLEADOS REGISTRADOS
void mostrarEmpleados(FILE * arch) {
  //VARIABLE TIPO ESTRUCTURA
  Empleado mostrar;

  //DECLARACI�N DEL VECTOR DIN�MICO
  float * monto = new float[TAM];

  //APERUTRA DEL ARCHIVO DE TEXTO
  arch = fopen("RegistroEmpleados.txt", "r");
  if (arch == NULL) {
    cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
    return;
  }

  cout << "\nDATOS DE LOS EMPLEADOS";
  //INSTRUCCI�N PARA COLOCARSE AL INICIO DEL ARCHIVO
  rewind(arch);
  //INSTRUCCI�N PARA EJECUTAR C�DIGO MIENTRAS NO SEA FIN DE ARCHIVO
  while (!(feof(arch))) {
    //EXTRAER DATOS DEL ARCHIVO
    fscanf(arch, "%d", &mostrar.codigoEmp);
    fscanf(arch, "%s", mostrar.nomEmpleado);
    fscanf(arch, "%d", &mostrar.FechaVta.dd);
    fscanf(arch, "%d", &mostrar.FechaVta.mm);
    fscanf(arch, "%d", &mostrar.FechaVta.aa);

    //EXTRAER LOS DATOS REGISTRADOS EN EL VECTOR DIN�MICO
    for (int i = 0; i < TAM; i++) {
      fscanf(arch, "%f", &monto[i]);
    }

    //MOSTRAR LOS DATOS EN EL COMPILADOR DEL PROGRAMA
    cout << endl;
    cout << "\nC�digo del empleado: " << mostrar.codigoEmp;
    cout << "\nNombre del empleado: " << mostrar.nomEmpleado;
    cout << "\nFECHA\n";
    cout << mostrar.FechaVta.dd << " / " << mostrar.FechaVta.mm << " / " << mostrar.FechaVta.aa << endl;

    //LLAMADA DE LA FUNCI�N 
    mostrarMon(monto);
    cout << endl;
    system("pause>null");
  }
  //CIERRE DEL ARCHIVO DE TEXTO
  fclose(arch);
}

//FUNCI�N PARA MOSTRAR EL CONTENIDO DEL VECTOR DIN�MICO EN EL COMPILADOR
void mostrarMon(float * monto) {
  for (int i = 0; i < TAM; i++) {
    cout << "\nEl monto de venta del mes " << i + 1 << " es: " << * (monto + i);
  }
}

//FUNCI�N PARA CAPTURAR TODOS LOS TIPOS DE EMPLEADOS QUE PERMITE EL PROGRAMA 
void capturaEmpleado(int op, FILE * arch, FILE * arch2) {
  op = 0;
  Empleado registro;

  float * monto = new float[TAM];
  monto = limpiarMonto(monto);

  do {
    cout << "\n\t\t\t\t\t\t***TIPOS DE EMPLEADOS***\n\n";
    //C�DIGO PARA HACER USO DE LA ENUMERACI�N DECLARADA ANTERIORMENTE
    for (int i = 0; i < 5; i++) {
      cout << "\t\t\t\t\t\t" << menuEmp[i] << "...." << i << endl;
    }

    cout << "\t\t\t\t\t\tIngrese una opci�n: ";
    if (scanf("%d", &op) != 1) {
      cout << "\n\t\t\t\t\t\tERROR!!! EL VALOR DEBE SER UN NUMERO\n";
      while (getchar() != '\n');
      return;
    }

    switch (op) {
    case ADMINISTRADOR:
      system("cls");

      arch = fopen("RegistroEmpleados.txt", "a+");
      if (arch == NULL) {
        cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
        return;
      }

      //INSTRUCCI�N PARA "ACCEDER" AL ELEMENTO DE LA ENUMERACI�N 
      cout << "\nHa seleccionado: " << menuEmp[0] << endl;

      cout << "Escribe el c�digo del empleado: ";
      if (scanf("%d", &registro.codigoEmp) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "Escribe el nombre del empleado: ";
      //INSTRUCCI�N PARA LIMPIAR EL BUFFER
      fflush(stdin);
      //INSTRUCCI�N PARA LEER UNA CADENA DE CARACTERES INCLUYENDO ESPACIOS
      gets(registro.nomEmpleado);

      cout << "\nFECHA\n";
      cout << "Escribe el dia: ";
      if (scanf("%d", &registro.FechaVta.dd) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "Escribe el mes: ";
      if (scanf("%d", &registro.FechaVta.mm) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "Escribe el a�o: ";
      if (scanf("%d", &registro.FechaVta.aa) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      //GRABAR LOS DATOS PROPORCIONADOS EN EL ARCHIVO
      fprintf(arch, "\n%d %s %d %d %d", registro.codigoEmp, registro.nomEmpleado, registro.FechaVta.dd, registro.FechaVta.mm, registro.FechaVta.aa);

      //LLAMADA DE LA FUNCI�N
      llenado(arch, monto);

      fclose(arch);

      cout << endl;
      system("pause");
      system("cls");
      break;

    case GERENTE:
      system("cls");

      arch = fopen("RegistroEmpleados.txt", "a+");
      if (arch == NULL) {
        cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
        return;
      }

      cout << "\nHa seleccionado: " << menuEmp[1] << endl;

      cout << "Escribe el c�digo del empleado: ";
      if (scanf("%d", &registro.codigoEmp) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "Escribe el nombre del empleado: ";
      fflush(stdin);
      gets(registro.nomEmpleado);

      cout << "\nFECHA\n";
      cout << "Escribe el dia: ";
      if (scanf("%d", &registro.FechaVta.dd) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "Escribe el mes: ";
      if (scanf("%d", &registro.FechaVta.mm) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "Escribe el a�o: ";
      if (scanf("%d", &registro.FechaVta.aa) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      fprintf(arch, "\n%d %s %d %d %d", registro.codigoEmp, registro.nomEmpleado, registro.FechaVta.dd, registro.FechaVta.mm, registro.FechaVta.aa);

      llenado(arch, monto);

      fclose(arch);

      cout << endl;
      system("pause");
      system("cls");
      break;

    case VENDEDOR:
      system("cls");

      arch = fopen("RegistroEmpleados.txt", "a+");
      if (arch == NULL) {
        cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
        return;
      }

      cout << "\nHa seleccionado: " << menuEmp[2] << endl;

      cout << "Escribe el c�digo del empleado: ";
      if (scanf("%d", &registro.codigoEmp) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "Escribe el nombre del empleado: ";
      fflush(stdin);
      gets(registro.nomEmpleado);

      cout << "\nFECHA\n";
      cout << "Escribe el dia: ";
      if (scanf("%d", &registro.FechaVta.dd) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "Escribe el mes: ";
      if (scanf("%d", &registro.FechaVta.mm) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "Escribe el a�o: ";
      if (scanf("%d", &registro.FechaVta.aa) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      fprintf(arch, "\n%d %s %d %d %d", registro.codigoEmp, registro.nomEmpleado, registro.FechaVta.dd, registro.FechaVta.mm, registro.FechaVta.aa);

      llenado(arch, monto);
      fclose(arch);

      cout << endl;
      system("pause");
      system("cls");
      break;

    case CONSULTA:
      system("cls");

      consulta(op, arch, arch2);

      cout << endl;
      system("pause");
      system("cls");
      break;

    case REGRESAR:
      return;

      //VALIDAR SI SE INGRESA UNA OPCI�N QUE NO EST� DENTRO DEL RANGO DE OPCIONES DEL MEN�
    default:
      cout << "\nESCRIBA UNA OPCION VALIDA\n";

      cout << endl;
      system("pause");
      system("cls");
      break;

    }
  } while (op != REGRESAR);
}

//FUNCI�N PARA BUSCAR INFORMACI�N ESPEC�FICA EN EL ARCHIVO Y PARA GENERAR UN REPORTE
void consulta(int op, FILE * arch, FILE * arch2) {
  op = 0;
  Empleado registro;

  float * monto = new float[TAM];
  monto = limpiarMonto(monto);

  int codigoCons = 0, bandera = 0, ddBus = 0, mmBus = 0, aaBus = 0, * ptr;
  float montoBus;

  do {
    cout << "\n\t\t\t\t\t\t***CONSULTA DE DATOS***\n ";
    cout << "\n\t\t\t\t\t\tConsulta por c�digo de empleado....1";
    cout << "\n\t\t\t\t\t\tConsulta por fecha.................2";
    cout << "\n\t\t\t\t\t\tConsulta por monto de venta........3";
    cout << "\n\t\t\t\t\t\tREGRESAR...........................4";

    cout << "\n\t\t\t\t\t\tIngrese una opci�n: ";
    if (scanf("%d", & op) != 1) {
      cout << "\n\t\t\t\t\t\tERROR!!! EL VALOR DEBE SER UN NUMERO\n";
      while (getchar() != '\n');
      return;
    }

    switch (op) {
    case 1:
      system("cls");
      bandera = 0;
      arch = fopen("RegistroEmpleados.txt", "r");
      if (arch == NULL) {
        cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
        return;
      }

      //APERTURA DE OTRO ARCHIVO DE TEXTO
      arch2 = fopen("ReporteEmpleados.txt", "a+");
      if (arch2 == NULL) {
        cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
        return;
      }

      ptr = & codigoCons;

      cout << "\nEscribe el c�digo del empleado a buscar: ";
      if (scanf("%d", ptr) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      rewind(arch);
      while (!feof(arch)) {
        fscanf(arch, "%d", &registro.codigoEmp);
        fscanf(arch, "%s", registro.nomEmpleado);
        fscanf(arch, "%d", &registro.FechaVta.dd);
        fscanf(arch, "%d", &registro.FechaVta.mm);
        fscanf(arch, "%d", &registro.FechaVta.aa);

        for (int i = 0; i < TAM; i++) {
          fscanf(arch, "%f", &monto[i]);
        }

        //CONDICI�N PARA CONSULTAR EL C�DIGO INGRESADO POR EL USUARIO
        if (codigoCons == registro.codigoEmp) {
          //INSTRUCCIONES PARA GRABAR LOS DATOS EN EL ARCHIVO REPORTE
          fprintf(arch2, "\n\nC�digo encontrado: %d", registro.codigoEmp);
          fprintf(arch2, "\n Nombre del empleado: %s \n FECHA \n D�a: %d \t Mes: %d \t A�o: %d \n ", registro.nomEmpleado, registro.FechaVta.dd, registro.FechaVta.mm, registro.FechaVta.aa);

          for (int i = 0; i < TAM; i++) {
            fprintf(arch2, "\nMonto de venta del mes %d: %.2f", i + 1, monto[i]);
          }
          bandera = 1;
        } else {
          bandera = 0;
        }
        if (bandera == 1) {
          break;
        }
      }

      //CONDICI�N PARA MOSTRAR UN MENSAJE AL USUARIO SI ES QUE SE ENCONTR� EL REGISTRO
      if (bandera == 1) {
        cout << "\nSe encontr� el c�digo !!! \n";
        cout << "\n***REPORTE GENERADO***\n";
      } else if (bandera == 0) {
        cout << "\nNo encontr� el c�digo !!! ";
      }

      //CIERRE DE LOS ARCHIVOS DE TEXTO
      fclose(arch);
      fclose(arch2);

      cout << endl;
      system("pause");
      system("cls");
      break;

    case 2:
      system("cls");
      bandera = 0;
      arch = fopen("RegistroEmpleados.txt", "r");
      if (arch == NULL) {
        cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
        return;
      }

      arch2 = fopen("ReporteEmpleados.txt", "a+");
      if (arch2 == NULL) {
        cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
        return;
      }

      cout << "\nEscribe el d�a a buscar: ";
      if (scanf("%d", &ddBus) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "\nEscribe el mes a buscar: ";
      if (scanf("%d", &mmBus) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      cout << "\nEscribe el a�o a buscar: ";
      if (scanf("%d", &aaBus) != 1) {
        cout << "\nERROR!!! EL VALOR DEBE SER UN NUMERO\n";
        while (getchar() != '\n');
        break;
      }

      rewind(arch);
      while (!feof(arch)) {
        fscanf(arch, "%d", &registro.codigoEmp);
        fscanf(arch, "%s", registro.nomEmpleado);
        fscanf(arch, "%d", &registro.FechaVta.dd);
        fscanf(arch, "%d", &registro.FechaVta.mm);
        fscanf(arch, "%d", &registro.FechaVta.aa);

        for (int i = 0; i < TAM; i++) {
          fscanf(arch, "%f", &monto[i]);
        }

        //CONDICI�N PARA CONSULTAR EL DIA, MES Y A�O INGRESADO POR EL USUARIO
        if (ddBus == registro.FechaVta.dd &&mmBus == registro.FechaVta.mm &&aaBus == registro.FechaVta.aa) {
          fprintf(arch2, "\n\nD�a encontrado: %d ", registro.FechaVta.dd);
          fprintf(arch2, "\n C�digo: %d \n Nombre del empleado: %s \n FECHA \n \t\t Mes: %d \t A�o: %d \n", registro.codigoEmp, registro.nomEmpleado, registro.FechaVta.mm, registro.FechaVta.aa);

          for (int i = 0; i < TAM; i++) {
            fprintf(arch2, "\nMonto de venta del mes %d: %.2f", i + 1, monto[i]);
          }

          fprintf(arch2, "\n\nMes encontrado: %d ", registro.FechaVta.mm);
          fprintf(arch2, "\n C�digo: %d \n Nombre del empleado: %s \n FECHA \n D�a: %d \t\t\t A�o: %d \n", registro.codigoEmp, registro.nomEmpleado, registro.FechaVta.dd, registro.FechaVta.aa);

          for (int i = 0; i < TAM; i++) {
            fprintf(arch2, "\nMonto de venta del mes %d: %.2f", i + 1, monto[i]);
          }

          fprintf(arch2, "\n\nA�o encontrado: %d ", registro.FechaVta.aa);
          fprintf(arch2, "\n C�digo: %d \n Nombre del empleado: %s \n FECHA \n D�a: %d \t Mes: %d \n", registro.codigoEmp, registro.nomEmpleado, registro.FechaVta.dd, registro.FechaVta.mm);

          for (int i = 0; i < TAM; i++) {
            fprintf(arch2, "\nMonto de venta del mes %d: %.2f", i + 1, monto[i]);
          }

          bandera = 1;
        } else {
          bandera = 0;
        }
        if (bandera == 1) {
          break;
        }
      }

      if (bandera == 1) {
        cout << "\nSe encontr� la fecha !!! \n";
        cout << "\n***REPORTE GENERADO***\n";
      } else if (bandera == 0) {
        cout << "\nNo encontr� la fecha !!! ";
      }

      fclose(arch);
      fclose(arch2);

      cout << endl;
      system("pause");
      system("cls");
      break;

    case 3:
      system("cls");
      bandera = 0;
      arch = fopen("RegistroEmpleados.txt", "r");
      if (arch == NULL) {
        cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
        return;
      }

      arch2 = fopen("ReporteEmpleados.txt", "a+");
      if (arch2 == NULL) {
        cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
        return;
      }

      cout << "\nEscribe el monto a buscar: ";
      cin >> montoBus;

      rewind(arch);
      while (!(feof(arch))) {
        fscanf(arch, "%d", &registro.codigoEmp);
        fscanf(arch, "%s", registro.nomEmpleado);
        fscanf(arch, "%d", &registro.FechaVta.dd);
        fscanf(arch, "%d", &registro.FechaVta.mm);
        fscanf(arch, "%d", &registro.FechaVta.aa);

        for (int i = 0; i < TAM; i++) {
          fscanf(arch, "%f", &monto[i]);
        }

        for (int i = 0; i < TAM; i++) {
          //CONDICI�N PARA CONSULTAR LOS MONTOS DE VENTA INGRESADOS POR EL USUARIO (REGISTRADOS EN EL VECTOR DIN�MICO)
          if ( * (monto + i) == montoBus) {
            fprintf(arch2, "\nMonto de venta del mes %d: %.2f", i + 1, monto[i]);
            fprintf(arch2, "\n \nC�digo: %d \n Nombre del empleado: %s \n FECHA \n D�a: %d \t Mes: %d \t A�o: %d \n", registro.codigoEmp, registro.nomEmpleado, registro.FechaVta.dd, registro.FechaVta.mm, registro.FechaVta.aa);
            bandera = 1;
          }
        }
      }

      if (bandera == 1) {
        cout << "\nSe encontr� el monto de la venta !!! ";
        cout << "\n***REPORTE GENERADO***\n";
        bandera = 0;
      } else if (bandera == 0) {
        cout << "\nNo se encontr� el monto de la venta !!! ";
      }

      fclose(arch);
      fclose(arch2);

      cout << endl;
      system("pause");
      system("cls");
      break;

    case 4:
      return;

    default:
      cout << "\nESCRIBA UNA OPCION VALIDA";

      cout << endl;
      system("pause");
      system("cls");
      break;
    }
  } while (op != 4);
}

//FUNCI�N PARA GENERAR LA GR�FICA DEL PROGRAMA
void graficas(FILE * arch) {
  int y1, band = 0, flood = 100;
  int mesM[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  int auxM;
  int x1 = 112, x2 = 90, xM = 107;
  char mes[25];
  Empleado reg;
  float * monto = new float[TAM];
  monto = limpiarMonto(monto);
  //INICIALIZAR GRAFICOS
  initwindow(720, 600, "Grafica");
  setbkcolor(WHITE);
  cleardevice();

  arch = fopen("RegistroEmpleados.txt", "r");
  if (arch == NULL) {
    cout << "\nERROR AL ABRIR EL ARCHIVO !!!";
    return;
  }

  rewind(arch);
  // CICLO WHILE SACAR DATOS Y GRAFICAS
  while (!feof(arch)) {
    fscanf(arch, "%d", &reg.codigoEmp);
    fscanf(arch, "%s", reg.nomEmpleado);
    fscanf(arch, "%d", &reg.FechaVta.dd);
    fscanf(arch, "%d", &reg.FechaVta.mm);
    fscanf(arch, "%d", &reg.FechaVta.aa);
    for (int i = 0; i < TAM; i++) {
      fscanf(arch, "%f", &monto[i]);
    }
    x1 = 112;
    x2 = 90;
    xM = 107;
    flood = 100;

    //IMPPRIMIR LAS BASES DE LA GRAFICA
    setcolor(BLACK);
    line(90, 0, 400, 0);
    line(90, 360, 90, 0);
    line(90, 270, 80, 270); //100
    line(90, 180, 80, 180); //200
    line(90, 90, 80, 90); //300
    line(90, 360, 80, 360); //400

    //IMPRIMIR EN LA VENTANA 
    setcolor(BLACK);
    settextstyle(6, 0, 1);
    outtextxy(5, 350, "$400");
    outtextxy(5, 260, "$300");
    outtextxy(5, 170, "$200");
    outtextxy(5, 80, "$100");

    //DATOS
    outtextxy(500, 130, "Nombre empleado:");
    outtextxy(550, 150, reg.nomEmpleado);

    float aux;
    //CICLO FOR PARA DETERMINAR EL MONTO MAYOR EN EL MES
    for (int i = 1; i < 12; i++) {
      for (int j = 0; j < 12 - i; j++) {
        if (monto[j] < monto[j + 1]) {
          aux = monto[j + 1];
          monto[j + 1] = monto[j];
          monto[j] = aux;

          auxM = mesM[j + 1];
          mesM[j + 1] = mesM[j];
          mesM[j] = auxM;
        }
      }
    }

    //FUNCION PARA GENERAR LAS BARRAS DE LAS GRAFICAS
    for (int i = 0; i < 12; i++) {
      y1 = regla3(monto, i);
      if (band == 1) {
        setcolor(GREEN);
      } else {
        setcolor(CYAN);
      }
      rectangle(x1, 1, x2, y1);
      if (band == 1) {
        setcolor(GREEN);
        setfillstyle(SOLID_FILL, GREEN);
        floodfill(flood, y1 / 2, GREEN);
        band = 0;
      } else {
        setcolor(CYAN);
        band = 1;
        setfillstyle(SOLID_FILL, CYAN);
        floodfill(flood, y1 / 2, CYAN);
      }
      x1 += 22;
      x2 += 22;
      flood += 22;
    }
    setcolor(BLACK);
    settextstyle(6, 0, 1);
    outtextxy(200, 550, "Mes");
    //FOR PARA IMPRIMIR LOS MESES EN initwindo();
    for (int i = 0; i < 13; i++) {
      switch (mesM[i]) {
      case 1:
        strcpy(mes, "Enero");
        break;
      case 2:
        strcpy(mes, "Febrero");
        break;
      case 3:
        strcpy(mes, "Marzo");
        break;
      case 4:
        strcpy(mes, "Abril");
        break;
      case 5:
        strcpy(mes, "Mayo");
        break;
      case 6:
        strcpy(mes, "Junio");
        break;
      case 7:
        strcpy(mes, "Julio");
        break;
      case 8:
        strcpy(mes, "Agosto");
        break;
      case 9:
        strcpy(mes, "Septiembre");
        break;
      case 10:
        strcpy(mes, "Octubre");
        break;
      case 11:
        strcpy(mes, "Noviembre");
        break;
      case 12:
        strcpy(mes, "Diciembre");
        break;
      }
      setcolor(BLACK);
      settextstyle(6, 1, 1);
      outtextxy(xM, 400, mes);
      xM += 23;
    }
    for (int i = 0; i < 12; i++) {
      mesM[i] = i + 1;
    }
    cout << "ID empleado: " << reg.codigoEmp << endl;

    cout << "Siguiente grafica....." << endl;
    system("pause");
    system("cls");
    cleardevice();
  }
  getch();
  fclose(arch);
}

//FUNCION PARA SACAR LA EQUIVALENCIA DE PUNTOS A PIXELES
int regla3(float * monto, int i) {
  return monto[i] * 90 / 100;
}
