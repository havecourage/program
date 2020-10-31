#coding -*- utf-8

import os
import requests
import re
import time
from time import sleep

while True:
    i = 5
    url = 'http://desk.zol.com.cn/meinv/1920x1080/{}.html'.format(i)
    headers = {'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36'}
    html = requests.get(url, headers=headers).text
    full_url = re.findall(r'src=.+?jpg',html)
    name = []
    for each in full_url:
        img = each.split('="')[-1]
        name0 = img.split('/')[-1]
        name.append(img.split('/')[-1])
        save_path = r'/home/rodalph/Pictures/beauty/{}'.format(name0)

        img0 = img.split('208x130')
        if len(img0) == 2:
            img = img0[0] + '1920x1080' +img0[1]
        else:
            continue

        with open(save_path,'wb') as f:
            photo = requests.get(img, headers=headers)
            f.write(photo.content)
            f.close()
    for each in name:
        os.system('gsettings set org.gnome.desktop.background picture-uri "/home/rodalph/Pictures/beauty/{}"'.format(each)) 
        #os.system('win32gui.SystemParametersInfo(win32con.SPI_SETDESKWALLPAPER, "/home/rodalph/Pictures/beauty/{}", 1+2)'.format(each)) #for windows
        sleep(14400)
    i += 1



