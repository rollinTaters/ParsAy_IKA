import json
import os
from PIL import Image

json_file = "augmented_numbers.json"
label_dir = "labels/"
os.makedirs(label_dir, exist_ok=True)

with open(json_file, "r") as f:
    data = json.load(f)

for item in data:
    image_path = item["image_path"]
    number = item["number"] - 1  # class ID from 0 to 15
    image = Image.open(image_path)
    width, height = image.size

    # Let's say the circle is centered and has size 64x64
    x_center = width // 2
    y_center = height // 2
    bbox_width = 64
    bbox_height = 64

    label_path = os.path.join(label_dir, os.path.splitext(os.path.basename(image_path))[0] + ".txt")
    with open(label_path, "w") as label_file:
        label_file.write(f"{number} {x_center} {y_center} {bbox_width} {bbox_height}\n")
