#!/usr/bin/env python3

import socket
import os
import logging

def main():

  pid = 100
  socket_file = "/tmp/{}.socket".format(pid)

  print("Try to connect to socket file: {}".format(socket_file))
  client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
  client.connect(socket_file)

  command = "gflags list"
  client.send(command.encode("utf-8"))
  client.close()

if __name__ == "__main__":
  main()
