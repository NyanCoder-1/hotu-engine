#!/bin/bash

# xdg-shell
wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml ./src/platform-linux/xdg-shell.h
wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml ./src/platform-linux/xdg-shell.c

