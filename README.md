# LSB Image Steganography Project
<p align="center">
  <img src="https://cdn-ilbnbgd.nitrocdn.com/DmbmBmOWioNwhwhvqRywPErjVBHKZywW/assets/images/optimized/rev-2bb310a/www.emertxe.com/wp-content/uploads/2023/06/c-project-image-steganography-1.jpg" alt="Image Steganography" width="500" />
</p>


## Introduction
The **LSB Image Steganography** project focuses on hiding secret information within an image file, using the Least Significant Bit (LSB) technique. Steganography is a technique that hides information within other information, often used to transmit sensitive data without raising suspicion. While it is similar to cryptography in the sense that it protects data, the key difference is that steganography hides the data in a way that appears to be an ordinary, unaltered file. In this project, the primary carrier is a BMP image file, and the hidden information is contained within the least significant bits of the pixel data.

Steganography is used to covertly store or transmit sensitive data. The hidden information is embedded in such a way that it cannot be easily detected by the human eye. This project will implement a simple command-line application that can both encode and decode messages within an image file using this technique.

## Objective
- Accept an image file (e.g., .bmp) and a text file containing the secret message to be hidden.
- Analyze the size of the text file and check if the message can fit inside the provided image file.
- Provide an option to embed a magic string in the image to indicate if the image has been steganographically modified.
- Provide an option to decrypt the hidden message from the image.

## Features
- **Message Embedding**: Hide a message in the image's LSB (Least Significant Bit) without affecting the image's visual integrity.
- **Magic String**: A unique identifier to indicate whether the image has been altered.
- **Decryption**: Extract the hidden message from the image and verify its contents.

## Requirements
- **Programming Language**: C
- **Libraries**: Standard C libraries for file handling, and image processing.

### Requirements Breakdown
1. **Input**: 
   - `.bmp` image file
   - `.txt` file containing the message to hide
   
2. **Operations**: 
   - Analyze the message size to ensure it fits within the image.
   - Use LSB encoding to hide the message in the image file.
   - Insert a magic string in the image to help identify if the image has been altered.
   - Provide a decoding option to retrieve the hidden message.

3. **Command-Line Application**:
   - Accept command-line arguments for the operations (encode or decode).
   - Support the following options:
     - `encode`: For embedding the message into the image.
     - `decode`: For extracting the message from the image.

