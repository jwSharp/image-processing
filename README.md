# Image Processor by Jacob Sharp

## Description

Image Processor is a utility for manipulating BMP images and performing operations like revealing hidden photos, inverting colors, grayscaling, flipping, and mirroring. It also includes a Python script for processing images, including removing metadata and converting/resizing images to BMP format.

_This project has ended. All functionality is final. Please enjoy!_

## Features

- **Display Header**: Print out the contents of the image header.
- **Hide an Image**: Embed one image inside another with minimal noticeable visual alteration.
- **Reveal an Image**: Extract the hidden image from the host image partially or fully.
- **Invert an Image**: Invert the hue of an image.
- **Grayscale an Image**: Convert a color image to grayscale.
- **Flip an Image**: Horizontally flip the image.
- **Mirror an Image**: Copy the left side of the photo, horizontally flipped, to the right side.

Operations correspond to a function in the C program. Consult the documentation within the code for a more detailed description on each operation.

## Usage

1. Run `make all` to compile and execute the Python script and C program. (See below how to execute each individually).
2. In the Python script, follow the prompts to process your images or type "no" to the first prompt to end.
3. In the C program, choose from the menu options to perform various image manipulation operations.

_Optional_: Add your own images to the images folder to run the Python script.

## Makefile

The Makefile contains targets for compiling the C program, running the Python script, and cleaning up the generated files. Here are the available targets:

- `all`: Execute the Python script and compile and run the C program.
- `compile`: Compile the C program.
- `run`: Run the compiled C program.
- `python`: Install Python dependencies and run the Python script.
- `clean`: Remove compiled files and the virtual environment created by Pipenv.

The Makefile included in this project takes care of installing Pipenv if it is not already installed on your system. Python dependencies, including Pillow, are installed via the Pipenv when you run the Makefile.
