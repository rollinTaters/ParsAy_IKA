# PLAN
1. Now u're using the old version of label syntax, 
	you gotta change it the result that u wanna anticipate
2. you are not using nvidia GPU right now configure the driver

```console
python3 train.py --img 640 --batch 16 --epochs 100 --data ../dataset.yaml --weights yolov5s.pt
```
