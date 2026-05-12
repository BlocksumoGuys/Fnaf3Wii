from PIL import Image
import os

scale = 0.625

def make_multiple_of_4(value):
    return (value // 4) * 4  # round DOWN to nearest multiple of 4

for filename in os.listdir("."):
    if filename.lower().endswith((".png", ".jpg", ".jpeg", ".bmp")):
        try:
            with Image.open(filename) as img:
                new_width = int(img.width * scale)
                new_height = int(img.height * scale)

                # ✅ force multiples of 4
                new_width = make_multiple_of_4(new_width)
                new_height = make_multiple_of_4(new_height)

                # safety check (avoid 0)
                if new_width < 4: new_width = 4
                if new_height < 4: new_height = 4

                resized = img.resize((new_width, new_height), Image.LANCZOS)
                resized.save(filename)

                print(f"{filename} -> {new_width}x{new_height}")

        except Exception as e:
            print(f"Failed: {filename} ({e})")