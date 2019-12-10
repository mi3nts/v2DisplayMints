
from PIL import Image, ImageDraw , ImageFont 
import time
import os
img = Image.new('RGB', (800,480), color = (0, 0, 0))
 
d = ImageDraw.Draw(img)

fnt = ImageFont.truetype('fonts/CANDARAB.TTF', 100)
fnt2 = ImageFont.truetype('fonts/CANDARAB.TTF', 50)
d.text((1,1), "            MINTS", font=fnt, fill="GREEN")
d.text((1,200), "              (Multi-scale Integrated ", font=fnt2, fill="WHITE")
d.text((1,250), "                 Sensing and Simulation)", font=fnt2, fill="WHITE")
  
rotated_image = img.rotate(270)
flipped_img = rotated_image.transpose(Image.FLIP_LEFT_RIGHT)

flipped_img.save("mints1.bmp")

while(True):
	os.system("vodisp bitmap mints1.bmp")
	time.sleep(2)
	os.system("vodisp bitmap mints2.bmp")
	time.sleep(2)
	os.system("vodisp bitmap mints5.bmp")
	time.sleep(2)
	os.system("vodisp bitmap mints4.bmp")
	time.sleep(2)
	os.system("vodisp bitmap mints3.bmp")
	time.sleep(2)

