# -*- coding: utf-8 -*-
import urllib,urllib2,re,xbmcplugin,xbmcgui,xbmc,xbmcaddon,os,codecs
from t0mm0.common.addon import Addon
from t0mm0.common.net import Net

#MD3-Audio - By Mucky Duck (12/05/2015)

addon_id = 'plugin.audio.MD3-Audio'
plugin = xbmcaddon.Addon(id=addon_id)
fanart = xbmc.translatePath(os.path.join('special://home/addons/' + addon_id , 'fanart.jpg'))
icon = xbmc.translatePath(os.path.join('special://home/addons/' + addon_id, 'icon.png'))
baseurl = 'http://www.itemvn.com'
single = 'http://www.itemvn.com/song/?s='
web = 'http:'
baseurl1 = 'http://mp3.mid.az'
listen ='/listen/'
net = Net()

def CATEGORIES():
#        addDir('[B][COLOR yellow]Artists A/Z[/COLOR][/B]',baseurl,10,icon,fanart,'')
        addDir('[B][COLOR yellow]Bestsellers[/COLOR][/B]',baseurl+'/bestsellers',11,icon,fanart,'')
        addDir('[B][COLOR yellow]Billboard Top 10[/COLOR][/B]',baseurl+'/hot100',8,icon,fanart,'')
        addDir('[B][COLOR yellow]Browse Songs[/COLOR][/B]',baseurl+'/browse_songs',9,icon,fanart,'')
        addDir('[B][COLOR yellow]Genre[/COLOR][/B]',baseurl,5,icon,fanart,'')
        addDir('[B][COLOR yellow]Hot Artists[/COLOR][/B]',baseurl+'/topartists',12,icon,fanart,'')
        addDir('[B][COLOR yellow]Just Added[/COLOR][/B]',baseurl+'/justadded',1,icon,fanart,'')
        addDir('[B][COLOR yellow]Most Discussed[/COLOR][/B]',baseurl+'/browse_songs/?sort=md&genre=&t=t',9,icon,fanart,'')
        addDir('[B][COLOR yellow]SpotLight Songs[/COLOR][/B]',baseurl+'/browse_songs/?sort=rf&genre=&t=t',9,icon,fanart,'')
        addDir('[B][COLOR yellow]Suggestions[/COLOR][/B]',baseurl+'/artist/?s=95D51AC15B',13,icon,fanart,'')
        addDir('[B][COLOR yellow]Top Favorited[/COLOR][/B]',baseurl+'/browse_songs/?sort=mf&genre=&t=t',9,icon,fanart,'')
        addDir('[B][COLOR yellow]Top Rated[/COLOR][/B]',baseurl+'/browse_songs/?sort=tr&genre=&t=t',9,icon,fanart,'')
        addDir('[B][COLOR yellow]Radio[/COLOR][/B]','url',60,icon,fanart,'')
        addDir('[B][COLOR yellow]Recent Songs[/COLOR][/B]',baseurl+'/browse_songs/?sort=mr&genre=&t=t',9,icon,fanart,'')
        addDir('[B][COLOR yellow]Search[/COLOR][/B]',baseurl1,40,icon,fanart,'')
        
        
        
def ALBUM(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match1=re.compile('<title>(.+?)</title>\r\n\t\t\t\t\t\t<META NAME="description" CONTENT=".+?">').findall(link)
        match2=re.compile('<a href=\'(.+?)\'><img src=".+?" id=".+?" class="artist_image_large" data-src="(.+?)" alt="(.+?)" />').findall(link)
        match3=re.compile("<a href='(.+?)'.+?>Next</a>").findall(link)
        for name in match1:
                name = name.replace('100','10')
                name = name.replace('- Itemvn','')
                addDir('[B][COLOR green]%s[/COLOR][/B]' %name,'url',None,icon,fanart,'')
        for url,thumb,name in match2:
                name = name.replace('&amp;','&')
                name = name.replace('Album Photo','')
                addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,baseurl+url,6,web+thumb,fanart,'')
        for url in match3:
                addDir('[B][COLOR green]Next Page>>[/COLOR][/B]' ,baseurl+url,1,icon,fanart,'')

def XMLTRACK(url,iconimage):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<object width="650" height="300">\r\n                                                            <param name="movie" value="(.+?)"></param>').findall(link)
        for url in match:
                url = url.replace('MPlayer.swf?configURL=','')
                url = url.replace('&autoPlay=false','')
                addDir('[COLOR yellow]%s[/COLOR]' %name,baseurl+url,7,iconimage,fanart,'')

def TRACK(url,iconimage):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<title>(.+?)</title>\r\n    <length>.+?</length>\r\n    <fileName>(.+?)</fileName>').findall(link)
        for name,url in match:
                name = name.replace('&amp;','&')
                addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,single+url,2,iconimage,fanart,'')


def FLINK(url,iconimage):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<audio id="ap" src="(.+?)" style="width:100%" controls="controls" autoplay="autoplay" loop="loop" preload="auto">Your browser does not support the audio element.</audio>').findall(link)
        for url in match:
                PLAY(name,url,iconimage)

def BILL(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<title>(.+?)</title>\r\n\t\t\t\t\t\t<META NAME="description" CONTENT=".+?">').findall(link)
        match1=re.compile('<a class="albumcover" href=\'(.+?)\' onmouseover=".+?" onmouseout=".+?">\r\n\t\t\t\t\t\t\t\t\t\t\t\t\t                            <img src="(.+?)" id=".+?" class=".+?" alt="(.+?)" />').findall(link)
        match2=re.compile("<a href='(.+?)'.+?>Next</a>").findall(link)
        for name in match:
                name = name.replace('100','10')
                name = name.replace('- Itemvn','')
                addDir('[B][COLOR green]%s[/COLOR][/B]' %name,'url',None,icon,fanart,'')
        for url,thumb,name in match1:
                name = name.replace('&amp;','&')
                name = name.replace('Album Photo','')
                addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,baseurl+url,2,web+thumb,fanart,'')
        for url in match2:
                addDir('[B][COLOR green]Next Page>>[/COLOR][/B]' ,baseurl+url,8,icon,fanart,'')


def SEARCHS(url):
        keyb = xbmc.Keyboard('', 'Search DnB-Sets')
        keyb.doModal()
        if (keyb.isConfirmed()):
                search = keyb.getText()
                encode=urllib.quote(search)
                print encode
                url = baseurl+'/listsong/?keyword='+encode
                print url
                match=re.compile('<a id=".+?" class="artist_underline" href="(.+?)">(.+?)</a>').findall(net.http_GET(url).content) 
                for name,url in match:
                        name = name.replace('<font color=fff788><u>','')
                        name = name.replace('</u></font>','')
                        addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,baseurl+url,2,icon,fanart,'')


def GENRE(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<td><a href="(.+?)" id=".+?" class="menu_genre_item"><div>(.+?)</div></a></td>').findall(link)
        for url,name in match:
                name = name.replace('&amp;','&')
                url = url.replace(' ','%20')
                addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,url,1,icon,fanart,'')


def BROWSE(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<title>(.+?)</title>\r\n\t\t\t\t\t\t<META NAME="description" CONTENT=".+?">').findall(link)
        match1=re.compile('<a class="albumcover" href=\'(.+?)\' onmouseover=".+?" onmouseout=".+?">\r\n\t\t\t\t\t\t\t\t\t\t\t\t\t                <img src=".+?" id=".+?" class="artist_image_large" data-src="(.+?)" alt="(.+?)" />').findall(link)
        match2=re.compile("<a href='(.+?)'.+?>Next</a>").findall(link)
        for name in match:
                name = name.replace('100','10')
                name = name.replace('- Itemvn','')
                addDir('[B][COLOR green]%s[/COLOR][/B]' %name,'url',None,icon,fanart,'')
        for url,thumb,name in match1:
                name = name.replace('&amp;','&')
                name = name.replace('Album Photo','')
                addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,baseurl+url,2,web+thumb,fanart,'')       
        for url in match2:
                addDir('[B][COLOR green]Next Page>>[/COLOR][/B]' ,baseurl+url,9,icon,fanart,'')


def ARTISTAZ(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<a href="(.+?)" id=".+?" class="search_letter">(.+?)</a>').findall(link)
        for url,name in match:
                name = name.replace('#','0/9')
                addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,url,1,icon,fanart,'')

def BESTSELLER(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<title>(.+?)</title>\r\n\t\t\t\t\t\t<META NAME="description" CONTENT=".+?">').findall(link)
        match1=re.compile('<a href=\'(.+?)\'><img src="(.+?)" id=".+?" class="album_image_large" alt="(.+?)" />').findall(link)
        for name in match:
                name = name.replace('Billboard 200 - ','')
                name = name.replace('- Itemvn','')
                addDir('[B][COLOR green]%s[/COLOR][/B]' %name,'url',None,icon,fanart,'')
        for url,thumb,name in match1:
                name = name.replace('&amp;','&')
                name = name.replace('Album Photo','')
                addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,baseurl+url,6,web+thumb,fanart,'')

def HOTARTIST(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<title>(.+?)</title>\r\n\t\t\t\t\t\t<META NAME="description" CONTENT=".+?">').findall(link)
        match1=re.compile('<a href=\'(.+?)\'><img src="(.+?)" id=".+?" class="album_image_large" alt="(.+?)" />').findall(link)
        for name in match:
                name = name.replace('Billboard 200 - ','')
                name = name.replace('- Itemvn','')
                addDir('[B][COLOR green]%s[/COLOR][/B]' %name,'url',None,icon,fanart,'')
        for url,thumb,name in match1:
                name = name.replace('&amp;','&')
                name = name.replace('Album Photo','')
                addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,baseurl+url,1,web+thumb,fanart,'')

def SUGGEST(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<a href=\'(.+?)\'><img src=".+?" id=".+?" class="artist_image_medium" data-src="(.+?)" alt="(.+?)" />').findall(link)
        for url,thumb,name in match:
                name = name.replace('&amp;','&')
                name = name.replace('Album Photo','')
                name = name.replace('Photo','')
                addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,baseurl+url,1,web+thumb,fanart,'')
                
############################################################################################################################
###########################                             mp3.mid.az                                     #####################
############################################################################################################################

def SEARCHMID(url):
        keyb = xbmc.Keyboard('', 'Search MD3 Audio')
        keyb.doModal()
        if (keyb.isConfirmed()):
                search = keyb.getText()
                encode=urllib.quote(search)
                print encode
                url = baseurl1+'/search/?q='+encode
                print url
                match=re.compile('<li data-id=".+?">\n<span class=".+?">\n</span>\n<span class="numbers">\n.+?\n</span>\n<a href="(.+?)">(.+?)</a>\n<span class=".+?">\n</span>\n</li>').findall(net.http_GET(url).content) 
                for url,name in match:
                        name = name.replace('&amp;','&')
                        name = name.replace('&#39;',"'")
                        urllib.quote(u'\u0130'.encode('UTF-8'))
                        name = name.replace(u'\u0130','')
                        urllib.quote(u'\xfc'.encode('UTF-8'))
                        name = name.replace(u'\xfc','')
                        urllib.quote(u'\u041b'.encode('UTF-8'))
                        name = name.replace(u'\u041b','')
                        urllib.quote(u'\u0438'.encode('UTF-8'))
                        name = name.replace(u'\u0438','')
                        urllib.quote(u'\u0448'.encode('UTF-8'))
                        name = name.replace(u'\u0448','')
                        urllib.quote(u'\u044c'.encode('UTF-8'))
                        name = name.replace(u'\u044c','')
                        urllib.quote(u'\u0414'.encode('UTF-8'))
                        name = name.replace(u'\u0414','')
                        urllib.quote(u'\u043e'.encode('UTF-8'))
                        name = name.replace(u'\u043e','')
                        urllib.quote(u'\u0423'.encode('UTF-8'))
                        name = name.replace(u'\u0423','')
                        urllib.quote(u'\u0442'.encode('UTF-8'))
                        name = name.replace(u'\u0442','')
                        urllib.quote(u'\u0440'.encode('UTF-8'))
                        name = name.replace(u'\u0440','')
                        urllib.quote(u'\u0430'.encode('UTF-8'))
                        name = name.replace(u'\u0430','')#
                        urllib.quote(u'\u0259'.encode('UTF-8'))
                        name = name.replace(u'\u0259','')
                        urllib.quote(u'\u0131'.encode('UTF-8'))
                        name = name.replace(u'\u0131','')
                        urllib.quote(u'\xf6'.encode('UTF-8'))
                        name = name.replace(u'\xf6','')
                        urllib.quote(u'\u0421'.encode('UTF-8'))
                        name = name.replace(u'\u0421','')
                        urllib.quote(u'\u0451'.encode('UTF-8'))
                        name = name.replace(u'\u0451','')
                        urllib.quote(u'\u0441'.encode('UTF-8'))
                        name = name.replace(u'\u0441','')
                        urllib.quote(u'\u044b'.encode('UTF-8'))
                        name = name.replace(u'\u044b','')
                        urllib.quote(u'\u0422'.encode('UTF-8'))
                        name = name.replace(u'\u0422','')
                        urllib.quote(u'\u043b'.encode('UTF-8'))
                        name = name.replace(u'\u043b','')
                        urllib.quote(u'\u043c'.encode('UTF-8'))
                        name = name.replace(u'\u043c','')
                        urllib.quote(u'\u0447'.encode('UTF-8'))
                        name = name.replace(u'\u0447','')
                        urllib.quote(u'\u0432'.encode('UTF-8'))
                        name = name.replace(u'\u0432','')#
                        urllib.quote(u'\xd6'.encode('UTF-8'))
                        name = name.replace(u'\xd6','')
                        urllib.quote(u'\xc7'.encode('UTF-8'))
                        name = name.replace(u'\xc7','')#
                        addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,baseurl1+url,41,icon,fanart,'')


def MIDTRACK(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<li class="play-inner" data-link="(.+?)" data-artist="(.+?)" data-song="(.+?)"><em></em><span>.+?</span></li>').findall(link)
        for url,artist,name in match:
                name = name.replace('&amp;','&')
                name = name.replace('&#39;',"'")
                addDir('[B][COLOR yellow]%s %s[/COLOR][/B]' %(artist,name),baseurl1+url,3,icon,fanart,'')


############################################################################################################################
###########################                                                                             ####################                
############################################################################################################################
############################################################################################################################
###########################                                   RADIO                                     ####################
############################################################################################################################
baseurl20 = 'http://www.listenlive.eu'
baseurl21 = 'http://kodimediaportal.ml/md3radiorequest.html'
baseurl22 = 'http://dir.xiph.org'



def RADIO(URL):
        addDir('[B][COLOR yellow]dir.xiph.org[/COLOR][/B]',baseurl22,70,icon,fanart,'')
        addDir('[B][COLOR yellow]www.listenlive.eu[/COLOR][/B]',baseurl20,61,icon,fanart,'')
        addDir('[B][COLOR yellow]Requested Stations[/COLOR][/B]',baseurl21,100,icon,fanart,'')


def LISTENLIVE(URL):
        addDir('[B][COLOR yellow]Top 40[/COLOR][/B]',baseurl20+'/top40.html',62,icon,fanart,'')
        addDir('[B][COLOR yellow]United Kingdom[/COLOR][/B]',baseurl20+'/uk.html',62,icon,fanart,'')

def LLIVEUK(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<td><a href=".+?"><b>(.+?)</b></a></td>\n<td>(.+?)</td>\n<td><img src="mp3.gif" width="12" height="12" alt="MP3" /></td>\n<td><a href="(.+?)">(.+?)</a></td>\n<td>(.+?)</td>').findall(link)
        for name,reg,url,qual,gen in match:
                name = name.replace('&amp;','&')
                addDir('[B][COLOR yellow]%s %s %s %s[/COLOR][/B]' %(name,gen,qual,reg),url,3,icon,fanart,'')

def XIPH(URL):
        addDir('[B][COLOR yellow]80s Radio[/COLOR][/B]',baseurl22+'/by_genre/80s',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Alternative[/COLOR][/B]',baseurl22+'/by_genre/Alternative',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Dance[/COLOR][/B]',baseurl22+'/by_genre/Dance',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Hardcore[/COLOR][/B]',baseurl22+'/search?search=hardcore',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Hip Hop[/COLOR][/B]',baseurl22+'/search?search=hip+hop',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Hits[/COLOR][/B]',baseurl22+'/by_genre/Hits',71,icon,fanart,'')
        addDir('[B][COLOR yellow]House[/COLOR][/B]',baseurl22+'/by_genre/House',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Jazz[/COLOR][/B]',baseurl22+'/by_genre/Jazz',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Jungle[/COLOR][/B]',baseurl22+'/search?search=jungle',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Misc[/COLOR][/B]',baseurl22+'/by_genre/Misc',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Music[/COLOR][/B]',baseurl22+'/by_genre/Music',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Oldies[/COLOR][/B]',baseurl22+'/by_genre/Oldies',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Pop[/COLOR][/B]',baseurl22+'/by_genre/Pop',71,icon,fanart,'')
        addDir('[B][COLOR yellow]R&B[/COLOR][/B]',baseurl22+'/search?search=R%26B',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Radio[/COLOR][/B]',baseurl22+'/by_genre/Radio',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Rap[/COLOR][/B]',baseurl22+'/search?search=rap',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Reggae[/COLOR][/B]',baseurl22+'/search?search=reggae',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Rock[/COLOR][/B]',baseurl22+'/by_genre/Rock',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Talk[/COLOR][/B]',baseurl22+'/by_genre/Talk',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Top40[/COLOR][/B]',baseurl22+'/by_genre/Top40',71,icon,fanart,'')
        addDir('[B][COLOR yellow]Various[/COLOR][/B]',baseurl22+'/by_genre/Various',71,icon,fanart,'')
        
        


def XIPHINDEX(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<a href="(.+?)" title="(.+?)" onclick=".+?">.+?</a>').findall(link)
        for url,name in match:
                nono = '.xspf'
                name = name.replace('&amp;','&')
                name = name.replace("'",'')
                name = name.replace('Listen to','')
                if nono not in url:
                        addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,baseurl22+url,3,icon,fanart,'')

def RADREQ(url):
        req = urllib2.Request(url)
        req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
        response = urllib2.urlopen(req)
        link=response.read()
        response.close()
        match=re.compile('<item>\n\t<title>(.+?)</title>         \n        <link>(.+?)</link>\n        <thumbnail></thumbnail>\n</item>').findall(link)
        for name,url in match:
                name = name.replace('&amp;','&')
                addDir('[B][COLOR yellow]%s[/COLOR][/B]' %name,url,3,icon,fanart,'')



############################################################################################################################
###########################                                                                             ####################                
############################################################################################################################
def PLAY(name,url,iconimage):
        addon_handle = int(sys.argv[1])
        xbmcplugin.setContent(addon_handle, 'audio')
        li = xbmcgui.ListItem('[COLOR green]PLAY[/COLOR] [COLOR yellow]%s[/COLOR]' %name, iconImage=iconimage, thumbnailImage=iconimage)
        li.setProperty('fanart_image', fanart)
        xbmcplugin.addDirectoryItem(handle=addon_handle, url=url, listitem=li)
        xbmcplugin.endOfDirectory(addon_handle)



def get_params():
        param=[]
        paramstring=sys.argv[2]
        if len(paramstring)>=2:
                params=sys.argv[2]
                cleanedparams=params.replace('?','')
                if (params[len(params)-1]=='/'):
                        params=params[0:len(params)-2]
                pairsofparams=cleanedparams.split('&')
                param={}
                for i in range(len(pairsofparams)):
                        splitparams={}
                        splitparams=pairsofparams[i].split('=')
                        if (len(splitparams))==2:
                                param[splitparams[0]]=splitparams[1]
                                
        return param



def addLink(name,url,iconimage):
        ok=True
        liz=xbmcgui.ListItem(name, iconImage="DefaultAudio.png", thumbnailImage=iconimage)
        liz.setInfo( type="Audio", infoLabels={ "Title": name } )
        ok=xbmcplugin.addDirectoryItem(handle=int(sys.argv[1]),url=url,listitem=liz,isFolder=False)
        return ok


def addDir(name,url,mode,iconimage,fanart,description):
        u=sys.argv[0]+"?url="+urllib.quote_plus(url)+"&mode="+str(mode)+"&name="+urllib.quote_plus(name)+"&iconimage="+urllib.quote_plus(iconimage)+"&fanart="+urllib.quote_plus(fanart)+"&description="+urllib.quote_plus(description)
        ok=True
        liz=xbmcgui.ListItem(name, iconImage="DefaultFolder.png", thumbnailImage=iconimage)
        liz.setInfo( type="Video", infoLabels={ "Title": name, "Plot": description } )
        liz.setProperty( "Fanart_Image", fanart )
        ok=xbmcplugin.addDirectoryItem(handle=int(sys.argv[1]),url=u,listitem=liz,isFolder=True)
        return ok

        
              
params=get_params()
url=None
name=None
mode=None


try:
        url=urllib.unquote_plus(params["url"])
except:
        pass
try:
        name=urllib.unquote_plus(params["name"])
except:
        pass
try:
        iconimage=urllib.unquote_plus(params["iconimage"])
except:
        pass
try:        
        mode=int(params["mode"])
except:
        pass
try:        
        fanart=urllib.unquote_plus(params["fanart"])
except:
        pass
try:        
        description=urllib.unquote_plus(params["description"])
except:
        pass
        
        

print "Mode: "+str(mode)
print "URL: "+str(url)
print "Name: "+str(name)

########################################
if mode==None or url==None or len(url)<1:
        print ""
        CATEGORIES()
        
elif mode==1:
        print ""+url
        ALBUM(url)

elif mode==2:
        print ""+url
        FLINK(url,iconimage)

elif mode==3:
        print ""+url
        PLAY(name,url,iconimage)

elif mode==4:
        print ""+url
        SEARCHS(url)

elif mode==5:
        print ""+url
        GENRE(url)

elif mode==6:
        print ""+url
        XMLTRACK(url,iconimage)

elif mode==7:
        print ""+url
        TRACK(url,iconimage)

elif mode==8:
        print ""+url
        BILL(url)

elif mode==9:
        print ""+url
        BROWSE(url)

elif mode==10:
        print ""+url
        ARTISTAZ(url)

elif mode==11:
        print ""+url
        BESTSELLER(url)

elif mode==12:
        print ""+url
        HOTARTIST(url)

elif mode==13:
        print ""+url
        SUGGEST(url)

elif mode==40:
        print ""+url
        SEARCHMID(url)

elif mode==41:
        print ""+url
        MIDTRACK(url)

elif mode==60:
        print ""+url
        RADIO(url)

elif mode==61:
        print ""+url
        LISTENLIVE(url)

elif mode==62:
        print ""+url
        LLIVEUK(url)

elif mode==70:
        print ""+url
        XIPH(url)

elif mode==71:
        print ""+url
        XIPHINDEX(url)

elif mode==100:
        print ""+url
        RADREQ(url)

xbmcplugin.endOfDirectory(int(sys.argv[1]))
