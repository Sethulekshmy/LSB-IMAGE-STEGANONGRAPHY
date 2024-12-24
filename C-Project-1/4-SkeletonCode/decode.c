#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include "string.h"

/* open files to decode*/
Status open_files_decode(DecodeInfo *decInfo)
{
  printf("INFO: Opening required files\n");
    // opening the stego file  in r mode to read data
   decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    printf("INFO: Opened stego_image.bmp\n");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }
    return e_success;

}
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char *str = strchr(argv[2],'.');
    if(strcmp(str,".bmp")!=0)
    {
        printf("Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
        return e_failure;
    }
    decInfo->stego_image_fname = argv[2];

     if(argv[3]!=NULL)
    {
        char* str=strchr(argv[3],'.');
        if(str!=NULL)
        {
            strncpy(decInfo->secret_file_name,argv[3],str-argv[3]);
            decInfo->secret_file_name[str-argv[3]]='\0';
        }
        else
        {
            strcpy(decInfo->secret_file_name,argv[3]);
        }
    }
    else
    {
        printf("INFO: Output file not mentioned.creating output.txt as default\n");
        strcpy(decInfo->secret_file_name,"output");
    }
}

Status do_decoding(DecodeInfo *decInfo)
{        /*open file function call to open all the files for decoding*/
    if(open_files_decode(decInfo) == e_failure)
    {
        return e_failure;
    }
    // skip bmp header to the 54th position
    if( skip_bmp_header(decInfo->fptr_stego_image)==e_failure)
    {
        return e_failure;
    }
     printf("INFO: ## Decoding Procedure Started ##\n");
     /*decode the magic string to get back function call*/
      printf("INFO: Decoding Magic String Signature\n");
    if(decode_magic_string(decInfo)==e_failure)
    {
      //printf("Error");
        return e_failure;
    }
        printf("INFO: Done\n");

    // Decode the size of the secret file extension
    if(decode_secret_file_extn_size(decInfo)==e_failure)
    {
      return e_failure;
    }
    // Decode the secret file extension 
    printf("INFO: Decoding Output File Extension\n");
    if(decode_secret_file_extn(decInfo)==e_failure)
    {
      return e_failure;
    }
     printf("INFO: Done\n");
  // Decode the size of the secret file
  printf("INFO: Decoding output.txt File Size \n");
    if(decode_secret_file_size(decInfo)==e_failure)
    {
      return e_failure;
    }
     printf("INFO: Done\n");
    // decode the secret file data
      printf("INFO: Decoding output.txt File Data\n");
    if(decode_secret_file_data(decInfo)==e_failure)
    {
      return e_failure;
    }
     printf("INFO: Done\n");
    
    return e_success;

}

Status skip_bmp_header(FILE *fptr_stego_image)
{
  // move the offset to the 54th position first 54 bytes will be skipped
    fseek(fptr_stego_image,54,SEEK_SET);
    return e_success;
}

/* Decode the magic string from the stego image  */
  
Status decode_magic_string( DecodeInfo *decInfo)
{
  int i;
  char image_buffer[8]; // buffer to hold 8 bytes of data
  char str[20]; // use entered magic string
  char magic_string_decode[20]; 
  // Read the magic string from user
 // printf("\nEnter the magic string"); 
  scanf("%s",str);
  // run a loop to read 8 bytes from the stego image
  for(i=0;i<2;i++)
  {
    fread(image_buffer,8,1,decInfo->fptr_stego_image);
    // decode LSB to byte
    magic_string_decode[i] = decode_lsb_to_byte(image_buffer);
  }
  magic_string_decode[i]='\0'; // store the last  null charcter to get back string
 // printf("Magic string:%s\n",magic_string_decode); // print magic string

  //comparing entered magic string with decoded .
  
    if(strcmp(magic_string_decode,str)!=0) // compare the decoded magic string and user entered magic string
    {
      return e_failure;
    }
    return e_success; // string matches
  
}
/*Decode each byte from the lsb*/
char decode_lsb_to_byte( char *image_buffer)
{
     char data = 0; //intialize  the data with all bits are stored as 0
      char lsb_bit;
     //loop through each of 8 bits
    for (int i = 0; i < 8; i++) { 
      //  get the LSB of each image buffer and shif it to the MSB
          lsb_bit = ((unsigned)(image_buffer[i]& 0x01))<<(7-i);
        data = data|lsb_bit; // merge the lsb with data
    }
    
    return data; // return the character data
}
/*decode each integer data from the lsb of image buffer*/
uint decode_lsb_to_int(char * image_buffer)
{
   int data = 0;  // variable to hold decodedd data intialize with 0
    int  lsb_bit=0;

    for (int i = 0; i < 32; i++) { 
       // Get the LSB from each image and move it to the MSB
          lsb_bit = ((unsigned)(image_buffer[i]& 0x01))<<(31-i);
          // merge it with data
        data = data|lsb_bit;
    }
    
    return data; // return the integer data

}
/* Decode the size of the secret file extension */
Status decode_secret_file_extn_size( DecodeInfo *decInfo)
{
  char image_buffer[32];// buffer to hold 32 bytes of data
  // read 32 bytes from the stego image to the buffer
  fread(image_buffer,32,1,decInfo->fptr_stego_image);
  // decode extension size
   int extn_size=decode_lsb_to_int(image_buffer);
   //printf("extension size: %d\n",extn_size);
   return e_success;

 }
 /* Decode the secret file extension from the stego image */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char image_buffer[8]; // buffer to hold 8 bytes of data
    char secret_extn_name[20]; // buffer to store decoded extension name
    int i;
    for( i=0;i<4;i++) // run a loop up to 4 times to get the extension name
    {
      fread(image_buffer,8,1,decInfo->fptr_stego_image);
      // Decode the byte and store it
       secret_extn_name[i]=decode_lsb_to_byte(image_buffer);
    }
     secret_extn_name[i]='\0';
     printf("INFO: done\n");
     // Append the extension to the secret file name
     strcat(decInfo->secret_file_name, secret_extn_name);
     //printf("extnsion is %s\n",secret_extn_name);
     //printf("file name is %s\n",decInfo->secret_file_name);
     // after merging  Open the output file for writing
     decInfo->fptr_output=fopen(decInfo->secret_file_name,"w");
     printf("INFO: opened output.txt\n");
     printf("INFO: Done.opened all required files\n");
     if(decInfo->fptr_output==NULL)
     {
      printf("file not exist");
      return e_failure;
     }
     return e_success;
 }
 /* Decode the size of the secret file from the stego image */
 Status decode_secret_file_size(DecodeInfo *decInfo)
 {
  char image_buffer[32];// buffer to hold 32 bytes
  fread(image_buffer,32,1,decInfo->fptr_stego_image);// read 32 bytes from the stego
  // call lsb to in function to decode the secret file size
   decInfo->file_size=decode_lsb_to_int(image_buffer);
  // printf("file size:%ld\n",decInfo->file_size);
 }
 /* Decode the secret file data and write it to the output file */
Status decode_secret_file_data(DecodeInfo *decInfo)
 {
    char image_buffer[8];// buffer to hold 8 bytes
    char ch;//variable to store ecah decoded character
  // loop to read secret file data up to file size
    for(int i=0;i<decInfo->file_size;i++)
    {
      fread(image_buffer,8,1,decInfo->fptr_stego_image);
      //call decode lsb to byte and store each character
      ch=decode_lsb_to_byte(image_buffer);
      // write the decoded charcter to the output file
      fwrite(&ch,1,1,decInfo->fptr_output);
    }
    fclose(decInfo->fptr_output);//close all the output file
    return e_success;
 }





