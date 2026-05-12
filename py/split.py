from PIL import Image
import os

LIMIT = 1024

def split_image(path):
    with Image.open(path) as img:
        w, h = img.size

        if w <= LIMIT:
            return

        base, ext = os.path.splitext(path)

        mid = w // 2

        left_box = (0, 0, mid, h)
        right_box = (mid, 0, w, h)

        left_img = img.crop(left_box)
        right_img = img.crop(right_box)

        left_name = f"{base}(1){ext}"
        right_name = f"{base}(2){ext}"

        left_img.save(left_name)
        right_img.save(right_name)

        print(f"Split: {path} -> {left_name}, {right_name}")

def main():
    for file in os.listdir("."):
        if file.lower().endswith(".png"):
            try:
                split_image(file)
            except Exception as e:
                print(f"Error with {file}: {e}")

if __name__ == "__main__":
    main()