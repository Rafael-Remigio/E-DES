# E-DES
Enhanced DES (E-DES), a exploration on creating a variant of DES


## C code

``` gcc -o e-des.out e-des.c -lssl -lcrypto | ./e-des.out -p password ```


```
$ echo "hacker" | ./e-des-encrypt.out -p password | ./e-des-decrypt.out -p password
hacker
```
```
$ echo "hacker mindset baby" | ./e-des-encrypt.out -p password | ./e-des-decrypt.out -p password
hacker mindset baby
```
