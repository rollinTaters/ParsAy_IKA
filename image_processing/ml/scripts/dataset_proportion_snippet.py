import os, shutil, random

images = os.listdir("images/")
random.shuffle(images)

train_split = int(0.8 * len(images))
train_images = images[:train_split]
val_images = images[train_split:]

for split, img_list in [("train", train_images), ("val", val_images)]:
    os.makedirs(f"dataset/images/{split}", exist_ok=True)
    os.makedirs(f"dataset/labels/{split}", exist_ok=True)
    print(split)
    for img in img_list:
        shutil.copy(f"images/{img}", f"dataset/images/{split}/{img}")
        label_file = img.replace(".png", ".txt")
        shutil.copy(f"labels/{label_file}", f"dataset/labels/{split}/{label_file}")
