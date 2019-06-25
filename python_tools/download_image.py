#!/usr/local/bin/python
#-*-coding:UTF-8-*-

import sys
import os
import re
import urllib2
import json

HEADERS = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.114 Safari/537.36',
            'Cookie': 'AspxAutoDetectCookieSupport=1'
        }
IMAGE_PATTERN = r"(https?://){0,1}.*\.(jpg|jpeg|png)"
ZHIHU_IMAGE_PATTERN = r'(https?://pic\d+.zhimg.com/\d+/[0-9a-zA-Z-_]*\.(jpg|png|jpeg))'
TUYIMM_PATTERN = r'(https?://i.ydds.cc/forum/\d+/\d+/[0-9a-zA-Z-_]*\.(jpg|png|jpeg))'
# data-original="https://i.ydds.cc/forum/201904/11/234752upp6blw82a26bb4s.jpg

test_str = "src=\"http://pic1.zhimg.com\u002F50\u002Fv2-f4757ce7eb93baaec64447928e0aafcc_hd.png\" data-rawwidth=\"1242\" data-rawheight=\"2208\" data-size=\"normal\" data-default-watermark-src=\"https:\u002F\u002Fpic1.zhimg.com\u002F50\u002Fv2-f4757ce7eb93baaec64447928e0aafcc_hd.jpg\" class=\"origin_image zh-lightbox-thumb\" width=\"1242\""

class ImageDownloader(object):
    
    def __init__(self):
        super(ImageDownloader,self).__init__()

    def testFind(self):
        pattern = re.compile(ZHIHU_IMAGE_PATTERN)
        teststr = test_str.replace("\u002F","/")
        print pattern.findall(teststr)
    
    def testGetAllHtmlImages(self,url):
        request = urllib2.Request(url,headers=HEADERS)
        print urllib2.urlopen(request).read()

    def getAllHtmlImages(self,url,pattern):
        request = urllib2.Request(url,headers=HEADERS)
        response = urllib2.urlopen(request).read()
        # response = response.replace("\u002F","/")
        # print response
        pattern = re.compile(pattern,re.I)
        imageUrlList = pattern.findall(response)
        imageUrlList = [x[0] for x in imageUrlList]
        print imageUrlList
        return imageUrlList
    
    def getImageSuffix(self,imgUrl):
        print imgUrl
        idx = imgUrl.rfind(".")
        return imgUrl[idx+1:len(imgUrl)]
    
    def getImageName(self,imgUrl):
        print imgUrl
        idx = imgUrl.rfind("/")
        return imgUrl[idx+1:len(imgUrl)]

    def downloadImage(self,imgUrl,i):
        request = urllib2.Request(imgUrl,None,headers=HEADERS)
        response = urllib2.urlopen(request)
        suffix = self.getImageSuffix(imgUrl)
        print suffix
        f = open("/Users/vincent/Desktop/images/{}.{}".format(i,suffix),'wb')
        f.write(response.read())
        f.close()
    
    def downloadTuyimImage(self,imgUrl):
        request = urllib2.Request(imgUrl,None,headers=HEADERS)
        response = urllib2.urlopen(request)
        imgName = self.getImageName(imgUrl)
        f = open("/Users/vincent/Desktop/images/{}".format(imgName),'wb')
        f.write(response.read())
        f.close()

if __name__ == "__main__":
    imageLoader = ImageDownloader()
    # imageLoader.testGetAllHtmlImages("https://www.tuyim.com/thread-21750-1-1.html")
    imgList = imageLoader.getAllHtmlImages("https://www.tuyim.com/thread-21750-1-1.html",TUYIMM_PATTERN)
    # imageLoader.downloadImage(imgList[0],0)
    imgList = list(set(imgList))
    for imgUrl in imgList:
        imageLoader.downloadTuyimImage(imgUrl)
