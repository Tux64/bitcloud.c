/*  This contains all Bitcloud networking code, leveraging UV library.
*/

#include "libuv/include/uv.h"   // make sure it is there
#include "bitcloud.h"
#include "stdio.h"

// socket structure

typedef struct bc_socket {
  char *address;        /* the sockaddr */
  int port;
  bc_error status;
  // what else?
} bc_socket;

uv_loop_t* bc_netloop = NULL;
uv_tcp_t* uv_sock;
uv_stream_t *bc_recieve;
uv_stream_t *bc_transmit;
uv_read_cb bc_process_in;

struct sockaddr_in* uv_addr;

// function prototypes/declarations

uv_tcp_t* bc_prepare_sockets (void);

bc_error bc_open_sockets (bc_socket *cur_sock); // and initiate listening

bc_error bc_close_sockets (bc_socket *cur_sock);

bc_error bc_sockets_connect (bc_socket *cur_sock); // reach out to another node

// not sure if these two will be needed

bc_error bc_sockets_transmit (bc_socket *cur_sock);

bc_error bc_sockets_receive (bc_socket *cur_sock);

void bc_allocbuf (uv_handle_t* bc_handle, size_t sugsize, uv_buf_t* bc_buf);




uv_tcp_t* bc_prepare_sockets (void) {
  uv_loop_init(bc_netloop);
  uv_tcp_init(bc_netloop, uv_sock);
  return uv_sock;
}

bc_error bc_open_sockets (bc_socket *cur_sock) {
  // opens socket at (*cur_sock).address:(*cur_sock).port
  uv_ip4_addr((*cur_sock).address, (*cur_sock).port, uv_addr);

  int status = uv_tcp_bind(uv_sock, (const struct sockaddr*)uv_addr, 0);
  bc_error i = BC_OK;
  if (status < 0) {
    printf("Connection Failed. Libuv has provided the code %d .", status);
    i = BC_NO_CONNECTION;
  }
  if (uv_accept(bc_transmit, (uv_stream_t*) bc_recieve) == 0) {
    uv_read_start((uv_stream_t*) bc_recieve, bc_allocbuf, bc_process_in);
  }
  return i;
}
