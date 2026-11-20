
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

def convert_mp3(file):
    base = os.path.splitext(file)[0]
    output = base + ".mp3"

    cmd = [
        "ffmpeg", "-y",
        "-i", file,
        "-codec:a", "libmp3lame",
        "-b:a", "96k",
        output
    ]
    subprocess.run(cmd)


def convert_wav(file):
    tmp = file + ".tmp.wav"

    cmd = [
        "ffmpeg", "-y",
        "-i", file,
        "-ar", "48000",
        "-acodec:a", "pcm_s16le",
        tmp
    ]
    subprocess.run(cmd)

    os.replace(tmp, file)


for file in FILES:
    convert_mp3(file)

for file in os.listdir("."):
    if file.lower().endswith(".wav") and file not in FILES: # this python shit is so funny like "NOT in files" :sob: like put sum ! or something like normal languages :v:
        convert_wav(file)
print("\nCompleted!")
