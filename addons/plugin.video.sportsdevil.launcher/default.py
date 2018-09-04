#      Copyright (C) 2015 noobsandnerds
#
#  This Program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2, or (at your option)
#  any later version.
#
#  This Program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with XBMC; see the file COPYING.  If not, write to
#  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
#  http://www.gnu.org/copyleft/gpl.html
#

import sys, xbmcplugin ,xbmcgui, xbmcaddon, xbmc, os, shutil, urllib, urllib2, re
import downloader, extract

AddonID     = 'plugin.video.SportsDevil'
dialog      = xbmcgui.Dialog()
ADDONS      = xbmc.translatePath(os.path.join('special://home','addons'))
sportsdevil = xbmc.translatePath(os.path.join(ADDONS,'plugin.video.SportsDevil'))
sd_data     = xbmc.translatePath(os.path.join('special://home','userdata','addon_data','plugin.video.SportsDevil','cache'))
addondata   = xbmc.translatePath(os.path.join('special://home','userdata','addon_data','plugin.video.sportsdevil.launcher'))
sd_temp     = xbmc.translatePath(os.path.join('special://home','userdata','addon_data','plugin.video.sportsdevil.launcher','old_addon'))
noobsrepo   = xbmc.translatePath(os.path.join(ADDONS,'repository.noobsandnerds'))
packages    = xbmc.translatePath(os.path.join(ADDONS,'packages'))
CP          = xbmc.translatePath(os.path.join(ADDONS,'plugin.program.totalinstaller'))
quit        = False
opencp      = False

if os.path.exists(sportsdevil):
    Addon       = xbmcaddon.Addon(AddonID)
    localver    = Addon.getAddonInfo('version')
else:
    localver    = 0

def Open_URL(url):
    req = urllib2.Request(url)
    req.add_header('User-Agent' , 'Mozilla/5.0 (Windows; U; Windows NT 10.0; WOW64; Windows NT 5.1; en-GB; rv:1.9.0.3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36 Gecko/2008092417 Firefox/3.0.3')
#    req.add_header('User-Agent','Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3')
    response = urllib2.urlopen(req)
    link     = response.read()
    response.close()
    return link.replace('\r','').replace('\n','').replace('\t','')


if not os.path.exists(noobsrepo):
    downloader.download('http://noobsandnerds.com/portal/noobsandnerds_repo.zip',os.path.join(packages,'noobsandnerds.zip'))
    extract.all(os.path.join(packages,'noobsandnerds.zip'),ADDONS)
    xbmc.executebuiltin('UpdateLocalAddons')
    xbmc.executebuiltin('UpdateAddonRepos')

if not os.path.exists(addondata):
    os.makedirs(addondata)

if not os.path.exists(CP):
    choice = dialog.yesno('Community Portal Required','In order to use this addon you need the Community Portal addon installed. Would you like to install now?')
    if choice == 1:
        xbmc.executebuiltin('ActivateWindow(10040,addons://repository.noobsandnerds/xbmc.addon.executable)')
    quit = True


if not quit:
    try:
        VersionCheck = Open_URL('http://noobsandnerds.com/TI/AddonPortal/latest_version.php?id=plugin.video.SportsDevil')
        if localver < VersionCheck and localver != 0:
            print"### New version of SportsDevil available"
            choice = dialog.yesno('New Version Available','A newer version of SportsDevil is available, would you like to search the noobsandnerds addon portal for the latest version?')
            if choice == 1:
                if os.path.exists(sd_temp):
                    shutil.rmtree(sd_temp)
                os.rename(sportsdevil, sd_temp)
                quit = True
                opencp = True

        else:
            print"### SportsDevil up to date"
    except: print"### Unable to access NaN server to check for updates"
        
    if not os.path.exists(sportsdevil) and os.path.exists(sd_temp) and not quit:
        choice = dialog.yesno('Sports Devil Backup Found','SportsDevil canonot be found in your addons folder but there is a backup on this system. Would you like to install the backup?')
        if choice == 1:
            os.rename(sd_temp,sportsdevil)


    if os.path.exists(sportsdevil) and not quit:
        if os.path.exists(sd_data):
            choice = dialog.yesno('Clear SportsDevil Cache?','Would you like to clear your SportsDevil Cache? Use this if you\'re having problems opening the addon and getting web request errors.')
            if choice == 1:
                try:
                    shutil.rmtree(sd_data)
                except:
                    pass
        xbmc.executebuiltin('RunAddon(plugin.video.SportsDevil)')

    if opencp:
        xbmc.executebuiltin('ActivateWindow(10001,"plugin://plugin.program.totalinstaller/?mode=grab_addons&url=%26redirect%26addonid%3dplugin.video.SportsDevil",return)')

    if not os.path.exists(sportsdevil) and not quit:
        choice = dialog.yesno('SportsDevil Not Detected','The SportsDevil addon cannot be found on your system, would you like to search for it now?')
        if choice == 1:
         xbmc.executebuiltin('ActivateWindow(10001,"plugin://plugin.program.totalinstaller/?mode=grab_addons&url=%26redirect%26addonid%3dplugin.video.SportsDevil",return)')