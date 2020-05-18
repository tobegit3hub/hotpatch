#!/usr/bin/env python3

import argparse
import socket
import sys
import os
import logging

parser = argparse.ArgumentParser()
parser.add_argument("-p", "--pid", type=int, required=True, help="Pid of the running process")
root_subparsers = parser.add_subparsers()

gflags_parser = root_subparsers.add_parser("gflags", help="Set gflags")
gflags_parser.add_argument("method", type=str, help="Methods", choices=["list", "set", "get"])

var_parser = root_subparsers.add_parser("var", help="Set var")
var_parser.add_argument("method", type=str, help="Methods", choices=["list", "set", "get"])

lib_parser = root_subparsers.add_parser("lib", help="Set lib")
lib_parser.add_argument("method", type=str, help="Methods", choices=["load", "unload"])

func_parser = root_subparsers.add_parser("func", help="Set func")
func_parser.add_argument("method", type=str, help="Methods", choices=["upgrade", "rollback"])

args = parser.parse_args()

def main():
  
  pid = args.pid
  command = " ".join(sys.argv[3:])

  socket_file = "/tmp/{}.socket".format(pid)
  print("Try to connect to socket file: {}".format(socket_file))
  try:
    client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    client.connect(socket_file)
  except socket.error as msg:
    print("Fail to connect socket: {}".format(msg))
    sys.exit(1)

  print("Try to send command: {}".format(command))
  client.sendall(command.encode("utf-8"))

  result = client.recv(16)
  print("Receive result: {}".format(result))

  client.close()

if __name__ == "__main__":
  main()
