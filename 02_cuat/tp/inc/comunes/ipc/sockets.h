#ifndef __SOCKETS_H
#define __SOCKETS_H

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>


/**
 * \fn int crear_socket(int dominio, int tipo, int protocolo, int puerto, struct sockaddr_in *info_socket, char* dir_ip, char rol)
 * \brief Funcion que crea un socket TCP o UDP.
 * \details Esta funcion crea un socket TCP o UDP, segun se especifique con SOCK_STREAM o SOCK_DGRAM. Crea
		 el socket y lo bindea a un puerto pasado tambien como parametro.
 * \param dominio especifica el dominio del socket: AF_INET o AF_UNIX.
 * \param tipo especifica si es SOCK_STREAM (TCP) o SOCK_DGRAM (UDP).
 * \param protocolo especifica el protocolo para el tipo de socket, segun el dominio. Dejando "0", hacemos que
	   el sistema elija el protocolo mas acorde.
 * \param puerto especifica el puerto al cual "bindearemos" el socket creado.
 * \param info_socket es un puntero a una estructura del tipo sockaddr_in donde almacenaremos los datos del
	   socket creado.
 * \param dir_ip con esto lo asinamos una direccion IP al socket. Con la macro INADDR_ANY hacemos que el
	   sistema seleccione la IP local (localhost o loopback, 127.0.0.1) o la IP asignada por el DHCP del
	   ISP que tengamos, o el router, o switch.
 * \param rol especifica el rol del socket, si sera cliente o servidor.
 * \return int retorna el identificador del socket creado.
 * \author	Luciano Ferreyro
 */
int crear_socket(int dominio, int tipo, int protocolo, int puerto, struct sockaddr_in *info_socket, char* dir_ip, char rol);

/**
 * \fn int atender_conexion(int SOCKET, (struct sockaddr*)info_socket_CLT,unsigned char *DATO)
 * \brief Funcion que atiende una conexion TCP con un cliente.
 * \details Esta funcion se encarga de realizar un handshake entre el servidor y el cliente nuevo. Hace 
		 el accept().
 * \param SOCKET Socket a traves del cual se hace el connect-accept
 * \param *info_socket_CLT se almacenan los datos del cliente que se conecto.
 * \param *datos puntero al dato (string generalmente, por ser TCP) a enviar.
 * \return int Ante exito, retorna el socket por donde se realiza la comunicacion. Retorna -1 si hubo error.
 * \author	Luciano Ferreyro
 * 
 **/
int atender_conexion(int SOCKET,struct sockaddr_in *info_socket_CLT);//, char *DATO);

/**
 * \fn int conectar_cliente(int SOCKET, (struct sockaddr*)info_socket,char *DIRECCION, int PORT, int DOMINIO)
 * \brief Funcion que conecta a un cliente.
 * \details Esta funcion se encarga de realizar un la conexion TCP entre el cliente y el servidor.
 * \param SOCKET Socket a traves del cual se hace el connect-accept
 * \param *info_socket se almacenan los datos del servidor donde nos conectaremos.
 * \param *DIRECCION es la direccion IP del servidor.
 * \param PORT es el puerto a traves del cual se realizara la conexion TCP.
 * \param DOMINIO es el dominio de la conxion: AF_INET o AF_UNIX
 * \return int Ante exito, retorna 1, y ante un error de connect devuelve -1.
 * \author	Luciano Ferreyro
**/
int conectar_cliente(int SOCKET, struct sockaddr_in *info_socket, char *DIRECCION, int PORT, int DOMINIO);

/**
 * \fn int cargar_ip_cliente (struct sockaddr_in *info_socket_TCP, struct sockaddr_in *info_socket_UDP, struct sockaddr_in *info_socket_UDP_DST)
 * \brief Funcion que carga parametros.
 * \details Esta funcion se encarga de preparar la estructura del tipo sockaddr_in con la IP (en particular) que utilizara 
  	     el servidor (en este caso) para enviar datos utilizando la funcion sendto.
 * \param SOCKET Socket a traves del cual se hace el connect-accept
 * \param *info_socket se almacenan los datos del servidor donde nos conectaremos.
 * \param *DIRECCION es la direccion IP del servidor.
 * \param PORT es el puerto a traves del cual se realizara la conexion TCP.
 * \param DOMINIO es el dominio de la conxion: AF_INET o AF_UNIX
 * \return int Ante exito, retorna 1, y ante un error de connect devuelve -1.
 * \author	Luciano Ferreyro
**/
void cargar_ip_cliente (struct sockaddr_in *info_socket_TCP, struct sockaddr_in *info_socket_UDP, struct sockaddr_in *info_socket_UDP_DST);

/**
 * @brief Crea un socket servidor
 * 
 * @param puerto Puerto a utilizar por el socket
 * @param info_socket Estructura donde se almacena la informacion del socket
 * @param backlog Cantidad de clientes en cola de espera
 * @return int En exito retorna el descriptor del socket (valor positivo), en error retorna -1
 */
int crear_servidor(int puerto, struct sockaddr_in *info_socket, int backlog);

/**
 * @brief Cierra el servidor.
 * 
 * @param servidor_fd Descriptor del servidor.
 * @return int 0 en exito, -1 en error.
 */
int cerrar_servidor(int servidor_fd);
#endif /*__SOCKETS_H*/
