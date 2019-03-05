#! /bin/bash

_IP=$(hostname -I) || true

if [ "$_IP" ]; then
	echo "$_IP"
else
	echo "No Detected"
fi