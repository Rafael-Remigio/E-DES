import ctypes
import hashlib
import getopt, sys
import binascii
import argparse
from Crypto.Cipher import DES
from Crypto.Random import get_random_bytes
import time

uint8_t = ctypes.c_uint8



BOX = [0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff]


def fisher_yates(array, vector):
    size = 256 * 16
    for i in range(size):
        # Get next value to be swapped using Fisher-Yates algorithm
        swapper = vector[i] % (size - i)

        # Swap the last value with a value at another position
        temp = array[swapper]
        array[swapper] = array[size - i-1]
        array[size - i-1] = temp

    return array



def generate_shuffle_vector(base_seed_bytes):
    string_index = "{:03d}"
    vector = []


    for i in range(256):
        seed_indexed = string_index.format(i)
        

        # This is very stupid, I hate python
        seed_bytes = base_seed_bytes

        for char in seed_indexed:
            seed_bytes += uint8_t(ord(char))

        hash_obj = hashlib.sha256(seed_bytes)
        hash_hex = hash_obj.hexdigest()
        #print(seed_indexed)
        #print(hash_hex)



        for c in range(0, 64, 4):
            value = sum(ord(hash_hex[c + j]) for j in range(4)) 
            vector.append(value)

    return vector



def preform_sbox_transformation(sbox, in_data):
    out = [0] * 4


    index = in_data[3].value % 256
    out[0] = sbox[index]
    # This is such a stupid line of code
    # I hate python.
    # The issue is that I can sum bytes in python, so i sum tem and them get the modulu by 256
    # unsupported operand type(s) for +: 'c_ubyte' and 'c_ubyte'
    
    index = (index + in_data[2].value) % 256
    out[1] = sbox[index]
    index = (index + int.from_bytes(in_data[1],byteorder='little')) % 256
    out[2] = sbox[index]
    index = (index + int.from_bytes(in_data[0],byteorder='little')) % 256
    out[3] = sbox[index]

    return out


def feistel(first_slice, second_slice, sbox):

    new_first_slice = second_slice[:]

    second_slice = preform_sbox_transformation(sbox, second_slice)


    for i in range(4):
        result = second_slice[i].value ^ first_slice[i].value
        second_slice[i] = uint8_t(result)


    return new_first_slice,second_slice


def feistelRounds(data,isEncrypting,SBOXES):

    first_slice = data[:4]
    second_slice = data[4:]

    # Encrypt
    if (isEncrypting):
        for i in range(16):
            first_slice, second_slice = feistel(first_slice,second_slice,SBOXES[i]);

        return  first_slice + second_slice 
    # Decrypt
    else:
        for i in range(15,-1,-1):
            second_slice, first_slice = feistel(second_slice,first_slice,SBOXES[i]);

        return  first_slice + second_slice
    

def update_file_with_new_time(new_time, file_path):
    try:
        # Open the file in read mode to read the previous time value
        with open(file_path, 'r') as file_pointer:
            previous_time = int(file_pointer.read())
        # Compare with the new time value
        if new_time < previous_time:
            # Open the file in write mode to overwrite with new time
            with open(file_path, 'w') as file_pointer:
                # Write the new time value to the file
                file_pointer.write(str(new_time))
                #print(f"File updated with new time: {new_time}")
            #print("New time is not less than the previous time. File remains unchanged.")
    except FileNotFoundError:
        # If the file does not exist, create it and write the new time
        with open(file_path, 'w') as file_pointer:
            file_pointer.write(str(new_time))
            #print(f"File created with initial time: {new_time}")
    except Exception as e:
        #print(f"Error: {e}")
        pass

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--password", type=str, help="Password to encrypt")
    parser.add_argument("-d", "--des",  action='store_true', help="Use DES mode")

    args = parser.parse_args()

    password = args.password
    des_mode = args.des


    if (not des_mode):
        # This is very stupid, I hate python
        password_bytes = b''
        for char in password:
            password_bytes += uint8_t(ord(char))


        hash_obj = hashlib.sha256(password_bytes)
        password_hash_bytes = hash_obj.digest()
        password_hash_hex = hash_obj.hexdigest()




        vector = generate_shuffle_vector(password_hash_bytes)
    

        base_Sbox = [0] * 256*16
        counter = 0
        for i in range(16):
            for j in range(256):
                base_Sbox[counter] = BOX[j]
                counter+=1

        s_boxes = fisher_yates(base_Sbox,vector);

        SBOXES = []
        single_box = []
        counter = 0
        for i in range(16):
            for j in range(256):
                single_box.append(uint8_t(s_boxes[counter]))
                counter+=1
            SBOXES.append(single_box)
            single_box = []


        data = [0]*8
        input_index = 0
        full_data = sys.stdin.buffer.read()
        byte_array = bytearray()

        start_time = time.perf_counter_ns()

        for ch in full_data:
            # add to block
            data[input_index] = uint8_t(ch)
            input_index+=1

            if (input_index == 8):
                # preform encryption
                cipher_data = feistelRounds(data,True,SBOXES);
                

                # print block as stdout
                for value in cipher_data:
                    byte_array.append(value.value) 

                input_index = 0;
            
        
        # Calculate needed padding
        padding_value = 8 - input_index
        
        # PKCS#7 padding
        for i in range(7, input_index - 1, -1):
            data[i] = uint8_t(padding_value)
        
        # Perform encryption on the final block using feistelRounds function
        cipher_data = feistelRounds(data, True, SBOXES)
        

        ##print block as stdout
        for value in cipher_data:
            byte_array.append(value.value ) 

        end_time = time.perf_counter_ns()
        execution_time_ns = end_time - start_time
        file_path = "python_edes_enc_time.txt"
        update_file_with_new_time(execution_time_ns, file_path)


        print(binascii.hexlify(byte_array).decode(),end="")

    else: 
        
        password_bytes = b''
        for char in password:
            password_bytes += uint8_t(ord(char))


        hash_obj = hashlib.sha256(password_bytes)
        password_hash_bytes = hash_obj.digest()
        password_hash_hex = hash_obj.hexdigest()
        

        # Create a DES cipher object in ECB mode
        cipher = DES.new(password_hash_bytes[:8],DES.MODE_ECB)

        data = [0]*8
        input_index = 0
        full_data = sys.stdin.buffer.read()

        byte_array = bytearray()
        full_data_array = bytearray()
        for value in full_data:
            input_index+=1
            full_data_array.append(uint8_t(value).value) 
            if (input_index == 8):
                input_index = 0

        # Calculate needed padding
        padding_value = 8 - input_index
        
        # PKCS#7 padding
        for i in range(7, input_index - 1, -1):
            full_data_array.append(uint8_t(padding_value).value)
        
        start_time =  time.perf_counter_ns()

        cipher_data = cipher.encrypt(full_data_array);

        end_time = time.perf_counter_ns()
        execution_time_ns = end_time - start_time
        file_path = "python_des_enc_time.txt"
        update_file_with_new_time(execution_time_ns, file_path)      

        print(binascii.hexlify(cipher_data).decode(),end="")

