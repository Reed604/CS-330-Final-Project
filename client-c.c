/*****************************************************************************
 * client-c.c   For Final Project                                                              
 * Name: Brad Schaeffer
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define SEND_BUFFER_SIZE 2048

char grid[10] = { 'o', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
int user1;
int user2;

void board() //Tik Tac Toe board
{
    //%c prints value of grid[index] while still holding formatting
    system("cls");
    printf("Player 1 => X  and  Player 2 => O\n\n\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", grid[1], grid[2], grid[3]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", grid[4], grid[5], grid[6]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", grid[7], grid[8], grid[9]);
    printf("     |     |     \n\n");

}

int main(int argc, char **argv) {

  struct hostent *hp;
  struct sockaddr_in sin;
  char *host, *port;
  char buf[SEND_BUFFER_SIZE];
  int s, new_s;
  double buf_len;
  char *URI;
  int size;

  if (argc==3) {
  host = argv[1];
  port = argv[2];
  }
  else {
  fprintf(stderr, "usage: %s host port\n", argv[0]);
  exit(1);
  }

  URI= argv[1];

  /* translate host name into peer's IP address */
  hp = gethostbyname(host);
  if (!hp) {
  fprintf(stderr, "%s: unknown host\n", host);
  exit(1);
  }

  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);

  /*open*/
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
  perror("simplex-talk: socket");
  exit(1);
  }
  //connect to server
  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
  perror("simplex-talk: connect");
  close(s);
  exit(1);
  }

 while(1){
    //Display Board To user
    board();
    //Prompt user1 to enter grid number
    printf("Player 1, enter a grid number:  ");
    //Prepare the Users input and place in buffer to send to server
    //scanf("%d", user1);
    int size; 
    size=fread(buf, 1, SEND_BUFFER_SIZE, stdin); //reads user input and puts in pointer buf
    scanf("%d", user1); //reads user input for purpose of updating grid array
    grid[user1] = 'X';//update grid array
    //sends char to server
    if(send(s, buf, sizeof(buf), 0) < 0) {
    perror("Didnt sent to server...");
    }
    //Receive from server (response will be if user1 has won the game)
    if (buf_len = recv(new_s, buf, sizeof(buf), 0)){
    printf("Response:\n%s\n",buf);
    close(new_s);
    }

    //Display updated board to User2
    board();

    //Prompt user2 to enter of where they want the 'O'
    printf("Player 2, enter a grid number:  ");
    size=fread(buf, 1, SEND_BUFFER_SIZE, stdin);
    scanf("%d", user2); //change to fread
    grid[user2] = 'O'; //update grid array
    //Prepare the Users input and place in buffer to send to server
    //send to server
    if(send(s, buf, sizeof(buf), 0) < 0) {
    perror("Didnt sent to server...");
    }
    
    //Receive from server (response will be if user2 has won the game)
    if (buf_len = recv(new_s, buf, sizeof(buf), 0)){
    printf("Response:\n%s\n",buf);
    close(new_s);
    }
 }

  //end while loop
  return 0;
}
