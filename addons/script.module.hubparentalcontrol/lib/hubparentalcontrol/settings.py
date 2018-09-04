import xbmc, xbmcaddon, xbmcgui, xbmcplugin
import os

ROOTADDON = xbmcaddon.Addon(id='plugin.video.xbmchubmaintenance')
ADDON = xbmcaddon.Addon(id='script.module.hubparentalcontrol')
DATA_PATH = os.path.join(xbmc.translatePath('special://profile/addon_data/script.module.hubparentalcontrol'), '')
ADDON_PATH = xbmc.translatePath(os.path.join('special://home/addons/script.module.hubparentalcontrol', ''))

def addon():
    return ADDON
	
#Parental controls
def pc_custom_pc_file():
    return create_file(DATA_PATH, "custom_pc.list")
	
def pc_enable_pc():
    if ROOTADDON.getSetting('pc_enable_pc') == "true":
        return True
    else:
        return False
		
def pc_watershed_pc():
    id = ROOTADDON.getSetting('pc_watershed_pc')
    if id == '9':
        return 25
    elif id == '8':
        return 23 
    elif id == '7':
        return 22 
    elif id == '6':
        return 21
    elif id == '5':
        return 20
    elif id == '4':
        return 19
    elif id == '3':
        return 18
    elif id == '2':
        return 17
    elif id == '1':
        return 16
    else:
        return 15
		
def pc_pw_required_at():
    id = ROOTADDON.getSetting('pc_pw_required_at')
    if id == '3':
        return 4
    elif id == '2':
        return 3
    elif id == '1':
        return 2
    else:
        return 1

def pc_enable_pc_settings():
    return ROOTADDON.getSetting('pc_enable_pc_settings')
		
def pc_pass():
    return ROOTADDON.getSetting('pc_pass')
	
def pc_default():
    id = ROOTADDON.getSetting('pc_default')
    if id == '1':
        return "REQUIRE PIN"
    else:
        return "PLAY"
	
#End Parental Control
	
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
	
	
create_directory(DATA_PATH, "")


		
