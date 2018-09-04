import xbmc, xbmcaddon, xbmcgui, xbmcplugin
import os

ADDON = xbmcaddon.Addon(id='plugin.program.logviewer')
DATA_PATH = os.path.join(xbmc.translatePath('special://profile/addon_data/plugin.program.logviewer'), '')
ADDON_PATH = xbmc.translatePath(os.path.join('special://home/addons/plugin.program.logviewer', ''))


def addon():
    return ADDON

def extract_directory():
    return ADDON.getSetting('extract_directory')
	
	
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

		
