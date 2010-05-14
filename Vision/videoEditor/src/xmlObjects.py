import xml.etree.ElementTree as ET
'''
Created on Apr 22, 2010

@author: diego
'''
import cv

def saveObjects(objectDict,file,name,capture,frameNumber):
    finalFrame=frameNumber;
    width=cv.GetCaptureProperty(capture,cv.CV_CAP_PROP_FRAME_WIDTH)
    height=cv.GetCaptureProperty(capture,cv.CV_CAP_PROP_FRAME_HEIGHT)
    fps=cv.GetCaptureProperty(capture,cv.CV_CAP_PROP_FPS)
    #attributes are written in alphabetical order
    root=ET.Element('objects', 
                    {'finalFrame':str(finalFrame), 
                     'fps':str(fps),
                     'video':'video',
                     'videoWidth':str(width),
                     'videoHeight':str(height)})
    
    for frameNumber in objectDict.keys():
        frame=ET.SubElement(root,'frame',{'number':str(frameNumber)})
        for object in objectDict[frameNumber]:
            cObject=ET.SubElement(frame,'object')
            x=ET.SubElement(cObject,'x')
            x.text=str(object.x)
            y=ET.SubElement(cObject,'y')
            y.text=str(object.y)
            w=ET.SubElement(cObject,'w')
            w.text=str(object.w)
            h=ET.SubElement(cObject,'h')
            h.text=str(object.h)
    tree=ET.ElementTree(root)
    tree.write(name + ".xml")
    print 'done writing filename' + name + ".xml" 
            
            
        
    
    
        