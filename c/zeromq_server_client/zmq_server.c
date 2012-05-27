#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//  Convert C string to 0MQ string and send to socket
static int
s_send (void *socket, char *string) {
  int rc;
  zmq_msg_t message;
  zmq_msg_init_size (&message, strlen (string));
  memcpy (zmq_msg_data (&message), string, strlen (string));
  rc = zmq_sendmsg (socket, &message, 0);
  zmq_msg_close (&message);
  return (rc);
}

//  Sends string as 0MQ string, as multipart non-terminal
static int
s_sendmore (void *socket, char *string) {
  int rc;
  zmq_msg_t message;
  zmq_msg_init_size (&message, strlen (string));
  memcpy (zmq_msg_data (&message), string, strlen (string));
  rc = zmq_sendmsg (socket, &message, ZMQ_SNDMORE);
  zmq_msg_close (&message);
  return (rc);
}

int main (void)
{
  void *context = zmq_init (1);

  //  Socket to talk to clients
  void *responder = zmq_socket (context, ZMQ_PUB);
  //zmq_bind (responder, "tcp://192.168.1.4:5558");
  zmq_bind (responder, "tcp://127.0.0.1:5558");

  while (1) {
    sleep (1);

    //  Send reply back to client
    /* zmq_msg_t msg; */
    /* zmq_msg_init_size (&msg, 5); */
    //char *test = (char*)malloc(sizeof(char)*5);
    //test = "A test";
    char *test = "C test";
    /* memcpy (zmq_msg_data(&msg), test, 5); */
    /* zmq_sendmsg (responder, &msg, 0); */
    /* zmq_msg_close (&msg); */
    //if ((s_sendmore(responder, "A") != -1) && (s_send(responder, test) != -1)) {
    if (s_send(responder, test) != -1) {
      printf("done '%s'\n", test);
    } else {
      printf("can't send\n");
    }
    //free(test);
  }
  //  We never get here but if we did, this would be how we end
  zmq_close (responder);
  zmq_term (context);
  return 0;
}
