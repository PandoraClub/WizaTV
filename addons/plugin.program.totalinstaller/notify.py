import urllib, urllib2, re, xbmcplugin, xbmcgui, xbmc, xbmcaddon, os, sys, time
#################################################
AddonID        = 'plugin.program.totalinstaller'
#################################################
dialog         =  xbmcgui.Dialog()
ADDON          =  xbmcaddon.Addon(id=AddonID)
ADDONDATA      =  xbmc.translatePath(os.path.join('special://home/userdata/addon_data',''))
TBSXML         =  xbmc.translatePath(os.path.join('special://home/addons/plugin.program.totalinstaller/addon.xml'))
sellername     =  'noobalerts'
mynotifycheck  =  ADDON.getSetting('mynotifycheck')
cbnotifycheck  =  ADDON.getSetting('cbnotifycheck')
idfile         =  xbmc.translatePath(os.path.join(ADDONDATA,AddonID,'id.xml'))
TBSDATA        =  xbmc.translatePath(os.path.join(ADDONDATA,AddonID,''))

def Notify_Check():
    print"#### noobsandnerds - CHECK FOR NOTIFICATION ###"
    isplaying = xbmc.Player().isPlaying()
    
    if isplaying == 0:
        tbsnotifypath = xbmc.translatePath(os.path.join(ADDONDATA,AddonID,'tbsnotification.txt'))
        
        if not os.path.exists(tbsnotifypath):
            localfile = open(tbsnotifypath, mode='w')
            localfile.write('20150101000000')
            localfile.close()
        
        olddate=open(tbsnotifypath, 'r').read()
        BaseURL='http://noobsandnerds.com/totalrevolution/Community_Builds/notify?reseller=%s' % (sellername)
        link = Open_URL(BaseURL).replace('\n','').replace('\r','')
        notifymatch = re.compile('notify="(.+?)"').findall(link)
        notifymsg = notifymatch[0] if (len(notifymatch) > 0) else 'No news items available'
        datematch = re.compile('date="(.+?)"').findall(link)
        datecheck = datematch[0] if (len(datematch) > 0) else '10000000000000'
        cleantime = datecheck.replace('-','').replace(' ','').replace(':','')
        
        if int(olddate)<int(cleantime):
            print"New Notification Message from "+sellername
            dialog.ok('Update From noobsandnerds',notifymsg)
            localfile = open(tbsnotifypath, mode='w')
            localfile.write(cleantime)
            localfile.close()
        
        else:
            pass

def CB_Notify_Check():
    print"#### CHECK FOR CB Notification ###"
    
    if localidcheck != 'None':
        isplaying = xbmc.Player().isPlaying()
        if isplaying == 0:
            notifypath = xbmc.translatePath(os.path.join(ADDONDATA,AddonID,'notification.txt'))
            
            if not os.path.exists(notifypath):
                localfile = open(notifypath, mode='w')
                localfile.write('20150101000000')
                localfile.close()
            
            olddate=open(notifypath, 'r').read()
            BaseURL='http://noobsandnerds.com/totalrevolution/Community_Builds/notify?id=%s' % (localidcheck)
            link = Open_URL(BaseURL).replace('\n','').replace('\r','')
            notifymatch = re.compile('notify="(.+?)"').findall(link)
            notifymsg = notifymatch[0] if (len(notifymatch) > 0) else 'No news items available'
            datematch = re.compile('date="(.+?)"').findall(link)
            datecheck = datematch[0] if (len(datematch) > 0) else '10000000000000'
            cleantime = datecheck.replace('-','').replace(' ','').replace(':','')
            
            if int(olddate)<int(cleantime):
                print"New CB Notification Message"
                dialog.ok('Community Build Update',notifymsg)
                localfile = open(notifypath, mode='w')
                localfile.write(cleantime)
                localfile.close()
            
            else:
                pass


def Open_URL(url):
    req      = urllib2.Request(url)
    req.add_header('User-Agent','Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
    response = urllib2.urlopen(req)
    link     = response.read()
    response.close()
    return link.replace('\r','').replace('\n','').replace('\t','')

if not os.path.exists(idfile):
    localfile = open(idfile, mode='w+')
    localfile.write('id="None"\nname="None"')
    localfile.close()

localfile     = open(idfile, mode='r')
content       = file.read(localfile)
file.close(localfile)
localidmatch  = re.compile('id="(.+?)"').findall(content)
localidcheck  = localidmatch[0] if (len(localidmatch) > 0) else 'None'
    
try:
    if mynotifycheck == 'true':
        Notify_Check()
    
    if cbnotifycheck == 'true':
        CB_Notify_Check()

except:
    pass