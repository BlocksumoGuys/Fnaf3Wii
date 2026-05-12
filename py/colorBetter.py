from PIL import Image
import os

INPUT_EXTS = (".png", ".jpg", ".jpeg", ".webp", ".bmp")

def reduce_color_depth(img):
    img = img.convert("RGBA")
    r, g, b, a = img.split()

    # Remove 2 bits (8-bit -> 6-bit)
    def reduce_channel(channel):
        return channel.point(lambda i: i & 252)

    r = reduce_channel(r)
    g = reduce_channel(g)
    b = reduce_channel(b)

    return Image.merge("RGBA", (r, g, b, a))


for file in os.listdir("."):
    if file.lower().endswith(INPUT_EXTS):
        try:
            img = Image.open(file)
            processed = reduce_color_depth(img)

            # overwrite original
            if file.lower().endswith(".png"):
                processed.save(file, "PNG", optimize=True)
            else:
                processed.save(file)

            print(f"[OK] Overwritten: {file}")

        except Exception as e:
            print(f"[ERROR] {file}: {e}")

print("Done.")