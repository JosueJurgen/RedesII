// Nombre : Cliente
// Proposito : Envia un mensaje y muestra el mensaje devuelto por el servidor
// Autor : Edwin Carrasco (Adaptado de [1])
// FCreacion : --
// FModific. : --
// Compilacion : gcc -o Cliente Cliente.c
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#define BUFFSIZE 32 

void AvisarError(char *mensaje)
{
 perror(mensaje);
 exit(1);
}
int main(int argc, char *argv[])
{
 // Variables
 int sock;
 struct sockaddr_in echoserver;
 char buffer[BUFFSIZE];
 unsigned int echolen;
 int received = 0;

 // Verificar argumentos necesarios para correr programa
 if (argc != 4)
 {
 fprintf(stderr, "USO: Cliente <ip de servidor> <mensaje> <puerto>\n");
 exit(1);
 }

 // Crear el socket TCP
 if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))< 0)
 {
 AvisarError("No se pudo crear socket");
 }

 /* Construir la estructura sockaddr_in del servidor */
 // Limpiar la estructura
 memset(&echoserver, 0, sizeof(echoserver));
 // Protocolo de Internet/IP
 echoserver.sin_family = AF_INET;
 // Direccion IP
 echoserver.sin_addr.s_addr = inet_addr(argv[1]);
 // Puerto del servidor
 echoserver.sin_port=htons(atoi(argv[3]));
 // Establecer la conexion
 if (connect(sock,(struct sockaddr *) &echoserver,
 sizeof(echoserver)) < 0 )
 {
 AvisarError("No se pudo conectar al servidor");
 }

 // Enviar en mensaje al servidor
 echolen = strlen(argv[2]);
if (send(sock, argv[2], echolen, 0) != echolen)
 {
 AvisarError("Inconsistencia en el numero de bytes enviados");
 }

 // Recibir el mensaje de vuelta desde el servidor
 fprintf(stdout, "Recibido: ");
 while (received < echolen)
 {
 int bytes = 0;
 if ((bytes = recv(sock, buffer, BUFFSIZE-1, 0)) < 1)
 {
 AvisarError("No se pudo recibir bytes desde el servidor");
 }

 received += bytes;
 buffer[bytes]='\0'; // Cerciorarse que la cadena termine en \0
 fprintf(stdout, buffer);
 }

 fprintf(stdout, "\n");
 close(sock);
 exit(0);
} 
