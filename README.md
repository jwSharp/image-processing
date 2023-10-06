# Image Steganography Toolkit by Jacob Sharp

## Description

This C-based repository is designed to hide one image inside another and reveal the hidden image as needed.

## Features

- **Hide an Image**: Embed one image inside another with minimal noticeable visual alteration.
- **Reveal an Image**: Extract the hidden image from the host image.
- **Invert an Image**: Invert the hue of an image.

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
// Function to hide the image
hide_image(host_image, hidden_image);
```

### Reveal an Image

```c
// Function to reveal the hidden image
reveal(host_image);
```

### Reveal an Image

```c
// Function to invert an image
invert(target_image);
```

## Roadmap

- [ ] **Grayscale**: Function to change an image to grayscale.
- [ ] **Flip Image**: Function to flip an image horizontally or vertically.
