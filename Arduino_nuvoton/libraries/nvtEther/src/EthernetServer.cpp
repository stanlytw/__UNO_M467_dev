extern "C" {
#include "string.h"
}

#include "nvtEthernet.h"
#include "EthernetClient.h"
#include "EthernetServer.h"

EthernetServer::EthernetServer(uint16_t port)
{
  _port = port;
  for (int i = 0; i < MAX_CLIENT; i++) {
    _tcp_client[i] = {};
  }
  _tcp_server = {};
}

void EthernetServer::begin()
{
  if (_tcp_server.pcb != NULL) {
    return;
  }

  _tcp_server.pcb = tcp_new();

  if (_tcp_server.pcb == NULL) {
    return;
  }

  tcp_arg(_tcp_server.pcb, &_tcp_client);
  _tcp_server.state = TCP_NONE;

  if (ERR_OK != tcp_bind(_tcp_server.pcb, IP_ADDR_ANY, _port)) {
    memp_free(MEMP_TCP_PCB, _tcp_server.pcb);
    _tcp_server.pcb = NULL;
    return;
  }

  _tcp_server.pcb = tcp_listen(_tcp_server.pcb);
  tcp_accept(_tcp_server.pcb, tcp_accept_callback);
}

void EthernetServer::begin(uint16_t port)
{
  _port = port;
  begin();
}

void EthernetServer::end(void)
{
  /* Free client */
  for (int n = 0; n < MAX_CLIENT; n++) {
    if (_tcp_client[n] != NULL) {
      EthernetClient client(_tcp_client[n]);
      client.stop();
      _tcp_client[n] = NULL;
    }
  }
  if (_tcp_server.pcb != NULL) {
    tcp_close(_tcp_server.pcb);
    _tcp_server.pcb = NULL;
  }
}

void EthernetServer::accept()
{
  /* Free client if disconnected */
  for (int n = 0; n < MAX_CLIENT; n++) {
    if (_tcp_client[n] != NULL) {
      EthernetClient client(_tcp_client[n]);
      if (client.status() == TCP_CLOSING) {
        mem_free(_tcp_client[n]);
        _tcp_client[n] = NULL;
      }
    }
  }
}

EthernetClient EthernetServer::available()
{
  accept();

  for (int n = 0; n < MAX_CLIENT; n++) {
    if (_tcp_client[n] != NULL) {
      if (_tcp_client[n]->pcb != NULL) {
        EthernetClient client(_tcp_client[n]);
        uint8_t s = client.status();
        if (s == TCP_ACCEPTED) {
          if (client.available()) {
            return client;
          }
        }
      }
    }
  }

  struct tcp_struct *default_client = NULL;
  return EthernetClient(default_client);
}

size_t EthernetServer::write(uint8_t b)
{
  return write(&b, 1);
}

size_t EthernetServer::write(const uint8_t *buffer, size_t size)
{
  size_t n = 0;

  accept();

  for (int i = 0; i < MAX_CLIENT; i++) {
    if (_tcp_client[i] != NULL) {
      if (_tcp_client[i]->pcb != NULL) {
        EthernetClient client(_tcp_client[i]);
        uint8_t s = client.status();
        if (s == TCP_ACCEPTED) {
          n += client.write(buffer, size);
        }
      }
    }
  }

  return n;
}

EthernetServer::operator bool()
{
  // server is listening for incoming clients
  return ((_tcp_server.pcb != NULL) && (_tcp_server.pcb->state == LISTEN));
}
