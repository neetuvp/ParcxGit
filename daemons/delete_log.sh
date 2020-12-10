#!/bin/bash
find /var/log/parcx/ -type f -name '*.log' -mtime +7 -delete
find /var/log/parcx/ -type f -name '*.txt' -mtime +7 -delete
