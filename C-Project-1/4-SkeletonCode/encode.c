#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "string.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
//Function to get the size of bmp file
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Move offse to the 18th byte where the width is stored
    fseek(fptr_image, 18, SEEK_SET);
    fseek(fptr_image, 18, SEEK_SET);
    

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
   // printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}
uint get_file_size(FILE*fptr_secret)
{
   /*finding secret file size*/
    fseek(fptr_secret,0,SEEK_END); /*move the ofset to the end of the file*/
    long secret_size=ftell(fptr_secret);//get the size of the fie
    fseek(fptr_secret, 0, SEEK_SET); // Reset the file pointer to the beginning

    return secret_size; // return secret file size
}
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
   printf("INFO: Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
     printf("INFO: Opened beautiful.bmp\n");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    //Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
      printf("INFO: Opened secret.txt\n");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)

    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    printf("INFO: Opened stego.bmp\n");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
        printf("Done\n");
    // No failure return e_success
    return e_success;
}


 Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
 {
  //check .bmp or not
    char* str=strchr(argv[2],'.');
    if(strcmp(str,".bmp")!=0)
    {
        printf("Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
        return e_failure;
    }
    encInfo->src_image_fname = argv[2]; 
    //checking for secret.txt
    char* str1=strchr(argv[3],'.');
    if(str1[1]>='A'&& str1[1]<='Z' || str1[1]>='a'&& str1[1]<='z')
    {
        encInfo->secret_fname = argv[3];
    }
    else
    {
        printf("Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
        return e_failure;
    }
//checking for stego.bmp
    if(argv[4]!=NULL)
    {
     char* str2=strchr(argv[4],'.');
    if(strcmp(str2,".bmp")==0)
    {
        encInfo->stego_image_fname = argv[4];  
    }
    else
    {
        printf("Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
        return e_failure;
    } 
    }
    else
    {
        printf("INFO: Output file not mentioned. Creating steged_img.bmp as default\n");
        encInfo->stego_image_fname = "stego_img.bmp";
        
    }
 }
  


Status do_encoding(EncodeInfo *encInfo)
{    

    /*open file function call*/
    if(open_files(encInfo) == e_failure)
    {
        return e_failure;
    }
      printf("INFO: ## Encoding Procedure Started ##\n");
    // Check if the image has enough capacity to hold the secret file
     printf("INFO: Checking for beautiful.bmp capacity to handle secret.txt\n");
    if ( check_capacity(encInfo) == e_failure)
    {
        return e_failure;
    }
      printf("INFO: Done. Found OK\n");
    // Copy the BMP header from source image to stego image
     printf("INFO: Copying Image Header\n");
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
     printf("INFO: Done\n");
    // encode magic string
      printf("INFO: Encoding Magic String Signature\n");
    if(encode_magic_string(MAGIC_STRING,encInfo)==e_failure)
    {
      printf("Error");
        return e_failure;
    }
     printf("INFO: Done\n");
    // encode secret file extension size
      printf("INFO: Checking for secret.txt size\n");
    if(encode_secret_file_extn_size(strlen( encInfo->extn_secret_file),encInfo)==e_failure)
    {
      return e_failure;
    }
    printf("INFO: Done. Not Empty\n");
    // encode secret file extension
      printf("INFO: Encoding secret.txt File Extension\n");
    if(encode_secret_file_extn( encInfo->extn_secret_file,encInfo)==e_failure)
    {
      return e_failure;
    }
      printf("INFO: Done\n");

    //printf("--%d\n",encInfo->size_secret_file);
    // Encode the secret file size
      printf("INFO: Encoding secret.txt File Size\n");
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_failure)
    {
      return e_failure;
    }
      printf("INFO: Done\n");
    // Encode the actual secret file data
      printf("INFO: Encoding secret.txt File Data\n");
    if(encode_secret_file_data(encInfo)==e_failure)
    {
      return e_failure;
    }
     printf("INFO: Done\n");
    // Copy any remaining image data from source to stego image
      printf("INFO: Copying Left Over Data\n");
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
      return e_failure;

    }
    printf("INFO: Done\n");
    
    return e_success; //Return success if all encoding steps completed

    


}

// Check if the image has enough capacity to hold the secret file
Status check_capacity(EncodeInfo *encInfo)
{
    /*find image size (bmp file size)*/
    uint img_size = get_image_size_for_bmp(encInfo->fptr_src_image);
    /*find secret file size*/
    uint secret_size = get_file_size(encInfo->fptr_secret);
    //printf("---->%d\n",secret_size);
    encInfo->size_secret_file = secret_size;
    /*magic string length*/
    uint magicstring_length=strlen(MAGIC_STRING);
    /* Calculate total data size required in the stego image*/
    /*header + ((magic_string_length + secret_file_extn_size(int)+sec file extension+sec_file_size(int)+sec_file_size)*8)*/
      uint data_size = 54+((magicstring_length+4+strlen(encInfo->extn_secret_file)+4+secret_size)*8);
      if(img_size<data_size)
      {
        return e_failure;
      }
      return e_success;

}

/*Copy bmp hedaer which copy 54 bytes of header into dest image*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
       /*rewind both src and the dest file pointer*/
       rewind(fptr_src_image);// to move the offset to the first
        rewind(fptr_dest_image);// move the offset to the first of dest image

       char buffer[54]; // creating a buffer (temporary storing).
       /*read 54 bytes from source file*/
       fread(buffer,54,1,fptr_src_image);
  
       /*write 54 bytes to dest file*/
       fwrite(buffer,54,1,fptr_dest_image);
       
       // Return success after copying header
   
       return e_success;


}
/* Store Magic String #* */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
   char image_buffer[8]; //buffer to hold 8 bytes of data.
   int magic_length=strlen(magic_string); //length of magic string #*
   /*for loop data length times*/
   for (int i = 0; i < magic_length; i++) 
   {
   /*read 8 bytes from src image file data*/
     fread(image_buffer,8,1,encInfo->fptr_src_image);
   /*call the byte to lsb*/
     encode_byte_to_lsb(magic_string[i],image_buffer);
   /*write to stego image file*/
    fwrite(image_buffer,8,1,encInfo->fptr_stego_image);
   }
   return e_success;
}
// Function to encode a byte into the lsb of an image buffer
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    char ch; // variable to store bits
  for(int i=0;i<8;i++)
  {
   
    //clear the LSB of image_buffer
    image_buffer[i]=image_buffer[i]& 0xFE;
    //get 1 bit from MSB of data
    ch=((unsigned)(data & (1<<(7-i)))>>(7-i));
    // Merge 1 bit of MSB with image buffer
    image_buffer[i]=image_buffer[i]|ch;

  }    
  return e_success;
}
/* Encode secret file extenstion */
Status encode_secret_file_extn_size(uint extn_size, EncodeInfo *encInfo)
{
    char image_buffer[32]; // Buffer to hold 32 bytes of data
    
    fread(image_buffer,32,1,encInfo->fptr_src_image);
    encode_int_to_lsb(extn_size, image_buffer);// encode fixed length of ext to the buffer.
    fwrite(image_buffer,32,1,encInfo->fptr_stego_image);

 
 return e_success;

}
// encode integer data to the lsb of image
Status encode_int_to_lsb(int data, char *image_buffer)
{
    char ch;
  for(int i=0;i<32;i++)
  {
   
    //clear the LSB of image_bffer
    image_buffer[i]=image_buffer[i]& 0xFE;
    //get 1 bit from MSB of data
    ch=((unsigned)(data & (1<<(31-i)))>>(31-i));
    // Merge 1 bit of MSB with image buffer
    image_buffer[i]=image_buffer[i]|ch;

  }   
  return e_success; 
}
/* Encode the secret file extension into the stego image */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
  char buffer[8]; // Buffer to hold 8 bytes of image data
  /*run a loop up to the length of extension*/
  for(int i=0;i<strlen(file_extn);i++)
  {
    // Read 8 bytes from the source image into the buffer
    fread(buffer,8,1,encInfo->fptr_src_image);
    // Encode each character of extension
    encode_byte_to_lsb(file_extn[i],buffer);
    // write the buffer to the stego_image.
    fwrite(buffer,8,1,encInfo->fptr_stego_image);
  }
  return e_success;
}
/* Encode the size of the secret file extension */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
  char buffer[32];  //Buffer to hold 32 bytes of image data
    //Read 32 bytes from source image file into the buffer
    fread(buffer,32,1,encInfo->fptr_src_image);
    //Encode the size of the secret file extension.
    encode_int_to_lsb(file_size,buffer);
    // write the buffer to the stego image.
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
    return e_success;

  } 

/* Encode the content of the secret file into the stego image */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
  char ch; /// Variable to hold each character from the secret file 
  char buffer[8]; // to hold 8 bytes of data
  // read one character at a time from the secret file until the end is reached store the character 
   while(fread(&ch,1,1,encInfo->fptr_secret)>0)
   {
    // Read 8 bytes from the source image into the buffer
     fread(buffer,8,1,encInfo->fptr_src_image);
     // encode the each character.
     encode_byte_to_lsb(ch,buffer);
     // write the buffer contents to the stego.
     fwrite(buffer,8,1,encInfo->fptr_stego_image);

   }
   return e_success;


}
// copy  remaing data to stego to orginal image and stego look a like 
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
   char ch; // variable to hold each character
   // read each one byte at a time  from the source image till the end of fie
   while(fread(&ch,1,1,fptr_src)>0)
   {
    // write it to stego without encoding
    fwrite(&ch,1,1,fptr_dest);
   }
   return e_success;

}
