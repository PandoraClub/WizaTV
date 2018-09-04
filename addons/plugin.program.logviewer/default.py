'''
kinkin
'''

import urllib,urllib2,re,xbmcplugin,xbmcgui,os
import settings
import time,datetime,time
import glob
import shutil


ADDON = settings.addon()
addon_path = os.path.join(xbmc.translatePath('special://home/addons'), '')
fanart = xbmc.translatePath(os.path.join('special://home/addons/plugin.program.logviewer', 'fanart.jpg'))
iconart = xbmc.translatePath(os.path.join('special://home/addons/plugin.program.logviewer', 'icon.png'))
USERDATA_PATH = os.path.join(xbmc.translatePath('special://profile'), '')
EXTRACT_PATH = settings.extract_directory()
dialog = xbmcgui.Dialog()
path=xbmc.translatePath('special://logpath')
temp_path=os.path.join(xbmc.translatePath('special://profile/addon_data/plugin.program.logviewer'), 'temp_kodi.log')
try:
    logfile=os.path.join(path, 'kodi.log')
except:
    try:
        logfile=os.path.join(path, 'xbmc.log')
    except:
        try:
            logfile=os.path.join(path, 'spmc.log')
        except:
            pass

shutil.copyfile(logfile, temp_path)
f = open(temp_path, 'r')
text = f.read()
f.close()
cerror=text.count('ERROR:')
cwarning=text.count('WARNING:')

def MENU(name):
    addDir("Errors Only (%s)" % cerror, 'ERROR',1, iconart,'blank','View all ')
    addDir("Warnings Only (%s)" % cwarning, 'WARNING',2, iconart,'blank','View all ')
    addDir("Errors and Warnings", 'ERROR_WARNING',3, iconart,'blank','View all ')
    addDir("Complete Log", 'ALL',4, iconart,'blank','View all ')

def view_logerror(name,method):
    ts=time.time()
    count=0
    error=""
    log_list=text.split('\n')
    for list in log_list:
        if not 'NOTICE:' in list and not 'WARNING:' in list and not 'DEBUG:' in list:
            if 'ERROR' in list or not ':T' in list:
                if 'ERROR:' in list:count=count+1
                else:count=count
                #timestr=list[:8]
                if count%2==0:
                    error='[COLOR gold]'+error+list+'[/COLOR]\n'
                else:
                    error='[COLOR cyan]'+error+list+'[/COLOR]\n'
    header="[B][COLOR red]LOG FILE ERRORS[/B][/COLOR]"
    if method=='view':
        msg=error
        TextBoxes(header,msg)
    else:
        exfile=create_file(EXTRACT_PATH, 'EasyLogViewer_ERRORS.log')
        write_to_file(exfile, error, False)
        dialog.ok("File Saved", "", exfile)
	
	
def view_logwarning(name,method):
    ts=time.time()
    count=0
    error=""
    log_list=text.split('\n')
    for list in log_list:
        if not 'NOTICE:' in list:
            if 'WARNING' in list:
                if 'WARNING:' in list:count=count+1
                else:count=count
                #timestr=list[:8]
                if count%2==0:
                    error='[COLOR gold]'+error+list+'[/COLOR]\n'
                else:
                    error='[COLOR cyan]'+error+list+'[/COLOR]\n'
    header="[B][COLOR red]LOG FILE WARNINGS[/B][/COLOR]"
    if method=='view':
        msg=error
        TextBoxes(header,msg)
    else:
        exfile=create_file(EXTRACT_PATH, 'EasyLogViewer_WARNINGS.log')
        write_to_file(exfile, error, False)
        dialog.ok("File Saved", "", exfile)
	
def view_logboth(name,method):
    ts=time.time()
    count=0
    error=""
    log_list=text.split('\n')
    for list in log_list:
        if not 'NOTICE:' in list:
            if 'ERROR' in list or 'WARNING' in list or not ':T' in list:
                if 'ERROR:' in list or 'WARNING:' in list:count=count+1
                else:count=count
                #timestr=list[:8]
                if count%2==0:
                    error='[COLOR gold]'+error+list+'[/COLOR]\n'
                else:
                    error='[COLOR cyan]'+error+list+'[/COLOR]\n'
    header="[B][COLOR red]LOG FILE ERRORS AND WARNINGS[/B][/COLOR]"
    if method=='view':
        msg=error
        TextBoxes(header,msg)
    else:
        exfile=create_file(EXTRACT_PATH, 'EasyLogViewer_ERRORS_WARNINGS.log')
        write_to_file(exfile, error, False)
        dialog.ok("File Saved", "", exfile)
	
def view_logall(name,method):
    ts=time.time()
    count=0
    error=""
    log_list=text.split('\n')
    for list in log_list:
        if 'ERROR:' in list or 'WARNING:' in list or 'NOTICE:' in list:count=count+1
        else:count=count
        #timestr=list[:8]
        if count%2==0:
             error='[COLOR gold]'+error+list+'[/COLOR]\n'
        else:
            error='[COLOR cyan]'+error+list+'[/COLOR]\n'
    header="[B][COLOR red]LOG FILE[/B][/COLOR]"
    if method=='view':
        msg=error
        TextBoxes(header,msg)
    else:
        exfile=create_file(EXTRACT_PATH, 'EasyLogViewer_ALL.log')
        write_to_file(exfile, error, False)
        dialog.ok("File Saved", "", exfile)

def TextBoxes(heading,anounce):
        class TextBox():
            """Thanks to BSTRDMKR for this code:)"""
            WINDOW=10147; CONTROL_LABEL=1; CONTROL_TEXTBOX=5 # constants
            def __init__(self,*args,**kwargs):
                xbmc.executebuiltin("ActivateWindow(%d)" % (self.WINDOW,)) # activate the text viewer window
                self.win=xbmcgui.Window(self.WINDOW) # get window
                xbmc.sleep(500) # give window time to initialize
                self.setControls()
            def setControls(self):
                self.win.getControl(self.CONTROL_LABEL).setLabel(heading) # set heading
                try: f=open(anounce); text=f.read()
                except: text=anounce
                self.win.getControl(self.CONTROL_TEXTBOX).setText(text); return
        TextBox()

def create_directory(dir_path, dir_name=None):
    if dir_name:
        dir_path = os.path.join(dir_path, dir_name)
    dir_path = dir_path.strip()
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)
    return dir_path

def create_file(dir_path, file_name=None):
    if file_name:
        file_path = os.path.join(dir_path, file_name)
    file_path = file_path.strip()
    if not os.path.exists(file_path):
        f = open(file_path, 'w')
        f.write('')
        f.close()
    return file_path

def regex_from_to(text, from_string, to_string, excluding=True):
    if excluding:
        r = re.search("(?i)" + from_string + "([\S\s]+?)" + to_string, text).group(1)
    else:
        r = re.search("(?i)(" + from_string + "[\S\s]+?" + to_string + ")", text).group(1)
    return r

def regex_get_all(text, start_with, end_with):
    r = re.findall("(?i)" + start_with + "([\S\s]+?)" + end_with, text)
    return r

def strip_text(r, f, t, excluding=True):
    r = re.search("(?i)" + f + "([\S\s]+?)" + t, r).group(1)
    return r


def find_list(query, search_file):
    try:
        content = read_from_file(search_file) 
        lines = content.split('\n')
        index = lines.index(query)
        return index
    except:
        return -1
		
def add_to_list(list, file):
    if find_list(list, file) >= 0:
        return

    if os.path.isfile(file):
        content = read_from_file(file)
    else:
        content = ""

    lines = content.split('\n')
    s = '%s\n' % list
    for line in lines:
        if len(line) > 0:
            s = s + line + '\n'
    write_to_file(file, s)
    xbmc.executebuiltin("Container.Refresh")
    
def remove_from_list(list, file):
    index = find_list(list, file)
    if index >= 0:
        content = read_from_file(file)
        lines = content.split('\n')
        lines.pop(index)
        s = ''
        for line in lines:
            if len(line) > 0:
                s = s + line + '\n'
        write_to_file(file, s)
        xbmc.executebuiltin("Container.Refresh")
		
def write_to_file(path, content, append, silent=False):
    try:
        if append:
            f = open(path, 'a')
        else:
            f = open(path, 'w')
        f.write(content)
        f.close()
        return True
    except:
        if not silent:
            print("Could not write to " + path)
        return False

def read_from_file(path, silent=False):
    try:
        f = open(path, 'r')
        r = f.read()
        f.close()
        return str(r)
    except:
        if not silent:
            print("Could not read from " + path)
        return None

		
def notification(title, message, ms, nart):
    xbmc.executebuiltin("XBMC.notification(" + title + "," + message + "," + ms + "," + nart + ")")
	
def setView(content, viewType):
    if content:
        xbmcplugin.setContent(int(sys.argv[1]), content)
        xbmc.executebuiltin("Container.SetViewMode(504)")
        #xbmc.executebuiltin("Container.SetViewMode(504)")
   

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


def addDir(name,url,mode,iconimage,list,description):
        u=sys.argv[0]+"?url="+urllib.quote_plus(url)+"&mode="+str(mode)+"&name="+urllib.quote(name)+"&iconimage="+str(iconimage)+"&list="+str(list)+"&description="+str(description)
        ok=True
        contextMenuItems = []
        contextMenuItems.append(("[COLOR cyan]Save to Extract Path[/COLOR]",'XBMC.RunPlugin(%s?name=%s&mode=5&url=%s)'%(sys.argv[0], urllib.quote(name), urllib.quote(url))))
        liz=xbmcgui.ListItem(name, iconImage=iconart, thumbnailImage=iconart)
        liz.setProperty('fanart_image', fanart )
        liz.addContextMenuItems(contextMenuItems, replaceItems=False)
        ok=xbmcplugin.addDirectoryItem(handle=int(sys.argv[1]),url=u,listitem=liz,isFolder=False)
        return ok
		
        
              
params=get_params()

url=None
name=None
mode=None
iconimage=None
options=None



try:
        url=urllib.unquote_plus(params["url"])
except:
        pass
try:
        name=urllib.unquote_plus(params["name"])
except:
        pass
try:
        mode=int(params["mode"])
except:
        pass
try:
        iconimage=urllib.unquote_plus(params["iconimage"])
except:
        pass
try:
        start=urllib.unquote_plus(params["start"])
except:
        pass
try:
        list=urllib.unquote_plus(params["list"])
except:
        pass
try:
        options=str(params["options"])
except:
        pass
try:
        description=str(params["description"])
except:
        pass

if mode==None or url==None or len(url)<1:
        MENU(name)
		
elif mode == 1:
        view_logerror(url,'view')
		
elif mode == 2:
        view_logwarning(url,'view')
		
elif mode == 3:
        view_logboth(url,'view')
		
elif mode == 4:
        view_logall(url,'view')
		
elif mode == 5:
        if url=='ERROR':
            view_logerror(url,'extract')
        elif url=='WARNING':
            view_logwarning(url,'extract')
        elif url=='ERROR_WARNING':
            view_logboth(url,'extract')
        else:
            view_logall(url,'extract')
	
		
xbmcplugin.endOfDirectory(int(sys.argv[1]))


