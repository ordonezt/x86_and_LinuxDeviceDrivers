#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../../../inc/comunes/ipc/datos.h"

#define ROL_CLIENTE		0
#define ROL_SERVIDOR	1

int crear_socket(int dominio, int tipo, int protocolo, int puerto, struct sockaddr_in *info_socket, char* dir_ip, char rol);
int atender_conexion(int,struct sockaddr_in *);//,char *);
int conectar_cliente(int , struct sockaddr_in *, char *, int , int );
void cargar_ip_cliente (struct sockaddr_in *, struct sockaddr_in *, struct sockaddr_in *);

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
int crear_socket(int dominio, int tipo, int protocolo, int puerto, struct sockaddr_in *info_socket, char* dir_ip, char rol)
{
	int socket_obtenido, yes = 1;
	struct sockaddr_in AUX;

	if ((socket_obtenido = socket(dominio,tipo,protocolo)) == -1) 	//Ubicamos un socket util
	{
		perror("socket");
		return -1;
	}
	
	if(setsockopt(socket_obtenido,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
	{
		perror("setsockopt");
		return -1;
	}
	
	if(rol) //Si es Server
	{
		bzero(info_socket,sizeof(*info_socket));	//Inicializo en cero.
		(*info_socket).sin_family = dominio;		//Asignamos el protocolo de comunicacion.
		(*info_socket).sin_port = htons(puerto);  	//Asignamos el puerto.
		(*info_socket).sin_addr.s_addr = htonl(INADDR_ANY); //Tomamos cualquier IP de la maquina. Veremos que aparece: 0.0.0.0, pero eso implica para 
														    //el socket, que se usaran cualquier de las direccion: 127.0.0.1 (localhost o loopback) o la ip
														    //que el ISP o el DHCP server de una red, le haya asignado a nuestra PC.
		bzero(((*info_socket).sin_zero),8);		
			
		printf("\nSocket servidor... %d\n", socket_obtenido);

		if (bind(socket_obtenido,(struct sockaddr*)info_socket, sizeof(struct sockaddr_in)) == -1) 	//Le asignamos un puerto al socket obtenido.
		{
			close(socket_obtenido);
			perror("bind");
			return -1;
		}
	}
	else
	{
		// printf("\nSocket cliente...");
		
		if(tipo == SOCK_DGRAM)
		{
			bzero(&info_socket,sizeof(*info_socket));		//Inicializo en cero.
			(*info_socket).sin_family = dominio;		//Asignamos el protocolo de comunicacion.
			(*info_socket).sin_port = htons(puerto);  	//Asignamos el puerto.
			(*info_socket).sin_addr.s_addr = inet_addr(dir_ip); 
			bzero(((*info_socket).sin_zero),8);	

			if (bind(socket_obtenido,(struct sockaddr*)info_socket, sizeof(struct sockaddr_in)) == -1) 
				{
				close(socket_obtenido);
				perror("bind");
				return (-1);
				}			
		}
		else
		{
			(*info_socket).sin_family = AF_INET;
			(*info_socket).sin_port = htons(puerto);
			(*info_socket).sin_addr.s_addr = inet_addr(dir_ip);
			bzero(((*info_socket).sin_zero),8);
			if(connect(socket_obtenido, (struct sockaddr*)info_socket, sizeof(struct sockaddr_in)) == -1)
			{
				perror("connect");
				exit(1);
			}
		}
	}
	
	//printf("\nAsignacion exitosa del puerto e IP del socket");
	fflush(stdout);		
	return socket_obtenido;
		
}

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
int atender_conexion(int SOCKET,struct sockaddr_in *info_socket_CLT)//, char *DATO)
	{
	socklen_t socket_size;
	int socket_nuevo;
	
	socket_size = sizeof(info_socket_CLT);
	//Conexion TCP	
	if((socket_nuevo = accept(SOCKET,(struct sockaddr*)info_socket_CLT, &socket_size)) == -1)
		{
		if (errno != EINTR)
		  perror("accept");
		return (-1);	
		}
	// //Si se supera al condicional, entonces quiere decir que se pudo efectuar una conexion. Fue aceptada.
	// //printf("\nNuevo cliente conectado. Socket OK!\n\n");
	// if (send(socket_nuevo, DATO, strlen(DATO), 0) == -1)
	// 	{
	// 	perror("send");		
	// 	return (-1);
	// 	}			
	return (socket_nuevo);
	}

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
int conectar_cliente(int SOCKET, struct sockaddr_in *info_socket, char *DIRECCION, int PORT, int DOMINIO)
	{
	//Aca deberia hacer un bind, si pretendo enviar cosas por el cliente. Por ahora no hago nada, solo recibo desde el servidor.
	//Cargo la estructura del socket del Server
	bzero(info_socket, sizeof(*info_socket));
	(*info_socket).sin_family = DOMINIO;
	(*info_socket).sin_port = htons(PORT);
	(*info_socket).sin_addr.s_addr = inet_addr(DIRECCION); //Ingresada por el usuario
	bzero((*info_socket).sin_zero,8);	

	if(connect(SOCKET,(struct sockaddr *)info_socket,sizeof(struct sockaddr_in)) == -1)
		{
		//Si no pudo conectarse, cerramos el SOCKET ya que no hace falta
		close(SOCKET);
		perror("\nError en el cliente: connect");
		return (-1);
		}
	return 1;	
	}

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
void cargar_ip_cliente (struct sockaddr_in *info_socket_TCP, struct sockaddr_in *info_socket_UDP, struct sockaddr_in *info_socket_UDP_DST)
	{
	bzero(info_socket_UDP_DST, sizeof(*info_socket_UDP_DST));
	(*info_socket_UDP_DST).sin_addr.s_addr = (*info_socket_TCP).sin_addr.s_addr;		
	(*info_socket_UDP_DST).sin_family = (*info_socket_UDP).sin_family;
	(*info_socket_UDP_DST).sin_port = (*info_socket_UDP).sin_port;
	bzero((*info_socket_UDP_DST).sin_zero,8);
	
	}
	
/**
 * @brief Crea un socket servidor
 * 
 * @param puerto Puerto a utilizar por el socket
 * @param info_socket Estructura donde se almacena la informacion del socket
 * @param backlog Cantidad de clientes en cola de espera
 * @return int En exito retorna el descriptor del socket (valor positivo), en error retorna -1
 */
int crear_servidor(int puerto, struct sockaddr_in *info_socket, int backlog)
{
	int socket_obtenido;

	socket_obtenido = crear_socket(PF_INET, SOCK_STREAM, 0, puerto, info_socket, 0, ROL_SERVIDOR);
	if(socket_obtenido == -1)
	{
		perror("crear_socket");
		return -1;
	}

	if(listen(socket_obtenido, backlog))
	{
		close(socket_obtenido);
		perror("listen");
		return -1;
	}

	return socket_obtenido;
}

/**
 * @brief Cierra el servidor.
 * 
 * @param servidor_fd Descriptor del servidor.
 * @return int 0 en exito, -1 en error.
 */
int cerrar_servidor(int servidor_fd)
{
	return close(servidor_fd);
}