import urllib,urllib2,re,xbmcplugin,xbmcgui,sys,xbmc,xbmcaddon,os,json,urlresolver,time

addon_id = 'plugin.video.tvmix'
fanart = xbmc.translatePath(os.path.join('special://home/addons/' + addon_id , 'fanart.jpg'))
icon = xbmc.translatePath(os.path.join('special://home/addons/' + addon_id, 'icon.png'))
art = xbmc.translatePath(os.path.join('special://home/addons/' + addon_id + '/resources/art/', ''))

def CATEGORIES():
        addDir('New Latest Episodes','http://www.watchepisodes1.com/',1,art+'latest.png',fanart)
        addDir('Most Watched Today','http://www.watchepisodes1.com/home/most-watched-today',1,art+'watched.png',fanart)
        addDir('New Series','http://www.watchepisodes1.com/home/new-series',3,art+'new.png',fanart)
        addDir('Popular Series','http://www.watchepisodes1.com/home/popular-series',3,art+'popular.png',fanart)
        addDir('Genres','http://www.watchepisodes1.com/home/series',7,art+'genres.png',fanart)
        addDir('Search Series','url',5,art+'search.png',fanart)
        xbmc.executebuiltin('Container.SetViewMode(50)')


def GETGENRES(url):
        link=open_url(url)
        match=re.compile('data-genrename="(.+?)"').findall(link)
        for genre in match:
                name=genre.capitalize()
                url='http://www.watchepisodes1.com/home/series?genres='+genre
                iconimage=art+name+'.png'
                addDir(name,url,8,iconimage,fanart)
        xbmc.executebuiltin('Container.SetViewMode(500)')

                
def GENRESERIES(url):
        link=open_url(url)
        match=re.compile('class="wb-image"><img src="(.+?)">.+?<h3 class="wb-name"><a href="(.+?)">(.+?)</a></h3>',re.DOTALL).findall(link)
        for iconimage,url,name in match:
                addDir(name,url,4,iconimage,fanart)

def SEARCH():
        search_entered =''
        keyboard = xbmc.Keyboard(search_entered, 'Search TV Mix')
        keyboard.doModal()
        if keyboard.isConfirmed():
                search_entered = keyboard.getText().replace(' ','+').replace('+and+','+%26+')
        if len(search_entered)>1:
                url = 'http://www.watchepisodes1.com/search/ajax_search?q='+ search_entered
                link=open_url(url)
                data = json.loads(link)
                data=data['series']
                for item in data:
                        name=item['value']
                        movurl=item['seo']
                        url='http://www.watchepisodes1.com/'+movurl
                        iconimage='http://www.watchepisodes1.com/movie_images/'+movurl+'.jpg'
                        addDir(name,url,4,iconimage,fanart)
      
def NEW_POPSERIES(url):
        link=open_url(url)
        match=re.compile('<a href="(.+?)" class="c-image"><img alt=".+?" title="(.+?)" src="(.+?)"></a>').findall(link)
        for url,name,iconimage in match:
                addDir(name,url,4,iconimage,fanart)
                       
def GETMAINMENUITEMS(name,url):
        sec=name
        link=open_url(url)
        link=link.replace('\n','').replace('  ','').replace("('",'"').replace("')",'')
        match=re.compile('class="hb-image" style="background-image: url"(.+?)".+?div class="hb-right".+?href="(.+?)" class="episode">(.+?)<').findall(link)
        for iconimage,url,name in match:
                name=name.replace('&amp;','&')
                if 'Latest' in sec:name=name[:-6]
                else:url=url.split('href="')[1]
                addDir(name,url,2,iconimage,fanart,description=sec)

def CHOICE(name,url,iconimage,description):
        if 'Latest' in description or 'Watched' in description:
                dialog = xbmcgui.Dialog()
                ret = dialog.yesno('', 'Select to watch selected episode','Or','Select to view complete episode list','See Episode List','Watch Episode')
                if ret == 1:
                        GETSOURCES(name,url,iconimage)
                else:
                        url=url.split('-season')[0]
                        GETSEASONS(name,url,iconimage)
        else:
                GETSOURCES(name,url,iconimage)

def GETSOURCES(name,url,iconimage):
        sec=name
        link=open_url(url)
        match=re.compile('<a href=".+?" data-episodeid=".+?" class="site-link" data-linkid=".+?"  data-actuallink="(.+?)" data-hostname="(.+?)"  >.+?</a>').findall(link)
        if len(match)<1:
                notification('TV Mix','No Compatible Streams Found','3000', icon)
                quit()

        for url,name in match:
                if urlresolver.HostedMediaFile(url).valid_url():
                        addLink(name,url,100,iconimage,fanart,description=sec)

def GETSEASONS(name,url,iconimage):
        link=open_url(url)
        match=re.compile('<a title=".+?" href="(.+?)">.+?<div class="season">(.+?)</div>.+?<div class="episode">(.+?)</div>.+?<div class="e-name">(.+?)</div>.+?<div class="date">(.+?)</div>',re.DOTALL).findall(link)
        for url,season,episode,title,dte in match:
                dte=dte.replace('\r\n','').replace(' ','').replace('-','/')
                date = dte
                try:
                        today = time.strftime("%d/%m/%Y")
                        date = time.strptime(date, "%d/%m/%Y")
                        today = time.strptime(today, "%d/%m/%Y")
                        if date > today:dte='[COLOR red]('+dte+')[/COLOR]'
                        if date == today:dte='[COLOR gold]('+dte+')[/COLOR]'
                        if date < today:dte='[COLOR green]('+dte+')[/COLOR]'
                except: pass
                name=season+episode+'  -  '+title+' '+dte
                if not '</div>' in name:
                        addDir(name,url,6,iconimage,fanart,'')

def PLAY(name,url,iconimage,description):
        try:
                stream_url = urlresolver.resolve(url)
                liz = xbmcgui.ListItem(description, iconImage='DefaultVideo.png', thumbnailImage=iconimage)
                liz.setPath(stream_url)
                xbmcplugin.setResolvedUrl(int(sys.argv[1]), True, liz)
        except:
                notification('TV Mix','Stream Unavailable','3000', icon)
                
def notification(title, message, ms, nart):
    xbmc.executebuiltin("XBMC.notification(" + title + "," + message + "," + ms + "," + nart + ")")        
        
def cleanHex(text):
    def fixup(m):
        text = m.group(0)
        if text[:3] == "&#x": return unichr(int(text[3:-1], 16)).encode('utf-8')
        else: return unichr(int(text[2:-1])).encode('utf-8')
    try :return re.sub("(?i)&#\w+;", fixup, text.decode('ISO-8859-1').encode('utf-8'))
    except:return re.sub("(?i)&#\w+;", fixup, text.encode("ascii", "ignore").encode('utf-8'))
    
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

def addDir(name,url,mode,iconimage,fanart,description=''):
        u=sys.argv[0]+"?url="+urllib.quote_plus(url)+"&mode="+str(mode)+"&name="+urllib.quote_plus(name)+"&description="+str(description)+"&iconimage="+urllib.quote_plus(iconimage)
        ok=True
        liz=xbmcgui.ListItem(name, iconImage="DefaultFolder.png", thumbnailImage=iconimage)
        liz.setProperty('fanart_image', fanart)
        ok=xbmcplugin.addDirectoryItem(handle=int(sys.argv[1]),url=u,listitem=liz,isFolder=True)
        return ok

def addLink(name,url,mode,iconimage,fanart,description=''):
        u=sys.argv[0]+"?url="+urllib.quote_plus(url)+"&mode="+str(mode)+"&name="+urllib.quote_plus(name)+"&description="+str(description)+"&iconimage="+urllib.quote_plus(iconimage)
        ok=True
        liz=xbmcgui.ListItem(name, iconImage="DefaultFolder.png", thumbnailImage=iconimage)
        liz.setProperty('fanart_image', fanart)
        liz.setProperty("IsPlayable","true")
        ok=xbmcplugin.addDirectoryItem(handle=int(sys.argv[1]),url=u,listitem=liz,isFolder=False)
        return ok
        
def open_url(url):
    req = urllib2.Request(url)
    req.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
    response = urllib2.urlopen(req)
    link=response.read()
    link=cleanHex(link)
    response.close()
    return link


params=get_params(); url=None; name=None; mode=None; site=None; iconimage=None
try: site=urllib.unquote_plus(params["site"])
except: pass
try: url=urllib.unquote_plus(params["url"])
except: pass
try: name=urllib.unquote_plus(params["name"])
except: pass
try: mode=int(params["mode"])
except: pass
try: iconimage=urllib.unquote_plus(params["iconimage"])
except: pass
try: description=urllib.unquote_plus(params["description"])
except: pass

if mode==None or url==None or len(url)<1: CATEGORIES()
elif mode==1: GETMAINMENUITEMS(name,url)
elif mode==2: CHOICE(name,url,iconimage,description)
elif mode==3: NEW_POPSERIES(url)
elif mode==4: GETSEASONS(name,url,iconimage)
elif mode==5: SEARCH()
elif mode==6: GETSOURCES(name,url,iconimage)
elif mode==7: GETGENRES(url)
elif mode==8: GENRESERIES(url)
elif mode==100: PLAY(name,url,iconimage,description)

xbmcplugin.endOfDirectory(int(sys.argv[1]))

