'''
Kinkin

HUB Parenal Controls module allows addon developers to add controls to your addons with only a few lines of code.
Note that this module is only installed with the XBMCHUB Maintenance Tool. All settings are controlled within the 
Maintenance Tool.

To use the module:

1. Add the following to your default.py to import:
    if os.path.exists(xbmc.translatePath("special://home/addons/")+'script.module.hubparentalcontrol'):
        HUBPC = True
        from hubparentalcontrol import parentalcontrol
    else:
        HUBPC = False
		
2. Add the following to your PLAY function. You can also add to download/trailer functions as required:
    if HUBPC:
        if videotype == 'movies':
            PC = parentalcontrol.checkrating(name,year,mpaa,'movies')
        else:
            PC = parentalcontrol.checkrating(showname,year,mpaa,'tvshow')
        if PC != 'PC_PLAY':
            return
			
name: Movie or TV Show name
year: Pass None if you don't have the year
mpaa: Module will use this mpaa rating if you have it. if not it will use metahandler to grab the rating. Pass None if not available
videotype: Either 'movies' or 'tvshow'. 

For a movie-only addon you could just add:
    if HUBPC:
        PC = parentalcontrol.checkrating(name,year,mpaa,'movies')
        if PC == 'PC_BLOCKED':
            return
'''
#coding=UTF8
import urllib,urllib2,re,xbmcplugin,xbmcgui,os,xbmcaddon,xbmc
import settings
import time,datetime
from datetime import date
from hashlib import md5
from metahandler import metahandlers
metainfo = metahandlers.MetaData()
PC_ENABLE = settings.pc_enable_pc()
PC_WATERSHED = settings.pc_watershed_pc()
PC_RATING = settings.pc_pw_required_at()
PC_PASS = settings.pc_pass()
PC_DEFAULT = settings.pc_default()
PC_TOGGLE = settings.pc_enable_pc_settings()
CUSTOM_PC = settings.pc_custom_pc_file()
addon_path = os.path.join(xbmc.translatePath('special://home/addons'), '')

mrating = ['NC-17', 'R','PG-13','PG','G']
tvrating = ['TV-MA','TV-14','TV-PG','TV-G','TV-Y7-FV','TV-Y7','TV-Y']


	
def setting():
    pw = ""
    dialog = xbmcgui.Dialog()
    keyboard = xbmc.Keyboard(pw, 'Enter your PIN/Password', True)
    keyboard.doModal()
    if keyboard.isConfirmed():
        pw = keyboard.getText()
        if pw == PC_PASS:
            if PC_TOGGLE == "UNLOCKED":
                ADDON.setSetting('enable_pc_settings', value='LOCKED')
                xbmc.executebuiltin("Container.Refresh")
            else:
                ADDON.setSetting('enable_pc_settings', value='UNLOCKED')
                xbmc.executebuiltin("Container.Refresh")
                ADDON.openSettings()				
        else:
            dialog.ok("Incorrect PIN/Password","")
			
def checkrating(name,year,mpaa,type):
    #PARENTAL CONTROL
    if ' (' in name and ')' in name:
        name = name.split(' (')[0]
        if year == None and type != 'movies':
            year = name.split(' (')[1].replace('(', '').replace(')', '')
    now = time.strftime("%H")
    dialog = xbmcgui.Dialog()
    if PC_ENABLE:
        mpaa,mpaaname = parental_control(name,year,mpaa,type)
    else:
        mpaa = 0
    
    pw=''
    if mpaa >= PC_RATING and PC_ENABLE and ((int(now) < int(PC_WATERSHED) and int(now) > 6) or int(PC_WATERSHED) == 25):
        keyboard = xbmc.Keyboard(pw, 'Rating = ' + mpaaname + ': Enter your PIN/Password to play', True)
        keyboard.doModal()
        if keyboard.isConfirmed():
            pw = keyboard.getText()
        else:
            pw=''
    if int(now) >= int(PC_WATERSHED) or not(PC_ENABLE) or ((pw == PC_PASS) or mpaa < PC_RATING or (int(now) < 6 and int(PC_WATERSHED) != 25)):
        return "PC_PLAY"
    else:
        dialog.ok("HUB Parental Control", "You cannot play this video","PIN incorrect")
        return "PC_BLOCKED"
		
def parental_control(name,year,mpaa,type):
    mpaa = PC_DEFAULT
    if type == 'movies':
        if mpaa in mrating:
            mpaa = mpaa
        else:
            infoLabels = infoLabels = get_meta(name,'movie',year=year)
            if infoLabels['mpaa']=='' or infoLabels['mpaa']=='N/A':
                mpaa = 'Unrated'
            else:
                mpaa = infoLabels['mpaa']
    else:
        if mpaa in tvrating:
            mpaa = mpaa
        else:
            infoLabels = get_meta(name,'tvshow',year=None,season=None,episode=None,imdb=None)
            if infoLabels['mpaa']=='' or infoLabels['mpaa']=='N/A':
                mpaa = 'Unrated'
            else:
                mpaa = infoLabels['mpaa']
		
    dialog = xbmcgui.Dialog()
    if type == 'movies':
        rating_list = ["No thanks, I'll choose later", "G", "PG","PG-13", "R", "NC-17"]
        rating_list_return = ["No thanks, I'll choose later", "G", "PG","PG-13", "R", "NC-17"]
    else:
        rating_list = ["No thanks, I'll choose later", "TV-Y", "TV-Y7-FG", "TV-PG", "TV-14", "TV-MA"]
        rating_list_return = ["No thanks, I'll choose later", "TV-Y", "TV-Y7-FG", "TV-PG", "TV-14", "TV-MA"]
    if mpaa=="Unrated" or mpaa=="":
        mpaa = PC_DEFAULT
        if os.path.isfile(CUSTOM_PC):
            s = read_from_file(CUSTOM_PC)
            if name in s:
                search_list = s.split('\n')
                for list in search_list:
                    if list != '':
                        list1 = list.split('<>')
                        title = list1[0]
                        cmpaa = list1[1]
                        vtype = list1[2]
                        if title==name and vtype==type:
                            mpaa=cmpaa
            else:		
                rating_id = dialog.select("No rating found....set/save your own?", rating_list)
                if(rating_id <= 0):
                    mpaa = PC_DEFAULT
                else:
                    pw=''
                    keyboard = xbmc.Keyboard(pw, 'Enter your PIN/Password to save the rating', True)
                    keyboard.doModal()
                    if keyboard.isConfirmed():
                        pw = keyboard.getText()
                    else:
                        pw=''
                    if pw == PC_PASS:
                        mpaa = rating_list_return[rating_id]
                        content = '%s<>%s<>%s' % (name, mpaa,type)
                        add_to_list(content,CUSTOM_PC)
                    else:
                        mpaa = PC_DEFAULT


    if mpaa == "PLAY":
        mpaa_n = 0
    elif mpaa == "G" or mpaa == "TV-Y" or mpaa == "TV-Y7-FG":
        mpaa_n = 1
    elif mpaa == "PG" or mpaa == "TV-PG":
        mpaa_n = 2
    elif mpaa == "PG-13" or mpaa == "TV-14":
        mpaa_n = 3
    elif mpaa == "R" or mpaa == "NC-17" or mpaa == "TV-MA" or mpaa == "REQUIRE PIN":
        mpaa_n = 4
    return mpaa_n,mpaa			


def get_meta(name,types=None,year=None,season=None,episode=None,imdb=None,episode_title=None):
    if 'movie' in types:
        meta = metainfo.get_meta('movie',name,year)
        infoLabels = {'rating': meta['rating'],'genre': meta['genre'],'mpaa': meta['mpaa'],'plot': meta['plot'],'title': meta['title'],'cover_url': meta['cover_url'],'fanart': meta['backdrop_url'],'Aired': meta['premiered'],'year': meta['year']}
    else:
        meta = metainfo.get_meta('tvshow',name,'','','')
        infoLabels = {'rating': meta['rating'],'genre': meta['genre'],'mpaa': meta['mpaa'],'plot': meta['plot'],'title': meta['title'],'cover_url': meta['cover_url'],'fanart': meta['backdrop_url'],'Episode': meta['episode'],'Aired': meta['premiered'],'Playcount': meta['playcount'],'Overlay': meta['overlay'],'year': meta['year']}
    return infoLabels


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


def strip_text(r, f, t, excluding=True):
    r = re.search("(?i)" + f + "([\S\s]+?)" + t, r).group(1)
    return r


def find_list(query, search_file):
    print query
    try:
        content = read_from_file(search_file) 
        lines = content.split('\n')
        for l in lines:
            print l
        index = lines.index(query)
        return index
    except:
        return -1
		
def add_to_list(list, file):
    list = list.replace('artist_pl', 'favartist_pl')
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
		
def write_to_file(path, content, append=False, silent=False):
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



