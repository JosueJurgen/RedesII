// Nombre : servidor
// Proposito : Recibe un mensaje y lo envia de vuelta al cliente
// Autor : Edwin Carrasco (Adaptado de [1])
// FCreacion : --
// FModific. : 24/04/2009
// Compilacion : gcc -o Servidor Servidor.c
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#define MAXPENDING 5 /* Nro maximo de conexiones */
#define BUFFSIZE 32
void AvisarError(char *mensaje)
{
 perror(mensaje);
 exit(1);
}
void AtenderCliente(int sock)
{
// Variables
 char buffer[BUFFSIZE];
 int received = -1;
 // Recibir mensaje
if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0)
 {
 AvisarError("No se pudo recibir bytes iniciales del cliente");
 }
// Envia bytes y verifica mas datos entrantes
while (received > 0)
 {
 // Enviar de vuelta los datos recibidos
 if (send(sock, buffer, received, 0) != received)
 {
 AvisarError("No se pudo enviar bytes al cliente");
 }

 // Verificar mas datos
 if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0)
 {
 AvisarError("No se pudo recibir bytes adicionales del cliente");
 }
 }
 close(sock);
}
int main(int argc, char *argv[])
{
 int serversock;
 int clientsock;
 struct sockaddr_in echoserver;
 struct sockaddr_in echoclient;

 if (argc != 2)
 {
 fprintf(stderr, "USO: servidor <puerto>\n");
 exit(1);
 }

 // Crear un socket TCP
 if ((serversock = socket(PF_INET, SOCK_STREAM,
 IPPROTO_TCP)) < 0)
 {
 AvisarError("No se pudo crear socket");
 }

 /* Construir la estructura sockaddr_in del servidor */
 // Limpiar la estructura
 memset(&echoserver, 0, sizeof(echoserver));
 // Protocolo Internet/IP
 echoserver.sin_family = AF_INET;
 // Direccion de ingreso
 echoserver.sin_addr.s_addr = htonl(INADDR_ANY);
 // Puerto en que escucha el servidor
 echoserver.sin_port = htons(atoi(argv[1]));
 // Enlazar el socket del servidor

 if (bind(serversock, (struct sockaddr *) &echoserver,
 sizeof(echoserver)) < 0)
 {
 AvisarError("No se pudo enlazar el socket de servidor");
 }

 // Escuchar en el socket del servidor
 if (listen(serversock, MAXPENDING) < 0)
 {
 AvisarError("No se pudo escuchar en el socket del servidor");
 }

 // Ejecutar hasta que se cancele
 while (1)
 {
 unsigned int clientlen = sizeof(echoclient);

 // Esperar conexion de cliente
 if ((clientsock=accept(serversock,(struct sockaddr *)
 &echoclient, &clientlen)) < 0 )
 {
 AvisarError("No se pudo aceptar conexion de cliente");
 }

 fprintf(stdout, "Cliente conectado: %s\n",
 inet_ntoa(echoclient.sin_addr));
 AtenderCliente(clientsock);
 }
}
