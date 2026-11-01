import subprocess
import os

FILES = [
    "night1final.wav",
    "night2final2.wav",
    "night3final.wav",
    "night4final.wav",
    "night5final.wav",
    "night6final.wav"
]

BITRATE = "96k"

def convert(file):
    if not os.path.exists(file):
        print(f"Missing: {file}")
        return

    base = os.path.splitext(file)[0]
    output = base + ".mp3"

    cmd = [
        "ffmpeg",
        "-y",
        "-i", file,
        "-codec:a", "libmp3lame",
        "-b:a", BITRATE,
        output
    ]

    print(f"Converting: {file} -> {output}")
    subprocess.run(cmd)

def main():
    for f in FILES:
        convert(f)

    print("\nDone.")

if __name__ == "__main__":
    main()