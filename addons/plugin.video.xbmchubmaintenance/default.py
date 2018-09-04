import xbmc,xbmcaddon,xbmcgui,xbmcplugin,os,sys,re,urllib2,urllib,shutil,upload,time,extract,datetime,settings,advancedsettings
from addon.common.addon import Addon
from addon.common.net import Net
from metahandler import metahandlers
metainfo=metahandlers.MetaData()
AddonTitle="Maintenance Tool"
addon_id='plugin.video.xbmchubmaintenance'
local=xbmcaddon.Addon(id=addon_id); maintenancepath=xbmc.translatePath(local.getAddonInfo('path'))
addon=Addon(addon_id,sys.argv); net=Net(); datapath=addon.get_profile()
art=maintenancepath+'/art'
TribecaUrl='http://tribeca.tvaddons.ag/'; #TribecaUrl='http://tribeca.xbmchub.com/'
mainurl=TribecaUrl+'tools/maintenance'
def getArt(n): 
    if os.path.isfile(os.path.join(art,n))==True: return os.path.join(art,n)
    else: return mainurl+'/thumbs/'+n
howtourl=TribecaUrl+'tools/maintenance/videos/master.xml'
defaulticon=xbmc.translatePath(os.path.join(maintenancepath,'icon.png')); #defaulticon=xbmc.translatePath(os.path.join(art,'icon.jpg')); 
defaultfanart=xbmc.translatePath(os.path.join(maintenancepath,'fanart.jpg')); #defaultfanart=os.path.join(art,'fanart.jpg')
wallpaperurl='http://wallpaperswide.com'
wallpaperurl2='http://www.freewallpapers.to'
wallpaperurl3='http://www.fwallpaper.net'
xbmcdownload='http://mirrors.xbmc.org/'
feed=TribecaUrl+'tools/maintenance/news/feed.txt'
PC_ENABLE=settings.pc_enable_pc(); PC_WATERSHED=settings.pc_watershed_pc(); PC_RATING=settings.pc_pw_required_at(); PC_PASS=settings.pc_pass(); PC_DEFAULT=settings.pc_default(); PC_TOGGLE=settings.pc_enable_pc_settings(); CUSTOM_PC=settings.pc_custom_pc_file()
restore_path=xbmc.translatePath(os.path.join('special://profile','addon_data'))
restorexbmc_path=xbmc.translatePath('special://profile')
backup_path=local.getSetting('backup_path')
def SetView(n=500): xbmc.executebuiltin("Container.SetViewMode("+str(n)+")")
def nolines(t):
	it=t.splitlines(); t=''
	for L in it: t=t+L
	t=((t.replace("\r","")).replace("\n","").replace("\a","").replace("\t","")); return t
def File_Open(path): #print 'File: '+path
	if os.path.isfile(path): file=open(path,'r'); contents=file.read(); file.close(); return contents ## File found. #print 'Found: '+path
	else: return '' ## File not found.
def CATEGORIES():
    count=int(local.getSetting('opened')); print count; add=count+1; addone=str(add); print add; local.setSetting('opened',addone); show=['1','10','20','30','40','50','60','70','80','90','100','110','120','130','140','150','160','170','180','190','200']
    if addone not in show: CATEGORIES2()
    else:
        dialog=xbmcgui.Dialog()
        if dialog.yesno(AddonTitle,"" , "This tool is intended for Advanced Users Only!",'Brought to you by WWW.TVADDONS.AG','Continue','Exit'): exit
        else: CATEGORIES2()
def CATEGORIES2():
    setfeed=int(local.getSetting('feed')); html=net.http_GET(feed).content; feedheader=re.compile('<FEED HEADER>(.+?)</FEED HEADER>').findall(html); feedheader=str(feedheader).strip('[]').strip("''"); print setfeed; feednumber=re.compile('<NEW FEED>(.+?)</NEW FEED>').findall(html); print feednumber; feedcount=str(feednumber).strip('[]').strip("''"); print feedcount
    if int(feedcount) > setfeed: local.setSetting('feed',feedcount); dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle, "[B][COLOR blue]THIS JUST IN[/COLOR] - %s[/B]" % feedheader,"Please make sure to check the News & Announcements.")
    addDir('News & Announcements',feed,57,getArt('imagebranding.jpg'),defaultfanart)
    addDir("How-To Videos",howtourl,1,getArt('howtovideos.jpg'),defaultfanart)
    addDir("General Maintenance",mainurl,7,getArt('generalmaintenance.jpg'),defaultfanart)
    addDir("System Tweaks",mainurl,10,getArt('systemtweaks.jpg'),defaultfanart)
    addDir('Factory Reset','url',25,getArt('freshstart.jpg'),defaultfanart)
    addDir('Config Wizard','url',28,getArt('configwizard.jpg'),defaultfanart)
    addDir('Addon Installer','url',22,getArt('addoninstaller.jpg'),defaultfanart)
    addDir('Fusion Installer','url',24,getArt('fusioninstaller.jpg'),defaultfanart)
    addDir('Log Uploader','url',23,getArt('xbmcloguploader.jpg'),defaultfanart)
    #addDir('XML Backup','Back Up',31,getArt('xmlbackup.jpg'),defaultfanart)
    addDir('Download XBMC',xbmcdownload,39,getArt('downloadxbmc.jpg'),defaultfanart)
    addDir("What's my IP?",'url',32,getArt('myip.jpg'),defaultfanart)
    addDir("Check XBMC Version",'url',38,getArt('xbmcversion.jpg'),defaultfanart)
    addDir("Parental Controls",'url',60,getArt('parentalcotnrols.jpg'),defaultfanart)
    addDir('Need Help?','url',27,getArt('needhelp.jpg'),defaultfanart)
def MAINTENANCE(url):
    addDir('Clear Cache','url',4,getArt('clearcache.jpg'),defaultfanart)
    addDir('Erase Logs','url',5,getArt('eraselogs.jpg'),defaultfanart)
    addDir('Purge Packages','url',6,getArt('purgepackages.jpg'),defaultfanart)
    addDir('Check Modules','script.',36,getArt('checkmodule.jpg'),defaultfanart)
    addDir('Addon Removal','plugin.',8,getArt('addonremoval.jpg'),defaultfanart)
    addDir('Repo Removal','repository.',8,getArt('reporemoval.jpg'),defaultfanart)
    addDir('Skin Removal','skin.',8,getArt('skinremoval.jpg'),defaultfanart)
    #addDir('Log Analyzer','url',58,getArt(''),defaultfanart)
def SYSTEMTWEAKS(url):
    addDir('Auto Start Addon','plugin.',29,getArt('autostart.jpg'),defaultfanart)
    if xbmc.getCondVisibility('system.platform.android'): addDir("Android External Player",mainurl+'/tweaks/playcore/playercorefactory.xml',26,getArt('androidexternalplayer.jpg'),defaultfanart)
    addDir('Wallpaper Downloads','url',11,getArt('wallaperdownoads.jpg'),defaultfanart)
    addDir('Set HomeScreen Shortcuts','url',20,getArt('homescreenshortcuts.jpg'),defaultfanart)
    addDir('Seven Menu Shortcuts Tweak','url',19,getArt('sevenicons.jpg'),defaultfanart)#MENU(name)
    addDir('Create/Edit Advanced Settings','url',59,getArt('tuxenxml.jpg'),defaultfanart)
    addDir('Enable Tuxen Advanced XML',mainurl+'/tweaks/advancedsettings/tuxen.xml',16,getArt('tuxenxml.jpg'),defaultfanart)
    addDir('Enable Zero Caching Settings',mainurl+'/tweaks/advancedsettings/0cache.xml',16,getArt('zerocache.jpg'),defaultfanart)
    addDir('Verify Advanced Settings','url',17,getArt('verifyadvancedsettings.jpg'),defaultfanart)
    addDir('Remove Advanced Settings',mainurl,18,getArt('removeadvancedsettings.jpg'),defaultfanart)
def WALLPAPERDIR():
    #SetView(500); 
    #addDir("wallpaperswide.com (Broken)",wallpaperurl,12,getArt('wallpapwerwide.png'),defaultfanart)
    #addDir("freewallpapers.to (error 22)",wallpaperurl2,12,getArt('freewallpapers.jpg'),defaultfanart)
    #addDir("fwallpaper.net",wallpaperurl3,12,getArt('fwallpaper.png'),defaultfanart)
    WALLPAPER("fwallpaper.net",wallpaperurl3,getArt('fwallpaper.jpg')) #usable to bypass this menu
def XMLBACKUP():
    addDir('Backup','url',33,getArt('xmlbackup.jpg'),defaultfanart)
    addDir('Restore','url',34,getArt('xmlrestore.jpg'),defaultfanart)
    addDir('Set backup path','url',35,getArt('setpath.jpg'),defaultfanart)
def XBMCVERSION(url): xbmc_version=xbmc.getInfoLabel("System.BuildVersion"); version=xbmc_version[:4]; print version; dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle, "Your version is: %s" % version)


################################
###       Clear Cache        ###
################################
  
def CLEARCACHE(url):
    print '###'+AddonTitle+' - CLEARING CACHE FILES###'
    xbmc_cache_path = os.path.join(xbmc.translatePath('special://home'), 'cache')
    if os.path.exists(xbmc_cache_path)==True:    
        for root, dirs, files in os.walk(xbmc_cache_path):
            file_count = 0
            file_count += len(files)
            if file_count > 0:
    
                dialog = xbmcgui.Dialog()
                if dialog.yesno("Delete XBMC Cache Files", str(file_count) + " files found", "Do you want to delete them?"):
                
                    for f in files:
                        try:
                            os.unlink(os.path.join(root, f))
                        except:
                            pass
                    for d in dirs:
                        try:
                            shutil.rmtree(os.path.join(root, d))
                        except:
                            pass
                        
            else:
                pass
    if xbmc.getCondVisibility('system.platform.ATV2'):
        atv2_cache_a = os.path.join('/private/var/mobile/Library/Caches/AppleTV/Video/', 'Other')
        
        for root, dirs, files in os.walk(atv2_cache_a):
            file_count = 0
            file_count += len(files)
        
            if file_count > 0:

                dialog = xbmcgui.Dialog()
                if dialog.yesno("Delete ATV2 Cache Files", str(file_count) + " files found in 'Other'", "Do you want to delete them?"):
                
                    for f in files:
                        os.unlink(os.path.join(root, f))
                    for d in dirs:
                        shutil.rmtree(os.path.join(root, d))
                        
            else:
                pass
        atv2_cache_b = os.path.join('/private/var/mobile/Library/Caches/AppleTV/Video/', 'LocalAndRental')
        
        for root, dirs, files in os.walk(atv2_cache_b):
            file_count = 0
            file_count += len(files)
        
            if file_count > 0:

                dialog = xbmcgui.Dialog()
                if dialog.yesno("Delete ATV2 Cache Files", str(file_count) + " files found in 'LocalAndRental'", "Do you want to delete them?"):
                
                    for f in files:
                        os.unlink(os.path.join(root, f))
                    for d in dirs:
                        shutil.rmtree(os.path.join(root, d))
                        
            else:
                pass
              # Set path to Cydia Archives cache files
                             

    # Set path to What th Furk cache files
    wtf_cache_path = os.path.join(xbmc.translatePath('special://profile/addon_data/plugin.video.whatthefurk/cache'), '')
    if os.path.exists(wtf_cache_path)==True:    
        for root, dirs, files in os.walk(wtf_cache_path):
            file_count = 0
            file_count += len(files)
        
        # Count files and give option to delete
            if file_count > 0:
    
                dialog = xbmcgui.Dialog()
                if dialog.yesno("Delete WTF Cache Files", str(file_count) + " files found", "Do you want to delete them?"):
                
                    for f in files:
                        os.unlink(os.path.join(root, f))
                    for d in dirs:
                        shutil.rmtree(os.path.join(root, d))
                        
            else:
                pass
                
                # Set path to 4oD cache files
    channel4_cache_path= os.path.join(xbmc.translatePath('special://profile/addon_data/plugin.video.4od/cache'), '')
    if os.path.exists(channel4_cache_path)==True:    
        for root, dirs, files in os.walk(channel4_cache_path):
            file_count = 0
            file_count += len(files)
        
        # Count files and give option to delete
            if file_count > 0:
    
                dialog = xbmcgui.Dialog()
                if dialog.yesno("Delete 4oD Cache Files", str(file_count) + " files found", "Do you want to delete them?"):
                
                    for f in files:
                        os.unlink(os.path.join(root, f))
                    for d in dirs:
                        shutil.rmtree(os.path.join(root, d))
                        
            else:
                pass
                
                # Set path to BBC iPlayer cache files
    iplayer_cache_path= os.path.join(xbmc.translatePath('special://profile/addon_data/plugin.video.iplayer/iplayer_http_cache'), '')
    if os.path.exists(iplayer_cache_path)==True:    
        for root, dirs, files in os.walk(iplayer_cache_path):
            file_count = 0
            file_count += len(files)
        
        # Count files and give option to delete
            if file_count > 0:
    
                dialog = xbmcgui.Dialog()
                if dialog.yesno("Delete BBC iPlayer Cache Files", str(file_count) + " files found", "Do you want to delete them?"):
                
                    for f in files:
                        os.unlink(os.path.join(root, f))
                    for d in dirs:
                        shutil.rmtree(os.path.join(root, d))
                        
            else:
                pass
                
                
                # Set path to Simple Downloader cache files
    downloader_cache_path = os.path.join(xbmc.translatePath('special://profile/addon_data/script.module.simple.downloader'), '')
    if os.path.exists(downloader_cache_path)==True:    
        for root, dirs, files in os.walk(downloader_cache_path):
            file_count = 0
            file_count += len(files)
        
        # Count files and give option to delete
            if file_count > 0:
    
                dialog = xbmcgui.Dialog()
                if dialog.yesno("Delete Simple Downloader Cache Files", str(file_count) + " files found", "Do you want to delete them?"):
                
                    for f in files:
                        os.unlink(os.path.join(root, f))
                    for d in dirs:
                        shutil.rmtree(os.path.join(root, d))
                        
            else:
                pass
                
                # Set path to ITV cache files
    itv_cache_path = os.path.join(xbmc.translatePath('special://profile/addon_data/plugin.video.itv/Images'), '')
    if os.path.exists(itv_cache_path)==True:    
        for root, dirs, files in os.walk(itv_cache_path):
            file_count = 0
            file_count += len(files)
        
        # Count files and give option to delete
            if file_count > 0:
    
                dialog = xbmcgui.Dialog()
                if dialog.yesno("Delete ITV Cache Files", str(file_count) + " files found", "Do you want to delete them?"):
                
                    for f in files:
                        os.unlink(os.path.join(root, f))
                    for d in dirs:
                        shutil.rmtree(os.path.join(root, d))
                        
            else:
                pass
    dialog = xbmcgui.Dialog()
    dialog.ok(AddonTitle, "       Done Clearing Cache files")
    
################################
###     End Clear Cache      ###
################################
    
    

################################
###        Erase Logs        ###
################################
    
def ERASELOGS(url):  
    print '###'+AddonTitle+' - DELETING CRASH LOGS###'
    dialog = xbmcgui.Dialog()
    if dialog.yesno("Delete Old Crash Logs", '', "Do you want to delete them?"):
        path=LOGLOCATION()
        for infile in glob.glob(os.path.join(path, 'xbmc_crashlog*.*')):
             File=infile
             print infile
             os.remove(infile)
             dialog = xbmcgui.Dialog()
             dialog.ok(AddonTitle, "Please Reboot XBMC To Take Effect !!")
    else:
            pass

def LOGLOCATION(): 
    versionNumber = int(xbmc.getInfoLabel("System.BuildVersion" )[0:2])
    if versionNumber < 12:
        if xbmc.getCondVisibility('system.platform.osx'):
            if xbmc.getCondVisibility('system.platform.atv2'):
                log_path = '/var/mobile/Library/Preferences'
            else:
                log_path = os.path.join(os.path.expanduser('~'), 'Library/Logs')
        elif xbmc.getCondVisibility('system.platform.ios'):
            log_path = '/var/mobile/Library/Preferences'
        elif xbmc.getCondVisibility('system.platform.windows'):
            log_path = xbmc.translatePath('special://home')
            log = os.path.join(log_path, 'xbmc.log')
        elif xbmc.getCondVisibility('system.platform.linux'):
            log_path = xbmc.translatePath('special://home/temp')
        else:
            log_path = xbmc.translatePath('special://logpath')
    elif versionNumber > 11:
        log_path = xbmc.translatePath('special://logpath')
        log = os.path.join(log_path, 'xbmc.log')
    return log_path

################################
###     End Erase Logs       ###
################################



################################
###     Purge Packages       ###
################################
    
def PURGEPACKAGES(url):
    print '###'+AddonTitle+' - DELETING PACKAGES###'
    packages_cache_path = xbmc.translatePath(os.path.join('special://home/addons/packages', ''))
    try:    
        for root, dirs, files in os.walk(packages_cache_path):
            file_count = 0
            file_count += len(files)
            
        # Count files and give option to delete
            if file_count > 0:
    
                dialog = xbmcgui.Dialog()
                if dialog.yesno("Delete Package Cache Files", str(file_count) + " files found", "Do you want to delete them?"):
                            
                    for f in files:
                        os.unlink(os.path.join(root, f))
                    for d in dirs:
                        shutil.rmtree(os.path.join(root, d))
                    dialog = xbmcgui.Dialog()
                    dialog.ok(AddonTitle, "       Deleting Packages all done")
                else:
                        pass
            else:
                dialog = xbmcgui.Dialog()
                dialog.ok(AddonTitle, "       No Packages to Purge")
    except: 
        dialog = xbmcgui.Dialog()
        dialog.ok(AddonTitle, "Error Deleting Packages please visit TVADDONS.AG forums")

################################
###    End Purge Packages    ###
################################
        
        

################################
###       Addon Removal      ###
################################
        
def FINDADDON(url,name):  
    print '###'+AddonTitle+' - ADDON REMOVAL###'
    pluginpath = xbmc.translatePath(os.path.join('special://home/addons',''))
    for file in os.listdir(pluginpath):
        if url in file:
                file = pluginpath+file; #print file; 
                if os.path.isfile(os.path.join(file,'addon.xml'))==True:
                    try:
                        html=nolines(File_Open(os.path.join(file,'addon.xml'))); #print str(len(html)); 
                        name=re.compile('<addon\s*.*?\s+name="(.+?)"\s*.*?>').findall(html)[0]
                    #print name
                    except:
                        try: name=re.compile("<addon\s*.*?\s+name='(.+?)'\s*.*?>").findall(html)
                        except: name=str(file).replace(pluginpath,'').replace('plugin.','').replace('audio.','').replace('video.','').replace('skin.','').replace('repository.','').replace('program.','').replace('image.','')
                else: name=str(file).replace(pluginpath,'').replace('plugin.','').replace('audio.','').replace('video.','').replace('skin.','').replace('repository.','').replace('program.','').replace('image.','')
                iconimage=(os.path.join(file,'icon.png'))
                fanart=(os.path.join(file,'fanart.jpg'))
                try: addDir(name,file,9,iconimage,fanart)
                except:
                    name=str(file).replace(pluginpath,'').replace('plugin.','').replace('audio.','').replace('video.','').replace('skin.','').replace('repository.','').replace('program.','').replace('image.','')
                    addDir(name,file,9,iconimage,fanart)

def REMOVEADDON(url):   
    dialog = xbmcgui.Dialog()
    if dialog.yesno(AddonTitle, '', "Do you want to Remove this Addon?"):
        for root, dirs, files in os.walk(url):
            for f in files:
                os.unlink(os.path.join(root, f))
            for d in dirs:
                shutil.rmtree(os.path.join(root, d))
        os.rmdir(url)
        xbmc.executebuiltin('Container.Refresh')

################################
###     End Addon Removal    ###
################################
        
        

################################
###         Wallpaper        ###
################################
        
def WALLPAPER(name,url,iconimage):
    html=net.http_GET(url).content
    if wallpaperurl in url:
        match=re.compile('<a href="(.+?)" title=".+?">(.+?)</a>  <small>(.+?)</small').findall(html)
        for link,name,amount in match: addDir(name+' '+amount,url+link,13,iconimage,defaultfanart)
    elif wallpaperurl2 in url:
        html=html.replace('</TD>','</TD\n>')
        match=re.compile('<TD><B>(.+?)\s*:\s*</B><BR><A HREF="(.+?)"><IMG class=thumbs SRC="(.+?)" TITLE=".+?"\s+BORDER=\d+ vspace=\d+ hspace=\d+></A>(?:<P>)?</TD').findall(html)
        for name,link,img in match: 
        	if (wallpaperurl2 not in link) and ('http://' not in link) and ('https://' not in link): link=wallpaperurl2+'/'+link
        	if (wallpaperurl2 not in img) and ('http://' not in img) and ('https://' not in img): img=wallpaperurl2+'/'+img
        	addDir(name+'',link,13,img,defaultfanart)
        addDir('Space',wallpaperurl2+'/space.htm',13,iconimage,defaultfanart)
        #SetView(50); 
    elif wallpaperurl3 in url:
        SetView(50); 
        try: html=html.split('<div class="stitle listicon">Categories</div>')[-1]
        except: pass
        try: html=html.split('<div class="stitle roleicon">New Wallpapers</div>')[0]
        except: pass
        html=html.replace('</li>','</li\n>')
        match=re.compile('<li><a href="(/.+?\.html)">\s*(.+?)\s*</a></li').findall(html)
        for link,name in match: 
        	if (wallpaperurl3 not in link) and ('http://' not in link) and ('https://' not in link): link=wallpaperurl3+''+link
        	addDir(name+'',link,13,iconimage,defaultfanart)
        SetView(50); 

def WALLPAPER2(name,url,iconimage): #mode:13
    html=net.http_GET(url).content
    if wallpaperurl3 in url:
        SetView(500); 
        html=nolines(html); html=html.replace('</div>','</div\n>'); 
        match=re.compile('<div class="ithumb">\s*<a href="(/.+?\.html)"><img src="(.+?)" alt="\s*(.+?)\s+Wallpapers" /></a></div').findall(html)
        for link,img,name in match:
            if (wallpaperurl3 not in img) and ('http://' not in img) and ('https://' not in img): img=wallpaperurl3+''+img
            if (not wallpaperurl3 in link) and (not 'http://' in link) and (not 'https://' in link): link=wallpaperurl3+''+link
            addDir(name+'',link,15,img,defaultfanart)
        try: html=html.split('<div class="wallnavi">')[-1]
        except: pass
        try: html=html.split('"rss"')[0]
        except: pass
        html=html.replace('</a>','</a\n>'); 
        match=re.compile('<a href="(.+?)">(\d+)(?:\.\.\.)?</a').findall(html)
        for link,name in match:
            if (not wallpaperurl3 in link) and (not 'http://' in link) and (not 'https://' in link): link=wallpaperurl3+'/'+link
            addDir('Page: '+name+'',link,13,iconimage,defaultfanart)
        SetView(500); 
    elif wallpaperurl2 in url:
        html=html.replace('</TD>','</TD\n>')
        #first round graphics
        match=re.compile('<TD><div id="fr"><IMG SRC="(.+?)" alt="(.+?)" WIDTH=\d+ HEIGHT=\d+ BORDER=\d+ vspace=\d+></div>(.+?)</TD').findall(html)
        for img,name,links in match:
            dotdot=False
            if ('../' in img): img=img.replace('../',''); dotdot=True
            if (wallpaperurl2 not in img) and ('http://' not in img) and ('https://' not in img): img=wallpaperurl2+'/'+img
            links=links.replace('</A>','</A\n>')
            match=re.compile('<A HREF="(.*?)">\s*(\d+x\d+)\s*</A').findall(links)
            for link,name2 in match:
                if (not wallpaperurl2 in link) and (not 'http://' in link) and (not 'https://' in link): link=wallpaperurl2+'/'+link
                addDir(name+'  ('+name2+')',url+link,13,img,defaultfanart)
        if len(match)==0:
            #second round graphics
            match=re.compile('<TD><div id="fr"><IMG SRC="(.+?)" WIDTH=\d+ HEIGHT=\d+ BORDER=\d+ vspace=\d+></div>(.+?)</TD').findall(html)
            for img,links in match:
                dotdot=False
                if ('../' in img): img=img.replace('../',''); dotdot=True
                if dotdot==True:
                        try: Folder=(url.replace(wallpaperurl2+'/','').split('/')[0])+'/'
                        except: Folder=''
                        img=Folder+img
                if (wallpaperurl2 not in img) and ('http://' not in img) and ('https://' not in img): img=wallpaperurl2+'/'+img
                links=links.replace('</A>','</A\n>')
                match=re.compile('<A HREF="((.*?\d+)\D*\.htm)">\s*(\d+x\d+)\s*</A').findall(links)
                for link,name,name2 in match:
                    if dotdot==True: link=Folder+link
                    if (not wallpaperurl2 in link) and (not 'http://' in link) and (not 'https://' in link): link=wallpaperurl2+'/'+link
                    addDir(name+'  ('+name2+')',url+link,13,img,defaultfanart)
        #first round pages
        match=re.compile('<TD><B>\s*(.+?)\s*:\s*</B><BR><A HREF="(.+?)"><div id="fr"><IMG SRC="(.+?)" (?:TITLE=".+?" )?WIDTH=\d+ HEIGHT=\d+ BORDER=\d+ vspace=\d+></div></A></TD').findall(html)
        for name,link,img in match:
            dotdot=False
            if ('../' in img): img=img.replace('../',''); dotdot=True
            if dotdot==True:
                        try: Folder=(url.replace(wallpaperurl2+'/','').split('/')[0])+'/'
                        except: Folder=''
                        img=Folder+img
                        link=Folder+link
            if (wallpaperurl2 not in link) and ('http://' not in link) and ('https://' not in link): link=wallpaperurl2+'/'+link
            if (wallpaperurl2 not in img) and ('http://' not in img) and ('https://' not in img): img=wallpaperurl2+'/'+img
            addDir(name+'',link,13,img,defaultfanart)
        
    else:
        match=re.compile('href="(.+?)" title=".+?"><p align="center">(.+?)</p>').findall(url)
        if not match: WALLPAPER3(name,url,iconimage)
        elif len(match)<2: addDir(name,url,14,iconimage,defaultfanart)
        else:
            for link,name in match: url=wallpaperurl+link; addDir(name,url,14,iconimage,defaultfanart)


def WALLPAPER3(name,url,iconimage):
        print url
        match=re.compile('href="(.+?)" title=".+?">\n.+?img src="(.+?)" alt="(.+?) HD Wide').findall(net.http_GET(url).content)
        nextpage=re.compile('<span class="selected">.+?</span><a href="(.+?)">').findall(net.http_GET(url).content)
        for link,iconimage,name in match:
                url = wallpaperurl+link
                addDir(name,url,15,iconimage,defaultfanart)
        if nextpage:
                        print nextpage
                        name= 'Next Page >>'
                        nexturl=nextpage[0]
                        print nextpage
                        nexturl=wallpaperurl+nexturl
                        print nexturl
                        addDir(name,nexturl,14,getArt('nextpage.jpg'),defaultfanart)

def WALLPAPERDOWNLOAD(name,url,iconimage):
    if local.getSetting('download-wallpaper')=='':
        dialog = xbmcgui.Dialog()
        dialog.ok(AddonTitle, "A New Window Will Now Open For You To Set A", "Download Path")
        local.openSettings()
    path = xbmc.translatePath(os.path.join(local.getSetting('download-wallpaper'),''))
    img=os.path.join(path, name+'.jpg')
    if wallpaperurl in url:
        html=net.http_GET(url).content
        match=re.compile('<a target="_self" href="(.+?)" title=".+?">1920x1080</a>').findall(html)
        url= url+match[0]; print url
        url= wallpaperurl+match[0]; print url
    elif wallpaperurl2 in url:
        return
    elif wallpaperurl3 in url:
        html=net.http_GET(url).content
        html=nolines(html); html=html.replace('</div>','</div\n>'); 
        match=re.compile('<div class="wallimg">\s*<a href="(/.+?\.html)" target="_blank"><img src=".+?" alt=".+?" /></a>\s*</div').findall(html)[0]
        if len(match)==0: return
        url=wallpaperurl3+''+match
        html=net.http_GET(url).content; html=html.replace('</a>','</a\n>'); 
        try: url=re.compile('<a href="(.+?)">Download</a').findall(html)[0]
        except: return
    ##
    try:
        DownloaderClass(url,img, True)
        print 'img = '+img
        APPLYWALPAPER(img)
    except Exception as e:
        try:
            os.remove(img)
        except:
            pass
        if str(e)=="Canceled": 
            pass

def DownloaderClass(url,dest, useReq = False):
    dp = xbmcgui.DialogProgress()
    dp.create(AddonTitle,"Downloading & Copying File",'')

    if useReq:
        import urllib2
        req = urllib2.Request(url)
        req.add_header('Referer', 'http://wallpaperswide.com/')
        f       = open(dest, mode='wb')
        resp    = urllib2.urlopen(req)
        content = int(resp.headers['Content-Length'])
        size    = content / 100
        total   = 0
        while True:
            if dp.iscanceled(): 
                raise Exception("Canceled")                
                dp.close()

            chunk = resp.read(size)
            if not chunk:            
                f.close()
                break

            f.write(chunk)
            total += len(chunk)
            percent = min(100 * total / content, 100)
            dp.update(percent)       
    else:
        urllib.urlretrieve(url,dest,lambda nb, bs, fs, url=url: _pbhook(nb,bs,fs,url,dp))

def _pbhook(numblocks, blocksize, filesize, url=None,dp=None):
    try:
        percent = min((numblocks*blocksize*100)/filesize, 100)
        dp.update(percent)
    except:
        percent = 100
        dp.update(percent)
    if dp.iscanceled(): 
        raise Exception("Canceled")
        dp.close()

def APPLYWALPAPER(img):
        xbmc.executebuiltin('Skin.SetString(CustomBackgroundPath,%s)' %img)
        xbmc.executebuiltin('Skin.SetBool(UseCustomBackground)')
        xbmc.executebuiltin("ReloadSkin()")
        #xbmc.executebuiltin(Skin.SetString(skin.confluence.UseCustomBackground,'true'))
        """
        gui_path = xbmc.translatePath(os.path.join('special://home', 'userdata'))
        gui = os.path.join(gui_path, 'guisettings.xml')
        guixml = open(gui, 'r').read()
        r='UseCustomBackground\">(.+?)</setting>'
        match=re.compile(r).findall(guixml)
        r2='CustomBackgroundPath\">(.+?)</setting>'
        match2=re.compile(r2).findall(guixml)
        enable_background = match
        background_url = match2
        print enable_background
        print background_url

        file_handle = open(gui, 'r')
        file_string = file_handle.read()
        file_handle.close()

        file_string = (re.sub('UseCustomBackground\">false</setting>', 'UseCustomBackground\">true</setting>', file_string))

        file_handle = open(gui, 'w')
        file_handle.write(file_string)
        file_handle.close()
        xbmc.executebuiltin("XBMC.UpdateLocalAddons()")
        xbmc.executebuiltin("ReloadSkin()")
        xbmc.executebuiltin('Container.Refresh')
        """

################################
###       End Wallpaper      ###
################################

        

################################
###       Advanced XML       ###
################################ 
    
def ADVANCEDXML(url,name):
    path = xbmc.translatePath(os.path.join('special://home/userdata',''))
    advance=os.path.join(path, 'advancedsettings.xml')
    dialog = xbmcgui.Dialog()
    bak=os.path.join(path, 'advancedsettings.xml.bak')
    if os.path.exists(bak)==False: 
        if dialog.yesno("Back Up Original", 'Have You Backed Up Your Original?','', "[B][COLOR red]     AS YOU CANNOT GO BACK !!![/B][/COLOR]"):
            print '###'+AddonTitle+' - ADVANCED XML###'
            path = xbmc.translatePath(os.path.join('special://home/userdata',''))
            advance=os.path.join(path, 'advancedsettings.xml')
            try:
                os.remove(advance)
                print '=== Maintenance Tool - REMOVING    '+str(advance)+'    ==='
            except:
                pass
            link=net.http_GET(url).content
            a = open(advance,"w") 
            a.write(link)
            a.close()
            print '=== Maintenance Tool - WRITING NEW    '+str(advance)+'    ==='
            dialog = xbmcgui.Dialog()
            dialog.ok(AddonTitle, "       Done Adding new Advanced XML")
    else: 
        print '###'+AddonTitle+' - ADVANCED XML###'
        path = xbmc.translatePath(os.path.join('special://home/userdata',''))
        advance=os.path.join(path, 'advancedsettings.xml')
        try:
            os.remove(advance)
            print '=== Maintenance Tool - REMOVING    '+str(advance)+'    ==='
        except:
            pass
        link=net.http_GET(url).content
        a = open(advance,"w") 
        a.write(link)
        a.close()
        print '=== Maintenance Tool - WRITING NEW    '+str(advance)+'    ==='
        dialog = xbmcgui.Dialog()
        dialog.ok(AddonTitle, "       Done Adding new Advanced XML")    
    
def CHECKADVANCEDXML(url,name):
    print '###'+AddonTitle+' - CHECK ADVANCE XML###'
    path = xbmc.translatePath(os.path.join('special://home/userdata',''))
    advance=os.path.join(path, 'advancedsettings.xml')
    try:
        a=open(advance).read()
        if 'zero' in a:
            name='Zero Caching'
        elif 'tuxen' in a:
            name='TUXENS'
    except:
        name="NO ADVANCED"
    dialog = xbmcgui.Dialog()
    dialog.ok(AddonTitle,"[COLOR yellow]YOU HAVE[/COLOR] "+ name+"[COLOR yellow] SETTINGS SETUP[/COLOR]")
       
def DELETEADVANCEDXML(url):
    print '###'+AddonTitle+' - DELETING ADVANCE XML###'
    path = xbmc.translatePath(os.path.join('special://home/userdata',''))
    advance=os.path.join(path, 'advancedsettings.xml')
    try:
        os.remove(advance)
        dialog = xbmcgui.Dialog()
        print '=== Maintenance Tool - DELETING    '+str(advance)+'    ==='
        dialog.ok(AddonTitle, "       Remove Advanced Settings Sucessfull")
    except:
        dialog = xbmcgui.Dialog()
        dialog.ok(AddonTitle, "       No Advanced Settings To Remove")

################################
###     End Advanced XML     ###
################################
        
    

################################
###       Add 7 Icons        ###
################################    
    
def ADD7ICONS(url):
        xbmc_version = xbmc.getInfoLabel( "System.BuildVersion" )
        version = xbmc_version[:2]
        print 'XBMC Version: '+version
        if '12' in version:
            url = mainurl+'/tweaks/esthetics/confluence_7.zip'; print 'Frodo Found'
        elif ('13' in version) or ('14' in version):
            url = mainurl+'/tweaks/esthetics/confluence_7_gotham.zip'; print 'Gotham or Higher Found'
        else:
            url = mainurl+'/tweaks/esthetics/confluence_7.zip'; print 'Earlier than Frodo Found'
        print url
        dialog = xbmcgui.Dialog()
        print '###'+AddonTitle+' - ADD 7 ICONS###'
        if dialog.yesno(AddonTitle,"" , "Add 7 Icons Or Restore To 5",'','Restore 5','Add 7'):
            path = xbmc.translatePath(os.path.join('special://home/addons','packages'))
            lib=os.path.join(path, 'confluence_7.zip')
            DownloaderClass(url,lib)
            time.sleep(3)
            addonfolder = xbmc.translatePath(os.path.join('special://home/addons',''))
            dp = xbmcgui.DialogProgress()
            dp.create(AddonTitle, "Extracting Zip Please Wait")
            extract.all(lib,addonfolder,dp)
            local.setSetting('7icon','false')
            xbmc.executebuiltin("XBMC.UpdateLocalAddons()")
            xbmc.executebuiltin("ReloadSkin()")
            if ('13' in version) or ('14' in version):
                    xbmc.executebuiltin("ActivateWindow(19)")
                    time.sleep(8)
                    dialog = xbmcgui.Dialog()
                    dialog.ok(AddonTitle, "Please make sure to select the new skin")
            else:
                    xbmc.executebuiltin('Container.Refresh')
                    time.sleep(8)
                    dialog = xbmcgui.Dialog()
                    dialog.ok(AddonTitle, "7 Icon Tweak added Succesfully")
        else:
                confluence = xbmc.translatePath(os.path.join('special://home/addons','skin.confluence'))
                if local.getSetting('7icon')=='false':
                    dialog = xbmcgui.Dialog()
                    dialog.ok(AddonTitle, "Run Me Again After Reboot To Completely Clean")
                    local.setSetting('7icon','true')
                else:
                    dialog=xbmcgui.Dialog()
                    dialog.ok(AddonTitle,"Great You Are Completely Clean Now")
                    local.setSetting('7icon','false')
                for root,dirs,files in os.walk(confluence):
                    try:
                        for f in files: os.unlink(os.path.join(root,f))
                        for d in dirs: shutil.rmtree(os.path.join(root,d))
                    except: pass
                try: os.rmdir(confluence)
                except: pass
################################
###     End Add 7 Icons      ###
################################

################################
###       Home Icons         ###
################################    
def SELECT(): dialog=xbmcgui.Dialog(); version_select=['Videos','Music','Program','Picture']; select=['Videos','Music','Programs','Pictures']; return version_select[xbmcgui.Dialog().select('Please Choose Your Build',select)]
def HOMEICONS(url):
    gui_path=xbmc.translatePath(os.path.join('special://home','userdata')); gui=os.path.join(gui_path,'guisettings.xml'); guixml=open(gui, 'r').read(); button=SELECT(); r="skin.confluence.Home%sButton(.+?)"%button; match=re.compile(r).findall(guixml)
    for number in match: addDir(button+' Button '+number,'url',21,'','')
def HOMEICONS2(name):
    print name
    if   'Video' in name:   r="Skin.SetAddon(Home%s,xbmc.addon.video,xbmc.addon.executable)"%name.replace(' ','')
    elif 'Music' in name:   r="Skin.SetAddon(Home%s,xbmc.addon.audio,xbmc.addon.executable)"%name.replace(' ','')
    elif 'Picture' in name: r="Skin.SetAddon(Home%s,xbmc.addon.image,xbmc.addon.executable)"%name.replace(' ','')
    elif 'Program' in name: r="Skin.SetAddon(Home%s,xbmc.addon.executable)"%name.replace(' ','')
    xbmc.executebuiltin(r); dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle,"Homescreen Shortcut Updated Successfully")
################################
###      End Home Icons      ###
################################

################################
###        Log Upload        ###
################################
def UPLOADLOG(url): 
    if local.getSetting('email')=='': dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle,"A New Window Will Now Open For You To Enter","Your Email Address For The Logfile To Be Emailed To"); local.openSettings()
    upload.LogUploader()
################################
###      End Log Upload      ###
################################

################################
###     Fusion Installer     ###
################################
def FUSIONINSTALLER(url):
    tagNAME='FUSION'; tagURL='http://fusion.tvaddons.ag'; 
    print '###'+AddonTitle+' - FUSION INSTALLER###'; path=os.path.join(xbmc.translatePath('special://home'),'userdata','sources.xml')
    if not os.path.exists(path): f=open(path,mode='w'); f.write('<sources><files><source><name>'+tagNAME+'</name><path pathversion="1">'+tagURL+'</path></source></files></sources>'); f.close(); dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle,"Reboot To Take Effect Then Come","Back Here To Install Your Plugins"); return
    f=open(path,mode='r'); str=f.read(); f.close()
    if tagURL in str:
        dialog=xbmcgui.Dialog()
        if dialog.yesno(AddonTitle,"Please Select Install From Zip Then ","Select "+tagNAME+" On The Right Hand Side"): xbmc.executebuiltin("XBMC.Container.Update(path,replace)"); xbmc.executebuiltin("XBMC.ActivateWindow(AddonBrowser)")
    if not tagURL in str:
        if '</files>' in str: str=str.replace('</files>','<source><name>'+tagNAME+'</name><path pathversion="1">'+tagURL+'</path></source></files>'); dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle,"Reboot To Take Effect Then Come","Back Here To Install Your Plugins"); f=open(path,mode='w'); f.write(str); f.close()
        else: str=str.replace('</sources>','<files><source><name>'+tagNAME+'</name><path pathversion="1">'+tagURL+'</path></source></files></sources>'); dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle,"Reboot To Take Effect Then Come","Back Here To Install Your Plugins"); f=open(path, mode='w'); f.write(str); f.close()
################################
###   End Fusion Installer   ###
################################

################################
###     Android Player       ###
################################
def ANDROIDPLAYER(url): 
    print '###'+AddonTitle+' - ANDROID ENABLE EXTERNAL PLAYER###'; path=xbmc.translatePath(os.path.join('special://home','userdata','')); playercore=os.path.join(path, 'playercorefactory.xml'); dialog=xbmcgui.Dialog()
    if dialog.yesno(AddonTitle,"","Android External Player",'','Disable','Enable'):
        try: os.remove(playercore); print '=== REMOVING    '+str(playercore)+'     ==='
        except: pass
        link=net.http_GET(url).content; a=open(playercore,"w"); a.write(link); a.close(); print '=== WRITING NEW    '+str(playercore)+'     ==='
        dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle, "Android External Player Enabled", "Please Enable Hardware Acceleration In Settings",'Please Reboot XBMC')
    else: ANDROIDPLAYEROFF(url)
def ANDROIDPLAYEROFF(url):
    print '###'+AddonTitle+' - ANDROID DISABLE EXTERNAL PLAYER###'; path=xbmc.translatePath(os.path.join('special://home','userdata','')); playercore=os.path.join(path,'playercorefactory.xml')
    try: os.remove(playercore); print '=== REMOVING    '+str(playercore)+'     ==='
    except: pass
    dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle,"Android External Player Disabled",'Please Reboot XBMC')
################################
###    End Android Player    ###
################################

################################
###       How to Videos      ###
################################
def HOWTOS(url,fanart):
    match=re.compile('<title>(.+?)</title>\r\n<link>(.+?)</link>\r\n<thumb>(.+?)</thumb>').findall(net.http_GET(url).content)
    for name,link,icon in match:
        if icon=='none': iconimage='http://www.tvaddons.ag/wp-content/uploads/2014/08/tvaddons_logo.png'
        else: iconimage=str(icon)
        fanart=str(fanart); addDir(name,link,2,iconimage,fanart)
def HOWTOVIDEOS(name,url,fanart):
    match=re.compile('<w:t>&lt;title&gt;(.+?);/title&gt;</w:t></w:r></w:p><w:p><w:pPr></w:pPr><w:r><w:rPr><w:rFonts w:ascii="Courier" w:h-ansi="Courier" w:cs="Courier"/><wx:font wx:val="Courier"/><w:sz w:val="26"/><w:sz-cs w:val="26"/></w:rPr><w:t>&lt;link&gt;(.+?);/link&gt;</w:t></w:r></w:p><w:p><w:pPr></w:pPr><w:r><w:rPr><w:rFonts w:ascii="Courier" w:h-ansi="Courier" w:cs="Courier"/><wx:font wx:val="Courier"/><w:sz w:val="26"/><w:sz-cs w:val="26"/></w:rPr><w:t>&lt;thumb&gt;</w:t></w:r></w:p><w:p><w:pPr></w:pPr><w:r><w:rPr><w:rFonts w:ascii="Courier" w:h-ansi="Courier" w:cs="Courier"/><wx:font wx:val="Courier"/><w:sz w:val="26"/><w:sz-cs w:val="26"/></w:rPr><w:t>(.+?)</w:t></w:r></w:p><w:p><w:pPr></w:pPr><w:r><w:rPr><w:rFonts w:ascii="Courier" w:h-ansi="Courier" w:cs="Courier"/><wx:font wx:val="Courier"/><w:sz w:val="26"/><w:sz-cs w:val="26"/></w:rPr><w:t>&lt;/thumb&gt;</w:t>').findall(net.http_GET(url).content)
    for name,link,icon in match:
        link=link.replace('http://www.youtube.com/embed?v=',''); link=link.replace('http://www.youtube.com/watch?v=',''); link=link.replace('&lt',''); name=name.replace('&lt','')
        if icon=='none': iconimage='http://i.ytimg.com/vi/%s/0.jpg' % link
        else: iconimage=str(icon)
        fanart=str(fanart); url='plugin://plugin.video.youtube/?path=root/video&action=play_video&videoid=%s' % link
        addDir(name,url,3,iconimage,fanart)        
def PLAY_STREAM(name,url,iconimage): liz=xbmcgui.ListItem(name,iconImage="DefaultVideo.png",thumbnailImage=iconimage); liz.setInfo(type="Video",infoLabels={"Title":name}); liz.setProperty("IsPlayable","true"); pl=xbmc.PlayList(xbmc.PLAYLIST_VIDEO); pl.clear(); pl.add(url,liz); xbmc.Player(xbmc.PLAYER_CORE_MPLAYER).play(pl)
################################
###    End How to Videos     ###
################################

################################
###        Auto Start        ###
################################
def AUTOSTART(url):
    print '###'+AddonTitle+' - AUTO START FAVORITE ADDON###'; path=xbmc.translatePath(os.path.join('special://home','userdata','')); advance=os.path.join(path,'autoexec.py'); dialog=xbmcgui.Dialog()
    if dialog.yesno(AddonTitle,"","Auto Start Your Favorite ADDON",'','Disable','Enable'):
        print '###'+AddonTitle+' - SELECT YOUR FAVORITE ADDON###'; pluginpath=xbmc.translatePath(os.path.join('special://home','addons','')); print 'Pluginpath = '+pluginpath
        for file in os.listdir(pluginpath):
            if url in file:
                file=pluginpath+file; name=str(file).replace(pluginpath,'').replace('plugin.','').replace('audio.','').replace('video.','').replace('skin.','').replace('repository.',''); name2=str(file).replace(pluginpath,''); print 'name2 ='+name2; print 'name ='+name
                iconimage=(os.path.join(file,'icon.png')); fanart=(os.path.join(file,'fanart.jpg'))
                addDir(name,name2,30,iconimage,fanart)
        else:
            print '###'+AddonTitle+' - AUTO START DISABLE###'
            try: os.remove(advance); print '=== REMOVING    '+str(advance)+'     ==='; dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle, "Auto Start For Your Favorite Addon Has Been Disabled",'Please Reboot XBMC'); xbmc.executebuiltin("Action(%s)" % "Back")
            except: dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle,"Auto Start has not been enabled yet"); pass
def AUTOSTARTADD(url,name):
    print '###'+AddonTitle+' - ADDING AUTO START ADDON###'; path=xbmc.translatePath(os.path.join('special://home','userdata','')); advance=os.path.join(path,'autoexec.py')
    try: os.remove(advance); print '=== REMOVING    '+str(advance)+'     ==='
    except: pass
    a=open(advance,"w"); a.write("import xbmc\nxbmc.executebuiltin('XBMC.RunAddon(%s)')\n" % url); a.close(); print '=== WRITING NEW    '+str(advance)+'     ==='
    dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle,"Auto Start For * %s * Has Been Enabled" % name,'Please Reboot XBMC')
################################
###      End Auto Start      ###
################################

################################
###      Addon Installer     ###
################################
def ADDONINSTALLER(url):
    pluginpath=os.path.exists(xbmc.translatePath(os.path.join('special://home','addons','plugin.program.addoninstaller')))
    if pluginpath: xbmc.executebuiltin("RunAddon(plugin.program.addoninstaller)")
    else:
        url=TribecaUrl+'tools/maintenance/tools/plugin.program.addoninstaller.zip'; path=xbmc.translatePath(os.path.join('special://home','addons','packages')); lib=os.path.join(path, 'plugin.program.addoninstaller.zip'); DownloaderClass(url,lib)
        time.sleep(3)
        addonfolder=xbmc.translatePath(os.path.join('special://home','addons','')); dp=xbmcgui.DialogProgress(); print '=== INSTALLING ADDON INSTALLER ==='; dp.create(AddonTitle,"Extracting Zip Please Wait")
        extract.all(lib,addonfolder,dp); xbmc.executebuiltin("XBMC.UpdateLocalAddons()"); xbmc.executebuiltin("RunAddon(plugin.program.addoninstaller)")
################################
###    End Addon Installer   ###
################################

################################
###      Config Wizard       ###
################################
def CONFIGWIZARD(url):
    pluginpath = os.path.exists(xbmc.translatePath(os.path.join('special://home','addons','plugin.video.hubwizard')))
    if pluginpath: xbmc.executebuiltin("RunAddon(plugin.video.hubwizard)")
    else:
        url=TribecaUrl+'tools/maintenance/tools/plugin.video.hubwizard.zip'; path=xbmc.translatePath(os.path.join('special://home','addons','packages')); lib=os.path.join(path,'plugin.program.hubwizard.zip'); DownloaderClass(url,lib)
        time.sleep(3)
        addonfolder=xbmc.translatePath(os.path.join('special://home','addons','')); dp=xbmcgui.DialogProgress(); print '=== INSTALLING CONFIG WIZARD ==='; dp.create(AddonTitle, "Extracting Zip Please Wait")
        extract.all(lib,addonfolder,dp); xbmc.executebuiltin("XBMC.UpdateLocalAddons()"); xbmc.executebuiltin("RunAddon(plugin.video.hubwizard)")
################################
###    End Config Wizard     ###
################################

################################
###      Factory Reset       ###
################################
def FACTORYRESET(url):
    pluginpath=os.path.exists(xbmc.translatePath(os.path.join('special://home','addons','plugin.video.freshstart')))
    if pluginpath: xbmc.executebuiltin("RunAddon(plugin.video.freshstart)")
    else:
        url=TribecaUrl+'tools/maintenance/tools/plugin.video.freshstart.zip'; path=xbmc.translatePath(os.path.join('special://home','addons','packages')); lib=os.path.join(path,'plugin.program.freshstart.zip'); DownloaderClass(url,lib)
        time.sleep(3)
        addonfolder=xbmc.translatePath(os.path.join('special://home','addons','')); dp=xbmcgui.DialogProgress(); print '=== INSTALLING Fresh Start ==='; dp.create(AddonTitle,"Extracting Zip Please Wait")
        extract.all(lib,addonfolder,dp); xbmc.executebuiltin("XBMC.UpdateLocalAddons()"); xbmc.executebuiltin("RunAddon(plugin.video.freshstart)")
################################
###    End Factory Reset     ###
################################

################################
###          My IP           ###
################################
def MYIP(url='http://www.iplocation.net/',inc=1):
    match=re.compile("<td width='80'>(.+?)</td><td>(.+?)</td><td>(.+?)</td><td>.+?</td><td>(.+?)</td>").findall(net.http_GET(url).content)
    for ip, region, country, isp in match:
        if inc <2: dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle,"[B][COLOR gold]Your IP Address is: [/COLOR][/B] %s" % ip, '[B][COLOR gold]Your IP is based in: [/COLOR][/B] %s' % country, '[B][COLOR gold]Your Service Provider is:[/COLOR][/B] %s' % isp)
        inc=inc+1
################################
###        End My IP         ###
################################
#for file in os.listdir(pluginpath):
#                        if url in file:
#                                file = pluginpath+file

################################
###        XML Backup        ###
################################
def backup_xml(url):
    print backup_path; print '###'+AddonTitle+' - XML BACKUP Starting###'
    if backup_path:
        print '###'+AddonTitle+' - XML BACKUP Path Found###' 
        for file in os.listdir(restorexbmc_path):
            if ".xml" in file: file=restorexbmc_path+file; print '###'+AddonTitle+' - XML BACKUP Backing up userdata/*.xml file###'; shutil.copy(file, backup_path)
        directories=os.listdir(restore_path)
        for d in directories:
            create_directory(backup_path,d); source=os.path.join(restore_path,d); destination=os.path.join(backup_path,d)
            for file in os.listdir(source):
                if "settings.xml" in file: file=os.path.join(xbmc.translatePath(source),file); print '###'+AddonTitle+' - XML BACKUP Backing up addon/settings.xml file###'; shutil.copy(file,destination)
            for file in os.listdir(source):
                if ".list" in file: file=os.path.join(xbmc.translatePath(source),file); shutil.copy(file,destination)
        print '###'+AddonTitle+' - XML BACKUP Succesfull###'		; dialog=xbmcgui.Dialog(); dialog.ok("XML Backup","All userdata/*.xml and addon 'settings.xml' files backed up")
    else: print '###'+AddonTitle+' - XML BACKUP Path Not Found##'; dialog=xbmcgui.Dialog(); dialog.ok("Maintenance Tool - XML Backup","You need to set a backup location in settings"); local.openSettings()
def restore_xml(url):
    if backup_path:
        print '###'+AddonTitle+' - XML BACKUP RESTORE Starting###'
        for file in os.listdir(backup_path):
            if ".xml" in file: file=backup_path+file; print file; print '###'+AddonTitle+' - XML BACKUP RESTORE Restoring userdata/*.xml file###'; shutil.copy(file,restorexbmc_path)
        directories=os.listdir(backup_path); print 'Directory '+str(directories)
        for d in directories:
            if (os.path.isfile(os.path.join(backup_path,d))==False) and (os.path.exists(os.path.join(backup_path,d))==True):
                source=os.path.join(backup_path,d); destination=os.path.join(restore_path,d); print 'Source directory '+source
                if (os.path.exists(destination)==False):
                    try: os.makedirs(destination); print 'folder made: '+destination
                    except: print 'unable to make folder: '+destination
                for file in os.listdir(source):
                    if ("settings.xml" in file) or (".list" in file):
                        try:    print file; print '###'+AddonTitle+' - XML BACKUP RESTORE Restoring userdata/addon_data/setttings.xml file###'; shutil.copy(os.path.join(source,file),destination)
                        except: print 'error copying '+file+' to '+destination
        dialog=xbmcgui.Dialog()
        if dialog.yesno("Maintenance Tool - XML Backup", "All userdata/*.xml and addon 'settings.xml' files restored", "Reboot to load restored gui settings settings", '', "Reboot Later", "Reboot Now"):
            if xbmc.getCondVisibility('system.platform.windows'): xbmc.executebuiltin('RestartApp')
            else: xbmc.executebuiltin('Reboot')
        else: pass
    else: print '###'+AddonTitle+' - XML BACKUP Path Not Found##'; dialog=xbmcgui.Dialog(); dialog.ok("XML Backup","You need to set a backup location in settings"); local.openSettings()
def create_directory(dir_path,dir_name=None):
    print '###'+AddonTitle+' - XML BACKUP Creating Backup Directories###' 
    if dir_name: dir_path=os.path.join(dir_path,dir_name)
    dir_path=dir_path.strip()
    if not os.path.exists(dir_path): os.makedirs(dir_path)
    return dir_path
################################
###      End XML Backup      ###
################################

################################
###         Modules          ###
################################
def MODULES(url):
    print '###'+AddonTitle+' - FIND INSTALLED MODULES###'; pluginpath=xbmc.translatePath(os.path.join('special://home','addons','')); modulepath=TribecaUrl+'tools/maintenance/modules/'; nameA=[]
    for file in os.listdir(pluginpath):
        if url in file:
            file=pluginpath+file; name2=str(file).replace(pluginpath,'');nameA.append(name2); iconimage=(os.path.join(file,'icon.png')); fanart=(os.path.join(file,'fanart.jpg'))
            addDir('[B][COLOR green] Installed [/COLOR][/B]'+name2,file,9,iconimage,fanart)
    match=re.compile("<li><a href=\"(.+?)\">(.+?)</a></li>").findall(net.http_GET(modulepath).content)
    for url,name in match:
        nono=['ParentDirectory','plugin.video.youtube','script.module.elementtree-1.2.7','script.module.metahandler-2.4.0']; name=name.replace(' ',''); url=modulepath+name; print url; name=name.replace('.zip',''); print name
        if name not in nono:
            if name not in nameA: addDir('[B][COLOR red] Not Installed [/COLOR][/B]'+name,url,37,'','')
def INSTALLMODULE(url):
    dialog = xbmcgui.Dialog()
    if dialog.yesno(AddonTitle, '', "Do you want to Install this Addon?"):
        path=xbmc.translatePath(os.path.join('special://home','addons','packages')); name=url.replace(TribecaUrl+'tools/maintenance/modules/',''); lib=os.path.join(path,name); DownloaderClass(url,lib)
        time.sleep(3)
        addonfolder=xbmc.translatePath(os.path.join('special://home','addons',''))
        dp=xbmcgui.DialogProgress(); print '=== INSTALLING MODULE ==='; dp.create(AddonTitle, "Extracting Zip Please Wait")
        extract.all(lib,addonfolder,dp); xbmc.executebuiltin("XBMC.UpdateLocalAddons()"); xbmc.executebuiltin('Container.Refresh')
    else: pass
################################
###       End Modules        ###
################################

################################
###      XBMC Download       ###
################################
def XBMCDOWNLOAD():
    if local.getSetting('xbmc_path')=='':
             dialog=xbmcgui.Dialog(); dialog.ok(AddonTitle, "A New Window Will Now Open For You To Enter", "A Path To Download XBMC Too."); local.openSettings()
    else:                   
             addDir('STABLE','http://mirrors.xbmc.org/releases/',40,getArt('stable.jpg'),defaultfanart)
             addDir('MONTHLY','http://mirrors.xbmc.org/snapshots/',45,getArt('monthly.jpg'),defaultfanart)
             addDir('NIGHTLIES','http://mirrors.xbmc.org/nightlies/',49,getArt('nightlies.jpg'),defaultfanart)
             addDir('TEST','http://mirrors.xbmc.org/test-builds/',53,getArt('test.jpg'),defaultfanart)
def XBMCDOWNLOAD2():
    addDir('Windows','http://mirrors.xbmc.org/releases/win32/',41,getArt('windows.jpg'),defaultfanart)
    addDir('Linux','http://mirrors.xbmc.org/releases/XBMCbuntu/',41,getArt('linux.jpg'),defaultfanart)
    addDir('OSX','http://mirrors.xbmc.org/releases/osx/',43,getArt('osx.jpg'),defaultfanart)
    addDir('Android','http://mirrors.xbmc.org/releases/android/',44,getArt('android.jpg'),defaultfanart)
def XBMCDOWNLOAD4(url):
    addDir('x86_64','http://mirrors.xbmc.org/releases/osx/x86_64/',41,getArt('osx.jpg'),defaultfanart)
    addDir('i386','http://mirrors.xbmc.org/releases/osx/i386/',41,getArt('osx.jpg'),defaultfanart)
    addDir('ppc','http://mirrors.xbmc.org/releases/osx/ppc/',41,getArt('osx.jpg'),defaultfanart)
    url1=url; match=re.compile('<img src=".+?" alt=".+?" width="16" height="16"> <a href="(.+?)">(.+?)</a> (.+?) <a').findall(net.http_GET(url).content)
    for url,name,info in match:
        nono=['Name']; print name
        if name not in nono: print url1+url; addDir('%s  :  %s' %(name,info),url1+url,42,'','')
def XBMCDOWNLOAD5(url):
    addDir('x86','http://mirrors.xbmc.org/releases/android/x86/',41,getArt('android.jpg'),defaultfanart)
    addDir('arm','http://mirrors.xbmc.org/releases/android/arm/',41,getArt('android.jpg'),defaultfanart)
    url1=url; match=re.compile('<img src=".+?" alt=".+?" width="16" height="16"> <a href="(.+?)">(.+?)</a> (.+?) <a').findall(net.http_GET(url).content)
    for url,name,info in match:
        nono=['Name']; print name
        if name not in nono: print url1+url; addDir('%s  :  %s' %(name,info),url1+url,42,'','')
def XBMCDOWNLOAD6(url):
    addDir('Windows','http://mirrors.xbmc.org/snapshots/win32/',41,getArt('windows.jpg'),defaultfanart)
    addDir('OSX','http://mirrors.xbmc.org/snapshots/osx/',46,getArt('osx.jpg'),defaultfanart)
    addDir('Android','http://mirrors.xbmc.org/snapshots/android/',47,getArt('android.jpg'),defaultfanart)
    addDir('Darwin','http://mirrors.xbmc.org/snapshots/darwin/',48,getArt('darwin.jpg'),defaultfanart)
def XBMCDOWNLOAD7(url):
    addDir('x86_64','http://mirrors.xbmc.org/snapshots/osx/x86_64/',41,getArt('osx.jpg'),defaultfanart)
    addDir('i386','http://mirrors.xbmc.org/snapshots/osx/i386/',41,getArt('osx.jpg'),defaultfanart)
def XBMCDOWNLOAD8(url):
    addDir('x86','http://mirrors.xbmc.org/snapshots/android/x86/',41,getArt('android.jpg'),defaultfanart)
    addDir('arm','http://mirrors.xbmc.org/snapshots/android/arm/',41,getArt('android.jpg'),defaultfanart)
def XBMCDOWNLOAD9(url):
    addDir('ios','http://mirrors.xbmc.org/snapshots/darwin/ios/',41,getArt('darwin.jpg'),defaultfanart)
    addDir('atv2','http://mirrors.xbmc.org/snapshots/darwin/atv2/',41,getArt('darwin.jpg'),defaultfanart)
def XBMCDOWNLOAD10(url):
    addDir('Windows','http://mirrors.xbmc.org/nightlies/win32/',41,getArt('windows.jpg'),defaultfanart)
    addDir('OSX','http://mirrors.xbmc.org/nightlies/osx/',50,getArt('osx.jpg'),defaultfanart)
    addDir('Android','http://mirrors.xbmc.org/nightlies/android/',51,getArt('android.jpg'),defaultfanart)
    addDir('Darwin','http://mirrors.xbmc.org/nightlies/darwin/',52,getArt('darwin.jpg'),defaultfanart)
def XBMCDOWNLOAD11(url):
    addDir('x86_64','http://mirrors.xbmc.org/nightlies/osx/x86_64/',41,getArt('osx.jpg'),defaultfanart)
    addDir('i386','http://mirrors.xbmc.org/nightlies/osx/i386/',41,getArt('osx.jpg'),defaultfanart)
def XBMCDOWNLOAD12(url):
    addDir('x86','http://mirrors.xbmc.org/nightlies/android/x86/',41,getArt('android.jpg'),defaultfanart)
    addDir('arm','http://mirrors.xbmc.org/nightlies/android/arm/',41,getArt('android.jpg'),defaultfanart)
def XBMCDOWNLOAD13(url):
    addDir('ios','http://mirrors.xbmc.org/nightlies/darwin/ios/',41,getArt('darwin.jpg'),defaultfanart)
    addDir('atv2','http://mirrors.xbmc.org/nightlies/darwin/atv2/',41,getArt('darwin.jpg'),defaultfanart)
def XBMCDOWNLOAD14(url):
    addDir('Windows','http://mirrors.xbmc.org/test-builds/win32/',41,getArt('windows.jpg'),defaultfanart)
    addDir('OSX','http://mirrors.xbmc.org/test-builds/osx/',54,getArt('osx.jpg'),defaultfanart)
    addDir('Android','http://mirrors.xbmc.org/test-builds/android/',55,getArt('android.jpg'),defaultfanart)
    addDir('Darwin','http://mirrors.xbmc.org/test-builds/darwin/',56,getArt('darwin.jpg'),defaultfanart)
def XBMCDOWNLOAD15(url):
    addDir('i386','http://mirrors.xbmc.org/test-builds/osx/i386/',41,getArt('osx.jpg'),defaultfanart)
    addDir('x86_64','http://mirrors.xbmc.org/test-builds/osx/x86_64/',41,getArt('osx.jpg'),defaultfanart)
def XBMCDOWNLOAD16(url):
    addDir('arm','http://mirrors.xbmc.org/test-builds/android/arm/',41,getArt('android.jpg'),defaultfanart)
    addDir('x86','http://mirrors.xbmc.org/test-builds/android/x86/',41,getArt('android.jpg'),defaultfanart)
def XBMCDOWNLOAD17(url):
    addDir('ios','http://mirrors.xbmc.org/test-builds/darwin/ios/',41,getArt('darwin.jpg'),defaultfanart)
    addDir('atv2','http://mirrors.xbmc.org/test-builds/darwin/atv2/',41,getArt('darwin.jpg'),defaultfanart)
def XBMCDOWNLOAD3(url):
    print url; url1=url; match=re.compile('<img src=".+?" alt=".+?" width="16" height="16"> <a href="(.+?)">(.+?)</a> (.+?) <a').findall(net.http_GET(url).content)
    for url,name,info in match:
        nono=['Name']; print name
        if name not in nono: print url1+url; addDir('%s  :  %s' %(name,info),url1+url+'@'+url1,42,'','')
def DownloaderClass2(url):
    url1=url.split('@')[1]; url=url.split('@')[0]; name=url.replace(url1,''); useReq=False; path=xbmc.translatePath(os.path.join(local.getSetting('xbmc_path'),'')); dest=os.path.join(path,name)
    dp=xbmcgui.DialogProgress(); dp.create(AddonTitle,"Downloading & Copying File",'')
    if useReq:
        import urllib2; req=urllib2.Request(url); req.add_header('Referer', 'http://mirrors.xbmc.org/'); f=open(dest, mode='wb'); resp=urllib2.urlopen(req); content=int(resp.headers['Content-Length']); size=content / 100; total=0
        while True:
            if dp.iscanceled(): raise Exception("Canceled"); dp.close()
            chunk=resp.read(size)
            if not chunk: f.close(); break
            f.write(chunk); total+=len(chunk); percent=min(100 * total / content,100); dp.update(percent)       
    else: urllib.urlretrieve(url,dest,lambda nb, bs, fs, url=url: _pbhook2(nb,bs,fs,url,dp))
def _pbhook2(numblocks,blocksize,filesize,url=None,dp=None):
    try: percent=min((numblocks*blocksize*100)/filesize,100); dp.update(percent)
    except: percent=100; dp.update(percent)
    if dp.iscanceled():  dp.close(); pass
################################
###    End XBMC Download     ###
################################

################################
###   News & Announcements   ###
################################
def NEWSANNOUNCEMENTS(url):
    match=re.compile('<FEED>(.+?)</FEED>').findall(net.http_GET(url).content)
    for message in match: addDir(message,'url','',getArt('imagebranding.jpg'),defaultfanart)
################################
### End News & Announcements ###
################################

################################
###      Log Analyzer        ###
################################
def LOGANALYZER(url):
    log_path=xbmc.translatePath(os.path.join('special://home')); log=os.path.join(log_path,'xbmc.log'); logread=open(log,'r').read(); r='###(.+?)###'; match=re.compile(r).findall(logread); r2=''; match2=re.compile(r2).findall(logread); findprint=str(match); finderror=match2
    if findprint: addDir(findprint,'url','',getArt('imagebranding.jpg'),defaultfanart)
################################
###     End Log Analyzer     ###
################################

################################
###       Help Pop Up        ###
################################
class HUB(xbmcgui.WindowXMLDialog):
    def __init__(self,*args,**kwargs): self.shut=kwargs['close_time']; xbmc.executebuiltin("Skin.Reset(AnimeWindowXMLDialogClose)"); xbmc.executebuiltin("Skin.SetBool(AnimeWindowXMLDialogClose)")
    def onFocus(self,controlID): pass
    def onClick(self,controlID): 
        if controlID==12: xbmc.Player().stop(); self._close_dialog()
    def onAction(self,action):
        if action in [5,6,7,9,10,92,117] or action.getButtonCode() in [275,257,261]: xbmc.Player().stop(); self._close_dialog()
    def _close_dialog(self):
        xbmc.executebuiltin("Skin.Reset(AnimeWindowXMLDialogClose)"); time.sleep( .4 ); self.close()
def pop():
    if xbmc.getCondVisibility('system.platform.ios'):
      if not xbmc.getCondVisibility('system.platform.atv'):
         popup=HUB('hub1.xml',local.getAddonInfo('path'),'DefaultSkin',close_time=34,logo_path='%s/resources/skins/DefaultSkin/media/Logo/'%local.getAddonInfo('path'))
    elif xbmc.getCondVisibility('system.platform.android'):
         popup=HUB('hub1.xml',local.getAddonInfo('path'),'DefaultSkin',close_time=34,logo_path='%s/resources/skins/DefaultSkin/media/Logo/'%local.getAddonInfo('path'))
    else: popup=HUB('hub.xml',local.getAddonInfo('path'),'DefaultSkin',close_time=34,logo_path='%s/resources/skins/DefaultSkin/media/Logo/'%local.getAddonInfo('path'))
    popup.doModal()
    del popup
################################
###     End Help Pop Up      ###
################################

################################
###     PARENTAL CONTROLS    ###
################################
#Check if HUB parental Control module is installed, install if it doesn't exist
pc_archive=xbmc.translatePath(os.path.join('special://home','addons',addon_id,'resources','help','script.module.hubparentalcontrol.zip'))
pc_addonfolder=xbmc.translatePath(os.path.join('special://home','addons',''))
if not os.path.exists(xbmc.translatePath(os.path.join('special://home','addons','script.module.hubparentalcontrol'))): extract.all(pc_archive,pc_addonfolder); xbmc.executebuiltin("XBMC.UpdateLocalAddons()")
def parental_controls(name):
    addDir("Settings",'url',63,getArt('howtovideos.jpg'),defaultfanart)
    addDir("Help",'url',61,getArt('howtovideos.jpg'),defaultfanart)
    if PC_TOGGLE=='LOCKED': addDir('[COLOR green]'+'UNLOCK Parental Control Settings'+'[/COLOR]','url',62,getArt('howtovideos.jpg'),defaultfanart)
    else: addDir('[COLOR red]'+'LOCK Parental Control Settings'+'[/COLOR]','url',62,getArt('howtovideos.jpg'),defaultfanart)
def pc_setting(pw=""): #Lock or unlock Parental Control Settings		
    dialog=xbmcgui.Dialog(); keyboard=xbmc.Keyboard(pw,'Enter your PIN/Password',True); keyboard.doModal()
    if keyboard.isConfirmed():
        pw=keyboard.getText()
        if pw==PC_PASS:
            if PC_TOGGLE=="UNLOCKED": local.setSetting('pc_enable_pc_settings',value='LOCKED'); xbmc.executebuiltin("Container.Refresh")
            else: local.setSetting('pc_enable_pc_settings',value='UNLOCKED'); xbmc.executebuiltin("Container.Refresh"); local.openSettings()				
        else: dialog.ok("HUB Parental Control","Incorrect PIN/Password")	
def parentalcontrol_help(text): header="[B][COLOR red]"+text+"[/B][/COLOR]"; msg=os.path.join(local.getAddonInfo('path'),'resources','help','parental_control.txt'); TextBoxes(header,msg)
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
################################
###    END PARENTAL CONTROLS ###
################################		
def addDir(name,url,mode,iconimage,fanart):
    u=sys.argv[0]+"?url="+urllib.quote_plus(url)+"&mode="+str(mode)+"&name="+urllib.quote_plus(name)+"&iconimage="+urllib.quote_plus(iconimage)+"&fanart="+urllib.quote_plus(fanart); ok=True
    liz=xbmcgui.ListItem(name, iconImage="DefaultFolder.png", thumbnailImage=iconimage); liz.setInfo(type="Video",infoLabels={"Title":name,"Plot":name}); liz.setProperty("Fanart_Image",fanart)
    if mode==3 or mode==9 or mode==6 or mode==15 or mode==17 or mode==21 or mode==23 or mode==28 or mode==42 or mode==37 or mode==38 or mode==25 or mode==32 or mode==33 or mode==34 or mode==35 or mode==22 or mode==19 or mode==26 or mode==18 or mode==16 or mode==27 or mode==5 or mode==61 or mode==62 or mode==63:
          ok=xbmcplugin.addDirectoryItem(handle=int(sys.argv[1]),url=u,listitem=liz,isFolder=False)
    else: ok=xbmcplugin.addDirectoryItem(handle=int(sys.argv[1]),url=u,listitem=liz,isFolder=True)
    return ok
def get_params(param=[]):
    paramstring=sys.argv[2]
    if len(paramstring)>=2:
        params=sys.argv[2]; cleanedparams=params.replace('?','')
        if (params[len(params)-1]=='/'): params=params[0:len(params)-2]
        pairsofparams=cleanedparams.split('&'); param={}
        for i in range(len(pairsofparams)):
             splitparams={}; splitparams=pairsofparams[i].split('=')
             if (len(splitparams))==2: param[splitparams[0]]=splitparams[1]
    return param
params=get_params(); url=None; name=None; mode=None; iconimage=None; fanart=None
try:    url=urllib.unquote_plus(params["url"])
except: pass
try:    name=urllib.unquote_plus(params["name"])
except: pass
try:    iconimage=urllib.unquote_plus(params["iconimage"])
except: pass
try:    mode=int(params["mode"])
except: pass
try:    fanart=urllib.unquote_plus(params["fanart"])
except: pass
print "Mode: "+str(mode); print "URL: "+str(url); print "Name: "+str(name); print "IconImage: "+str(iconimage)
if mode==None or url==None or len(url)<1: CATEGORIES(); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==1: HOWTOS(url,fanart); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==2: HOWTOVIDEOS(name,url,fanart)
elif mode==3: PLAY_STREAM(name,url,iconimage)
elif mode==4: CLEARCACHE(url)
elif mode==5: ERASELOGS(url)
elif mode==6: PURGEPACKAGES(url)
elif mode==7: MAINTENANCE(url); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==8: FINDADDON(url,name)
elif mode==9: REMOVEADDON(url)
elif mode==10: SYSTEMTWEAKS(url); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==11: WALLPAPERDIR()
elif mode==12: WALLPAPER(name,url,iconimage)
elif mode==13: WALLPAPER2(name,url,iconimage)
elif mode==14: WALLPAPER3(name,url,iconimage); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==15: WALLPAPERDOWNLOAD(name,url,iconimage)
elif mode==16: ADVANCEDXML(url,name)
elif mode==17: CHECKADVANCEDXML(url,name)
elif mode==18: DELETEADVANCEDXML(url)
elif mode==19: ADD7ICONS(url)
elif mode==20: HOMEICONS(url)
elif mode==21: HOMEICONS2(name)
elif mode==22: ADDONINSTALLER(url)
elif mode==23: UPLOADLOG(url)
elif mode==24: FUSIONINSTALLER(url)
elif mode==25: FACTORYRESET(url)
elif mode==26: ANDROIDPLAYER(url)
elif mode==27: pop()
elif mode==28: CONFIGWIZARD(url)
elif mode==29: AUTOSTART(url); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==30: AUTOSTARTADD(url,name)
elif mode==31: XMLBACKUP(); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==32: MYIP()
elif mode==33: backup_xml(url)
elif mode==34: restore_xml(url)
elif mode==35: local.openSettings()
elif mode==36: MODULES(url); xbmc.executebuiltin("Container.SetViewMode(50)")
elif mode==37: INSTALLMODULE(url)
elif mode==38: XBMCVERSION(url)
elif mode==39: XBMCDOWNLOAD(); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==40: XBMCDOWNLOAD2(); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==41: XBMCDOWNLOAD3(url)
elif mode==42: DownloaderClass2(url)
elif mode==43: XBMCDOWNLOAD4(url)
elif mode==44: XBMCDOWNLOAD5(url)
elif mode==45: XBMCDOWNLOAD6(url); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==46: XBMCDOWNLOAD7(url)
elif mode==47: XBMCDOWNLOAD8(url)
elif mode==48: XBMCDOWNLOAD9(url)
elif mode==49: XBMCDOWNLOAD10(url); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==50: XBMCDOWNLOAD11(url)
elif mode==51: XBMCDOWNLOAD12(url)
elif mode==52: XBMCDOWNLOAD13(url)
elif mode==53: XBMCDOWNLOAD14(url); xbmc.executebuiltin("Container.SetViewMode(500)")
elif mode==54: XBMCDOWNLOAD15(url)
elif mode==55: XBMCDOWNLOAD16(url)
elif mode==56: XBMCDOWNLOAD17(url)
elif mode==57: NEWSANNOUNCEMENTS(url); xbmc.executebuiltin("Container.SetViewMode(51)")
elif mode==58: LOGANALYZER(url); xbmc.executebuiltin("Container.SetViewMode(51)")
elif mode==59: advancedsettings.MENU(name)
elif mode==60: parental_controls(name)
elif mode==61: parentalcontrol_help(name)
elif mode==62: pc_setting()
elif mode==63: local.openSettings()	
xbmcplugin.endOfDirectory(int(sys.argv[1]))
