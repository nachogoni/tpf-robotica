#!/usr/bin/env python

import cv


class ObjectBox:

	def __init__(self,(x,y,w,h)):
		self.x=x
		self.y=y
		self.w=w
		self.h=h
 	
 	def containsPoint(self,(x,y)):
 		return x>self.x and x<(self.x+self.w) and y>self.y and y<(self.y+self.w)
 	
 	def getRectangle(self):
 		return (self.x,self.y,self.w,self.h)
 	def move(self,xdelta,ydelta):
 		self.x=self.x+xdelta
 		self.y=self.y+ydelta
 		