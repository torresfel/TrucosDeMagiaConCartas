// Jose Luis Moreno Varillas y Desiree Vetencourt Rosas (1B-G33)
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>
using namespace std;

const int MAXCARTAS = 53;
const int CARTASPORPALO = 13;
const int CENTINELA = 52;

typedef enum { picas, treboles, diamantes, corazones } tPalo;
typedef enum { as, dos, tres, cuatro, cinco, seis, siete, ocho, nueve, diez, jack, queen, king } tNumero;
typedef int tCarta;
typedef int tMazo[MAXCARTAS];

//Prototipos de funciones
//-- Version 1 --
void crearMazoVacio(tMazo mazo); // Array vacio con el centinela en 1º posicion;
int cuantasEnMazo(const tMazo mazo); // Devuelve nº de cartas en mazo;
tPalo darPalo(tCarta carta); // Devuelve el palo de la carta;
tNumero darNumero(tCarta carta); //Devuelve el numero de la carta;
void escribirCarta(tCarta carta); // Muestra por pantalla una carta;
void escribirMazo(const tMazo mazo); //Muestra por pantalla el mazo;
bool cargarMazo(tMazo mazo); // Devuelve false si el archivo no puede abrirse, y true en caso afirmativo y lo cargara en tMazo;
void barajarMazo(tMazo mazo); // Se cambian cartas entre si 3*numCartas veces;
bool unirMazos(tMazo mazoOriginal, const tMazo nuevoMazo); //Añade al final de mazoOriginal las cartas de nuevoMazo. Devuelve false en caso de superar la union de mazos el maximo de cartas;
bool partirMazo(tMazo mazoOrigen, int cuantasCoger, tMazo mazoDestino); // Quita a mazoOrigen sus primeras cuantasCoger cartas, formando con ellas el mazoDestino. Devuelve false en caso de nu haber suficientes cartas en mazoOrigen;
void cortarMazo(tMazo mazo, int cuantas); // Corta el mazo por cuantas. En vez de desplazar el array de mazo, se pasan las cartas a un mazoAux, se le une al final las cartas de mazo y despues se copia todo el contenido de mazoAux a mazo;
void guardarMazo(const tMazo mazo); //Guarda el mazo en un archivo con un nombre pedido por el usuario;
int menu();
//-- Version 2 --
void repartirNegroRojo(const tMazo mazo, tMazo mazoNegro, tMazo mazoRojo); //Separa en dos mazos cartas negras y cartas rojas
void repartirBajaAlta(const tMazo mazo, tMazo mazoBajas, tMazo mazoAltas); //Bajas: A-7||Altas: 8-K
void repartirIntercalando(const tMazo mazo, int enCuantosMazos, int queMazoPido, tMazo mazoNuevo); // Reparte intercalando, es decir, una a cada monton, hasta que se agote el mazo
//-- Version 3 --
void trucoTresMontones();
//-- Version 4 --
void trucoPosada();
//-- Parte opcional
void trucoJugadorDesconfiado();
void repartirParImpar(const tMazo mazo, tMazo izq, tMazo der); //Separa en dos mazos cartas pares (Q=12) y cartas impares (J=11, K=13)
void repartirFiguraNoFigura(const tMazo mazo, tMazo izq, tMazo der); //Separa en dos mazos cartas con figuras (A,J,Q,K) y sin figuras (las demas cartas)
//-- Funciones añadidas
bool mazoVacio(const tMazo mazo); //devuelve true en caso de que el mazo este vacio. En caso contrario, devuelve true
void pasarAMazo(tMazo mazo, const tMazo mazoAux); // Pasa las cartas de mazoAux a mazo;
char pasarAPalo(tPalo palo); // dado un elemento del enum tPalo, nos devuelve la letra del palo en cuestion;
void opcionCargarMazo(tMazo mazo); // lleva a cabo la funcion de cargarMazo(...)
void opcionBarajar(tMazo mazo); // lleva a cabo la funcion de barajarMazo(...)
void opcionAnadirMazo(tMazo mazo); // lleva a cabo la funcion de anadirMazo(...)
void opcionCortarMazo(tMazo mazo); // lleva a cabo la funcion de cortarMazo(...)
void opcionRepartirNegroRojo(tMazo mazo); // lleva a cabo la funcion de repartirNegroRojo(...)
void opcionRepartirBajaAlta(tMazo mazo); // lleva a cabo la funcion de repartirBajaAlta(...)
void opcionRepartirMontones(const tMazo mazo); // lleva a cabo la funcion de repartirMontones(...)
void intercambiarMazos(tMazo mazo1, tMazo mazo2); //Intercambia las cartas de mazo1 a mazo2
void repartirYMostrar(const tMazo mazo, int enCuantosMazos, int queMazoPido, tMazo mazoNuevo); //Funcion utilizada en la parte opcional que incorpora las funciones repartirIntercalando(...) y escribirMazo(...)
//Programa

int main(){
	tMazo mazo;
	int op;
	bool ok = false;
	srand(time(NULL));
	crearMazoVacio(mazo);
	op = menu();
	while (op != 0){
		switch (op){
		case 1:
			opcionCargarMazo(mazo);
			break;
		case 2:
			opcionBarajar(mazo);
			break;
		case 3:
			opcionAnadirMazo(mazo);
			break;
		case 4:
			opcionCortarMazo(mazo);
			break;
		case 5:
			guardarMazo(mazo);
			break;
		case 6:
			opcionRepartirNegroRojo(mazo);
			break;
		case 7:
			opcionRepartirBajaAlta(mazo);
			break;
		case 8:
			opcionRepartirMontones(mazo);
			break;
		case 9:
			trucoTresMontones();
			break;
		case 10:
			trucoPosada();
			break;
		case 11:
			trucoJugadorDesconfiado();
			break;
		}
		op = menu();
	}
	cout << "Hasta pronto!" << endl;
	system("pause");
	return 0;
}

int menu(){
	int op;
	do{
		cout << "1- Cargar un mazo de cartas de fichero" << endl;
		cout << "2- Barajar mazo de cartas" << endl;
		cout << "3- Anadir mazo" << endl;
		cout << "4- Cortar mazo" << endl;
		cout << "5- Guardar mazo" << endl;
		cout << "6- Separar entre cartas negras y rojas" << endl;
		cout << "7- Separar entre cartas bajas y altas" << endl;
		cout << "8- Repartir mazo alternativamente en 3 montones" << endl;
		cout << "9- Truco de los Tres Montones" << endl;
		cout << "10- Truco de la posada" << endl;
		cout << "11- Truco del jugador Desconfiado" << endl;
		cout << "0- Salir" << endl;
		cout << "Opcion (0-11): ";
		cin >> op;
		cout << endl;
		if ((op<0) && (op>11)) cout << "Seleccione una opcion de las mostradas" << endl;
	} while ((op<0) && (op>11));
	return op;
}

void crearMazoVacio(tMazo mazo){
	mazo[0] = CENTINELA;
}

bool mazoVacio(const tMazo mazo){
	bool vacio = true;
	if (cuantasEnMazo(mazo) > 0) vacio = false;
	return vacio;
}

int cuantasEnMazo(const tMazo mazo){
	int i = 0;
	while(mazo[i]!=CENTINELA){
		i++;
	}
	return i;
}

tPalo darPalo(tCarta carta){
	tPalo palo;
	palo = tPalo(carta / CARTASPORPALO);
	return palo;
}

tNumero darNumero(tCarta carta){
	tNumero numero;
	numero = tNumero(carta%CARTASPORPALO);
	return numero;
}

void escribirCarta(tCarta carta){
	tNumero numero;
	tPalo palo;
	numero = tNumero(darNumero(carta));
	palo = tPalo(darPalo(carta));
	switch (numero){
	case 0:
		cout << "As";
		break;
	case 10:
		cout << "Jota";
		break;
	case 11:
		cout << "Reina";
		break;
	case 12:
		cout << "Rey";
		break;
	default:
		cout << numero + 1;
	}
	cout << " de ";
	switch (palo){
	case 0:
		cout << "picas" << endl;
		break;
	case 1:
		cout << "treboles" << endl;
		break;
	case 2:
		cout << "diamantes" << endl;
		break;
	case 3:
		cout << "corazones" << endl;
		break;
	}
}

void escribirMazo(const tMazo mazo){
	int i = 0;
	while (mazo[i] != CENTINELA){
		escribirCarta(mazo[i]);
		i++;
	}
	cout << endl;
}

bool cargarMazo(tMazo mazo){
	bool ok = false;
	char palo;
	int numero, i = 0;
	ifstream archivo;
	string nombArch;
	cout << "Introduce el nombre del mazo a cargar (SIN EXTENSION!): ";
	cin >> nombArch;
	nombArch = nombArch + ".txt";
	archivo.open(nombArch.c_str());
	if (archivo.is_open()){
		crearMazoVacio(mazo);
		archivo >> palo;
		while ((palo != 'x') && (i < 52)){
			archivo >> numero;
			switch (palo){
			case 'p':
				numero = numero - 1;
				break;
			case 't':
				numero = CARTASPORPALO + numero - 1;
				break;	
			case 'd':
				numero = 2*CARTASPORPALO + numero - 1;
				break;
			case 'c':
				numero = 3*CARTASPORPALO + numero - 1;
				break;
			}//switch
			mazo[i] = numero;
			i++;
			archivo >> palo;
		}//while
		ok = true;
		mazo[i] = CENTINELA;
	}
	return ok;
}

void opcionCargarMazo(tMazo mazo){
	if (cargarMazo(mazo)) escribirMazo(mazo);
	else cout << "Error al cargar la baraja solicitada. Intentelo de nuevo." << endl << endl;
}

void barajarMazo(tMazo mazo){
	int cantidad = cuantasEnMazo(mazo), intercambios = 3 * cantidad, pos1, pos2, aux;
	for (int i = intercambios; i > 0; i--){
		do{
			pos1 = rand() % cantidad;
			pos2 = rand() % cantidad;
		} while (pos1 == pos2);
		aux = mazo[pos2];
		mazo[pos2] = mazo[pos1];
		mazo[pos1] = aux;
	}

}

void opcionBarajar(tMazo mazo){
	if (mazoVacio(mazo)) cout << "El mazo esta vacio, no hay nada que barajar" << endl << endl;
	else{
		escribirMazo(mazo);
		barajarMazo(mazo);
		escribirMazo(mazo);
	}
}

bool unirMazos(tMazo mazoOriginal, const tMazo nuevoMazo){
	bool ok = false;
	int cantOriginal = cuantasEnMazo(mazoOriginal), cantNuevo = cuantasEnMazo(nuevoMazo);
	if ((cantOriginal + cantNuevo) < 52){
		for (int i = 0; i < cantNuevo; i++) {
			mazoOriginal[cantOriginal + i] = nuevoMazo[i];
		}
		ok = true;
		mazoOriginal[(cantOriginal + cantNuevo)] = CENTINELA;
	}
	return ok;
}

void opcionAnadirMazo(tMazo mazo){
	tMazo mazoAux;
	crearMazoVacio(mazoAux);
	if (cargarMazo(mazoAux)) {
		if (unirMazos(mazo, mazoAux)) escribirMazo(mazo);
		else cout << "Error: la union supera el numero maximo de cartas" << endl << endl;
	}
	else cout << "Error al cargar la baraja solicitada. Intentelo de nuevo." << endl << endl;
}

bool partirMazo(tMazo mazoOrigen, int cuantasCoger, tMazo mazoDestino){
	bool ok = false;
	int cantMazoOrigen = cuantasEnMazo(mazoOrigen), i;
	crearMazoVacio(mazoDestino);
	if (cuantasCoger < cantMazoOrigen){
		for (i = 0; i < cuantasCoger; i++){
			mazoDestino[i] = mazoOrigen[(cantMazoOrigen - (cuantasCoger - i))];
		}
		mazoDestino[i] = CENTINELA;
		mazoOrigen[(cantMazoOrigen - cuantasCoger)] = CENTINELA;
		ok = true;
	}
	return ok;
}

void cortarMazo(tMazo mazo, int cuantas){
	tMazo mazoAux;
	int cant = cuantasEnMazo(mazo);
	if (partirMazo(mazo, cuantas, mazoAux)){
		unirMazos(mazoAux, mazo);
		pasarAMazo(mazo, mazoAux);
	}
}

void pasarAMazo(tMazo mazo, const tMazo mazoAux){
	int cant = cuantasEnMazo(mazoAux);
	for (int i = 0; i <= cant; i++) mazo[i] = mazoAux[i];
}

void opcionCortarMazo(tMazo mazo){
	int corte;
	if (mazoVacio(mazo)) cout << "El mazo esta vacio, no hay nada que cortar" << endl << endl;
	else{
		cout << "Mazo origianl:" << endl;
		escribirMazo(mazo);
		cout << "Corta el mazo. (Cuantas cartas de arriba tomas para ponerlas debajo?): ";
		cin >> corte;
		cortarMazo(mazo, corte);
		cout << endl << "El mazo cortado queda asi:" << endl;
		escribirMazo(mazo);
	}
}

void guardarMazo(const tMazo mazo){
	tPalo palo;
	tNumero numero;
	ofstream archivo;
	string nombArch;
	int cant = cuantasEnMazo(mazo);
	cout << "Introduce un nombre para el mazo a guardar (SIN EXTENSION!): ";
	cin >> nombArch;
	nombArch = nombArch + ".txt";
	archivo.open(nombArch.c_str());
	for (int i = 0; i < cant; i++){
		palo = darPalo(mazo[i]);
		numero = darNumero(mazo[i]);
		archivo << pasarAPalo(palo);
		archivo << (numero + 1) << endl;
	}
	archivo << "x";
	cout << endl;
}

char pasarAPalo(tPalo palo){
	char paloAux;
	switch (palo){
	case 0:
		paloAux = 'p';
		break;
	case 1:
		paloAux = 't';
		break;
	case 2:
		paloAux = 'd';
		break;
	case 3:
		paloAux = 'c';
		break;
	}
	return paloAux;
}

void repartirNegroRojo(const tMazo mazo, tMazo mazoNegro, tMazo mazoRojo){
	int cantNegro, cantRojo, cantMazo = cuantasEnMazo(mazo);
	crearMazoVacio(mazoNegro);
	crearMazoVacio(mazoRojo);
	cantNegro = cuantasEnMazo(mazoNegro); //Estas dos variables siempre seran igual a 0, 
	cantRojo = cuantasEnMazo(mazoRojo); // asi que se podria poner cantXXX = 0
	for (int i = 0; i < cantMazo; i++){
		switch (darPalo(mazo[i])){
		case 0: case 1:
			mazoNegro[cantNegro] = mazo[i];
			cantNegro++;
			break;
		case 2: case 3:
			mazoRojo[cantRojo] = mazo[i];
			cantRojo++;
			break;
		}
	}
	mazoNegro[cantNegro] = CENTINELA; //Ponemos el CENTINELA al final de
	mazoRojo[cantRojo] = CENTINELA; //de cada mazo
}

void opcionRepartirNegroRojo(tMazo mazo){
	tMazo mazoNegro, mazoRojo;
	if (mazoVacio(mazo)) cout << "El mazo esta vacio, no hay nada que repartir" << endl << endl;
	else{
		crearMazoVacio(mazoNegro);
		crearMazoVacio(mazoRojo);
		cout << "Mazo original:" << endl;
		escribirMazo(mazo);
		repartirNegroRojo(mazo, mazoNegro, mazoRojo);
		cout << "Cartas Negras:" << endl;
		escribirMazo(mazoNegro);
		cout << "Cartas Rojas:" << endl;
		escribirMazo(mazoRojo);
	}
}

void repartirBajaAlta(const tMazo mazo, tMazo mazoBajas, tMazo mazoAltas){
	int cantBajas, cantAltas, cantMazo = cuantasEnMazo(mazo);
	crearMazoVacio(mazoBajas);
	crearMazoVacio(mazoAltas);
	cantAltas = cuantasEnMazo(mazoAltas); //Estas dos variables siempre seran igual a 0, 
	cantBajas = cuantasEnMazo(mazoBajas); // asi que se podria poner cantXXX = 0
	for (int i = 0; i < cantMazo; i++){
		if (darNumero(mazo[i]) < 7) {
			mazoBajas[cantBajas] = mazo[i];
			cantBajas++;
		}else{
			mazoAltas[cantAltas] = mazo[i];
			cantAltas++;
		}
	}
	mazoBajas[cantBajas] = CENTINELA; //Ponemos el CENTINELA al final de
	mazoAltas[cantAltas] = CENTINELA; //de cada mazo
}

void opcionRepartirBajaAlta(tMazo mazo){
	tMazo mazoBajas, mazoAltas;
	if (mazoVacio(mazo)) cout << "El mazo esta vacio, no hay nada que repartir" << endl << endl;
	else{
		crearMazoVacio(mazoBajas);
		crearMazoVacio(mazoAltas);
		cout << "Mazo original:" << endl;
		escribirMazo(mazo);
		repartirBajaAlta(mazo, mazoBajas, mazoAltas);
		cout << "Cartas Bajas (A-7):" << endl;
		escribirMazo(mazoBajas);
		cout << "Cartas Altas (8-K):" << endl;
		escribirMazo(mazoAltas);
	}
}

void repartirIntercalando(const tMazo mazo, int enCuantosMazos, int queMazoPido, tMazo mazoNuevo){
	int cantMazo = cuantasEnMazo(mazo), i=0;
	crearMazoVacio(mazoNuevo);
	if ((enCuantosMazos > 0)&&(enCuantosMazos >= queMazoPido)&&(enCuantosMazos <= cantMazo)){
		while (((queMazoPido - 1) + i*enCuantosMazos) < cantMazo){
			mazoNuevo[i] = mazo[(queMazoPido - 1) + i*enCuantosMazos];
			i++;
		}
		mazoNuevo[i] = CENTINELA;
	}
}

void opcionRepartirMontones(const tMazo mazo){
	tMazo mazoNuevo;
	if (mazoVacio(mazo)) cout << "El mazo esta vacio, no hay nada que repartir" << endl << endl;
	else{
		cout << "Mazo original:" << endl;
		escribirMazo(mazo);
		for (int i = 1; i <= 3; i++){
			repartirIntercalando(mazo, 3, i, mazoNuevo);
			cout << "Monton numero " << i <<  ":" << endl;
			escribirMazo(mazoNuevo);
		}
	}
}

void trucoTresMontones(){
	int cantMazo, lugar;
	tMazo mazo, monton1, monton2, monton3;
	crearMazoVacio(mazo);
	if (cargarMazo(mazo)){
		cantMazo = cuantasEnMazo(mazo);
		if (cantMazo == 21){
			barajarMazo(mazo);
			for (int i = 0; i < 3; i++){
				repartirIntercalando(mazo, 3, 1, monton1);
				cout << "Monton numero " << 1 << ":" << endl;
				escribirMazo(monton1);
				repartirIntercalando(mazo, 3, 2, monton2);
				cout << "Monton numero " << 2 << ":" << endl;
				escribirMazo(monton2);
				repartirIntercalando(mazo, 3, 3, monton3);
				cout << "Monton numero " << 3 << ":" << endl;
				escribirMazo(monton3);
				cout << "Memoriza alguna carta de algun monton y di en que monton esta (1-3): ";
				cin >> lugar;
				cout << endl;
				switch (lugar){
				case 1:
					unirMazos(monton2, monton1);
					unirMazos(monton2, monton3);
					pasarAMazo(mazo, monton2);
					break;
				case 2:
					unirMazos(monton1, monton2);
					unirMazos(monton1, monton3);
					pasarAMazo(mazo, monton1);
					break;
				case 3:
					unirMazos(monton1, monton3);
					unirMazos(monton1, monton2);
					pasarAMazo(mazo, monton1);
					break;
				}
			}
			cout << endl << "Tu carta es: ";
			escribirCarta(mazo[10]);
			cout << endl;
		} else cout << "El mazo seleccionado no tiene la cantidad requerida (21)" << endl << endl;
	} else cout << "Error al cargar el mazo o no se encuentra" << endl << endl;
}

void trucoPosada(){
	tMazo mazo, mazoJ, mazoQ, mazoK, mazoAs;
	int cantMazo, corte;
	crearMazoVacio(mazo);
	if (cargarMazo(mazo)){
		cantMazo = cuantasEnMazo(mazo);
		if (cantMazo == 16){
			for (int i = 0; i < 2; i++){
				repartirIntercalando(mazo, 4, 1, mazoJ);
				repartirIntercalando(mazo, 4, 2, mazoQ);
				repartirIntercalando(mazo, 4, 3, mazoK);
				repartirIntercalando(mazo, 4, 4, mazoAs);
				cout << endl << "Habitacion 1:" << endl;
				escribirMazo(mazoJ);
				cout << "Habitacion 2:" << endl;
				escribirMazo(mazoQ);
				cout << "Habitacion 3:" << endl;
				escribirMazo(mazoK);
				cout << "Habitacion 4:" << endl;
				escribirMazo(mazoAs);
				if (i == 0){
					unirMazos(mazoJ, mazoQ);
					unirMazos(mazoJ, mazoK);
					unirMazos(mazoJ, mazoAs);
					pasarAMazo(mazo, mazoJ);
					cout << "Ahora uniremos los cuatros mazos en un solo mazo" << endl;
					cout << "Introduce la cantidad de cartas a coger para realizar el corte: ";
					cin >> corte;
					cortarMazo(mazo, corte);
				}
			}
		} else cout << "El mazo seleccionado no tiene la cantidad requerida (16)" << endl << endl;
	} else cout << "Error al cargar el mazo o no se encuentra" << endl << endl;
}

void trucoJugadorDesconfiado(){
	tMazo mazo, mazo1, mazo1Izq, mazo1Der, mazo2, mazo2Izq, mazo2Der, mazo3, mazo3Izq, mazo3Der, mazo4, mazo4Izq, mazo4Der, mazoAux1, mazoAux2;
	if (cargarMazo(mazo)){
		if (cuantasEnMazo(mazo) == 20){
			barajarMazo(mazo);
			cout << "Se repartiran las siguientes 20 cartas entre los cuatro jugadores:" << endl;
			escribirMazo(mazo);
			cout << "Reparto inicial de cartas entre los cuatro jugadores:" << endl;
			repartirYMostrar(mazo, 4, 1, mazo1);
			repartirYMostrar(mazo, 4, 2, mazo2);
			repartirYMostrar(mazo, 4, 3, mazo3);
			repartirYMostrar(mazo, 4, 4, mazo4);
			for (int i = 0; i <= 2; i++){//Paso de dividir
				repartirNegroRojo(mazo1, mazo1Izq, mazo1Der);
				repartirBajaAlta(mazo2, mazo2Izq, mazo2Der);
				repartirParImpar(mazo3, mazo3Izq, mazo3Der);
				repartirFiguraNoFigura(mazo4, mazo4Izq, mazo4Der);
				if (i < 2){
					intercambiarMazos(mazo1Izq, mazo4Der);
					intercambiarMazos(mazo2Izq, mazo1Der);
					intercambiarMazos(mazo3Izq, mazo2Der);
					intercambiarMazos(mazo4Izq, mazo3Der);
				}
				else{
					crearMazoVacio(mazoAux1);
					crearMazoVacio(mazoAux2);
					pasarAMazo(mazoAux1, mazo4Izq);
					pasarAMazo(mazo4Izq, mazo1Izq);
					pasarAMazo(mazoAux2, mazo3Izq);
					pasarAMazo(mazo3Izq, mazoAux1);
					pasarAMazo(mazoAux1, mazo2Izq);
					pasarAMazo(mazo2Izq, mazoAux2);
					pasarAMazo(mazo1Izq, mazoAux1);
				}
				unirMazos(mazo1Izq, mazo1Der);
				pasarAMazo(mazo1, mazo1Izq);
				unirMazos(mazo2Izq, mazo2Der);
				pasarAMazo(mazo2, mazo2Izq);
				unirMazos(mazo3Izq, mazo3Der);
				pasarAMazo(mazo3, mazo3Izq);
				unirMazos(mazo4Izq, mazo4Der);
				pasarAMazo(mazo4, mazo4Izq);
				cout << "Distribucion tras el paso " << i + 1 << ":" << endl;
				cout << "Cartas del Jugador 1:" << endl;
				escribirMazo(mazo1);
				cout << "Cartas del Jugador 2:" << endl;
				escribirMazo(mazo2);
				cout << "Cartas del Jugador 3:" << endl;
				escribirMazo(mazo3);
				cout << "Cartas del Jugador 4:" << endl;
				escribirMazo(mazo4);
			}
			cout << "Distribucion final alcanzada (ahora los jugadores apostaran con esa mano)." << endl << endl;
		}
		else cout << "El mazo seleccionado no tiene la cantidad requerida (20)" << endl << endl;
	}else cout << "Error al cargar el mazo o no se encuentra" << endl << endl;
}

void repartirYMostrar(const tMazo mazo, int enCuantosMazos, int queMazoPido, tMazo mazoNuevo){
	repartirIntercalando(mazo, enCuantosMazos, queMazoPido, mazoNuevo);
	cout << "Cartas del Jugador " << queMazoPido << ":" << endl;
	escribirMazo(mazoNuevo);
}

void intercambiarMazos(tMazo mazo1, tMazo mazo2){
	int vueltas, cant1 = cuantasEnMazo(mazo1),cant2 = cuantasEnMazo(mazo2), aux;
	if (cant1 > cant2) vueltas = cant1; // cogemos la mayor cantidad que contengan los mazos, y esas seran los intercambios que las cartas haran
	else vueltas = cant2;
	for (int i = 0; i < vueltas; i++){
		aux = mazo1[i];
		mazo1[i] = mazo2[i];
		mazo2[i] = aux;
	}
	mazo1[cant2] = CENTINELA; // como el mazo1 es igual al mazo2 de antes, la cantidad del mazo2 corresponde a la cantidad de mazo1 despues de hacer el intercambio
	mazo2[cant1] = CENTINELA; // lo mismo explicado en la linea anterior, solo que al reves
}

void repartirParImpar(const tMazo mazo, tMazo izq, tMazo der){ // Izq = Pares; Der = Impares
	int cantPar, cantImpar, cantMazo = cuantasEnMazo(mazo);
	crearMazoVacio(izq);
	crearMazoVacio(der);
	for (int i = 0; i < cantMazo; i++){
		if (((darNumero(mazo[i]) + 1) % 2) == 0) {
			cantPar = cuantasEnMazo(izq);
			izq[cantPar] = mazo[i];
			izq[cantPar + 1] = CENTINELA;
		}else{
			cantImpar = cuantasEnMazo(der);
			der[cantImpar] = mazo[i];
			der[cantImpar + 1] = CENTINELA;
		}
	}
}

void repartirFiguraNoFigura(const tMazo mazo, tMazo izq, tMazo der){ // Izq = A, J, Q, K; Der = El resto
	tNumero numero;
	int cantFiguras, cantNumeros, cantMazo = cuantasEnMazo(mazo);
	crearMazoVacio(izq);
	crearMazoVacio(der);
	for (int i = 0; i < cantMazo; i++){
		numero = darNumero(mazo[i]);
		if ((numero == as) || (numero == jack) || (numero == queen) || (numero == king)){
			cantFiguras = cuantasEnMazo(izq);
			izq[cantFiguras] = mazo[i];
			izq[cantFiguras + 1] = CENTINELA;
		}else{
			cantNumeros = cuantasEnMazo(der);
			der[cantNumeros] = mazo[i];
			der[cantNumeros + 1] = CENTINELA;
		}
	}
}