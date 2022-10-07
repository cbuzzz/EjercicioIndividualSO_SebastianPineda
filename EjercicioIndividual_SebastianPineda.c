#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	int cantidad;
	char jugador[20];
	char recurso[20];
	char consulta[80];
	//Creamos una conexion al servidor MYSQL
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","user", "pass", "db_catan",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	// consulta SQL para obtener la cantidad de un determinado recurso
	// que tiene un determinado jugador
    printf ("Escribe el nombre del jugador y el recurso del que deseas saber la cantidad");
	err = scanf ("%s %s", jugador, recurso);
	err=mysql_query (conn, "SELECT cantidad FROM (jugador,recursos,inventario) WHERE jugador.nombre = %s AND jugador.id = inventario.id_j AND inventario.id_r = recurso.id AND recurso.nombre = %s", jugador, recurso);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	// En una fila hay tantas columnas como datos tiene una
	// jugador.
	if (row == NULL)
		printf ("No se han obtenido datos en la consulta\n");
	else
		while (row !=NULL) { 
			cantidad = atoi (row[2]);
			// las columnas 0 y 1 contienen DNI y nombre
			printf ("El jugador indicado tiene %d unidades del recurso indicado\n", cantidad);
			// obtenemos la siguiente fila
			row = mysql_fetch_row (resultado);
	}