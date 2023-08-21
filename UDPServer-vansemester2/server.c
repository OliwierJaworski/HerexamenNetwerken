#ifdef _WIN32
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <winsock2.h> //for all socket programming
#include <ws2tcpip.h> //for getaddrinfo, inet_pton, inet_ntop
#include <stdio.h> //for fprintf, perror
#include <unistd.h> //for close
#include <stdlib.h> //for exit
#include <string.h> //for memset
#include <time.h>
#include <sys/timeb.h>
void OSInit( void )
{
    WSADATA wsaData;
    int WSAError = WSAStartup( MAKEWORD( 2, 0 ), &wsaData );
    if( WSAError != 0 )
    {
        fprintf( stderr, "WSAStartup errno = %d\n", WSAError );
        exit( -1 );
    }
}
void OSCleanup( void )
{
    WSACleanup();
}
#define perror(string) fprintf( stderr, string ": WSA errno = %d\n", WSAGetLastError() )
#else
#include <sys/socket.h> //for sockaddr, socket, socket
#include <sys/types.h> //for size_t
#include <netdb.h> //for getaddrinfo
#include <netinet/in.h> //for sockaddr_in
#include <arpa/inet.h> //for htons, htonl, inet_pton, inet_ntop
#include <errno.h> //for errno
#include <stdio.h> //for fprintf, perror
#include <unistd.h> //for close
#include <stdlib.h> //for exit
#include <string.h> //for memset
int OSInit( void ) {}
int OSCleanup( void ) {}
#endif

int initialization();
void execution( int internet_socket );
void cleanup( int internet_socket );

int main( int argc, char * argv[] )
{
    //////////////////
    //Initialization//
    //////////////////

    OSInit();

    int internet_socket = initialization();

    /////////////
    //Execution//
    /////////////

    execution( internet_socket );


    ////////////
    //Clean up//
    ////////////

    cleanup( internet_socket );

    OSCleanup();
    while(1);
    return 0;
}

int initialization()
{
    //Step 1.1
    struct addrinfo internet_address_setup;
    struct addrinfo * internet_address_result;
    memset( &internet_address_setup, 0, sizeof internet_address_setup );
    internet_address_setup.ai_family = AF_UNSPEC;
    internet_address_setup.ai_socktype = SOCK_DGRAM;
    internet_address_setup.ai_flags = AI_PASSIVE;
    int getaddrinfo_return = getaddrinfo( NULL, "62573", &internet_address_setup, &internet_address_result );
    if( getaddrinfo_return != 0 )
    {
        fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( getaddrinfo_return ) );
        exit( 1 );
    }

    int internet_socket = -1;
    struct addrinfo * internet_address_result_iterator = internet_address_result;
    while( internet_address_result_iterator != NULL )
    {
        //Step 1.2
        internet_socket = socket( internet_address_result_iterator->ai_family, internet_address_result_iterator->ai_socktype, internet_address_result_iterator->ai_protocol );
        if( internet_socket == -1 )
        {
            perror( "socket" );
        }
        else
        {
            //Step 1.3
            int bind_return = bind( internet_socket, internet_address_result_iterator->ai_addr, internet_address_result_iterator->ai_addrlen );
            if( bind_return == -1 )
            {
                close( internet_socket );
                perror( "bind" );
            }
            else
            {
                break;
            }
        }
        internet_address_result_iterator = internet_address_result_iterator->ai_next;
    }

    freeaddrinfo( internet_address_result );

    if( internet_socket == -1 )
    {
        fprintf( stderr, "socket: no valid socket address found\n" );
        exit( 2 );
    }

    return internet_socket;
}

void execution( int internet_socket )
{
    DWORD timeout = 1 * 1000;
    setsockopt(internet_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);

    clock_t start_time = clock();
    clock_t current_time = clock();
    clock_t elapsed_time = current_time - start_time;




    //Step 2.1
    int number_of_bytes_received = 0;
    char buffer[1000];
    struct sockaddr_storage client_internet_address;
    socklen_t client_internet_address_length = sizeof client_internet_address;
    number_of_bytes_received = recvfrom(internet_socket, buffer, (sizeof buffer) - 1, 0,
                                        (struct sockaddr *) &client_internet_address,
                                        &client_internet_address_length);
    if (number_of_bytes_received == -1) {
        perror("recvfrom");
    } else {
        buffer[number_of_bytes_received] = '\0';
        printf("Received : %s\n", buffer);
    }


    //als de client GO send->code uitvoeren


    if( strncmp(buffer,"GO",2)==0) {
        for (int iteration = 0; iteration < 2; iteration++) {
            {


                for (int i = 0; i < 43; i++) {
                    int RandomGetal = rand();
                    char StrRandomGetal[100];
                    sprintf(StrRandomGetal, "%d", RandomGetal);
                    //Step 2.2
                    int number_of_bytes_send = 0;
                    number_of_bytes_send = sendto(internet_socket, StrRandomGetal, strlen(StrRandomGetal), 0,
                                                  (struct sockaddr *) &client_internet_address,
                                                  client_internet_address_length);
                    if (number_of_bytes_send == -1) {
                        perror("sendto");
                    }
                }

            }

            int receive_numbers[42];
            int num_values_received = 0;

            //ontvangen van grootste waarde
            while (elapsed_time < timeout && num_values_received < 43) {
                clock_t current_time = clock();
                elapsed_time = current_time - start_time;

                num_values_received += 1;
                int number_of_bytes_received = 0;
                char buffer[1000];
                struct sockaddr_storage client_internet_address;
                socklen_t client_internet_address_length = sizeof client_internet_address;
                number_of_bytes_received = recvfrom(internet_socket, buffer, (sizeof buffer) - 1, 0,
                                                    (struct sockaddr *) &client_internet_address,
                                                    &client_internet_address_length);
                if (number_of_bytes_received == -1) {
                    perror("recvfrom");
                } else {
                    buffer[number_of_bytes_received] = '\0';
                    printf("Received : %s\n", buffer);
                }
            }

        }
    }
    int number_of_bytes_send = 0;
    number_of_bytes_send = sendto(internet_socket, "OK", 3, 0,
                                  (struct sockaddr *) &client_internet_address,
                                  client_internet_address_length);
    if (number_of_bytes_send == -1) {
        perror("sendto");
    }


}

void cleanup( int internet_socket )
{
    //Step 3.1
    close( internet_socket );
}