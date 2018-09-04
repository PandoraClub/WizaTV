import urllib,urllib2,re,xbmcplugin,xbmcgui,xbmc, xbmcaddon, os, sys
import time
from t0mm0_common_addon import Addon
from t0mm0_common_net import Net

addon_id = 'plugin.program.wizetv'
addon = Addon(addon_id)#, sys.argv)
net = Net()
# settings = xbmcaddon.Addon(id=addon_id)
net.set_user_agent('Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3'); 


def OPEN_URL(url): req=urllib2.Request(url); req.add_header('User-Agent','Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3'); response=urllib2.urlopen(req); link=response.read(); response.close(); return link

def check_internet():
    try:
        response = urllib2.urlopen('http://www.google.com', timeout=5)
        return True
    except urllib2.URLError as err:
        pass
        
    addon.show_ok_dialog(["It seems that your box is not connected to internet. Internet is required to run this app", "Please connect your box to internet and try again."], title="No Internet Connectivity!", is_error=True);             
    
    return False


def DoA(a): xbmc.executebuiltin("Action(%s)" % a) #DoA('Back'); # to move to previous screen.

def eod(): addon.end_of_directory()


#First see if there is any trial file
xbmc.executebuiltin('ActivateWindow(home)')
exit()
