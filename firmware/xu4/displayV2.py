
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
	try:

		imgBase               = Image.new('RGB', (800,480), color = (0, 0, 0))

		OPCN3 ,validOPCN3         = mL.readJSONLatestAll("OPCN3")
		BME280,validBME280        = mL.readJSONLatestAll("BME280")
		LIBRAD ,validLIBRAD       = mL.readJSONLatestAll("LIBRAD")
		APDS9002,validAPDS9002    = mL.readJSONLatestAll("APDS9002")
		TB108L,validTB108L        = mL.readJSONLatestAll("TB108L")

		pm1   = str(OPCN3['pm1']).rjust(7," ")
		pm2_5 = str(OPCN3['pm2_5']).rjust(7," ")
		pm10  = str(OPCN3['pm10']).rjust(7," ")
		ozone = str(TB108L['ozone']).rjust(7," ")

		temperature = BME280['temperature']
		pressure    = float(BME280['pressure'])/100
		humidity    = BME280['humidity']

		luminance   = APDS9002['luminance']
		radiation   = LIBRAD['radiationValue']

		lastUpdate  = str(OPCN3['dateTime'])
		d = ImageDraw.Draw(imgBase)

		d.text((1,1),  "  ")
		d.text((1,5),  "  MINTS (Multi-Scale Integrated Sensing and Simulation)", font=fnt, fill="GREEN")
		d.text((1,30), "--------------------------------------------------------------------------------------------------", font=fnt, fill="WHITE")
		d.text((1,55), "  PM1   = ".translate(SUB)  + pm1   + " mg/m3".translate(SUP) , font=fnt, fill="WHITE")
		d.text((1,85), "  PM2.5 = ".translate(SUB)  + pm2_5 + " mg/m3".translate(SUP) , font=fnt, fill="WHITE")
		d.text((1,115),"  PM10  = ".translate(SUB)  + pm10  + " mg/m3".translate(SUP) , font=fnt, fill="WHITE")
		d.text((1,145),"  O3  = ".translate(SUB)  + ozone  + " ppb" , font=fnt, fill="WHITE")
		d.text((1,175),"--------------------------------------------------------------------------------------------------", font=fnt, fill="WHITE")
		d.text((1,205),"  Temperature ="   + temperature + " C", font=fnt, fill="WHITE")
		d.text((1,235),"  Pressure    = "  + str(pressure) + " mbar"  , font=fnt, fill="WHITE")
		d.text((1,265),"  Humidity    = "  + humidity+  " %", font=fnt, fill="WHITE")
		d.text((1,295),"--------------------------------------------------------------------------------------------------", font=fnt, fill="WHITE")
		d.text((1,325),"  Luminance = "   + luminance , font=fnt, fill="WHITE")
		d.text((1,355),"  Radiation  = "  + radiation , font=fnt, fill="WHITE")
		d.text((1,385),"--------------------------------------------------------------------------------------------------", font=fnt, fill="WHITE")
		d.text((1,420),"  Last Updated ="   + lastUpdate , font=fnt, fill="WHITE")
		d.text((1,455),"  MINTS (Multi-Scale Integrated Sensing and Simulation)", font=fnt, fill="GREEN")
		rotated_image = imgBase.rotate(90)
		flipped_img   = rotated_image.transpose(Image.FLIP_LEFT_RIGHT)
		flipped_img.save("mintsLive.bmp")
		time.sleep(2)
		os.system("vodisp bitmap mintsLive.bmp")

	except:
	    	print("Loop incomplete")
