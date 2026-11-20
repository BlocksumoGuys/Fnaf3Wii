
from PIL import Image
import os

def mult_4(value):
    return max(4, (value // 4) * 4)

def process_image(filename):
    with Image.open(filename) as img:
        width = mult_4(int(img.width * 0.625))
        height = mult_4(int(img.height * 0.625)) 

        resized = img.resize(
            (width, height),
            Image.Resampling.LANCZOS
        )
        resized.save(filename)

        if width > 1024:
            base, ext = os.path.splitext(filename)
            mid = width // 2
            s1 = resized.crop((0, 0, mid, height))
            s2 = resized.crop((mid, 0, width, height))
            s1n = f"{base}(1){ext}"
            s2n = f"{base}(2){ext}"
            s1.save(s1n)
            s1.save(s2n)

        print(f"Proceeded {filename}")


for filename in os.listdir("."):
    if filename.lower().endswith("png"):
        process_image(filename)
print("\nCompleted!")
