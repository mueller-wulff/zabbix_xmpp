#!/bin/bash

openssl genrsa -out $1/roakey.pem 4096
openssl req -new -key $1/roakey.pem -out $1/certreq.csr -batch
openssl x509 -req -days 3650 -in $1/certreq.csr -signkey $1/roakey.pem -out $1/roacert.pem

