#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"
int main(int argc,char* argv[])
{
    EncodeInfo encInfo; // structure to hold encoding information
    DecodeInfo decInfo; // structure to hold decoding information
    uint img_size;
    if(check_operation_type(argv[1])==e_encode)
    {
        if (argc < 4 || argc > 5) {
           printf("Invalid number of arguments for encoding.\n");
           // printf("Usage: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
           return e_failure;
        }
        //encoding
        // Fill with sample filenames
     // encInfo.src_image_fname = argv[2];
      //encInfo.secret_fname = argv[3];
     // encInfo.stego_image_fname = "stego_img.bmp";
     if(read_and_validate_encode_args(argv,&encInfo)==e_failure)
    {
        return e_failure;
    }
   //copy the file extension from thord command line
   strcpy(encInfo. extn_secret_file ,strchr(argv[3],'.'));

     //read_and_validation(
       // printf("Selected Encoding\n");
        if(do_encoding(&encInfo)==e_success)
        {
            printf("Encoding is done succesfully\n");
        }
    }
    else if(check_operation_type(argv[1])==e_decode)
    {   
         if (argc < 3 || argc > 4) {
           printf("Invalid number of arguments for decoding.\n");
           // printf("Usage: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
           return e_failure;
        }//do decoding
      // decInfo.stego_image_fname=argv[2];
      if(read_and_validate_decode_args(argv, &decInfo)==e_failure)
    {
        return e_failure;
    }
       // strcpy(decInfo.secret_file_name,"output");
         //printf("Selected Decoding\n");
         if(do_decoding(&decInfo)==e_success)
         {
        printf("Decoding done successfully\n");
         }
        
    }
    else
    {
        printf("Inavlid:unsupported operation ");
        //invalid
    }
}
OperationType check_operation_type(char *argv)
{
    if(strcmp(argv,"-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv,"-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}