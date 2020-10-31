#coding -*- utf-8

'''用来爬取糗事百科成人版若干图片的脚本'''
import requests
import urllib
import re
import urllib.parse
import urllib3
import os
from time import sleep
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

def get_file(url):
    html = requests.get(url).text
    p = re.compile(r'[^<]*?<img[^>]*?data-lazy-src="([^"]*)[^>]*?alt="([^"]*)')
    content = p.findall(html)
    content.pop(0)
    for each in (content):
        form = each[0].split('.')
        form = form[len(form)-1]
        name = each[1]
        save_path = r'H:\糗事百科\{}.{}'.format(name,form)
        mode = 'ab' if os.path.exists(save_path) else 'wb'
        try:
            with open(save_path,mode) as f:
                r = requests.get(each[0])
                f.write(r.content)
        except Exception as e:
            print(e)


    '''
        urlretrieve不能够下载http开头的页面
        '''
        #urllib.request.urlretrieve(str(content[0]), 'H:\糗事百科\{}.{}'.format(name,form))
'''
        a = requests.get(each[0])
        save_path = r'H:\糗事百科\{}.{}'.format(name,form)
        mode = 'a' if os.path.exists(save_path) else 'wb'
        with open(save_path,mode) as f:
        f.write(a.content)

'''



for i in range(1444):
    url = 'http://www.qiumeimei.com/page/{}'.format(i)
    get_file(url)
    print('第{}页内容下载完毕'.format(i ))
