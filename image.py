from PIL import Image
import os

def process_image(input_path, new_width, new_height):
    try:
        # Remove metadata
        with Image.open(input_path) as img:
            data = list(img.getdata())
            img_no_meta = Image.new(img.mode, img.size)
            img_no_meta.putdata(data)

            # Overwrite the original image
            os.remove(input_path)
            img_no_meta.save(input_path)
            print(f"The metadata has been removed from {input_path}")

        # Convert and resize image to 24bpp BMP
        with Image.open(input_path) as img:
            img = img.convert('RGB')  # Convert image to RGB mode

            # Resize the image to fit within the specified dimensions while maintaining aspect ratio
            img.thumbnail((new_width, new_height))

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

while True:
    choice = input("Do you want to process an image? (yes/no): ").lower().strip()
    if choice != 'yes':
        break
    
    input_path = input("Enter the path of the image: ")
    output_path = input_path.rsplit('.', 1)[0] + '.bmp'

    process_image(input_path, 900, 900)