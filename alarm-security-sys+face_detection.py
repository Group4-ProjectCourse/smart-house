
import face_recognition
import imutils
import pickle
import time
from imutils.video import VideoStream
from imutils.video import FPS
import cv2
from pydub import AudioSegment
from pydub.playback import play
from firebase import firebase
import serial
import time
import re

FBConn = firebase.FirebaseApplication('https://smart-house-ae2d9-default-rtdb.europe-west1.firebasedatabase.app/', None)

song = AudioSegment.from_mp3("soundclip.mp3")

currentname = "unknown"
currentname2 = "unknown"
autolamp=0

encodingsP = "encodings.pickle"

print("Loading face detection & extraction of characteristics")
data = pickle.loads(open(encodingsP, "rb").read())

vs = VideoStream(src=0,framerate=10).start()
time.sleep(2.0)

fps = FPS().start()

while True:
	
	
	frame = vs.read()
	
	frame = imutils.resize(frame, width=500)
	boxes = face_recognition.face_locations(frame)
	encodings = face_recognition.face_encodings(frame, boxes)
	names = []

	for encoding in encodings:

		matches = face_recognition.compare_faces(data["encodings"],
			encoding)

		if True in matches:
            
			matchedIdxs = [i for (i, b) in enumerate(matches) if b]
			counts = {}
			
			for i in matchedIdxs:
				name = data["names"][i]
				counts[name] = counts.get(name, 0) + 1

			name = max(counts, key=counts.get)
			while currentname != name:
				currentname = name
				while name == "Alex":
				     print("Security Alarm Deactivated")
				     lamp = FBConn.put("/Security","lazerX",autolamp)
				     name = "unknown2"
				     break
			    
				     
				     



