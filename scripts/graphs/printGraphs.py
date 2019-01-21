#!/usr/bin/python

import distanceVsTotalCover

def main():
	fbProtocols = ["fb-100", "fb-300", "fb-500"]
	stProtocols = ["st100-100", "st300-300", "st500-500"]
	distanceVsTotalCover.printDistanceVsTotalCover(fbProtocols, "fb", "Fast Broadcast protocols")
	distanceVsTotalCover.printDistanceVsTotalCover(stProtocols, "st", "Static protocols")

if __name__ == "__main__":
	main()

