import urllib, urllib2, re, xbmcplugin, xbmcgui, xbmc, xbmcaddon, os, sys, time
import shutil
import time

#################################################
AddonID        = 'plugin.program.totalinstaller'
#################################################
dialog         =  xbmcgui.Dialog()
dp             =  xbmcgui.DialogProgress()
ADDON          =  xbmcaddon.Addon(id=AddonID)
ADDONDATA      =  xbmc.translatePath(os.path.join('special://home','userdata','addon_data'))
clean_cache    =  ADDON.getSetting('cleancache')
internetcheck  =  ADDON.getSetting('internetcheck')
cbnotifycheck  =  ADDON.getSetting('cbnotifycheck')
mynotifycheck  =  ADDON.getSetting('mynotifycheck')
idfile         =  os.path.join(ADDONDATA,AddonID,'id.xml')
TBSDATA        =  os.path.join(ADDONDATA,AddonID,'')
update_file    =  os.path.join(ADDONDATA,AddonID,'updating')

print'###### Community Portal Update Service ######'

if not os.path.exists(TBSDATA):
    os.makedirs(TBSDATA)

if not os.path.exists(idfile):
    localfile = open(idfile, mode='w+')
    localfile.write('id="None"\nname="None"')
    localfile.close()

if os.path.exists(os.path.join(TBSDATA,'scripts')):
	shutil.rmtree(os.path.join(TBSDATA,'scripts'))
	
if os.path.exists(update_file):
    xbmc.sleep(4000)
    while xbmc.Player().isPlaying():
        xbmc.sleep(500)
    dialog.ok('Update In Progress','The updates for your build are being installed. Please be patient, the skin will automatically switch to the correct one when ready. DO NOT manually attempt to switch skin at this time')
    shutil.rmtree(update_file)

if internetcheck == 'true':
    xbmc.executebuiltin('XBMC.AlarmClock(internetloop,XBMC.RunScript(special://home/addons/'+AddonID+'/connectivity.py,silent=true),00:01:00,silent,loop)')

if clean_cache == 'true':
    xbmc.executebuiltin('XBMC.AlarmClock(internetloop,XBMC.RunScript(special://home/addons/'+AddonID+'/cleancache.py,silent=true),12:00:00,silent,loop)')