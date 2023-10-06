# Image Steganography Toolkit by Jacob Sharp

## Description

This C-based repository is designed to hide one image inside another and reveal the hidden image as needed.

## Features

- **Hide an Image**: Embed one image inside another with minimal noticeable visual alteration.
- **Reveal an Image**: Extract the hidden image from the host image.

## Installation

Clone this repository and navigate into the directory:

```bash
git clone https://github.com/jwsharp/image_stenography.git
```

Compile the source code:

```bash
make
```

## Usage

### Hide an Image

```c
// Path to host image and the image to be hidden
char host_image_path[] = "host_image.jpg";
char hidden_image_path[] = "hidden_image.jpg";

// Function to hide the image
hide_image(host_image_path, hidden_image_path);
```

### Reveal an Image

```c
// Function to reveal the hidden image
reveal(host_image_path);
```

## Roadmap

- [ ] **Invert Colors**: Function to invert the colors of an image.
- [ ] **Grayscale**: Function to change an image to grayscale.
- [ ] **Flip Image**: Function to flip an image horizontally or vertically.
