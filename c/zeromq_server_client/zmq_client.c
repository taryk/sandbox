#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

//  Receive 0MQ string from socket and convert into C string
static char *
s_recv (void *socket) {
  zmq_msg_t message;
  zmq_msg_init (&message);
  zmq_recvmsg (socket, &message, 0);
  // return (NULL);
  int size = zmq_msg_size (&message);
  char *string = malloc (size + 1);
  memcpy (string, zmq_msg_data (&message), size);
  zmq_msg_close (&message);
  string [size] = 0;
  printf("s: %s\n",string);
  return (string);
}

int main (void)
{
  void *context = zmq_init (1);

  //  Socket to talk to server
  printf ("Connecting to hello world server…\n");
  void *requester = zmq_socket (context, ZMQ_SUB);
  //zsockopt_set_subscribe (requester, "");
  //int rc = zmq_connect (requester, "tcp://192.168.1.4:5558");
  int rc = zmq_connect (requester, "tcp://127.0.0.1:5558");
  zmq_setsockopt (requester, ZMQ_SUBSCRIBE, "C", 1);
  printf("rc %d\n", rc);

  int request_nbr;
  //for (request_nbr = 0; request_nbr != 10; request_nbr++) {
    /* zmq_msg_t request; */
    /* zmq_msg_init_size (&request, 5); */
    /* memcpy (zmq_msg_data (&request), "Hello", 5); */
    /* printf ("Sending Hello %d…\n", request_nbr); */
    /* zmq_send (requester, &request, 0); */
    /* zmq_msg_close (&request); */

    
  while (1) {
    sleep(1);
    char *str = s_recv(requester);
    printf("recieved '%s'\n", str);
    /* zmq_msg_t recieved_msg; */
    /* zmq_msg_init (&recieved_msg); */
    /* printf("try init\n"); */
    /* zmq_recvmsg (requester, &recieved_msg, 0); */
    /* printf("try recv\n"); */
    /* int size = zmq_msg_size (&recieved_msg); */
    /* char *string = malloc (size + 1); */
    /* memcpy (string, zmq_msg_data (&recieved_msg), size); */
    /* printf("recieved '%s'\n", string); */
    /* zmq_msg_close (&recieved_msg); */
    /* free(string); */
  }
  zmq_close (requester);
  zmq_term (context);
  return 0;
}

