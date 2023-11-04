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


### E-DES Mode
```
$ echo "hacker mindset" | ./e-des-encrypt.out -p password 
���4#���
!~{�b
```
```
$ echo "hacker mindset baby" | ./e-des-encrypt.out -p password | ./e-des-decrypt.out -p password
hacker mindset baby
```

### DES Mode
```
$ echo "hacker mindset" | ./e-des-encrypt.out -p password -d
X�{�H��a�/#t�W
```

```
echo "hacker mindset" | ./e-des-encrypt.out -p password -d | ./e-des-decrypt.out -p password -d
hacker mindset
```
