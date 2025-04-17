import json
import os
from PIL import Image

def convert_to_yolo(x_center, y_center, width, height, img_width, img_height):
    return (
        x_center / img_width,
        y_center / img_height,
        width / img_width,
        height / img_height,
    )

json_file = "augmented_numbers.json"
label_dir = "labels/"
os.makedirs(label_dir, exist_ok=True)

with open(json_file, "r") as f:
    data = json.load(f)

for item in data:
    image_path = item["image_path"].strip()
    number = item["number"] - 1 
    
    try:
        image = Image.open(image_path)
        width, height = image.size
    except Exception as e:
        print(f"ERROR: Could not open image: {image_path} — {e}")
        continue

    bbox_width = 64
    bbox_height = 64
    x_center = width // 2
    y_center = height // 2

    x_norm, y_norm, w_norm, h_norm = convert_to_yolo(x_center, y_center, bbox_width, bbox_height, width, height)

    if not all(0 <= v <= 1 for v in [x_norm, y_norm, w_norm, h_norm]):
        print(f"SKIP: Invalid normalized values in {image_path}")
        continue

    label_filename = os.path.splitext(os.path.basename(image_path))[0] + ".txt"
    label_path = os.path.join(label_dir, label_filename)
    
    with open(label_path, "w") as label_file:
        label_file.write(f"{number} {x_norm:.6f} {y_norm:.6f} {w_norm:.6f} {h_norm:.6f}\n")

    print(f"OK: Wrote label for {image_path}")

