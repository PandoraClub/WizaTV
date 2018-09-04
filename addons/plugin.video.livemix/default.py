import xbmc,xbmcaddon,xbmcgui,xbmcplugin,os,sys,urlparse,re,time,urllib,urllib2,json,random,net,base64,logging,hashlib,pyaes
import pyxbmct.addonwindow as pyxbmct
from addon.common.addon import Addon

net = net.Net()
addon_id = 'plugin.video.livemix'
selfAddon = xbmcaddon.Addon(id=addon_id)
skintheme=selfAddon.getSetting('skin')
artpath='/resources/'+skintheme
searchlist = 'http://metalkettle.co/ukturk.jpg'
icon = xbmc.translatePath(os.path.join('special://home/addons/' + addon_id, 'icon.png'))
fanart = xbmc.translatePath(os.path.join('special://home/addons/' + addon_id, 'fanart.jpg'))
button_quit= xbmc.translatePath(os.path.join('special://home/addons/' + addon_id + artpath, 'power.png'))
button_quit_focus= xbmc.translatePath(os.path.join('special://home/addons/' + addon_id + artpath, 'power_focus.png'))
button_focus = xbmc.translatePath(os.path.join('special://home/addons/' + addon_id + artpath, 'button_focus1.png'))
button_no_focus = xbmc.translatePath(os.path.join('special://home/addons/' + addon_id + artpath, 'button_no_focus1.png'))
bg = xbmc.translatePath(os.path.join('special://home/addons/' + addon_id + artpath, 'main-bg2.png'))
window  = pyxbmct.AddonDialogWindow('')
window.setGeometry(1240, 650, 100, 50)
background=pyxbmct.Image(bg)
window.placeControl(background, -5, 0, 125, 51)
addon = Addon(addon_id, sys.argv)
mode = addon.queries.get('mode', '')
Username='metalkettle2'

def START():
	global Entertainment
	global Movies
	global Music
	global News
	global Sport
	global Docs
	global Kids
	global Food
	global USA
	global Religion
	global List
	global Icon

	#create butttons
	if 'Red' in button_quit:text='0xffffffff'
	elif 'Mono' in button_quit:text='0xffffffff'
	else:text='0xFF000000'
	Entertainment= pyxbmct.Button('Entertain',focusTexture=button_focus,noFocusTexture=button_no_focus,textColor=text,focusedColor=text)
	Movies = pyxbmct.Button('Movies',focusTexture=button_focus,noFocusTexture=button_no_focus,textColor=text,focusedColor=text)
	Music = pyxbmct.Button('Music',focusTexture=button_focus,noFocusTexture=button_no_focus,textColor=text,focusedColor=text)
	News = pyxbmct.Button('News',focusTexture=button_focus,noFocusTexture=button_no_focus,textColor=text,focusedColor=text)
	Sport = pyxbmct.Button('Sport',focusTexture=button_focus,noFocusTexture=button_no_focus,textColor=text,focusedColor=text)
	Docs = pyxbmct.Button('Documentary',focusTexture=button_focus,noFocusTexture=button_no_focus,textColor=text,focusedColor=text)
	Kids= pyxbmct.Button('Kids',focusTexture=button_focus,noFocusTexture=button_no_focus,textColor=text,focusedColor=text)
	Food = pyxbmct.Button('Food',focusTexture=button_focus,noFocusTexture=button_no_focus,textColor=text,focusedColor=text)
	USA = pyxbmct.Button('USA',focusTexture=button_focus,noFocusTexture=button_no_focus,textColor=text,focusedColor=text)
	Religion = pyxbmct.Button('Religion',focusTexture=button_focus,noFocusTexture=button_no_focus,textColor=text)
	List = pyxbmct.List(buttonFocusTexture=button_focus,buttonTexture=button_no_focus,_space=11,_itemTextYOffset=-7,textColor=text)
	Icon=pyxbmct.Image(icon, aspectRatio=2)
	Icon.setImage(icon)
	Quit = pyxbmct.Button(' ',noFocusTexture=button_quit,focusTexture=button_quit_focus)

	#place buttons
	window.placeControl(Entertainment,20, 1,  8, 5)
	window.placeControl(Movies ,20, 6, 8, 5)
	window.placeControl(Music,20, 11, 8, 5)
	window.placeControl(News,20, 16, 8, 5)
	window.placeControl(Sport,20, 21, 8, 5)
	window.placeControl(Docs,20, 26, 8, 5)
	window.placeControl(Kids,20, 31, 8, 5)
	window.placeControl(Food,20, 36, 8, 5)
	window.placeControl(USA,20, 41, 8, 5)
	window.placeControl(Religion,20, 46, 8, 5)
	window.placeControl(List, 30, 1, 90, 30)
	window.placeControl(Icon, 45, 32, 60, 18)
	window.placeControl(Quit, 110, 48, 10, 3)

	#capture mouse moves or up down arrows
	window.connectEventList(
	[pyxbmct.ACTION_MOVE_DOWN,
	pyxbmct.ACTION_MOVE_UP,
		pyxbmct.ACTION_MOUSE_MOVE],
	LIST_UPDATE)

	#navigation
	Entertainment.controlRight(Movies)
	Entertainment.controlLeft(Quit)
	Entertainment.controlDown(List)
	Movies.controlRight(Music)
	Movies.controlLeft(Entertainment)
	Movies.controlDown(List)
	Music.controlRight(News)
	Music.controlLeft(Movies)
	Music.controlDown(List)
	News.controlRight(Sport)
	News.controlLeft(Music)
	News.controlDown(List)
	Sport.controlRight(Docs)
	Sport.controlLeft(News)
	Sport.controlDown(List)
	Docs.controlRight(Kids)
	Docs.controlLeft(Sport)
	Docs.controlDown(List)
	Kids.controlRight(Food)
	Kids.controlLeft(Docs)
	Kids.controlDown(List)
	Food.controlRight(USA)
	Food.controlLeft(Kids)
	Food.controlDown(List)
	USA.controlRight(Religion)
	USA.controlLeft(Food)
	USA.controlDown(List)
	Religion.controlRight(Quit)
	Religion.controlLeft(USA)
	Religion.controlDown(List)
	List.controlUp(Entertainment)
	List.controlLeft(Entertainment)
	List.controlRight(Quit)
	Quit.controlLeft(Religion)
	Quit.controlRight(Entertainment)
	Icon.setImage(icon)
	
	#button actions	
	window.connect(Entertainment,ENT)
	window.connect(Movies,MOVIES)
	window.connect(Music,MUSIC)
	window.connect(News,NEWS)
	window.connect(Sport,SPORT)
	window.connect(Docs,DOCS)
	window.connect(Kids,KIDS)
	window.connect(Food,FOOD)
	window.connect(USA,USAMERICA)
	window.connect(Religion,RELIGION)
	window.connect(List, PLAY_STREAM)
	window.connect(Quit, window.close)
	secstore=selfAddon.getSetting('secstore')
	GetChannels(int(secstore))
	if secstore=='1':window.setFocus(Entertainment)
	if secstore=='2':window.setFocus(Movies)
	if secstore=='3':window.setFocus(Music)
	if secstore=='4':window.setFocus(News)
	if secstore=='5':window.setFocus(Sport)
	if secstore=='6':window.setFocus(Docs)
	if secstore=='7':window.setFocus(Kids)
	if secstore=='8':window.setFocus(Food)
	if secstore=='9':window.setFocus(Religion)
	if secstore=='10':window.setFocus(USA)

def PLAY_STREAM():
        UAToken = GetToken('http://uktvnow.net/app2/v3/get_user_agent', Username)
        headers={'User-Agent':'USER-AGENT-UKTVNOW-APP-V2','app-token':UAToken}
        postdata={'User-Agent':'USER-AGENT-UKTVNOW-APP-V2','app-token':UAToken,'version':'5.7'}
        UAPage = net.http_POST('http://uktvnow.net/app2/v3/get_user_agent',postdata, headers).content
	UAString=re.compile('"msg":{".+?":"(.+?)"}}').findall(UAPage)[0]
	#UA=magicness(UAString)
        UA=UAString
	playlist_token = GetToken('http://uktvnow.net/app2/v3/get_valid_link', Username+channelid)
	postdata = {'useragent':UA,'username':Username,'channel_id':channelid,'version':'5.7'}	
	headers={'User-Agent':'USER-AGENT-UKTVNOW-APP-V2','app-token':playlist_token}
	channels = net.http_POST('http://uktvnow.net/app2/v3/get_valid_link',postdata, headers).content
	match=re.compile('"channel_name":"(.+?)","img":".+?","http_stream":"(.+?)","rtmp_stream":"(.+?)"').findall(channels)
	for name,stream1,stream2 in match:
            stream1= magicness(stream1)+"|User-Agent=EMVideoView 2.5.6 (25600) / Android 6.0.1 / SM-G935F"
            stream2= magicness(stream2)+' timeout=10'
            liz=xbmcgui.ListItem(name, iconImage=iconimage,thumbnailImage=iconimage)
            window.close()
            xbmc.Player ().play(stream1, liz, False)

def ENT():
	List.reset()
	logging.warning('ENT SELECTED')
	sec=1
	selfAddon.setSetting('secstore',str(sec))
	GetChannels(sec)
	
def MOVIES():
	List.reset()
	logging.warning('MOVIES SELECTED')
	sec=2
	selfAddon.setSetting('secstore',str(sec))
	GetChannels(sec)

def MUSIC():
	List.reset()
	logging.warning('MUSIC SELECTED')
	sec=3
	selfAddon.setSetting('secstore',str(sec))
	GetChannels(sec)
	
def NEWS():
	List.reset()
	logging.warning('NEWS SELECTED')
	sec=4
	selfAddon.setSetting('secstore',str(sec))
	GetChannels(sec)
	
def SPORT():
	List.reset()
	logging.warning('SPORT SELECTED')
	sec=5
	selfAddon.setSetting('secstore',str(sec))
	GetChannels(sec)
	
def DOCS():
	List.reset()
	logging.warning('DOCS SELECTED')
	sec=6
	selfAddon.setSetting('secstore',str(sec))
	GetChannels(sec)
	
def KIDS():
	List.reset()
	logging.warning('KIDS SELECTED')
	sec=7
	selfAddon.setSetting('secstore',str(sec))
	GetChannels(sec)
	
def FOOD():
	List.reset()
	logging.warning('FOOD SELECTED')
	sec=8
	selfAddon.setSetting('secstore',str(sec))
	GetChannels(sec)
	
def USAMERICA():
	List.reset()
	logging.warning('USA SELECTED')
	sec=10
	selfAddon.setSetting('secstore',str(sec))
	GetChannels(sec)
	
def RELIGION():
	List.reset()
	logging.warning('RELIGION SELECTED')
	sec=9
	selfAddon.setSetting('secstore',str(sec))
	GetChannels(sec)

def GetChannels(sec):
	global chid
	global chname
	global chicon
	global chstream1
	global chstream2
	chid=[]
	chname=[]
	chicon=[]
	chstream1=[]
	chstream2=[]
	match = GetContent()
	match = GetContent()
	for channelid,name,iconimage,stream1,stream2,cat in match:
		thumb='https://app.uktvnow.net/'+iconimage+'|User-Agent=Dalvik/2.1.0 (Linux; U; Android 6.0.1; SM-G935F Build/MMB29K)'
		if int(cat)==sec:
			chid.append(channelid)
			chname.append(name)
			chicon.append(thumb)
			chstream1.append(stream1)
			chstream2.append(stream2)
			List.addItem(name)

def magicness(url):
        magic="1579547dfghuh,difj389rjf83ff90,45h4jggf5f6g,f5fg65jj46,gr04jhsf47890$93".split(',')
        decryptor = pyaes.new(magic[1], pyaes.MODE_CBC, IV=magic[4])
        url= decryptor.decrypt(url.decode("hex")).split('\0')[0]
        return url

def CreateUser():
	import random
	import string
       	token=getAPIToken('http://app.uktvnow.net/v1/signup','')
       	d_id = ''.join([random.choice(string.ascii_letters + string.digits) for n in xrange(16)])
       	username = ''.join([random.choice(string.ascii_letters + string.digits) for n in xrange(10)])
       	email = ''.join([random.choice(string.ascii_letters + string.digits) for n in xrange(10)])
     	email2 = ''.join([random.choice(string.ascii_letters + string.digits) for n in xrange(4)])
       	email=email+'@'+email2+'.com'
       	password=''.join([random.choice(string.ascii_letters + string.digits) for n in xrange(9)])
       	headers={'User-Agent':'USER-AGENT-UKTVNOW-APP-V1',
			 'Content-Type':'application/x-www-form-urlencoded; charset=UTF-8',
			 'Accept-Encoding' : 'gzip',
			 'app-token':token,
			 'Connection':'Keep-Alive',
			 'Host':'app.uktvnow.net'}
       	postdata={'email': email,
		  'username' : username,
		  'password' : password,
		  'device_id' : d_id}
       	channels = net.http_POST('http://app.uktvnow.net/v1/signup',postdata, headers).content
	return username
		   
def GetContent():
	token=GetToken('http://uktvnow.net/app2/v3/get_all_channels',Username)
	headers={'User-Agent':'USER-AGENT-UKTVNOW-APP-V2','app-token':token}
	postdata={'username':Username}
	channels = net.http_POST('http://uktvnow.net/app2/v3/get_all_channels',postdata, headers).content
	channels = channels.replace('\/','/')
        match=re.compile('"pk_id":"(.+?)","channel_name":"(.+?)","img":"(.+?)","http_stream":"(.+?)","rtmp_stream":"(.+?)","cat_id":"(.+?)"').findall(channels)
	return match
	
def GetToken(url,username):
	if 'get_valid_link' in url:
		s = "uktvnow-token--_|_-http://uktvnow.net/app2/v3/get_valid_link-uktvnow_token_generation-"+username+"-_|_-123456_uktvnow_654321-_|_-uktvnow_link_token"
	else:
		s = "uktvnow-token--_|_-"+url+"-uktvnow_token_generation-"+username+"-_|_-123456_uktvnow_654321"
	return hashlib.md5(s).hexdigest()


def cleanHex(text):
	def fixup(m):
		text = m.group(0)
		if text[:3] == "&#x": return unichr(int(text[3:-1], 16)).encode('utf-8')
		else: return unichr(int(text[2:-1])).encode('utf-8')
	try :return re.sub("(?i)&#\w+;", fixup, text.decode('ISO-8859-1').encode('utf-8'))
	except:return re.sub("(?i)&#\w+;", fixup, text.encode("ascii", "ignore").encode('utf-8'))
	
	 
########################################## ADDON SPECIFIC FUNCTIONS
def LIST_UPDATE():
	global channelid
	global playurl
	global playurl2
	global iconimage
	global name
	if window.getFocus() == List:
		pos=List.getSelectedPosition()
		iconimage=chicon[pos]
		name=chname[pos]
		Icon.setImage(iconimage)
		playurl=chstream1[pos]
		playurl2=chstream2[pos]
		channelid=chid[pos]
		  
def addLink(name,url,mode,iconimage,fanart,description=''):
		u=sys.argv[0]+"?url="+urllib.quote_plus(url)+"&mode="+str(mode)+"&name="+urllib.quote_plus(name)+"&description="+str(description)
		ok=True
		liz=xbmcgui.ListItem(name, iconImage="DefaultFolder.png", thumbnailImage=iconimage)
		liz.setInfo( type="Video", infoLabels={ "Title": name, 'plot': description } )
		liz.setProperty('fanart_image', fanart)
		ok=xbmcplugin.addDirectoryItem(handle=int(sys.argv[1]),url=u,listitem=liz,isFolder=False)
		return ok

START()
#CreateUser()
window.doModal()
del window
if mode==1:START()
#addLink('[COLOR gold]Launch Gui[/COLOR]','url',1,icon,fanart,description='')
xbmcplugin.endOfDirectory(int(sys.argv[1]))
