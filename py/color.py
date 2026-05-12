from PIL import Image
import os

INPUT_EXTS = (".png", ".jpg", ".jpeg", ".webp", ".bmp")
OUTPUT_FOLDER = "output"

os.makedirs(OUTPUT_FOLDER, exist_ok=True)

def reduce_color_depth(img):
    # Ensure RGBA so we always preserve alpha safely
    img = img.convert("RGBA")
    r, g, b, a = img.split()

    # Reduce color depth by shifting bits (8-bit → ~7-bit look)
    def reduce_channel(channel):
        return channel.point(lambda i: (i // 2) * 2)

    r = reduce_channel(r)
    g = reduce_channel(g)
    b = reduce_channel(b)

    return Image.merge("RGBA", (r, g, b, a))


for file in os.listdir("."):
    if file.lower().endswith(INPUT_EXTS):
        try:
            img = Image.open(file)
            processed = reduce_color_depth(img)

            save_path = os.path.join(OUTPUT_FOLDER, file)

            # Preserve PNG transparency properly
            if file.lower().endswith(".png"):
                processed.save(save_path, "PNG")
            else:
                processed.save(save_path)

            print(f"[OK] Processed: {file}")

        except Exception as e:
            print(f"[ERROR] {file}: {e}")

print("Done.")