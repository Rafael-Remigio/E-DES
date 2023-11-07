#compile the c code
gcc  -o edes_enc.out e-des-encrypt.c -lssl -lcrypto && chmod +x edes_enc.out  
gcc  -o edes_dec.out e-des-decrypt.c -lssl -lcrypto  && chmod +x edes_dec.out 

#create random values
dd if=/dev/urandom of=random_buffer bs=4K count=1

clear


# function to generate random passwords
generate_password() {
    CHARACTER_SET="A-Za-z0-9!@#$%^&*.?"
    PASSWORD_LENGTH=12
    password=$(tr -dc "$CHARACTER_SET" < /dev/urandom | head -c $PASSWORD_LENGTH)
    echo "$password"
}

for i in {1..10}
do
clear
echo -n "$i/1.000"
# run c code in both modes

# generate random password
temp_password=$(generate_password)
cat random_buffer | ./edes_enc.out -p "$temp_password" > /dev/null | ./edes_dec.out -p "$temp_password" > /dev/null

temp_password=$(generate_password)
cat random_buffer  | ./edes_enc.out -p "$temp_password" -d > /dev/null | ./edes_dec.out -p "$temp_password" -d > /dev/null

# run python code in both modes
temp_password=$(generate_password)
cat random_buffer | python3 e-des-encrypt.py -p "$temp_password" > /dev/null| python3 e-des-decrypt.py -p "$temp_password" > /dev/null
temp_password=$(generate_password)
cat random_buffer | python3 e-des-encrypt.py -p "$temp_password" -d > /dev/null| python3 e-des-decrypt.py -p "$temp_password" -d > /dev/null


done

#show results
echo
echo "C-DES:"
echo -n "Encrypt: " && cat ./c_des_encrypt.txt && echo
echo -n "Decrypt: " && cat ./c_des_decrypt.txt && echo
echo
echo "C-EDES:"
echo -n "Encrypt: " && cat ./c_edes_encrypt.txt && echo
echo -n "Decrypt: " && cat ./c_edes_decrypt.txt && echo
echo
echo "Python-DES:"
echo -n "Encrypt: " && cat ./python_des_enc_time.txt && echo
echo -n "Decrypt: " && cat ./python_des_dec_time.txt && echo
echo
echo "Python-EDES:"
echo -n "Encrypt: " && cat ./python_edes_enc_time.txt && echo
echo -n "Decrypt: " && cat ./python_edes_dec_time.txt && echo


# remove files
rm -f c_des_decrypt.txt
rm -f c_des_encrypt.txt
rm -f c_edes_decrypt.txt
rm -f c_edes_encrypt.txt

rm -f edes_dec.out
rm -f edes_enc.out

rm -f python_edes_dec_time.txt
rm -f python_edes_enc_time.txt
rm -f python_des_dec_time.txt
rm -f python_des_enc_time.txt

rm -f random_buffer
