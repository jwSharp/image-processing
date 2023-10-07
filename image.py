from PIL import Image
import os

def process_image(input_path, new_width, new_height):
    try:
        # Convert and resize image to 24bpp BMP
        with Image.open(input_path) as img:
            img = img.convert('RGB')  # Convert image to RGB mode

            # Resize the image to fit within the specified dimensions while maintaining aspect ratio
            img.thumbnail((new_width + new_width/3, new_height + new_width/3))

            # Calculate the coordinates of the cropped area
            left = (img.width - new_width) / 2
            upper = (img.height - new_height) / 2
            right = (img.width + new_width) / 2
            lower = (img.height + new_height) / 2

            # Crop the image to the specified width and height, centered
            cropped_img = img.crop((left, upper, right, lower))

            # Save the resized image in BMP format to a new file
            output_path = input_path.rsplit('.', 1)[0] + '.bmp'
            cropped_img.save(output_path, 'BMP')
            print(f"Resized and converted image saved to {output_path}")

    except Exception as e:
        print(f"An error occurred: {e}")

# Specify the dimensions to resize
new_width = 900
new_height = 900

# Test the function with your actual image path
process_image('goat.jpg', new_width, new_height)