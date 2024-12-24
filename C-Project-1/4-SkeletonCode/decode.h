#ifndef DECODE_H
#define DECODE_H

#include "types.h"
typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;
    
    //uint extn_size;
   char secret_extn_name[20];
   char secret_file_name[20];
     long file_size;
   // char *output_fname;   
    FILE *fptr_output;
   
} DecodeInfo;
 
/* Check operation type */
OperationType check_operation_type(char *argv);

/* Read and validate Encode args from argv */
//Status read_and_validate_encode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *decInfo);

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
/* skip bmp header */
Status skip_bmp_header(FILE *fptr_stego_image);

/* Decode  Magic String */
Status decode_magic_string( DecodeInfo *decInfo);

/*Decode secret file extension size*/
Status decode_secret_file_extn_size( DecodeInfo *decInfo);


/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size( DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode a LSB into byte of secret data */
char decode_lsb_to_byte( char *image_buffer);

/* Decode a LSB into size of secret data */
uint decode_lsb_to_int(char * image_buffer);
#endif

