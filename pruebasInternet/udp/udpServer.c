#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
 
int puerto_Servidor = 60000;
int buffer_Recepcion = 1024;
 
int main(int argc, char *argv[])
{
 int sockfd; /* descriptor para el socket */
 struct sockaddr_in my_addr; /* direccion IP y numero de puerto local */
 struct sockaddr_in their_addr; /* direccion IP y numero de puerto del cliente */
 /* addr_len contendra el tamaño de la estructura sockadd_in y numbytes el
 * numero de bytes recibidos
 */
 int addr_len, numbytes;
 char buf[buffer_Recepcion]; /* Buffer de recepción */
 
 printf("Puerto de escucha: %d\n", puerto_Servidor);
 //comienza la parte importante!
 while(1) {
    /* se crea el socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
       perror("socket");
       exit(1);
       }
 
   /* Se establece la estructura my_addr para luego llamar a bind() */
   my_addr.sin_family = AF_INET; /* usa host byte order */
   my_addr.sin_port = htons(puerto_Servidor); /* usa network byte order */
   my_addr.sin_addr.s_addr = INADDR_ANY; /* escuchamos en todas las IPs */
   bzero(&(my_addr.sin_zero), 8); /* rellena con ceros el resto de la estructura */

   /* Se le da un nombre al socket (se lo asocia al puerto e IPs) */
   printf("Creando socket ....\n");
   
   if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) { //si da error invoca a perror (que imprime un mensaje personalizado de error)
      perror("bind error");
      exit(1);
      }

    /* Se reciben los datos (directamente, UDP no necesita conexión) */
    addr_len = sizeof(struct sockaddr);
    printf("Esperando datos ....\n");
    if ((numbytes=recvfrom(sockfd, buf, buffer_Recepcion, 0, (struct sockaddr *)&their_addr, (socklen_t *)&addr_len)) == -1) {
       perror("recvfrom");
       exit(1);
       }
 
 
    /* Se visualiza lo recibido */
    printf("Paquete UDP proveniente de : %s\n",inet_ntoa(their_addr.sin_addr));
    printf("Longitud del paquete en bytes : %d\n",numbytes);
    buf[numbytes] = '\0';
    printf("Información recibida: %s\n\n", buf);
 
    /* cerramos descriptor del socket */
    close(sockfd);
}
 return 0;
}
