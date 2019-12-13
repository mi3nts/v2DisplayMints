
from PIL import Image, ImageDraw , ImageFont
import time
import os
from mintsXU4 import mintsSensorReader as mSR
from mintsXU4 import mintsDefinitions as mD
from mintsXU4 import mintsLatest as mL


dataFolder            = mD.dataFolder
imgBase               = Image.new('RGB', (800,480), color = (0, 0, 0))
fnt                   = ImageFont.truetype('fonts/CANDARAB.TTF', 33)
fnt2                  = ImageFont.truetype('fonts/CANDARAB.TTF', 50)
SUB = str.maketrans("0123456789", "₀₁₂₃₄₅₆₇₈₉")
SUP = str.maketrans("0123456789", "⁰¹²³⁴⁵⁶⁷⁸⁹")


def showIntro():

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

showIntro()

while(True):

	OPCN3 ,validOPCN3  = mL.readJSONLatestAll("OPCN3")
	BME280,validBME280 = mL.readJSONLatestAll("BME280")


	pm1   = str(OPCN3['pm1']).rjust(7," ")
	pm2_5 = str(OPCN3['pm2_5']).rjust(7," ")
	pm10  = str(OPCN3['pm10']).rjust(7," ")
	temperature 	= BME280['temperature']
	pressure    	= BME280['pressure']/100
	humidity    	= BME280['humidity']
	lastUpdate  	= str(OPCN3['dateTime'])
	ozone       	= 20.49
	radiationCount  = 23.45
	lightLevel      = 889.00

	d = ImageDraw.Draw(imgBase)
	d.text((1,1),  "  ")
	d.text((1,5),  "  MINTS (Multi-Scale Integrated Sensing and Simulation)", font=fnt, fill="GREEN")
	d.text((1,30), "--------------------------------------------------------------------------------------------------", font=fnt, fill="WHITE")
	d.text((1,55), "  PM1   =".translate(SUB)   + pm1 , font=fnt, fill="WHITE")
	d.text((1,85), "  PM2.5 = ".translate(SUB)  + pm2_5 , font=fnt, fill="WHITE")
	d.text((1,115),"  PM10 = ".translate(SUB)  + pm10 , font=fnt, fill="WHITE")
	d.text((1,145),"--------------------------------------------------------------------------------------------------", font=fnt, fill="WHITE")
	d.text((1,175),"  Temperature ="   + temperature, font=fnt, fill="WHITE")
	d.text((1,205),"  Pressure    = "  + pressure, font=fnt, fill="WHITE")
	d.text((1,235),"  Humidity    = "  + humidity, font=fnt, fill="WHITE")
	d.text((1,275),"--------------------------------------------------------------------------------------------------", font=fnt, fill="WHITE")
	d.text((1,305),"  Ozone       ="   + ozone, font=fnt, fill="WHITE")
	d.text((1,335),"  Radiation   = "  + radiationCount, font=fnt, fill="WHITE")
	d.text((1,375),"  Light Level = "  + lightLevel, font=fnt, fill="WHITE")
	d.text((1,405),"--------------------------------------------------------------------------------------------------", font=fnt, fill="WHITE")
	d.text((1,435),"  Last Updated ="   + lastUpdate , font=fnt, fill="WHITE")
	d.text((1,440),"--------------------------------------------------------------------------------------------------", font=fnt, fill="WHITE")
	d.text((1,450),"  MINTS (Multi-Scale Integrated Sensing and Simulation)", font=fnt, fill="GREEN")

	rotated_image = imgBase.rotate(90)
	flipped_img = rotated_image.transpose(Image.FLIP_LEFT_RIGHT)
	flipped_img.save("mintsLive.bmp")

	time.sleep(2)

	os.system("vodisp bitmap mintsLive.bmp")
	time.sleep(2)
